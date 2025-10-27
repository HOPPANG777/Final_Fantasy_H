#pragma once

#include <memory>
#include <string>
#include <directxtk/SimpleMath.h>

class TileSet;

// \ud0c0\uc77c\uc138\ud2b8 \uad00\ub9ac\uc790\ub294 \ud604\uc7ac \uc801\uc6a9 \ub41c \ud0c0\uc77c\uc138\ud2b8\ub97c \ubcf4\uad00\ud558\uace0
// ImGui\ub97c \ud1b5\ud574 \uc720\uc120\uc744 \uc120\ud0dd\ud560 \uc218 \uc788\ub294 \ud654\uba74\uc744 \uc81c\uacf5\ud55c\ub2e4.
class TileSetManager
{
public:
    TileSetManager();
    ~TileSetManager() = default;

    void ImGUI();

    void LoadTileSet(const std::wstring& atlasPath, UINT columns, UINT rows);
    void ClearTileSet();

    std::shared_ptr<TileSet> GetActiveTileSet() const noexcept { return activeTileSet; }

    DirectX::SimpleMath::Vector2 GetSelectedStartUV() const noexcept { return selectedStartUV; }
    DirectX::SimpleMath::Vector2 GetSelectedEndUV() const noexcept { return selectedStartUV + tileUVSize; }
    DirectX::SimpleMath::Vector2 GetTileUVSize() const noexcept { return tileUVSize; }

    const std::wstring& GetTilePath() const noexcept { return tilePath; }
    UINT GetColumns() const noexcept { return columns; }
    UINT GetRows() const noexcept { return rows; }

    bool HasTileSet() const noexcept { return static_cast<bool>(activeTileSet); }

    void SetSelection(UINT column, UINT row);

private:
    void DrawTileSelector();

private:
    std::shared_ptr<TileSet> activeTileSet;

    std::wstring tilePath;
    UINT columns = 0;
    UINT rows = 0;

    DirectX::SimpleMath::Vector2 tileUVSize = DirectX::SimpleMath::Vector2::Zero;
    DirectX::SimpleMath::Vector2 selectedStartUV = DirectX::SimpleMath::Vector2::Zero;
};

