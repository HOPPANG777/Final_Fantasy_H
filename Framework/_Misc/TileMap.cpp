#include "Framework.h"
#include "TileMap.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <utility>

#include "Tile.h"
#include "TileSet.h"
#include "TileSetManager.h"
#include "Systems/MouseManager.h"
#include "Systems/CameraManager.h"

using namespace DirectX::SimpleMath;

TileMap::TileMap(
    UINT width,
    UINT height,
    UINT spacing,
    std::shared_ptr<TileSetManager> tileSetManager,
    MouseManager& mouseManager,
    CameraManager& cameraManager,
    bool editMode)
    : tileSetManager(std::move(tileSetManager))
    , mouseManager(mouseManager)
    , cameraManager(cameraManager)
    , width(width)
    , height(height)
    , spacing(spacing)
    , editMode(editMode)
{
    GenerateTileMap();
}

void TileMap::Update()
{
    SynchronizeTileSet();

    for (auto& row : tiles)
    {
        for (auto& tile : row)
        {
            if (tile)
            {
                tile->Update();
            }
        }
    }

    if (editMode)
    {
        UpdateEditing();
    }
}

void TileMap::Render()
{
    for (auto& row : tiles)
    {
        for (auto& tile : row)
        {
            if (tile)
            {
                tile->Render();
            }
        }
    }
}

void TileMap::ImGUI()
{
    if (tileSetManager)
    {
        tileSetManager->ImGUI();
    }
}

void TileMap::GenerateTileMap()
{
    tiles.clear();
    tiles.resize(height);

    auto activeTileSet = tileSetManager ? tileSetManager->GetActiveTileSet() : nullptr;
    cachedTileSet = activeTileSet;

    UINT index = 0;
    for (UINT rowIndex = 0; rowIndex < height; ++rowIndex)
    {
        tiles[rowIndex].resize(width);
        for (UINT columnIndex = 0; columnIndex < width; ++columnIndex)
        {
            const Vector3 position(
                static_cast<float>(columnIndex * spacing),
                static_cast<float>(rowIndex * spacing),
                0.0f);

            // \uac01 \uce35\uc758 \ud0c0\uc77c\uc740 GameObject \uae30\ubc18\uc73c\ub85c \uc0dd\uc131\ud558\uc5ec \ud3ec\ud2b8 \uc7ac\uc0dd\uc131\uc744 \uc81c\uac70\ud558\uba70 \uadf8\ub798\ud53d \ucef4\ud3ec\ub10c\ud2b8\ub97c \uc9c0\uc6d0\ud55c\ub2e4.
            auto tile = std::make_shared<Tile>(position, static_cast<float>(spacing), activeTileSet, index++);
            tiles[rowIndex][columnIndex] = tile;
        }
    }
}

void TileMap::ResizeMap(UINT newWidth, UINT newHeight)
{
    std::vector<std::vector<std::shared_ptr<Tile>>> newTiles(newHeight);
    auto activeTileSet = tileSetManager ? tileSetManager->GetActiveTileSet() : nullptr;

    UINT index = 0;
    for (UINT rowIndex = 0; rowIndex < newHeight; ++rowIndex)
    {
        newTiles[rowIndex].resize(newWidth);
        for (UINT columnIndex = 0; columnIndex < newWidth; ++columnIndex)
        {
            std::shared_ptr<Tile> tile;
            if (rowIndex < height && columnIndex < width)
            {
                tile = tiles[rowIndex][columnIndex];
            }

            if (!tile)
            {
                const Vector3 position(
                    static_cast<float>(columnIndex * spacing),
                    static_cast<float>(rowIndex * spacing),
                    0.0f);

                // \uc0c8\ub85c\uc6b4 \ud06c\uae30\uc5d0 \ub9de\uac8c \ubd88\uc694\ud55c \ud0c0\uc77c\uc744 \ucd94\uac00\ud558\uba70 \uae30\uc874 \ub370\uc774\ud130\ub97c \uacf5\uc720\ud55c\ub2e4.
                tile = std::make_shared<Tile>(position, static_cast<float>(spacing), activeTileSet, index);
            }

            tile->SetIndex(index);
            tile->SetPosition(Vector3(static_cast<float>(columnIndex * spacing), static_cast<float>(rowIndex * spacing), 0.0f));
            tile->SetTileSet(activeTileSet);

            newTiles[rowIndex][columnIndex] = tile;
            ++index;
        }
    }

    tiles = std::move(newTiles);
    width = newWidth;
    height = newHeight;

    cachedTileSet = tileSetManager ? tileSetManager->GetActiveTileSet() : nullptr;
}

void TileMap::Save(const std::string& path)
{
    std::ofstream fout(path);
    if (!fout.is_open())
    {
        std::cout << "\ud30c\uc77c\uc744 \uc5f4 \uc218 \uc5c6\uc2b5\ub2c8\ub2e4." << std::endl;
        return;
    }

    // \ud14d\uc2a4\ucc98\uc640 \ud0c0\uc77c \uc815\ubcf4, \uae0d\ubc14\uaf2c UV \ub370\uc774\ud130\ub97c \ud30c\uc77c\ub85c \uc800\uc7a5\ud574 \ub2e4\uc74c \ubc84\c804\uacfc \uacf5\uc720\ud558\ub3c4\ub85d \ud55c\ub2e4.
    fout << width << ' ' << height << ' ' << spacing << '\n';

    std::shared_ptr<TileSet> tileSet = tileSetManager ? tileSetManager->GetActiveTileSet() : nullptr;
    if (tileSet)
    {
        std::string imagePath(tileSet->GetAtlasPath().begin(), tileSet->GetAtlasPath().end());
        fout << imagePath << ' ' << tileSet->GetColumns() << ' ' << tileSet->GetRows() << '\n';
    }
    else
    {
        fout << "none" << ' ' << 0 << ' ' << 0 << '\n';
    }

    for (UINT rowIndex = 0; rowIndex < height; ++rowIndex)
    {
        for (UINT columnIndex = 0; columnIndex < width; ++columnIndex)
        {
            const std::shared_ptr<Tile>& tile = tiles[rowIndex][columnIndex];
            const bool isActive = tile && tile->HasTexture();

            fout << (isActive ? 1 : 0) << ' ';

            const Vector2 start = tile ? tile->GetStartUV() : Vector2::Zero;
            const Vector2 end = tile ? tile->GetEndUV() : Vector2::Zero;

            fout << start.x << ' ' << start.y << ' ' << end.x << ' ' << end.y << '\n';
        }
    }
}

