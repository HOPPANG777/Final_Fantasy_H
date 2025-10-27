#pragma once

struct TextureDesc
{
    std::wstring filePath = L"";
    UINT width = 0;
    UINT height = 0;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

    bool operator==(const TextureDesc& other) const
    {
        bool b = true;
        b &= (filePath == other.filePath);
        b &= (width == other.width);
        b &= (height == other.height);

        return b;
    }
};

class Texture2DManager
{
	DECLARE_SINGLETON(Texture2DManager);

public:
	void Load(Texture2D* texture);
	void Delete();

private:
	static std::vector<TextureDesc> descs;

};

