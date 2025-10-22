#pragma once
class InputLayout
{
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;

public:
	void Create(const D3D11_INPUT_ELEMENT_DESC* desc, const size_t& count, ID3DBlob* blob)
	{
		HRESULT hr = GraphicsManager::Get()->GetDevice()->CreateInputLayout
		(
			desc,
			UINT(count),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&_inputLayout
		);
		CHECK(hr);
	}

	void SetIA()
	{
		GraphicsManager::Get()->GetDC()->IASetInputLayout(_inputLayout.Get());
	}


public:
	ID3D11InputLayout* GetResource() const { return _inputLayout.Get(); }

};