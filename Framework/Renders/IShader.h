#pragma once

#include <algorithm>
#include <vector>

class IShader
{
protected:
	std::wstring path = L"";
	std::string entryName = "";
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

public:
	virtual ~IShader() = default;

	virtual void Create(const std::wstring& path, const std::string& entryName) = 0;

	virtual void Clear() = 0;
	virtual void SetShader() = 0;

public:
	ID3DBlob* GetBlob() const { return blob.Get(); }

protected:
	void CompileShader(const std::wstring& path, const std::string& entryName, const std::string& profile, ID3DBlob** blob)
	{
#if defined(_DEBUG)
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hr = E_FAIL;
		const auto candidates = BuildSourceShaderSearchPaths(path);
		for (const auto& candidate : candidates)
		{
			if (candidate.empty())
				continue;

			error.Reset();
			hr = D3DCompileFromFile
			(
				candidate.c_str(),
				nullptr,
				nullptr,
				entryName.c_str(),
				profile.c_str(),
				compileFlags,
				0,
				blob,
				&error
			);

			if (SUCCEEDED(hr))
				break;
		}

		if (FAILED(hr))
		{
			CheckShaderError(hr, error.Get());
			return;
		}

		SaveCompiledShader(path, *blob);
#else
		LoadCompiledShader(path, blob);
#endif
	}

private:
	void CheckShaderError(const HRESULT& hr, ID3DBlob* error)
	{
		if (FAILED(hr))
		{
			if (error)
			{
				const std::string& str = (const char*)error->GetBufferPointer();
				MessageBoxA(nullptr, str.c_str(), "Shader Error", MB_OK);
			}
			assert(false);
		}
	}

	void SaveCompiledShader(const std::wstring& sourcePath, ID3DBlob* compiledBlob)
	{
		if (compiledBlob == nullptr)
			return;

		try
		{
			const std::wstring binaryPath = ResolveBinaryPath(sourcePath);
			if (binaryPath.empty())
				return;

			WriteBlobToFile(binaryPath, compiledBlob);

			const std::wstring relativeBinary = ExtractRelativeShaderPath(binaryPath);
			const std::wstring sourceRoot = ResolveSourceShaderRoot();
			if (!relativeBinary.empty() && !sourceRoot.empty())
			{
				WriteBlobToFile(CombinePath(sourceRoot, relativeBinary), compiledBlob);
			}
		}
		catch (...)
		{
			// 캐시 저장 실패는 치명적이지 않으므로 무시한다.
		}
	}

	void LoadCompiledShader(const std::wstring& sourcePath, ID3DBlob** blob)
	{
		const std::wstring binaryPath = ResolveBinaryPath(sourcePath);
		if (binaryPath.empty())
			throw std::runtime_error("Invalid shader path");

		const auto candidates = BuildCompiledShaderSearchPaths(binaryPath);
		std::string lastError;

		for (const auto& candidate : candidates)
		{
			if (candidate.empty())
				continue;

			try
			{
				DX::ReadData(candidate.c_str(), blob);
				return;
			}
			catch (const std::exception& e)
			{
				lastError = e.what();
			}
			catch (...)
			{
				lastError = "Unknown error while loading shader cache.";
			}
		}

		if (lastError.empty())
			lastError = "Unable to locate compiled shader file.";

		MessageBoxA(nullptr, lastError.c_str(), "Shader Cache Load Error", MB_OK);
		throw std::runtime_error(lastError);
	}

	std::wstring ResolveBinaryPath(const std::wstring& sourcePath) const
	{
		if (sourcePath.empty())
			return L"";

		std::wstring binaryPath = sourcePath;
		const size_t dotPos = binaryPath.find_last_of(L'.');
		if (dotPos != std::wstring::npos)
		{
			binaryPath.replace(dotPos, std::wstring::npos, L".cso");
		}
		else
		{
			binaryPath += L".cso";
		}

		return NormalizePath(binaryPath);
	}

	void WriteBlobToFile(const std::wstring& targetPath, ID3DBlob* compiledBlob)
	{
		if (targetPath.empty() || compiledBlob == nullptr)
			return;

		const std::wstring normalized = NormalizePath(targetPath);
		const std::wstring directory = GetDirectoryName(normalized);
		if (!directory.empty())
			CreateDirectoriesRecursively(directory);

		HANDLE file = CreateFileW(normalized.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (file == INVALID_HANDLE_VALUE)
			return;

		const BYTE* buffer = static_cast<const BYTE*>(compiledBlob->GetBufferPointer());
		DWORD remaining = static_cast<DWORD>(compiledBlob->GetBufferSize());
		DWORD offset = 0;
		while (remaining > 0)
		{
			DWORD bytesWritten = 0;
			if (!WriteFile(file, buffer + offset, remaining, &bytesWritten, nullptr) || bytesWritten == 0)
				break;

			remaining -= bytesWritten;
			offset += bytesWritten;
		}
		CloseHandle(file);
	}

	std::wstring ExtractRelativeShaderPath(const std::wstring& shaderPath) const
	{
		const std::wstring normalized = NormalizePath(shaderPath);
		std::wstring lowered = normalized;
		std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](wchar_t ch) { return static_cast<wchar_t>(::towlower(ch)); });

