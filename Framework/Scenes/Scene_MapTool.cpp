#include "Framework.h"
#include "Scene_MapTool.h"

#include <algorithm>

namespace
{
    constexpr const char* kMapDirectory = "Contents/_Maps/";

    void CopyStringToBuffer(const std::string& source, std::array<char, 260>& destination)
    {
        std::fill(destination.begin(), destination.end(), '\0');
        const size_t copyLength = std::min(source.size(), destination.size() - 1);
        std::copy_n(source.begin(), copyLength, destination.begin());
    }

    void CopyStringToBuffer(const std::string& source, std::array<char, 128>& destination)
    {
        std::fill(destination.begin(), destination.end(), '\0');
        const size_t copyLength = std::min(source.size(), destination.size() - 1);
        std::copy_n(source.begin(), copyLength, destination.begin());
    }
}

void Scene_MapTool::Awake()
{
    isLoaded = false;

    tileSet = std::make_shared<TileSet>();

    spacing = 64;
    mapWidth = std::max<UINT>(1, (Values::GetWinWidth() * 2) / spacing);
    mapHeight = std::max<UINT>(1, (Values::GetWinHeight() * 2) / spacing);

    resizeWidth = mapWidth;
    resizeHeight = mapHeight;

    tileSetColumns = tileSet ? tileSet->GetColumns() : 0;
    tileSetRows = tileSet ? tileSet->GetRows() : 0;

    CopyStringToBuffer("map01", mapFileNameBuffer);
    CopyStringToBuffer("Tilesets/tile.png", tileSetRelativePathBuffer);

    CreateTileMap();

    CameraManager::Get()->Reset();

    isLoaded = true;
}

void Scene_MapTool::Start()
{
}

void Scene_MapTool::Destroy()
{
    tileMap.reset();
    tileSet.reset();
}

void Scene_MapTool::FixedUpdate()
{
}

void Scene_MapTool::Update()
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    if (tileMap)
    {
        tileMap->Update();
    }
}

void Scene_MapTool::LateUpdate()
{
}

void Scene_MapTool::Render()
{
    if (tileMap)
    {
        tileMap->Render();
    }
}

void Scene_MapTool::PostRender()
{
}

