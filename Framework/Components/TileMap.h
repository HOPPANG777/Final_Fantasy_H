#pragma once

class Tile;
class TileSet;
class MouseManager;
class CameraManager;

// \ud0c0\uc77c\ub9f5\uc740 Tile \uac1d\uccb4\ub97c \uc815\ub82c\ud558\uc5ec \uad6c\uc131\ub41c 2D \uadf8\ub9ac\ub4dc \uc815\ubcf4\uc774\ub2e4.
class TileMap
{
public:
    TileMap(
        UINT width,
        UINT height,
        UINT spacing,
        std::shared_ptr<TileSet> tileSet,
        MouseManager& mouseManager,
        CameraManager& cameraManager,
        bool editMode = false
    );
    ~TileMap() = default;

    void Update();
    void Render();

    void ImGUI();

public:
    void GenerateTileMap();
    void ResizeMap(UINT newWidth, UINT newHeight);

    void Save(const std::string& path);
    void Load(const std::string& path);

    Tile* GetTile(const DirectX::SimpleMath::Vector3& worldPosition);

private:
    void SynchronizeTileSet();
    void ApplyTileSetToAllTiles(const std::shared_ptr<TileSet>& tileSet);
    void UpdateEditing();

private:
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;

    std::shared_ptr<TileSet> tileSet;
    uint64_t cachedTileSetVersion = 0;

    MouseManager& mouseManager;
    CameraManager& cameraManager;

    UINT width = 0;
    UINT height = 0;
    UINT spacing = 0;

    bool editMode = false;
};

