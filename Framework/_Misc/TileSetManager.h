#pragma once

class TileSetManager
{
    DECLARE_SINGLETON(TileSetManager);

public:
    void Init();

    void ImGUI();

    // SRV   ComPtr Get() 
    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return tileSRV; }

    void ChangeTileMap(const std::wstring& imagePath, UINT xCount, UINT yCount);

public:
    wstring GetTilePath() { return tilePath; }

    UINT GetXCount() { return tileXCount; }
    UINT GetYCount() { return tileYCount; }

public:
    Vector2 selectedStartUV = Values::ZeroVec2;
    Vector2 texelTileSize = Values::ZeroVec2;

private:
    ComPtr<ID3D11ShaderResourceView> tileSRV;

    UINT tileXCount = 0;
    UINT tileYCount = 0;

    wstring tilePath = L"";

};

