#include "Framework.h"
#include "ColorRect.h"

ColorRect::ColorRect(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& scale,
    float rotation,
    const DirectX::SimpleMath::Color& color,
    bool useRectTransform
)
    : Drawable("ColorRect", position, scale, rotation, useRectTransform, ShaderPath + L"Color.hlsl"), _color(color)
{
    InitializeGeometry();
}

void ColorRect::Update()
{
    __super::Update();
}

void ColorRect::Render()
{
    __super::Render();
    DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ColorRect::GUI()
{
    ImGui::Begin("Color", &isOpen);

    // 특정 UI기능이 작동했다면 특정 함수를 실행하게 할수도 있음
    if (ImGui::ColorPicker4("ColorRect", (float*)&_color)) UpdateVertexColors();

    ImGui::End();
}

DirectX::SimpleMath::Color ColorRect::GetColor() const
{
    return _color;
}

void ColorRect::SetColor(const DirectX::SimpleMath::Color& color)
{
    _color = color;
    UpdateVertexColors();
}

void ColorRect::InitializeGeometry()
{
    _vertices.assign(4, VertexColor());

    _vertices[0]._position = { -0.5f, -0.5f };
    _vertices[1]._position = { -0.5f,  0.5f };
    _vertices[2]._position = {  0.5f, -0.5f };
    _vertices[3]._position = {  0.5f,  0.5f };

    for (VertexColor& vertex : _vertices)
    {
        vertex._color = _color;
    }

    vertexBuffer->Create(_vertices, D3D11_USAGE_DYNAMIC);
    _indices = { 0, 1, 2, 2, 1, 3 };
    indexBuffer->Create(_indices, D3D11_USAGE_IMMUTABLE);
    inputLayout->Create(VertexColor::ieDescs.data(), VertexColor::ieDescs.size(), vertexShader->GetBlob());
}

void ColorRect::UpdateVertexColors()
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
