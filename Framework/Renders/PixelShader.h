#pragma once

class PixelShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;

public:
	void Create(const std::wstring& path, const std::string& entryName) override
	{
		this->path = path;
		this->entryName = entryName;

		CompileShader(path, entryName, "ps_5_0", blob.GetAddressOf());
		HRESULT hr = GraphicsManager::Get()->GetDevice()->CreatePixelShader
		(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&shader
		);
		CHECK(hr);
	}

	void Clear() override
	{
		shader.Reset();
		blob.Reset();
	}

	void SetShader() override
	{
		GraphicsManager::Get()->GetDC()->PSSetShader(shader.Get(), nullptr, 0);
	}

public:
	ID3D11PixelShader* GetResource() const { return shader.Get(); }

};

