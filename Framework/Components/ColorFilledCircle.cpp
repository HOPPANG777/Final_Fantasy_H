
#include "Framework.h"
#include "ColorFilledCircle.h"

#include <cmath>

ColorFilledCircle::ColorFilledCircle(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& scale,
    float rotation,
    size_t segments,
    const DirectX::SimpleMath::Color& color,
    bool useRectTransform
)
    : Drawable("ColorFilledCircle", position, scale, rotation, useRectTransform, ShaderPath + L"Color.hlsl"),
      _segments(segments),
      _color(color)
{
    if (_segments < 3)
    {
        _segments = 3;
    }

    InitializeGeometry();
}

void ColorFilledCircle::Update()
{
    Drawable::Update();
}

void ColorFilledCircle::Render()
{
    Drawable::Render();
    DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

DirectX::SimpleMath::Color ColorFilledCircle::GetColor() const
{
    return _color;
}

void ColorFilledCircle::SetColor(const DirectX::SimpleMath::Color& color)
{
    _color = color;
    UpdateVertexColors();
}

void ColorFilledCircle::InitializeGeometry()
{
    _vertices.assign(_segments + 1, VertexColor());
    _vertices[0]._position = { 0.0f, 0.0f };

    const float segmentStep = DirectX::XM_2PI / static_cast<float>(_segments);
    for (size_t index = 1; index <= _segments; ++index)
    {
        const float theta = segmentStep * static_cast<float>(index);
        const float x = std::sinf(theta) * 0.5f;
        const float y = std::cosf(theta) * 0.5f;

        _vertices[index]._position = { x, y };
    }

    for (VertexColor& vertex : _vertices)
    {
        vertex._color = _color;
    }

    _indices.assign(_segments * 3, 0);
    for (size_t index = 0; index < _segments; ++index)
    {
        _indices[index * 3] = 0;
        _indices[index * 3 + 1] = static_cast<UINT>(index) + 1;
        _indices[index * 3 + 2] = static_cast<UINT>(index) + 2;
    }
    _indices.back() = 1;

    vertexBuffer->Create(_vertices, D3D11_USAGE_DYNAMIC);
    indexBuffer->Create(_indices, D3D11_USAGE_IMMUTABLE);
    inputLayout->Create(VertexColor::ieDescs.data(), VertexColor::ieDescs.size(), vertexShader->GetBlob());
}

void ColorFilledCircle::UpdateVertexColors()
{
    ID3D11DeviceContext* deviceContext = GraphicsManager::Get()->GetDC();
    D3D11_MAPPED_SUBRESOURCE mappedSubResource;
    HRESULT hr = deviceContext->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
    CHECK(hr);

    for (VertexColor& vertex : _vertices)
    {
        vertex._color = _color;
    }

    memcpy(mappedSubResource.pData, _vertices.data(), vertexBuffer->GetCount() * vertexBuffer->GetStride());
    deviceContext->Unmap(vertexBuffer->GetResource(), 0);
}