void Scene_MapTool::ImGUI()
{
    ImGui::Begin("TileMap Tool");
    {
        int spacingInput = static_cast<int>(spacing);
        if (ImGui::InputInt("Spacing", &spacingInput))
        {
            spacing = spacingInput <= 0 ? spacing : static_cast<UINT>(spacingInput);
        }
        ImGui::Text("Suggested Width: %u", (Values::GetWinWidth() * 2) / std::max<UINT>(spacing, 1));
        ImGui::Text("Suggested Height: %u", (Values::GetWinHeight() * 2) / std::max<UINT>(spacing, 1));

        ImGui::Text("Current Map Size: %ux%u", mapWidth, mapHeight);

        int widthInput = static_cast<int>(resizeWidth);
        if (ImGui::InputInt("New Map Width", &widthInput))
        {
            resizeWidth = widthInput <= 0 ? resizeWidth : static_cast<UINT>(widthInput);
        }

        int heightInput = static_cast<int>(resizeHeight);
        if (ImGui::InputInt("New Map Height", &heightInput))
        {
            resizeHeight = heightInput <= 0 ? resizeHeight : static_cast<UINT>(heightInput);
        }

        ImGui::InputText("Map File Name", mapFileNameBuffer.data(), mapFileNameBuffer.size());

        if (ImGui::Button("Create New Map"))
        {
            CreateTileMap();
        }
        ImGui::SameLine();
        if (ImGui::Button("Resize Map") && tileMap)
        {
            tileMap->ResizeMap(resizeWidth, resizeHeight);
            mapWidth = tileMap->GetWidth();
            mapHeight = tileMap->GetHeight();
        }

        if (ImGui::Button("Save Map") && tileMap)
        {
            SaveCurrentMap(std::string(mapFileNameBuffer.data()));
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Map"))
        {
            LoadMapFromFile(std::string(mapFileNameBuffer.data()));
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Tileset Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputText("Tileset Relative Path", tileSetRelativePathBuffer.data(), tileSetRelativePathBuffer.size());

            int tileSetColumnsInput = static_cast<int>(tileSetColumns);
            if (ImGui::InputInt("Tileset Columns", &tileSetColumnsInput))
            {
                tileSetColumns = tileSetColumnsInput <= 0 ? tileSetColumns : static_cast<UINT>(tileSetColumnsInput);
            }

            int tileSetRowsInput = static_cast<int>(tileSetRows);
            if (ImGui::InputInt("Tileset Rows", &tileSetRowsInput))
            {
                tileSetRows = tileSetRowsInput <= 0 ? tileSetRows : static_cast<UINT>(tileSetRowsInput);
            }

            if (ImGui::Button("Load Tileset") && tileSet)
            {
                std::string relativePath(tileSetRelativePathBuffer.data());
                std::wstring tilesetPath = TexturePath + String::ToWstring(relativePath);
                tileSet->Load(tilesetPath, tileSetColumns, tileSetRows);
                tileSetColumns = tileSet->GetColumns();
                tileSetRows = tileSet->GetRows();
            }

            if (tileSet)
            {
                tileSet->ImGUI();
            }
        }

        if (tileMap)
        {
            tileMap->ImGUI();
        }
    }
    ImGui::End();
}

void Scene_MapTool::CreateTileMap()
{
    const UINT validSpacing = std::max<UINT>(spacing, 1);
    const UINT validWidth = std::max<UINT>(resizeWidth > 0 ? resizeWidth : mapWidth, 1);
    const UINT validHeight = std::max<UINT>(resizeHeight > 0 ? resizeHeight : mapHeight, 1);

    tileMap = std::make_unique<TileMap>(
        validWidth,
        validHeight,
        validSpacing,
        tileSet,
        *MouseManager::Get(),
        *CameraManager::Get(),
        true);

    spacing = tileMap->GetSpacing();
    mapWidth = tileMap->GetWidth();
    mapHeight = tileMap->GetHeight();

    resizeWidth = mapWidth;
    resizeHeight = mapHeight;

    if (tileSet)
    {
        tileSetColumns = tileSet->GetColumns();
        tileSetRows = tileSet->GetRows();
    }
}

void Scene_MapTool::SaveCurrentMap(const std::string& fileName)
{
    if (!tileMap || fileName.empty())
    {
        return;
    }

    EnsureMapDirectoryExists();

    const std::string dataPath = BuildMapPath(fileName, ".data");
    const std::string textPath = BuildMapPath(fileName, ".txt");

    tileMap->Save(dataPath);
    tileMap->Save(textPath);
}

void Scene_MapTool::LoadMapFromFile(const std::string& fileName)
{
    if (fileName.empty())
    {
        return;
    }

    EnsureMapDirectoryExists();

    const std::string dataPath = BuildMapPath(fileName, ".data");

    if (!tileMap)
    {
        CreateTileMap();
    }

    tileMap->Load(dataPath);

    spacing = tileMap->GetSpacing();
    mapWidth = tileMap->GetWidth();
    mapHeight = tileMap->GetHeight();

    resizeWidth = mapWidth;
    resizeHeight = mapHeight;

    if (tileSet)
    {
        tileSetColumns = tileSet->GetColumns();
        tileSetRows = tileSet->GetRows();
    }
}

std::string Scene_MapTool::BuildMapPath(const std::string& fileName, const std::string& extension) const
{
    std::string finalName = fileName;
    if (Path::GetExtension(fileName).empty())
    {
        finalName += extension;
    }
    else if (!extension.empty())
    {
        finalName = Path::GetFileNameWithoutExtension(fileName) + extension;
    }

    return std::string(kMapDirectory) + finalName;
}

void Scene_MapTool::EnsureMapDirectoryExists() const
{
    Path::CreateFolders(String::ToWstring(kMapDirectory));
}
