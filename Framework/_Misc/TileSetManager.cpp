#include "Framework.h"
#include "TileSetManager.h"

TileSetManager::TileSetManager()
{
    //Init();
}

TileSetManager::~TileSetManager()
{
}

void TileSetManager::Init()
{
    tilePath = TexturePath + L"Tilesets/tile.png";

    const auto& tex = std::make_unique<Texture2D>(tilePath);
    tileSRV = tex->GetSRV();

    tileXCount = 11;
    tileYCount = 7;

    texelTileSize = Vector2(1.0f / static_cast<float>(tileXCount), 1.0f / static_cast<float>(tileYCount));
}

void TileSetManager::ImGUI()
{
    ImGui::Begin("TileSetManager");
    {
        ImGui::BeginChild("TileScrollRegion", ImVec2(700, 500), true, ImGuiWindowFlags_HorizontalScrollbar);

        int count = 0;
        for (UINT y = 0; y < tileYCount; y++)
        {
            for (UINT x = 0; x < tileXCount; x++)
            {
                float startX = texelTileSize.x * x;
                float startY = texelTileSize.y * y;

                ImGui::PushID(count);
                if (ImGui::ImageButton("##TileButton", (ImTextureID)tileSRV.Get(), ImVec2(40, 40),
                    ImVec2(startX, startY), ImVec2(startX + texelTileSize.x, startY + texelTileSize.y)))
                {
                    selectedStartUV = Vector2(startX, startY);
                }
                ImGui::PopID();

                count++;

                if ((count % tileXCount) != 0)
                    ImGui::SameLine();
            }
        }

        ImGui::EndChild();
    }
    ImGui::End();
}

void TileSetManager::ChangeTileMap(const std::wstring& imagePath, UINT xCount, UINT yCount)
{
    tilePath = imagePath;

    const auto& tex = std::make_unique<Texture2D>(tilePath);
    tileSRV = tex->GetSRV();

    tileXCount = xCount;
    tileYCount = yCount;

    texelTileSize = Vector2(1.0f / static_cast<float>(tileXCount), 1.0f / static_cast<float>(tileYCount));
}