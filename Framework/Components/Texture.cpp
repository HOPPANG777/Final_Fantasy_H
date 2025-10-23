#include "Framework.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace
{
    struct LoadedImage
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        std::vector<uint8_t> pixels;
    };

    bool LoadImageFile(const std::wstring& filePath, LoadedImage& outImage)
    {
        if (filePath.empty())
        {
            return false;
        }

        const std::string narrowPath = String::ToString(filePath);

        int width = 0;
        int height = 0;
        int channels = 0;
        stbi_uc* data = stbi_load(narrowPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (data == nullptr)
        {
            return false;
        }

        outImage.width = width;
        outImage.height = height;
        outImage.channels = 4;

        const size_t pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(outImage.channels);
        outImage.pixels.assign(data, data + pixelCount);

        stbi_image_free(data);
        return true;
    }
}

Texture::Texture(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& scale,
    float rotation,
    const std::wstring& texturePath,
    bool useRectTransform
)
    : Drawable("Texture", position, scale, rotation, useRectTransform, ShaderPath + L"Texture.hlsl")
{
    InitializeGeometry();
    InitializeStates();

    _colorBuffer = std::make_unique<ColorBuffer>();

    if (!texturePath.empty())
    {
        CreateTextureFromImage(texturePath);
    }
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
    LoadedImage image;
    if (!LoadImageFile(filePath, image))
    {
        _texture.Reset();
        _shaderResourceView.Reset();
        return false;
    }

    ID3D11Device* device = GraphicsManager::Get()->GetDevice();

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = static_cast<UINT>(image.width);
    textureDesc.Height = static_cast<UINT>(image.height);
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem = image.pixels.data();
    subresourceData.SysMemPitch = static_cast<UINT>(image.width * image.channels);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    HRESULT hr = device->CreateTexture2D(&textureDesc, &subresourceData, texture.GetAddressOf());
    CHECK(hr);

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    hr = device->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView.GetAddressOf());
    CHECK(hr);

    _texture = texture;
    _shaderResourceView = shaderResourceView;

    return true;
}

