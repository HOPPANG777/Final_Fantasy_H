#include "Framework.h"
#include "Texture2DCache.h"
#include "Texture2D.h"

#include <algorithm>
#include <cwctype>

namespace
{
    // 확장자를 소문자로 정규화하는 헬퍼 함수
    std::wstring NormalizeExtension(const std::wstring& extension)
    {
        std::wstring normalized = extension;
        std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](wchar_t character)
        {
            return static_cast<wchar_t>(std::towlower(character));
        });
        return normalized;
    }
}

bool Texture2DCacheDesc::operator==(const Texture2DCacheDesc& other) const
{
    return filePath == other.filePath && width == other.width && height == other.height;
}

void Texture2DCache::Load(Texture2D& texture)
{
    // 경로가 비어있다면 캐시를 초기화하고 즉시 반환
    if (texture._filePath.empty())
    {
        texture._srv.Reset();
        texture._metaData = {};
        return;
    }

    DirectX::TexMetadata metaData = {};
    std::wstring extension = NormalizeExtension(Path::GetExtension(texture._filePath));

    HRESULT hr = S_OK;

    // 파일 확장자에 따라 메타데이터만 먼저 로드하여 규격 확인
    if (extension == L"tga")
    {
        hr = DirectX::GetMetadataFromTGAFile(texture._filePath.c_str(), metaData);
    }
    else if (extension == L"dds")
    {
        hr = DirectX::GetMetadataFromDDSFile(texture._filePath.c_str(), DirectX::DDS_FLAGS_NONE, metaData);
    }
    else
    {
        hr = DirectX::GetMetadataFromWICFile(texture._filePath.c_str(), DirectX::WIC_FLAGS_NONE, metaData);
    }
    CHECK(hr);

    Texture2DCacheDesc desc = {};
    desc.filePath = texture._filePath;
    desc.width = static_cast<UINT>(metaData.width);
    desc.height = static_cast<UINT>(metaData.height);

    // 이미 동일한 텍스처가 캐시에 있으면 그 정보를 그대로 재사용
    if (const Texture2DCacheDesc* cachedDesc = FindCachedDesc(desc))
    {
        texture._srv = cachedDesc->shaderResourceView;
        texture._metaData = cachedDesc->metadata;
        return;
    }

    DirectX::ScratchImage image;

    // 이미지 원본을 읽어들여 CPU 메모리에 임시 보관
    if (extension == L"tga")
    {
        hr = DirectX::LoadFromTGAFile(texture._filePath.c_str(), &metaData, image);
    }
    else if (extension == L"dds")
    {
        hr = DirectX::LoadFromDDSFile(texture._filePath.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, image);
    }
    else
    {
        hr = DirectX::LoadFromWICFile(texture._filePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData, image);
    }
    CHECK(hr);

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
    // GPU에 업로드하여 SRV를 생성
    hr = DirectX::CreateShaderResourceView(
        GraphicsManager::Get()->GetDevice(),
        image.GetImages(),
        image.GetImageCount(),
        metaData,
        shaderResourceView.GetAddressOf());
    CHECK(hr);

    desc.shaderResourceView = shaderResourceView;
    desc.metadata = metaData;

    // Texture2D에 로드 결과 전달
    texture._srv = shaderResourceView;
    texture._metaData = metaData;

    // 캐시에 새 항목으로 저장하여 다음 호출에서 재사용
    _cache.emplace_back(desc);
}

void Texture2DCache::Clear()
{
    // 모든 캐시 엔트리의 GPU 리소스를 해제
    for (auto& desc : _cache)
    {
        desc.shaderResourceView.Reset();
    }
    _cache.clear();
}

const Texture2DCacheDesc* Texture2DCache::FindCachedDesc(const Texture2DCacheDesc& desc) const
{
    // 선형 탐색으로 동일한 키를 가진 항목을 찾는다
    auto found = std::find(_cache.begin(), _cache.end(), desc);
    if (found == _cache.end())
    {
        return nullptr;
    }
    return &(*found);
}

