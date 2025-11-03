#include "Framework.h"
#include "SpriteText.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace
{
    constexpr wchar_t kCarriageReturn = L'\r';
}

SpriteText::SpriteText(
    const Vector3& position,
    const Vector3& scale,
    float rotation,
    const std::wstring& texturePath,
    bool useRectTransform,
    const Vector2& glyphSize,
    size_t columns,
    wchar_t firstChar,
    const std::string& componentName)
    : Texture(position, scale, rotation, texturePath, useRectTransform, componentName)
    , _glyphSize(glyphSize)
    , _requestedColumns(columns)
    , _firstChar(firstChar)
{
    RefreshAtlasInfo();
    UpdateGeometry();
}

void SpriteText::Update()
{
    if (_geometryDirty)
    {
        UpdateGeometry();
    }

    Texture::Update();
}

void SpriteText::SetText(const std::wstring& text)
{
    if (_text == text)
    {
        return;
    }

    _text = text;
    _geometryDirty = true;
}

void SpriteText::SetSpacing(float horizontal, float vertical)
{
    Vector2 newSpacing(horizontal, vertical);
    if (newSpacing == _spacing)
    {
        return;
    }

    _spacing = newSpacing;
    _geometryDirty = true;
}

void SpriteText::SetGlyphSize(const Vector2& glyphSize)
{
    if (glyphSize == _glyphSize)
    {
        return;
    }

    _glyphSize = glyphSize;
    RefreshAtlasInfo();
}

void SpriteText::SetFirstCharacter(wchar_t firstChar)
{
    if (_firstChar == firstChar)
    {
        return;
    }

    _firstChar = firstChar;
    _geometryDirty = true;
}

void SpriteText::SetColumns(size_t columns)
{
    if (_requestedColumns == columns)
    {
        return;
    }

    _requestedColumns = columns;
    RefreshAtlasInfo();
}

void SpriteText::SetTextureAtlas(const std::wstring& texturePath)
{
    SetTexture(texturePath);
    RefreshAtlasInfo();
}

void SpriteText::SetGlyphOrder(const std::wstring& glyphOrder)
{
    if (_glyphOrder == glyphOrder)
    {
        return;
    }

    _glyphOrder = glyphOrder;
    RebuildGlyphLookup();
    RefreshAtlasInfo();
}

void SpriteText::RefreshAtlasInfo()
{
    _atlasSize = GetTextureSize();

    const bool hasValidGlyphSize = (_glyphSize.x > 0.0f && _glyphSize.y > 0.0f);
    const bool hasAtlas = (_atlasSize.x > 0.0f && _atlasSize.y > 0.0f && hasValidGlyphSize);

    size_t columnsFromTexture = 0;
    if (hasAtlas)
    {
        columnsFromTexture = static_cast<size_t>(_atlasSize.x / _glyphSize.x);
    }

    if (_requestedColumns == 0)
    {
        _effectiveColumns = columnsFromTexture;
    }
    else if (columnsFromTexture > 0)
    {
        _effectiveColumns = min(_requestedColumns, columnsFromTexture);
    }
    else
    {
        _effectiveColumns = _requestedColumns;
    }

    if (_effectiveColumns == 0 && !_glyphOrder.empty())
    {
        _effectiveColumns = std::max<size_t>(1, min(_glyphOrder.size(), (_requestedColumns == 0) ? _glyphOrder.size() : _requestedColumns));
    }

    if (hasAtlas && _effectiveColumns > 0)
    {
        _rows = std::max<size_t>(1, static_cast<size_t>(_atlasSize.y / _glyphSize.y));
        _glyphCount = _rows * _effectiveColumns;
    }
    else
    {
        _rows = 0;
        _glyphCount = 0;
    }

    if (!_glyphOrder.empty())
    {
        _glyphCount = max(_glyphCount, static_cast<size_t>(_glyphOrder.size()));
        if (_effectiveColumns == 0)
        {
            _effectiveColumns = std::max<size_t>(1, _glyphCount);
        }
        if (_rows == 0 && _effectiveColumns > 0)
        {
            _rows = (_glyphCount + _effectiveColumns - 1) / _effectiveColumns;
        }
    }

    _geometryDirty = true;
}

