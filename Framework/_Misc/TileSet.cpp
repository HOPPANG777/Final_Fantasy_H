#include "Framework.h"
#include "TileSet.h"

#include "Components/Texture.h"

using namespace DirectX::SimpleMath;

TileSet::TileSet(const std::wstring& atlasPath, UINT columns, UINT rows)
{
    Load(atlasPath, columns, rows);
}

void TileSet::Load(const std::wstring& atlasPath, UINT columns, UINT rows)
{
    path = atlasPath;
    columnCount = columns == 0 ? 1 : columns;
    rowCount = rows == 0 ? 1 : rows;

    // Texture \ucef4\ud3ec\ub10c\ud2b8\uc5d0\uac8c \uc0c8 \uc774\ubbf8\uc9c0\ub97c \ub85c\ub4dc\ud558\uace0 \ud14d\uc140 \ubd84\ud560 \uc815\ubcf4\ub97c \ubc18\uc601\ud55c\ub2e4.
    InitializeTexture();
    UpdateTileSize();
}

Vector2 TileSet::CalculateStartUV(UINT column, UINT row) const noexcept
{
    const float u = tileUVSize.x * static_cast<float>(column);
    const float v = tileUVSize.y * static_cast<float>(row);
    return Vector2(u, v);
}

Vector2 TileSet::CalculateEndUV(UINT column, UINT row) const noexcept
{
    Vector2 start = CalculateStartUV(column, row);
    return start + tileUVSize;
}

ID3D11ShaderResourceView* TileSet::GetShaderResourceView() const noexcept
{
    if (!textureComponent)
    {
        return nullptr;
    }

    return textureComponent->GetShaderResourceView();
}

void TileSet::InitializeTexture()
{
    if (!textureComponent)
    {
        textureComponent = std::make_shared<Texture>(Vector3::Zero, Vector3::One, 0.0f, path);
    }
    else
    {
        textureComponent->SetTexture(path);
    }
}

void TileSet::UpdateTileSize()
{
    tileUVSize = Vector2(1.0f / static_cast<float>(columnCount), 1.0f / static_cast<float>(rowCount));
}

