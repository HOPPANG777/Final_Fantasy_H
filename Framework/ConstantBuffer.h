#pragma once

class ConstantBuffer
{
private:
	D3D11_BUFFER_DESC desc = { 0 };
	Microsoft::WRL::ComPtr<ID3D11Buffer> _buffer;

	void* _data;
	UINT _dataSize;

public:
	void SetVSBuffer(const UINT slot)
	{
		MapData(_data, _dataSize);

		GraphicsManager::Get()->GetDC()->VSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
	}

	void SetPSBuffer(const UINT slot)
	{
		MapData(_data, _dataSize);

		GraphicsManager::Get()->GetDC()->PSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
	}

protected:
	ConstantBuffer(void* data, const UINT dataSize)
		: _data(data), _dataSize(dataSize)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = dataSize;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = GraphicsManager::Get()->GetDevice()->CreateBuffer(&desc, nullptr, &_buffer);
		CHECK(hr);
	}

private:
	void MapData(void* data, const UINT dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		GraphicsManager::Get()->GetDC()->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		{
			memcpy(mappedSubResource.pData, data, dataSize);
		}
		GraphicsManager::Get()->GetDC()->Unmap(_buffer.Get(), 0);
	}

};

