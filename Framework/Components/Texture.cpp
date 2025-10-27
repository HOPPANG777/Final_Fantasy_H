#include "Framework.h"
#include "Texture.h"

#include <algorithm>
#include <cwctype>
#include <cstring>

Texture::Texture(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& scale,
    float rotation,
    const std::wstring& texturePath,
    bool useRectTransform,
    std::string componentName
)
    : Drawable(componentName, position, scale, rotation, useRectTransform, ShaderPath + L"Texture.hlsl")
{
    InitializeGeometry();
    InitializeStates();

    if (!texturePath.empty())
    {
        CreateTextureFromImage(texturePath);
    }

    _colorBuffer = std::make_unique<ColorBuffer>();
}

void Texture::Update()
{
    __super::Update();
}

void Texture::Render()
{
    __super::Render();

    ID3D11DeviceContext* context = GraphicsManager::Get()->GetDC();

    if (_colorBuffer)
    {
        _colorBuffer->SetPSBuffer(2);
    }

    if (_samplerLinear)
    {
        ID3D11SamplerState* sampler = _samplerLinear.Get();
        context->PSSetSamplers(0, 1, &sampler);
    }

    if (_shaderResourceView)
    {
        ID3D11ShaderResourceView* resource = _shaderResourceView.Get();
        context->PSSetShaderResources(0, 1, &resource);
    }
    else
    {
        ID3D11ShaderResourceView* nullResource = nullptr;
        context->PSSetShaderResources(0, 1, &nullResource);
    }

    if (_rasterizerState)
    {
        context->RSSetState(_rasterizerState.Get());
    }

    if (_blendStateAlpha)
    {
        const float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
        context->OMSetBlendState(_blendStateAlpha.Get(), blendFactor, 0xFFFFFFFF);
    }

    DrawCall(_topology, _useIndices);

    // Restore blend state to default after drawing.
    const float defaultBlend[4] = { 0.f, 0.f, 0.f, 0.f };
    context->OMSetBlendState(nullptr, defaultBlend, 0xFFFFFFFF);
}

DirectX::SimpleMath::Color Texture::GetColor() const
{
    if (_colorBuffer)
    {
        return _colorBuffer->GetColor();
    }

    return DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 1.f);
}

void Texture::SetColor(const DirectX::SimpleMath::Color& color)
{
    if (_colorBuffer)
    {
        _colorBuffer->SetColor(color);
    }
}

void Texture::SetTexture(const std::wstring& filePath)
{
    if (filePath.empty())
    {
        _texture.Reset();
        _shaderResourceView.Reset();
        return;
    }

    CreateTextureFromImage(filePath);
}

void Texture::SetSRV(ID3D11ShaderResourceView* shaderResourceView)
{
    _texture.Reset();
    _shaderResourceView.Reset();

    if (shaderResourceView)
    {
        shaderResourceView->AddRef();
        _shaderResourceView.Attach(shaderResourceView);
    }
}

void Texture::SetGeometry(const std::vector<VertexTexture>& vertices, const std::vector<UINT>& indices)
{
    _vertices = vertices;
    _indices = indices;

    UpdateVertexBuffer();
    UpdateIndexBuffer();
}

void Texture::SetShapeFromPositions(
    const std::vector<DirectX::SimpleMath::Vector3>& positions,
    const std::vector<UINT>& indices)
{
    if (positions.empty())
    {
        return;
    }

    DirectX::SimpleMath::Vector3 minPosition = positions.front();
    DirectX::SimpleMath::Vector3 maxPosition = positions.front();

    for (const DirectX::SimpleMath::Vector3& position : positions)
    {
        minPosition = DirectX::SimpleMath::Vector3::Min(minPosition, position);
        maxPosition = DirectX::SimpleMath::Vector3::Max(maxPosition, position);
    }

    const float width = maxPosition.x - minPosition.x;
    const float height = maxPosition.y - minPosition.y;

    _vertices.resize(positions.size());
    for (size_t index = 0; index < positions.size(); ++index)
    {
        const DirectX::SimpleMath::Vector3& position = positions[index];

        float u = 0.5f;
        float v = 0.5f;

        if (width > 0.0f)
        {
            u = (position.x - minPosition.x) / width;
        }

        if (height > 0.0f)
        {
            v = 1.0f - (position.y - minPosition.y) / height;
        }

        _vertices[index] = VertexTexture(position, DirectX::SimpleMath::Vector2(u, v));
    }

    _indices = indices;

    UpdateVertexBuffer();
    UpdateIndexBuffer();
}