void SpriteText::UpdateGeometry()
{
    _geometryDirty = false;

    if (_glyphCount == 0 || _glyphSize.x <= 0.0f || _glyphSize.y <= 0.0f)
    {
        SetGeometry({}, {});

        if (GameObject* owner = GetOwner())
        {
            if (const auto& rectTransform = owner->GetRectTransform())
            {
                rectTransform->SetScale(Values::ZeroVector3);
                rectTransform->SetSize(Values::ZeroVector2);
            }
            else if (const auto& transform = owner->GetTransform())
            {
                transform->SetScale(Values::ZeroVector3);
            }
        }
        return;
    }

    std::vector<size_t> lineCharacterCounts;
    lineCharacterCounts.reserve(8);

    size_t currentLineCount = 0;
    bool hasAnyCharacter = false;

    for (wchar_t character : _text)
    {
        if (character == kCarriageReturn)
        {
            continue;
        }

        if (character == L'\n')
        {
            lineCharacterCounts.push_back(currentLineCount);
            currentLineCount = 0;
            continue;
        }

        ++currentLineCount;
        hasAnyCharacter = true;
    }

    if (hasAnyCharacter || (!_text.empty() && _text.back() == L'\n'))
    {
        lineCharacterCounts.push_back(currentLineCount);
    }

    if (lineCharacterCounts.empty())
    {
        SetGeometry({}, {});

        if (GameObject* owner = GetOwner())
        {
            if (const auto& rectTransform = owner->GetRectTransform())
            {
                rectTransform->SetScale(Values::ZeroVector3);
                rectTransform->SetSize(Values::ZeroVector2);
            }
            else if (const auto& transform = owner->GetTransform())
            {
                transform->SetScale(Values::ZeroVector3);
            }
        }

        return;
    }

    const size_t lineCount = lineCharacterCounts.size();

    float totalWidth = 0.0f;
    for (size_t count : lineCharacterCounts)
    {
        if (count == 0)
        {
            continue;
        }

        const float width = static_cast<float>(count) * _glyphSize.x + static_cast<float>(count - 1) * _spacing.x;
        totalWidth = max(totalWidth, width);
    }

    if (totalWidth <= 0.0f)
    {
        totalWidth = _glyphSize.x;
    }

    const float totalHeight = static_cast<float>(lineCount) * _glyphSize.y + static_cast<float>(lineCount - 1) * _spacing.y;
    if (totalHeight <= 0.0f)
    {
        SetGeometry({}, {});

        if (GameObject* owner = GetOwner())
        {
            if (const auto& rectTransform = owner->GetRectTransform())
            {
                rectTransform->SetScale(Values::ZeroVector3);
                rectTransform->SetSize(Values::ZeroVector2);
            }
            else if (const auto& transform = owner->GetTransform())
            {
                transform->SetScale(Values::ZeroVector3);
            }
        }

        return;
    }

    const float safeWidth = max(totalWidth, 1.0f);
    const float safeHeight = max(totalHeight, 1.0f);

    std::vector<VertexTexture> vertices;
    std::vector<UINT> indices;
    vertices.reserve(_text.size() * 4);
    indices.reserve(_text.size() * 6);

    size_t lineIndex = 0;
    size_t characterIndexInLine = 0;
    UINT baseVertexIndex = 0;

    for (wchar_t character : _text)
    {
        if (character == kCarriageReturn)
        {
            continue;
        }

        if (character == L'\n')
        {
            ++lineIndex;
            characterIndexInLine = 0;
            continue;
        }

        const float baseX = static_cast<float>(characterIndexInLine) * (_glyphSize.x + _spacing.x);
        const float x0 = (baseX / safeWidth) - 0.5f;
        const float x1 = ((baseX + _glyphSize.x) / safeWidth) - 0.5f;

        const float baseY = static_cast<float>(lineIndex) * (_glyphSize.y + _spacing.y);
        const float y0 = 0.5f - baseY / safeHeight;
        const float y1 = 0.5f - (baseY + _glyphSize.y) / safeHeight;

        size_t glyphIndex = 0;
        const bool hasGlyph = TryGetGlyphIndex(character, glyphIndex);

        if (hasGlyph && _atlasSize.x > 0.0f && _atlasSize.y > 0.0f && _effectiveColumns > 0)
        {
            const size_t column = glyphIndex % _effectiveColumns;
            const size_t row = glyphIndex / _effectiveColumns;

            const float u0 = (static_cast<float>(column) * _glyphSize.x) / _atlasSize.x;
            const float v0 = (static_cast<float>(row) * _glyphSize.y) / _atlasSize.y;
            const float u1 = (static_cast<float>(column + 1) * _glyphSize.x) / _atlasSize.x;
            const float v1 = (static_cast<float>(row + 1) * _glyphSize.y) / _atlasSize.y;

            vertices.emplace_back(Vector3(x0, y0, 0.0f), Vector2(u0, v0));
            vertices.emplace_back(Vector3(x0, y1, 0.0f), Vector2(u0, v1));
            vertices.emplace_back(Vector3(x1, y0, 0.0f), Vector2(u1, v0));
            vertices.emplace_back(Vector3(x1, y1, 0.0f), Vector2(u1, v1));

            indices.push_back(baseVertexIndex + 0);
            indices.push_back(baseVertexIndex + 1);
            indices.push_back(baseVertexIndex + 2);
            indices.push_back(baseVertexIndex + 2);
            indices.push_back(baseVertexIndex + 1);
            indices.push_back(baseVertexIndex + 3);

            baseVertexIndex += 4;
        }

        ++characterIndexInLine;
    }

    SetGeometry(vertices, indices);

    if (GameObject* owner = GetOwner())
    {
        const Vector3 finalScale(totalWidth, totalHeight, 1.0f);
        const Vector2 finalSize(totalWidth, totalHeight);

        if (const auto& rectTransform = owner->GetRectTransform())
        {
            rectTransform->SetScale(finalScale);
            rectTransform->SetSize(finalSize);
        }
        else if (const auto& transform = owner->GetTransform())
        {
            transform->SetScale(finalScale);
        }
    }
}

bool SpriteText::TryGetGlyphIndex(wchar_t character, size_t& outIndex) const
{
    if (!_glyphLookup.empty())
    {
        const auto it = _glyphLookup.find(character);
        if (it == _glyphLookup.end())
        {
            return false;
        }

        outIndex = it->second;
        return true;
    }

    if (character < _firstChar)
    {
        return false;
    }

    const size_t index = static_cast<size_t>(character - _firstChar);
    if (_glyphCount > 0 && index >= _glyphCount)
    {
        return false;
    }

    outIndex = index;
    return true;
}

void SpriteText::RebuildGlyphLookup()
{
    _glyphLookup.clear();
    size_t index = 0;
    _glyphLookup.reserve(_glyphOrder.size());

    for (wchar_t character : _glyphOrder)
    {
        auto result = _glyphLookup.emplace(character, index);
        if (!result.second)
        {
            result.first->second = index;
        }
        ++index;
    }

}
