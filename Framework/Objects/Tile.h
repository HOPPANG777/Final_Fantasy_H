#pragma once

class Texture;
class TileSet;

// \ud0c0\uc77c\uc740 GameObject\ub97c \uc0c1\uc18d\ud558\uc5ec Texture \ucef4\ud3ec\ub10c\ud2b8\ub85c \ad11\ud589\ud55c\ub2e4.
class Tile : public GameObject
{
public:
    Tile(
        const DirectX::SimpleMath::Vector3& position,
        float tileSize,
        const std::shared_ptr<TileSet>& tileSet,
        UINT tileIndex
    );
    ~Tile() override = default;

    void Update() override;
    void Render() override;

public:
    void SetTileSet(const std::shared_ptr<TileSet>& tileSet);

    void SetUV(const DirectX::SimpleMath::Vector2& start, const DirectX::SimpleMath::Vector2& end);
    void ClearUV();

    DirectX::SimpleMath::Vector2 GetStartUV() const noexcept { return startUV; }
    DirectX::SimpleMath::Vector2 GetEndUV() const noexcept { return endUV; }

    DirectX::SimpleMath::Color GetColor() const;
    void SetColor(const DirectX::SimpleMath::Color& color);

    DirectX::SimpleMath::Vector3 GetPosition() const;
    void SetPosition(const DirectX::SimpleMath::Vector3& position);

    UINT GetIndex() const noexcept { return index; }
    void SetIndex(UINT newIndex) noexcept { index = newIndex; }

    float GetTileSize() const noexcept { return tileSize; }

    bool HasTexture() const noexcept { return hasUV; }

private:
    void ApplyUVToTexture();

private:
    std::shared_ptr<Texture> textureComponent;

    DirectX::SimpleMath::Vector2 startUV = DirectX::SimpleMath::Vector2::Zero;
    DirectX::SimpleMath::Vector2 endUV = DirectX::SimpleMath::Vector2::Zero;

    float tileSize = 0.0f;
    UINT index = 0;
    bool hasUV = false;
};

