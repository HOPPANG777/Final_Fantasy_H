#pragma once

class VertexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _buffer;
	UINT _stride = 0;
	UINT _offset = 0;
	UINT _count = 0;

public:
	template <typename T>
	void Create(const std::vector<T>& vertices, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);

	void SetIA()
	{
		GraphicsManager::Get()->GetDC()->IASetVertexBuffers(0, 1, _buffer.GetAddressOf(), &_stride, &_offset);
	};

public:
	ID3D11Buffer* GetResource() const { return _buffer.Get(); }
	UINT GetStride() const { return _stride; }
	UINT GetOffset() const { return _offset; }
	UINT GetCount() const { return _count; }
};

template<typename T>
inline void VertexBuffer::Create(const std::vector<T>& vertices, const D3D11_USAGE& usage)
{
	_stride = sizeof(T);
	_count = UINT(vertices.size());

	D3D11_BUFFER_DESC desc = { 0 };

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = _stride * _count;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE:
		break;
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = vertices.data();

	HRESULT hr = GraphicsManager::Get()->GetDevice()->CreateBuffer(&desc, &subData, &_buffer);
	CHECK(hr);
}