#include "Framework.h"
#include "TileSetManager.h"

#include <algorithm>

#include "TileSet.h"

using namespace DirectX::SimpleMath;

TileSetManager::TileSetManager()
{
    // \uae30\ubcf8 \ud0c0\uc77c\uc138\ud2b8 \ud30c\uc77c\uc744 \ub85c\ub4dc\ud558\uc5ec \uace0\uc815 \ud658\uacbd\uc744 \ucd94\uc815\ud55c\ub2e4.
    const std::wstring defaultPath = TexturePath + L"Tilesets/tile.png";
    LoadTileSet(defaultPath, 11, 7);
    SetSelection(0, 0);
}

void TileSetManager::ImGUI()
{
    ImGui::Begin("TileSetManager");
    {
        if (!HasTileSet())
        {
            ImGui::Text("\ud0c0\uc77c\uc138\ud2b8\uac00 \ub85c\ub4dc\ub418\uc9c0 \uc54a\uc558\uc2b5\ub2c8\ub2e4.");
        }
        else
        {
            ImGui::Text("\ud0c0\uc77c\uc138\ud2b8: %ls", tilePath.c_str());
            ImGui::Separator();
            DrawTileSelector();
        }
    }
    ImGui::End();
}

void TileSetManager::LoadTileSet(const std::wstring& atlasPath, UINT columns, UINT rows)
{
    activeTileSet = std::make_shared<TileSet>(atlasPath, columns, rows);
    tilePath = atlasPath;
    this->columns = activeTileSet->GetColumns();
    this->rows = activeTileSet->GetRows();

    tileUVSize = activeTileSet->GetTileUVSize();
    selectedStartUV = Vector2::Zero;
}

void TileSetManager::ClearTileSet()
{
    activeTileSet.reset();
    tilePath.clear();
    columns = 0;
    rows = 0;
    tileUVSize = Vector2::Zero;
    selectedStartUV = Vector2::Zero;
}

void TileSetManager::SetSelection(UINT column, UINT row)
{
    if (!activeTileSet)
    {
        return;
    }

    column = std::min(column, columns > 0 ? columns - 1 : 0);
    row = std::min(row, rows > 0 ? rows - 1 : 0);

    selectedStartUV = activeTileSet->CalculateStartUV(column, row);
}

void TileSetManager::DrawTileSelector()
{
    if (!activeTileSet)
    {
        return;
    }

    ID3D11ShaderResourceView* srv = activeTileSet->GetShaderResourceView();
    if (!srv)
    {
        ImGui::Text("\uc0ac\uc6a9 \uac00\ub2a5\ud55c \ud0c0\uc77c\uc138\ud2b8 \uc774\ubbf8\uc9c0\uac00 \uc5c6\uc2b5\ub2c8\ub2e4.");
        return;
    }

    // ImGui \ud558\uc774\ub4dc \ubc31\uc744 \uc774\uc6a9\ud558\uc5ec \ud0c0\uc77c \ucf64\ubcf4 \uadf8\ub798\ubc0d \uc720\ud615\uc744 \uad6c\ud604\ud55c\ub2e4.
    ImGui::BeginChild("TileScrollRegion", ImVec2(700, 500), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (UINT rowIndex = 0; rowIndex < rows; ++rowIndex)
    {
        for (UINT columnIndex = 0; columnIndex < columns; ++columnIndex)
        {
            const Vector2 uvStart = activeTileSet->CalculateStartUV(columnIndex, rowIndex);
            const Vector2 uvEnd = activeTileSet->CalculateEndUV(columnIndex, rowIndex);

            ImGui::PushID(static_cast<int>(rowIndex * columns + columnIndex));
            if (ImGui::ImageButton(
                "##TileButton",
                reinterpret_cast<ImTextureID>(srv),
                ImVec2(40.0f, 40.0f),
                ImVec2(uvStart.x, uvStart.y),
                ImVec2(uvEnd.x, uvEnd.y)))
            {
                SetSelection(columnIndex, rowIndex);
            }
            ImGui::PopID();

            if (columnIndex + 1 < columns)
            {
                ImGui::SameLine();
            }
        }
    }

    ImGui::EndChild();
}

