#include "Framework.h"
#include "Texture2DManager.h"

std::vector<TextureDesc> Texture2DManager::descs;

Texture2DManager::Texture2DManager() {}

Texture2DManager::~Texture2DManager() {}

void Texture2DManager::Delete()
{
    for (auto& desc : descs)
    {
        desc.srv.Reset();
    }
    descs.clear();
}

void Texture2DManager::Load(Texture2D* texture)
{
    HRESULT hr;
    DirectX::TexMetadata metaData;

    std::wstring ext = Path::GetExtension(texture->filePath);

    if (ext == L"tga")
    {
        hr = DirectX::GetMetadataFromTGAFile(texture->filePath.c_str(), metaData);
        CHECK(hr);
    }
    else if (ext == L"dds")
    {
        hr = DirectX::GetMetadataFromDDSFile(texture->filePath.c_str(), DirectX::DDS_FLAGS_NONE, metaData);
        CHECK(hr);
    }
    else
    {
        hr = DirectX::GetMetadataFromWICFile(texture->filePath.c_str(), DirectX::WIC_FLAGS_NONE, metaData);
        CHECK(hr);
    }
    
    UINT width = metaData.width;
    UINT height = metaData.height;

    TextureDesc desc;
    desc.filePath = texture->filePath;
    desc.width = width;
    desc.height = height;

    bool bExist = false;
    TextureDesc existDesc;
    for (const auto& temp : descs)
    {
        if (desc == temp)
        {
            bExist = true;
            existDesc = temp;
            break;
        }
    }

    if (bExist)
    {
        texture->srv = existDesc.srv;
        texture->metaData = metaData;
    }
    else
    {
        DirectX::ScratchImage image;

        if (ext == L"tga")
        {
            hr = DirectX::LoadFromTGAFile(texture->filePath.c_str(), &metaData, image);
            CHECK(hr);
        }
        else if (ext == L"dds")
        {
            hr = DirectX::LoadFromDDSFile(texture->filePath.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, image);
            CHECK(hr);
        }
        else
        {
            hr = DirectX::LoadFromWICFile(texture->filePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData, image);
            CHECK(hr);
        }

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
        hr = DirectX::CreateShaderResourceView
        (
            GraphicsManager::Get()->GetDevice(),
            image.GetImages(),
            image.GetImageCount(),
            metaData,
            &shaderResourceView
        );
        CHECK(hr);

        desc.srv = shaderResourceView;
        texture->srv = shaderResourceView;
        texture->metaData = metaData;
        descs.push_back(desc);
    }
}
