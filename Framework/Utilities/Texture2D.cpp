#include "Framework.h"
#include "Texture2D.h"
#include "Texture2DCache.h"

#include <cstdint>
#include <stdexcept>
#include <utility>

std::shared_ptr<ITexture2DCache> Texture2D::_defaultCache = std::make_shared<Texture2DCache>();

Texture2D::Texture2D(const std::wstring& filePath)
    : _filePath(filePath)
{
    // 기본 캐시가 초기화되어 있어야 정상 동작한다.
    if (!_defaultCache)
    {
        throw std::runtime_error("Texture2D default cache is not initialized.");
    }

    // 기본 캐시를 통해 텍스처를 로드한다.
    LoadTexture(*_defaultCache);
}

Texture2D::Texture2D(const std::wstring& filePath, ITexture2DCache& cache)
    : _filePath(filePath)
{
    // 주입받은 캐시 객체로 로딩을 수행한다.
    LoadTexture(cache);
}

void Texture2D::ReadPixel(std::vector<Color>* pixels)
{
    // 결과 버퍼가 없으면 아무 작업도 하지 않는다.
    if (pixels == nullptr)
    {
        return;
    }

    Microsoft::WRL::ComPtr<ID3D11Resource> resource = nullptr;
    // SRV가 비어있으면 픽셀 정보 역시 비워준다.
    if (_srv == nullptr)
    {
        pixels->clear();
        return;
    }

    _srv->GetResource(resource.GetAddressOf());

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
    resource.As(&texture);

    ReadPixel(texture, pixels);
}

void Texture2D::ReadPixel(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, std::vector<Color>* pixels)
{
    // 유효한 텍스처와 결과 버퍼가 필요하다.
    if (texture == nullptr || pixels == nullptr)
    {
        return;
    }

    D3D11_TEXTURE2D_DESC sourceDesc = {};
    texture->GetDesc(&sourceDesc);

    D3D11_TEXTURE2D_DESC stagingDesc = {};
    stagingDesc.Width = sourceDesc.Width;
    stagingDesc.Height = sourceDesc.Height;
    stagingDesc.MipLevels = 1;
    stagingDesc.ArraySize = 1;
    stagingDesc.Format = sourceDesc.Format;
    stagingDesc.SampleDesc = sourceDesc.SampleDesc;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture = nullptr;
    HRESULT hr = GraphicsManager::Get()->GetDevice()->CreateTexture2D(&stagingDesc, nullptr, stagingTexture.GetAddressOf());
    CHECK(hr);

    GraphicsManager::Get()->GetDC()->CopyResource(stagingTexture.Get(), texture.Get());

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    hr = GraphicsManager::Get()->GetDC()->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
    CHECK(hr);

    const size_t pixelCount = static_cast<size_t>(stagingDesc.Width) * static_cast<size_t>(stagingDesc.Height);
    const size_t bytesPerPixel = DirectX::BitsPerPixel(stagingDesc.Format) / 8;

    // 압축 텍스처 등은 직접 읽어올 수 없으므로 기본값으로 채운다.
    if (bytesPerPixel == 0)
    {
        GraphicsManager::Get()->GetDC()->Unmap(stagingTexture.Get(), 0);
        pixels->assign(pixelCount, Color(0.0f, 0.0f, 0.0f, 1.0f));
        return;
    }

    std::vector<Color> localPixels(pixelCount);

    const auto* mappedBytes = static_cast<const std::uint8_t*>(mappedResource.pData);

    for (UINT y = 0; y < stagingDesc.Height; ++y)
    {
        const std::uint8_t* row = mappedBytes + static_cast<size_t>(y) * mappedResource.RowPitch;
        for (UINT x = 0; x < stagingDesc.Width; ++x)
        {
            const size_t index = static_cast<size_t>(y) * stagingDesc.Width + x;
            const std::uint8_t* pixel = row + static_cast<size_t>(x) * bytesPerPixel;

            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;
            float a = 1.0f;

            if (bytesPerPixel >= 3)
            {
                // BGRA 포맷은 R과 B의 순서를 교환한다.
                if (stagingDesc.Format == DXGI_FORMAT_B8G8R8A8_UNORM || stagingDesc.Format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB)
                {
                    b = static_cast<float>(pixel[0]) / 255.0f;
                    g = static_cast<float>(pixel[1]) / 255.0f;
                    r = static_cast<float>(pixel[2]) / 255.0f;
                    if (bytesPerPixel == 4)
                    {
                        a = static_cast<float>(pixel[3]) / 255.0f;
                    }
                }
                else
                {
                    r = static_cast<float>(pixel[0]) / 255.0f;
                    if (bytesPerPixel > 1)
                    {
                        g = static_cast<float>(pixel[1]) / 255.0f;
                    }
                    if (bytesPerPixel > 2)
                    {
                        b = static_cast<float>(pixel[2]) / 255.0f;
                    }
                    if (bytesPerPixel > 3)
                    {
                        a = static_cast<float>(pixel[3]) / 255.0f;
                    }
                }
            }

            localPixels[index] = Color(r, g, b, a);
        }
    }

    GraphicsManager::Get()->GetDC()->Unmap(stagingTexture.Get(), 0);

    // 계산된 픽셀 데이터를 결과 컨테이너에 전달한다.
    *pixels = std::move(localPixels);
}

void Texture2D::SetDefaultCache(std::shared_ptr<ITexture2DCache> cache)
{
    // 기본 캐시를 외부에서 교체할 수 있도록 한다.
    _defaultCache = std::move(cache);
}

std::shared_ptr<ITexture2DCache> Texture2D::GetDefaultCache()
{
    // 현재 사용 중인 기본 캐시를 반환한다.
    return _defaultCache;
}

void Texture2D::LoadTexture(ITexture2DCache& cache)
{
    // 캐시에 로드를 위임한다.
    cache.Load(*this);
}

