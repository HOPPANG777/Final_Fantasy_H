#pragma once

#include "Texture.h"

#include <unordered_map>

class SpriteText : public Texture
{
public:
    SpriteText(
        const DirectX::SimpleMath::Vector3& position = Values::ZeroVector3,
        const DirectX::SimpleMath::Vector3& scale = Values::OneVector3,
        float rotation = 0.0f,
        const std::wstring& texturePath = L"",
        bool useRectTransform = true,
        const DirectX::SimpleMath::Vector2& glyphSize = DirectX::SimpleMath::Vector2(8.0f, 8.0f),
        size_t columns = 16,
        wchar_t firstChar = L' ',
        const std::string& componentName = "SpriteText"
    );

    void Update() override;

    void SetText(const std::wstring& text);
    const std::wstring& GetText() const { return _text; }

    void SetSpacing(float horizontal, float vertical = 0.0f);
    void SetGlyphSize(const DirectX::SimpleMath::Vector2& glyphSize);
    void SetFirstCharacter(wchar_t firstChar);
    void SetColumns(size_t columns);
    void SetTextureAtlas(const std::wstring& texturePath);
    void SetGlyphOrder(const std::wstring& glyphOrder);

private:
    void RefreshAtlasInfo();
    void UpdateGeometry();
    bool TryGetGlyphIndex(wchar_t character, size_t& outIndex) const;
    void RebuildGlyphLookup();

private:
    std::wstring _text;
    DirectX::SimpleMath::Vector2 _glyphSize;
    DirectX::SimpleMath::Vector2 _spacing = DirectX::SimpleMath::Vector2(0.0f, 0.0f);

    size_t _requestedColumns = 0;
    size_t _effectiveColumns = 0;
    size_t _rows = 0;
    size_t _glyphCount = 0;
    wchar_t _firstChar = L' ';

    DirectX::SimpleMath::Vector2 _atlasSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);

    std::wstring _glyphOrder;
    std::unordered_map<wchar_t, size_t> _glyphLookup;

    bool _geometryDirty = true;
};
