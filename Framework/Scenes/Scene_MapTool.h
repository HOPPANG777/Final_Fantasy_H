#pragma once

#include <array>
#include <memory>
#include <string>

class TileMap;
class TileSet;

class Scene_MapTool : public Scene
{
public:
    void Awake() override;
    void Start() override;

    void Destroy() override;

    void FixedUpdate() override;
    void Update() override;
    void LateUpdate() override;

    void Render() override;
    void PostRender() override;

    void ImGUI() override;

    bool IsLoaded() const override { return isLoaded; }

private:
    void CreateTileMap();
    void SaveCurrentMap(const std::string& fileName);
    void LoadMapFromFile(const std::string& fileName);
    std::string BuildMapPath(const std::string& fileName, const std::string& extension) const;
    void EnsureMapDirectoryExists() const;

private:
    std::unique_ptr<TileMap> tileMap;
    std::shared_ptr<TileSet> tileSet;

    UINT spacing = 64;
    UINT mapWidth = 0;
    UINT mapHeight = 0;

    UINT resizeWidth = 0;
    UINT resizeHeight = 0;

    UINT tileSetColumns = 0;
    UINT tileSetRows = 0;

    std::array<char, 128> mapFileNameBuffer{};
    std::array<char, 260> tileSetRelativePathBuffer{};

    bool isLoaded = false;
};