void Texture::SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY& topology)
{
    _topology = topology;
}

void Texture::InitializeGeometry()
{
    _vertices =
    {
        VertexTexture({ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f }),
        VertexTexture({ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f }),
        VertexTexture({  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }),
        VertexTexture({  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f })
    };

    _indices = { 0, 1, 2, 2, 1, 3 };

    UpdateVertexBuffer();
    UpdateIndexBuffer();

    inputLayout->Create(VertexTexture::ieDescs.data(), VertexTexture::ieDescs.size(), vertexShader->GetBlob());
}

void Texture::InitializeStates()
{
    ID3D11Device* device = GraphicsManager::Get()->GetDevice();

    D3D11_RASTERIZER_DESC rasterizerDesc = RenderStates::GetRasterizerDesc();
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    _rasterizerState = RenderStates::CreateRasterizer(device, rasterizerDesc);

    D3D11_SAMPLER_DESC samplerDesc = RenderStates::GetSamplerDesc();
    _samplerLinear = RenderStates::CreateSampler(device, samplerDesc);

    D3D11_BLEND_DESC blendDesc = RenderStates::GetBlendDesc();
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    _blendStateAlpha = RenderStates::CreateBlend(device, blendDesc);
}

void Texture::UpdateVertexBuffer()
{
    if (_vertices.empty())
    {
        vertexBuffer = std::make_unique<VertexBuffer>();
        return;
    }

    vertexBuffer->Create(_vertices, D3D11_USAGE_DYNAMIC);
}

void Texture::UpdateIndexBuffer()
{
    if (_indices.empty())
    {
        _useIndices = false;
        indexBuffer = std::make_unique<IndexBuffer>();
        return;
    }

    _useIndices = true;
    indexBuffer->Create(_indices, D3D11_USAGE_IMMUTABLE);
}

bool Texture::CreateTextureFromImage(const std::wstring& filePath)
{
    _texture.Reset();
    _shaderResourceView.Reset();

    if (filePath.empty())
    {
        return false;
    }

    DirectX::ScratchImage scratchImage;
    DirectX::TexMetadata metadata;

    std::wstring extension = Path::GetExtension(filePath);
    std::transform(extension.begin(), extension.end(), extension.begin(), [](wchar_t character)
    {
        return static_cast<wchar_t>(std::towlower(character));
    });

    HRESULT hr = S_OK;

    if (extension == L"dds")
    {
        hr = DirectX::LoadFromDDSFile(filePath.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, scratchImage);
    }
    else if (extension == L"tga")
    {
        hr = DirectX::LoadFromTGAFile(filePath.c_str(), &metadata, scratchImage);
    }
    else
    {
        hr = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
    }

    if (FAILED(hr))
    {
        return false;
    }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    hr = DirectX::CreateShaderResourceView(
        GraphicsManager::Get()->GetDevice(),
        scratchImage.GetImages(),
        scratchImage.GetImageCount(),
        metadata,
        shaderResourceView.GetAddressOf());
    CHECK(hr);

    Microsoft::WRL::ComPtr<ID3D11Resource> resource;
    shaderResourceView->GetResource(resource.GetAddressOf());

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    resource.As(&texture);

    _texture = texture;
    _shaderResourceView = shaderResourceView;

    return true;
}

std::vector<VertexTexture>& Texture::AccessVertices()
{
    return _vertices;
}

void Texture::CommitVertices()
{
    if (!_vertices.empty() && vertexBuffer && vertexBuffer->GetResource())
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource = {};
        ID3D11DeviceContext* context = GraphicsManager::Get()->GetDC();

        HRESULT result = context->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        CHECK(result);

        std::memcpy(mappedResource.pData, _vertices.data(), sizeof(VertexTexture) * _vertices.size());
        context->Unmap(vertexBuffer->GetResource(), 0);
    }
}