		const std::wstring marker = L"\\_shaders\\";
		const size_t position = lowered.find(marker);
		if (position == std::wstring::npos)
			return L"";

		const size_t start = position + marker.length();
		return normalized.substr(start);
	}

	std::wstring ResolveSourceShaderRoot() const
	{
		wchar_t modulePath[MAX_PATH] = { 0 };
		if (!GetModuleFileNameW(nullptr, modulePath, MAX_PATH))
			return {};

		std::wstring directory = GetDirectoryName(modulePath);

		while (!directory.empty())
		{
			const std::wstring shaderDirectory = CombinePath(directory, L"_Shaders");
			if (DirectoryExists(shaderDirectory))
				return shaderDirectory;

			const std::wstring parent = GetDirectoryName(directory);
			if (parent == directory)
				break;
			directory = parent;
		}

		return L"";
	}

	std::wstring NormalizePath(const std::wstring& path) const
	{
		std::wstring normalized = path;
		for (auto& ch : normalized)
		{
			if (ch == L'/')
				ch = L'\\';
		}
		return normalized;
	}

	std::wstring GetDirectoryName(const std::wstring& path) const
	{
		const std::wstring normalized = NormalizePath(path);
		const size_t pos = normalized.find_last_of(L'\\');
		if (pos == std::wstring::npos)
			return L"";
		if (pos == 0)
			return normalized.substr(0, 1);
		return normalized.substr(0, pos);
	}

	std::wstring GetFileName(const std::wstring& path) const
	{
		const std::wstring normalized = NormalizePath(path);
		const size_t pos = normalized.find_last_of(L'\\');
		if (pos == std::wstring::npos)
			return normalized;
		return normalized.substr(pos + 1);
	}

	std::wstring CombinePath(const std::wstring& base, const std::wstring& relative) const
	{
		if (base.empty())
			return NormalizePath(relative);
		if (relative.empty())
			return NormalizePath(base);

		std::wstring normalizedBase = NormalizePath(base);
		std::wstring normalizedRelative = NormalizePath(relative);
		if (normalizedBase.back() != L'\\')
			normalizedBase += L'\\';
		return normalizedBase + normalizedRelative;
	}

	bool DirectoryExists(const std::wstring& path) const
	{
		if (path.empty())
			return false;

		const DWORD attributes = GetFileAttributesW(path.c_str());
		return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	void CreateDirectoriesRecursively(const std::wstring& directory) const
	{
		if (directory.empty())
			return;

		const std::wstring normalized = NormalizePath(directory);
		if (DirectoryExists(normalized))
			return;

		const std::wstring parent = GetDirectoryName(normalized);
		if (!parent.empty() && parent != normalized)
			CreateDirectoriesRecursively(parent);

		CreateDirectoryW(normalized.c_str(), nullptr);
	}

	std::vector<std::wstring> BuildSourceShaderSearchPaths(const std::wstring& requestedPath) const
	{
		std::vector<std::wstring> candidates;
		AddCandidatePath(candidates, NormalizePath(requestedPath));

		const std::wstring relative = ExtractRelativeShaderPath(requestedPath);
		if (!relative.empty())
		{
			const std::wstring root = ResolveSourceShaderRoot();
			if (!root.empty())
				AddCandidatePath(candidates, CombinePath(root, relative));
		}

		return candidates;
	}

	std::vector<std::wstring> BuildCompiledShaderSearchPaths(const std::wstring& binaryPath) const
	{
		std::vector<std::wstring> candidates;
		AddCandidatePath(candidates, NormalizePath(binaryPath));

		const std::wstring relative = ExtractRelativeShaderPath(binaryPath);
		if (!relative.empty())
		{
			const std::wstring root = ResolveSourceShaderRoot();
			if (!root.empty())
				AddCandidatePath(candidates, CombinePath(root, relative));
		}

		return candidates;
	}

	void AddCandidatePath(std::vector<std::wstring>& candidates, std::wstring path) const
	{
		if (path.empty())
			return;

		path = NormalizePath(path);
		if (std::find(candidates.begin(), candidates.end(), path) == candidates.end())
			candidates.push_back(path);
	}


};

