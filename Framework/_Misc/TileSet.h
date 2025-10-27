#pragma once

#include <memory>
#include <string>
#include <directxtk/SimpleMath.h>

class Texture;

// \ud0c0\uc77c \uc138\ud2b8\ub294 \ud30c\uc77c \uacbd\ub85c\uc640 \ubc30\uce58 \uc815\ubcf4\ub97c \ub4f1\b85d\ud558\uc5ec \ucd94\ud6c4 \ud0c0\uc77c\uc744 \uc120\ud0dd\ud560 \ub54c \ucc38\uc870\ud55c\ub2e4.
class TileSet
{
public:
    TileSet(const std::wstring& atlasPath, UINT columns, UINT rows);
    ~TileSet() = default;

    void Load(const std::wstring& atlasPath, UINT columns, UINT rows);

    const std::wstring& GetAtlasPath() const noexcept { return path; }
    UINT GetColumns() const noexcept { return columnCount; }
    UINT GetRows() const noexcept { return rowCount; }

    DirectX::SimpleMath::Vector2 GetTileUVSize() const noexcept { return tileUVSize; }

    DirectX::SimpleMath::Vector2 CalculateStartUV(UINT column, UINT row) const noexcept;
    DirectX::SimpleMath::Vector2 CalculateEndUV(UINT column, UINT row) const noexcept;

    ID3D11ShaderResourceView* GetShaderResourceView() const noexcept;

    bool IsValid() const noexcept { return static_cast<bool>(textureComponent); }

private:
    void InitializeTexture();
    void UpdateTileSize();

private:
    std::shared_ptr<Texture> textureComponent;
    std::wstring path;
    UINT columnCount = 1;
    UINT rowCount = 1;
    DirectX::SimpleMath::Vector2 tileUVSize = DirectX::SimpleMath::Vector2::One;
};

