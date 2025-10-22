#pragma once

#include <Windows.h>
#include <d3dcompiler.h>
#include <exception>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include "../Utilities/ReadData.h"

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

                HRESULT hr = D3DCompileFromFile
                (
                        path.c_str(),
                        nullptr,
                        nullptr,
                        entryName.c_str(),
                        profile.c_str(),
                        compileFlags,
                        0,
                        blob,
                        &error
                );

                CheckShaderError(hr, error.Get());
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

                        std::filesystem::path runtimeBinary(binaryPath);
                        WriteBlobToFile(runtimeBinary, compiledBlob);

                        const std::filesystem::path relativeBinary = ExtractRelativeShaderPath(runtimeBinary);
                        const std::filesystem::path sourceRoot = ResolveSourceShaderRoot();
                        if (!relativeBinary.empty() && !sourceRoot.empty())
                        {
                                WriteBlobToFile(sourceRoot / relativeBinary, compiledBlob);
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

                try
                {
                        DX::ReadData(binaryPath.c_str(), blob);
                }
                catch (const std::exception& e)
                {
                        MessageBoxA(nullptr, e.what(), "Shader Cache Load Error", MB_OK);
                        throw;
                }
        }

        std::wstring ResolveBinaryPath(const std::wstring& sourcePath) const
        {
                if (sourcePath.empty())
                        return L"";

                std::filesystem::path shaderPath(sourcePath);
                shaderPath.replace_extension(L".cso");
                return shaderPath.wstring();
        }

        void WriteBlobToFile(const std::filesystem::path& targetPath, ID3DBlob* compiledBlob)
        {
                if (targetPath.empty() || compiledBlob == nullptr)
                        return;

                std::filesystem::path normalized = targetPath;
                if (normalized.has_parent_path())
                        std::filesystem::create_directories(normalized.parent_path());

                std::ofstream outFile(normalized, std::ios::binary | std::ios::out | std::ios::trunc);
                if (!outFile.is_open())
                        return;

                outFile.write(static_cast<const char*>(compiledBlob->GetBufferPointer()), compiledBlob->GetBufferSize());
                outFile.close();
        }

        std::filesystem::path ExtractRelativeShaderPath(const std::filesystem::path& shaderPath) const
        {
                std::filesystem::path relative;
                bool foundShadersFolder = false;

                for (const auto& part : shaderPath)
                {
                        const auto partName = part.wstring();
                        if (!foundShadersFolder)
                        {
                                if (partName == L"_Shaders" || partName == L"_shaders")
                                        foundShadersFolder = true;
                                continue;
                        }

                        relative /= part;
                }

                return relative;
        }

        std::filesystem::path ResolveSourceShaderRoot() const
        {
                wchar_t modulePath[MAX_PATH] = { 0 };
                if (!GetModuleFileNameW(nullptr, modulePath, MAX_PATH))
                        return {};

                std::filesystem::path directory(modulePath);
                directory = directory.parent_path();

                while (!directory.empty())
                {
                        if (directory.filename().wstring() == L"Build")
                        {
                                std::filesystem::path root = directory.parent_path();
                                if (root.empty())
                                        break;

                                std::filesystem::path shaderRoot = root / L"Framework" / L"_Shaders";
                                if (std::filesystem::exists(shaderRoot))
                                        return shaderRoot;
                                break;
                        }

                        directory = directory.parent_path();
                }

                return {};
        }

};

