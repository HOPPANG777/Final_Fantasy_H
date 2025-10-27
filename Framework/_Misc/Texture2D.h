#pragma once

class Texture2DManager;

class Texture2D
{
public:
    Texture2D(const std::wstring& filePath)
        : filePath(filePath)
    {
        Texture2DManager::Get()->Load(this);
    }

public:
    void ReadPixel(std::vector<DirectX::SimpleMath::Color>* pixels)
    {
        // srv  Texture2D COM  
        Microsoft::WRL::ComPtr<ID3D11Resource> resource;
        srv->GetResource(&resource);

        Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
        resource.As(&texture2D);

        ReadPixel(texture2D, pixels);
    }

    static void ReadPixel(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, std::vector<DirectX::SimpleMath::Color>* pixels)
    {
        D3D11_TEXTURE2D_DESC srvDesc;
        texture->GetDesc(&srvDesc);

        D3D11_TEXTURE2D_DESC destDesc = {};
        destDesc.Usage = D3D11_USAGE_STAGING;
        destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        destDesc.Width = srvDesc.Width;
        destDesc.Height = srvDesc.Height;
        destDesc.MipLevels = 1;
        destDesc.ArraySize = 1;
        destDesc.Format = srvDesc.Format;
        destDesc.SampleDesc = srvDesc.SampleDesc;

        // CreateTexture2D
        Microsoft::WRL::ComPtr<ID3D11Texture2D> destTex;
        HRESULT hr = GraphicsManager::Get()->GetDevice()->CreateTexture2D(&destDesc, nullptr, &destTex);
        CHECK(hr);

        hr = D3DX11LoadTextureFromTexture(GraphicsManager::Get()->GetDC(), texture.Get(), nullptr, destTex.Get());
        CHECK(hr);
        
        const size_t pixelCount = static_cast<size_t>(destDesc.Width) * static_cast<size_t>(destDesc.Height);

        const std::unique_ptr<UINT[]>& colors = std::make_unique<UINT[]>(pixelCount);

        D3D11_MAPPED_SUBRESOURCE subRe;
        GraphicsManager::Get()->GetDC()->Map(destTex.Get(), 0, D3D11_MAP_READ, 0, &subRe);
        {
            memcpy(colors.get(), subRe.pData, sizeof(UINT) * pixelCount);
        }
        GraphicsManager::Get()->GetDC()->Unmap(destTex.Get(), 0);

        {
            const std::unique_ptr<UINT[]>& alpha =  std::make_unique<UINT[]>(pixelCount);
            const std::unique_ptr<UINT[]>& red =    std::make_unique<UINT[]>(pixelCount);
            const std::unique_ptr<UINT[]>& green =  std::make_unique<UINT[]>(pixelCount);
            const std::unique_ptr<UINT[]>& blue =   std::make_unique<UINT[]>(pixelCount);

            for (size_t i = 0; i < pixelCount; i++)
            {
                alpha[i] = (colors[i] & 0xFF000000);
                red[i] = (colors[i] & 0x00FF0000);
                green[i] = (colors[i] & 0x0000FF00);
                blue[i] = (colors[i] & 0x000000FF);

                red[i] = colors[i] >> 16;
                blue[i] = colors[i] << 16;

                colors[i] = alpha[i] | red[i] | green[i] | blue[i];
            }

            pixels->assign(colors.get(), colors.get() + pixelCount);
        }
    }

public:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV() const { return srv; }

    std::wstring GetFilePath() const { return filePath; }
    
    float GetWidth() const { return static_cast<float>(metaData.width); }
    float GetHeight() const { return static_cast<float>(metaData.height); }

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    DirectX::TexMetadata metaData;
    std::wstring filePath = L"";

private:
    friend class Texture2DManager;

};