void TileMap::Load(const std::string& path)
{
    std::ifstream fin(path);
    if (!fin.is_open())
    {
        std::cout << "\ud30c\uc77c\uc744 \uc77d\uc744 \uc218 \uc5c6\uc2b5\ub2c8\ub2e4. \ucd94\uac00\uc815\ubcf4\ub97c \ud655\uc778\ud558\uc138\uc694." << std::endl;
        return;
    }

    // \uc800\uc7a5\ub41c \ud30c\uc77c\uc5d0\uc11c \ud0c0\uc77c \ub9f5 \uc815\ubcf4\ub97c \ud30c\uc2f1\ud558\uc5ec \uc7ac\uad6c\uc131\ud55c\ub2e4.
    fin >> width >> height >> spacing;

    std::string imagePath;
    UINT columns = 0;
    UINT rows = 0;
    fin >> imagePath >> columns >> rows;

    if (tileSetManager)
    {
        if (imagePath != "none")
        {
            std::wstring atlasPath(imagePath.begin(), imagePath.end());
            tileSetManager->LoadTileSet(atlasPath, columns, rows);
        }
        else
        {
            tileSetManager->ClearTileSet();
        }
    }

    GenerateTileMap();

    for (UINT rowIndex = 0; rowIndex < height; ++rowIndex)
    {
        for (UINT columnIndex = 0; columnIndex < width; ++columnIndex)
        {
            int activeFlag = 0;
            float sx = 0.0f;
            float sy = 0.0f;
            float ex = 0.0f;
            float ey = 0.0f;

            fin >> activeFlag >> sx >> sy >> ex >> ey;

            const std::shared_ptr<Tile>& tile = tiles[rowIndex][columnIndex];
            if (!tile)
            {
                continue;
            }

            if (activeFlag == 1)
            {
                tile->SetUV(Vector2(sx, sy), Vector2(ex, ey));
            }
            else
            {
                tile->ClearUV();
            }
        }
    }
}

Tile* TileMap::GetTile(const Vector3& worldPosition)
{
    if (spacing == 0)
    {
        return nullptr;
    }

    const int xIndex = static_cast<int>(std::floor(worldPosition.x / static_cast<float>(spacing)));
    const int yIndex = static_cast<int>(std::floor(worldPosition.y / static_cast<float>(spacing)));

    if (xIndex < 0 || yIndex < 0)
    {
        return nullptr;
    }

    if (xIndex >= static_cast<int>(width) || yIndex >= static_cast<int>(height))
    {
        return nullptr;
    }

    const std::shared_ptr<Tile>& tile = tiles[yIndex][xIndex];
    return tile ? tile.get() : nullptr;
}

void TileMap::SynchronizeTileSet()
{
    if (!tileSetManager)
    {
        return;
    }

    std::shared_ptr<TileSet> activeTileSet = tileSetManager->GetActiveTileSet();
    std::shared_ptr<TileSet> cached = cachedTileSet.lock();

    if (activeTileSet != cached)
    {
        // \ucd5c\uc2e0 \ud0c0\uc77c\uc138\ud2b8 \uc815\ubcf4\uc640 \ub9e4\ud551\ub418\ub294 \ubaa8\ub4e0 \ud0c0\uc77c \ub370\uc774\ud130\ub97c \ud558\ub098\uc758 \uc5f0\uc0b0\uc73c\ub85c \ub2e4\uc2dc \uac31\uc2e0\ud55c\ub2e4.
        ApplyTileSetToAllTiles(activeTileSet);
        cachedTileSet = activeTileSet;
    }
}

void TileMap::ApplyTileSetToAllTiles(const std::shared_ptr<TileSet>& tileSet)
{
    for (auto& row : tiles)
    {
        for (auto& tile : row)
        {
            if (tile)
            {
                tile->SetTileSet(tileSet);
            }
        }
    }
}

void TileMap::UpdateEditing()
{
    if (!tileSetManager)
    {
        return;
    }

    const std::shared_ptr<TileSet> tileSet = tileSetManager->GetActiveTileSet();
    if (!tileSet)
    {
        return;
    }

    Vector3 mousePosition = mouseManager.GetMousePos();
    mousePosition += cameraManager.GetPosition();

    Tile* tile = GetTile(mousePosition);
    if (!tile)
    {
        return;
    }

    if (mouseManager.Press(0))
    {
        // \uba38\uc2a4 \ud074\ub9ad\uc744 \ud1b5\ud574 \uc120\ud0dd\ub41c UV\ub97c \uc2a4\ud06c\ub9bd\ud2b8\uc5d0 \uae30\ubc18\ud558\uc5ec \ud0c0\uc77c\uc5d0 \uc801\uc6a9\ud55c\ub2e4.
        const Vector2 startUV = tileSetManager->GetSelectedStartUV();
        const Vector2 endUV = tileSetManager->GetSelectedEndUV();

        tile->SetTileSet(tileSet);
        tile->SetUV(startUV, endUV);
    }
}

