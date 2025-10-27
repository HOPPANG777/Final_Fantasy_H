#pragma once

class Texture;

// \ud0c0\uc77c \uc138\ud2b8\ub294 \ud30c\uc77c \ubc30\uce58 \uc815\ubcf4\ub97c \uad00\ub9ac\ud558\uace0 ImGui\ub97c \ud1b5\ud574 \ud0c0\uc77c \uc120\ud0dd \uae30\ub2a5\uc744 \uc81c\uacf5\ud55c\ub2e4.
class TileSet
{
public:
    TileSet();
    TileSet(const std::wstring& atlasPath, UINT columns, UINT rows);
    ~TileSet() = default;

    void ImGUI();

    void Load(const std::wstring& atlasPath, UINT columns, UINT rows);
    void Clear();

    const std::wstring& GetAtlasPath() const noexcept { return path; }
    UINT GetColumns() const noexcept { return columnCount; }
    UINT GetRows() const noexcept { return rowCount; }

    DirectX::SimpleMath::Vector2 GetTileUVSize() const noexcept { return tileUVSize; }

    DirectX::SimpleMath::Vector2 GetSelectedStartUV() const noexcept { return selectedStartUV; }
    DirectX::SimpleMath::Vector2 GetSelectedEndUV() const noexcept { return selectedStartUV + tileUVSize; }

    DirectX::SimpleMath::Vector2 CalculateStartUV(UINT column, UINT row) const noexcept;
    DirectX::SimpleMath::Vector2 CalculateEndUV(UINT column, UINT row) const noexcept;

    void SetSelection(UINT column, UINT row);

    ID3D11ShaderResourceView* GetShaderResourceView() const noexcept;

    bool IsValid() const noexcept { return static_cast<bool>(textureComponent); }

    uint64_t GetVersion() const noexcept { return version; }

private:
    void InitializeTexture();
    void UpdateTileSize();
    void DrawTileSelector();

private:
    std::shared_ptr<Texture> textureComponent;
    std::wstring path;
    UINT columnCount = 1;
    UINT rowCount = 1;
    DirectX::SimpleMath::Vector2 tileUVSize = DirectX::SimpleMath::Vector2::One;
    DirectX::SimpleMath::Vector2 selectedStartUV = DirectX::SimpleMath::Vector2::Zero;
    uint64_t version = 0;
};

