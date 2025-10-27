#include "Framework.h"
#include "TileMap.h"

using namespace DirectX::SimpleMath;

TileMap::TileMap(
    UINT width,
    UINT height,
    UINT spacing,
    std::shared_ptr<TileSet> tileSet,
    MouseManager& mouseManager,
    CameraManager& cameraManager,
    bool editMode)
    : tileSet(tileSet ? std::move(tileSet) : std::make_shared<TileSet>())
    , mouseManager(mouseManager)
    , cameraManager(cameraManager)
    , width(width)
    , height(height)
    , spacing(spacing)
    , editMode(editMode)
{
    cachedTileSetVersion = this->tileSet ? this->tileSet->GetVersion() : 0;
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
    if (tileSet)
    {
        tileSet->ImGUI();
    }
}

void TileMap::GenerateTileMap()
{
    tiles.clear();
    tiles.resize(height);

    auto activeTileSet = tileSet;
    cachedTileSetVersion = activeTileSet ? activeTileSet->GetVersion() : 0;

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

            auto tile = std::make_shared<Tile>(position, static_cast<float>(spacing), activeTileSet, index++);
            tiles[rowIndex][columnIndex] = tile;
        }
    }
}

void TileMap::ResizeMap(UINT newWidth, UINT newHeight)
{
    std::vector<std::vector<std::shared_ptr<Tile>>> newTiles(newHeight);
    auto activeTileSet = tileSet;

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

    cachedTileSetVersion = activeTileSet ? activeTileSet->GetVersion() : 0;
}

void TileMap::Save(const std::string& path)
{
    std::ofstream fout(path);
    if (!fout.is_open())
    {
        std::cout << "\ud30c\uc77c\uc744 \uc5f4 \uc218 \uc5c6\uc2b5\ub2c8\ub2e4." << std::endl;
        return;
    }

    fout << width << ' ' << height << ' ' << spacing << '\n';

    std::shared_ptr<TileSet> activeTileSet = tileSet;
    if (activeTileSet && activeTileSet->IsValid())
    {
        std::string imagePath(activeTileSet->GetAtlasPath().begin(), activeTileSet->GetAtlasPath().end());
        fout << imagePath << ' ' << activeTileSet->GetColumns() << ' ' << activeTileSet->GetRows() << '\n';
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

            const Vector2 start = tile ? tile->GetStartUV() : Values::ZeroVector2;
            const Vector2 end = tile ? tile->GetEndUV() : Values::ZeroVector2;

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

    fin >> width >> height >> spacing;

    std::string imagePath;
    UINT columns = 0;
    UINT rows = 0;
    fin >> imagePath >> columns >> rows;

    if (tileSet)
    {
        if (imagePath != "none")
        {
            std::wstring atlasPath(imagePath.begin(), imagePath.end());
            tileSet->Load(atlasPath, columns, rows);
        }
        else
        {
            tileSet->Clear();
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
    if (!tileSet)
    {
        return;
    }

    const uint64_t currentVersion = tileSet->GetVersion();

    if (currentVersion != cachedTileSetVersion)
    {
        ApplyTileSetToAllTiles(tileSet);
        cachedTileSetVersion = currentVersion;
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
    if (!tileSet || !tileSet->IsValid())
    {
        return;
    }

    const std::shared_ptr<TileSet> activeTileSet = tileSet;
    Vector3 mousePosition = mouseManager.GetMousePos();
    mousePosition += cameraManager.GetPosition();

    Tile* tile = GetTile(mousePosition);
    if (!tile)
    {
        return;
    }

    if (mouseManager.Press(0))
    {
        const Vector2 startUV = activeTileSet->GetSelectedStartUV();
        const Vector2 endUV = activeTileSet->GetSelectedEndUV();

        tile->SetTileSet(activeTileSet);
        tile->SetUV(startUV, endUV);
    }
}

