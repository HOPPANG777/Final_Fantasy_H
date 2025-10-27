#include "Framework.h"
#include "TileSet.h"

#include <algorithm>

#include "Components/Texture.h"

using namespace DirectX::SimpleMath;

TileSet::TileSet()
{
    const std::wstring defaultPath = TexturePath + L"Tilesets/tile.png";
    Load(defaultPath, 11, 7);
}

TileSet::TileSet(const std::wstring& atlasPath, UINT columns, UINT rows)
{
    Load(atlasPath, columns, rows);
}

void TileSet::ImGUI()
{
    ImGui::Begin("TileSet");
    {
        if (!IsValid())
        {
            ImGui::Text("\ud0c0\uc77c\uc138\ud2b8\uac00 \ub85c\ub4dc\ub418\uc9c0 \uc54a\uc558\uc2b5\ub2c8\ub2e4.");
        }
        else
        {
            ImGui::Text("\ud0c0\uc77c\uc138\ud2b8: %ls", path.c_str());
            ImGui::Separator();
            DrawTileSelector();
        }
    }
    ImGui::End();
}

void TileSet::Load(const std::wstring& atlasPath, UINT columns, UINT rows)
{
    path = atlasPath;
    columnCount = columns == 0 ? 1 : columns;
    rowCount = rows == 0 ? 1 : rows;

    InitializeTexture();
    UpdateTileSize();
    SetSelection(0, 0);
    ++version;
}

void TileSet::Clear()
{
    textureComponent.reset();
    path.clear();
    columnCount = 0;
    rowCount = 0;
    tileUVSize = Vector2::Zero;
    selectedStartUV = Vector2::Zero;
    ++version;
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

void TileSet::SetSelection(UINT column, UINT row)
{
    if (!IsValid())
    {
        return;
    }

    column = std::min(column, columnCount > 0 ? columnCount - 1 : 0);
    row = std::min(row, rowCount > 0 ? rowCount - 1 : 0);

    selectedStartUV = CalculateStartUV(column, row);
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

void TileSet::DrawTileSelector()
{
    if (!IsValid())
    {
        return;
    }

    ID3D11ShaderResourceView* srv = GetShaderResourceView();
    if (!srv)
    {
        ImGui::Text("\uc0ac\uc6a9 \uac00\ub2a5\ud55c \ud0c0\uc77c\uc138\ud2b8 \uc774\ubbf8\uc9c0\uac00 \uc5c6\uc2b5\ub2c8\ub2e4.");
        return;
    }

    ImGui::BeginChild("TileScrollRegion", ImVec2(700, 500), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (UINT rowIndex = 0; rowIndex < rowCount; ++rowIndex)
    {
        for (UINT columnIndex = 0; columnIndex < columnCount; ++columnIndex)
        {
            const Vector2 uvStart = CalculateStartUV(columnIndex, rowIndex);
            const Vector2 uvEnd = CalculateEndUV(columnIndex, rowIndex);

            ImGui::PushID(static_cast<int>(rowIndex * columnCount + columnIndex));
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

            if (columnIndex + 1 < columnCount)
            {
                ImGui::SameLine();
            }
        }
    }

    ImGui::EndChild();
}

