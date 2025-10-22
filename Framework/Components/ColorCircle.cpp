#include "Framework.h"
#include "ColorCircle.h"

#include <cmath>

ColorCircle::ColorCircle(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& scale,
    float rotation,
    size_t segments,
    const DirectX::SimpleMath::Color& color,
    bool useRectTransform
)
    : Drawable("ColorCircle", position, scale, rotation, useRectTransform, ShaderPath + L"Color.hlsl"),
      _segments(segments),
      _color(color)
{
    if (_segments < 3)
    {
        _segments = 3;
    }
    InitializeGeometry();
}

void ColorCircle::Update()
{
    Drawable::Update();
}

void ColorCircle::Render()
{
    Drawable::Render();
    DrawCall(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, false);
}

DirectX::SimpleMath::Color ColorCircle::GetColor() const
{
    return _color;
}

void ColorCircle::SetColor(const DirectX::SimpleMath::Color& color)
{
    _color = color;
    UpdateVertexColors();
}

void ColorCircle::InitializeGeometry()
{
    _vertices.assign(_segments + 1, VertexColor());

    const float segmentStep = DirectX::XM_2PI / static_cast<float>(_segments);
    for (size_t index = 0; index <= _segments; ++index)
    {
        const float theta = segmentStep * static_cast<float>(index);
        const float x = std::sinf(theta) * 0.5f;
        const float y = std::cosf(theta) * 0.5f;

        _vertices[index]._position = { x, y };
        _vertices[index]._color = _color;
    }

    vertexBuffer->Create(_vertices, D3D11_USAGE_DYNAMIC);
    inputLayout->Create(VertexColor::ieDescs.data(), VertexColor::ieDescs.size(), vertexShader->GetBlob());
}

void ColorCircle::UpdateVertexColors()
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
