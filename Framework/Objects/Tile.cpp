#include "Framework.h"
#include "Tile.h"

using namespace DirectX::SimpleMath;

Tile::Tile(
    const Vector3& position,
    float tileSize,
    const std::shared_ptr<TileSet>& tileSet,
    UINT tileIndex)
    : GameObject(
        "Tile",
        position + Vector3(tileSize * 0.5f, tileSize * 0.5f, 0.0f),
        Vector3(tileSize, tileSize, 1.0f),
        0.0f,
        false)
    , tileSize(tileSize)
    , index(tileIndex)
{
    // \ud0c0\uc77c \uad6c\uc131\uc744 \uc704\ud55c Texture \ucef4\ud3ec\ub10c\ud2b8\ub97c \uc0dd\uc131\ud55c\ub2e4.
    const Vector3 worldPosition = position + Vector3(tileSize * 0.5f, tileSize * 0.5f, 0.0f);
    textureComponent = std::make_shared<Texture>(worldPosition, Vector3(tileSize, tileSize, 1.0f), 0.0f);
    AddComponent(textureComponent);

    SetTileSet(tileSet);

    // \ucd08\uae30\ud654 \uc2dc \uba48\uce58\ub294 \ucf54\ub108\ub97c \ud30c\uc0dd\ud55c\ub2e4.
    SetColor(Values::Transparent);
}

void Tile::Update()
{
    GameObject::Update();
}

void Tile::Render()
{
    GameObject::Render();
}

void Tile::SetTileSet(const std::shared_ptr<TileSet>& tileSet)
{
    // \ub3d9\uc77c\ud55c SRV\ub97c \uacf5\uc720\ud558\uc5ec \ub2e4\ub978 \ud0c0\uc77c\ub4e4\uacfc \uc77c\uad04\ub418\ub294 \uc5f0\uc0b0\uc774 \uac00\ub2a5\ud558\ub3c4\ub85d \ub9de\ucda4.
    if (textureComponent)
    {
        if (tileSet)
        {
            textureComponent->SetSRV(tileSet->GetShaderResourceView());
        }
        else
        {
            textureComponent->SetSRV(nullptr);
        }
    }
}

void Tile::SetUV(const Vector2& start, const Vector2& end)
{
    startUV = start;
    endUV = end;
    hasUV = true;

    ApplyUVToTexture();
}

void Tile::ClearUV()
{
    startUV = Values::ZeroVector2;
    endUV = Values::ZeroVector2;
    hasUV = false;

    ApplyUVToTexture();
}

Color Tile::GetColor() const
{
    if (textureComponent)
    {
        return textureComponent->GetColor();
    }

    return Values::Transparent;
}

void Tile::SetColor(const Color& color)
{
    if (textureComponent)
    {
        textureComponent->SetColor(color);
    }
}

Vector3 Tile::GetPosition() const
{
    const auto transform = GetTransform();
    if (transform)
    {
        const Vector3 center = transform->GetPosition();
        return center - Vector3(tileSize * 0.5f, tileSize * 0.5f, 0.0f);
    }

    return Values::ZeroVector3;
}

void Tile::SetPosition(const Vector3& position)
{
    const auto transform = GetTransform();
    if (transform)
    {
        transform->SetPosition(position + Vector3(tileSize * 0.5f, tileSize * 0.5f, 0.0f));
    }
}

void Tile::ApplyUVToTexture()
{
    if (!textureComponent)
    {
        return;
    }

    std::vector<VertexTexture> vertices =
    {
        // \uccab \ubc88\uc9f8 \uc815\ub82c: \uc67c\uc544\ub978 \uc544\ub798 -> \uc67c\uc544\ub978 \uc704 -> \uc624\ub978\uc544\ub978 \uc544\ub798 -> \uc624\ub978\uc544\ub978 \uc704
        VertexTexture({ -0.5f, -0.5f, 0.0f }, { startUV.x, endUV.y }),
        VertexTexture({ -0.5f,  0.5f, 0.0f }, { startUV.x, startUV.y }),
        VertexTexture({  0.5f, -0.5f, 0.0f }, { endUV.x,   endUV.y }),
        VertexTexture({  0.5f,  0.5f, 0.0f }, { endUV.x,   startUV.y })
    };

    const std::vector<UINT> indices = { 0, 1, 2, 2, 1, 3 };

    // Texture \ucef4\ud3ec\ub10c\ud2b8\uc5d0\uac8c \uc0c8\ub85c\uc6b4 UV \uc815\ub82c\uc744 \uc801\uc6a9\ud558\uc5ec \uadf8\ub8f9\uc5d0\uc11c \ub2f9\uc2e0\uc774 \uc120\ud0dd\ud55c \ud3ec\uba67\uc744 \uc815\ud655\ud558\uac8c \uad6c\ud604\ud55c\ub2e4.
    textureComponent->SetGeometry(vertices, indices);

    if (hasUV)
    {
        textureComponent->SetColor(Values::White);
    }
    else
    {
        textureComponent->SetColor(Values::Transparent);
    }
}

