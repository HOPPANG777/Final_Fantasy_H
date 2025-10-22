#include "Framework.h"
#include "ColorOutlineRect.h"

ColorOutlineRect::ColorOutlineRect(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& scale,
    float rotation,
    const DirectX::SimpleMath::Color& color,
    bool useRectTransform
)
    : Drawable("ColorOutlineRect", position, scale, rotation, useRectTransform, ShaderPath + L"VertexColor.hlsl"),
      _color(color)
{
    InitializeGeometry();
}

void ColorOutlineRect::Update()
{
    Drawable::Update();
}

void ColorOutlineRect::Render()
{
    Drawable::Render();
    DrawCall(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, false);
}

DirectX::SimpleMath::Color ColorOutlineRect::GetColor() const
{
    return _color;
}

void ColorOutlineRect::SetColor(const DirectX::SimpleMath::Color& color)
{
    _color = color;
    UpdateVertexColors();
}

void ColorOutlineRect::InitializeGeometry()
{
    _vertices.assign(5, VertexColor());

    _vertices[0]._position = { -0.5f, -0.5f };
    _vertices[1]._position = { -0.5f,  0.5f };
    _vertices[2]._position = {  0.5f,  0.5f };
    _vertices[3]._position = {  0.5f, -0.5f };
    _vertices[4] = _vertices[0];

    for (VertexColor& vertex : _vertices)
    {
        vertex._color = _color;
    }

    vertexBuffer->Create(_vertices, D3D11_USAGE_DYNAMIC);
    inputLayout->Create(VertexColor::ieDescs.data(), VertexColor::ieDescs.size(), vertexShader->GetBlob());
}

void ColorOutlineRect::UpdateVertexColors()
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
