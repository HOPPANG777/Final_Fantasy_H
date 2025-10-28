#include "Framework.h"
#include "ColorLine.h"

ColorLine::ColorLine(
    const DirectX::SimpleMath::Vector3& position,
    float length,
    float rotation,
    const DirectX::SimpleMath::Color& color,
    bool useRectTransform
)
    : Drawable("ColorLine", position, DirectX::SimpleMath::Vector3(length, 0.0f, 0.0f), rotation, useRectTransform, ShaderPath + L"Color.hlsl"),
      _color(color)
{
    InitializeGeometry();
}

void ColorLine::Update()
{
    Drawable::Update();
}

void ColorLine::Render()
{
    Drawable::Render();
    DrawCall(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, false);
}

void ColorLine::GUI()
{
    ImGui::Begin("Color", &isOpen);

    // 특정 UI기능이 작동했다면 특정 함수를 실행하게 할수도 있음
    if (ImGui::ColorPicker4("ColorLine", (float*)&_color)) UpdateVertexColors();

    ImGui::End();
}

DirectX::SimpleMath::Color ColorLine::GetColor() const
{
    return _color;
}

void ColorLine::SetColor(const DirectX::SimpleMath::Color& color)
{
    _color = color;
    UpdateVertexColors();
}

void ColorLine::InitializeGeometry()
{
    _vertices.assign(2, VertexColor());

    _vertices[0]._position = { 0.0f, 0.0f };
    _vertices[1]._position = { 1.0f, 0.0f };

    for (VertexColor& vertex : _vertices)
    {
        vertex._color = _color;
    }

    vertexBuffer->Create(_vertices, D3D11_USAGE_DYNAMIC);
    inputLayout->Create(VertexColor::ieDescs.data(), VertexColor::ieDescs.size(), vertexShader->GetBlob());
}

void ColorLine::UpdateVertexColors()
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
