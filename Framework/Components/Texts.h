#pragma once

// Texts Ʈ  ڿ ϴ Drawable  Ŭ.
// ۲, ,   ؽƮ Ӽ Ѵ.

struct FontBrushDesc
{
    DirectX::SimpleMath::Color color;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush = nullptr;

    bool operator==(const FontBrushDesc& val) const
    {
        return	color.R() == val.color.R() &&
            color.G() == val.color.G() &&
            color.B() == val.color.B() &&
            color.A() == val.color.A();
    }
};

struct FontTextDesc
{
    std::wstring font;
    float fontSize;

    DWRITE_FONT_WEIGHT weight;
    DWRITE_FONT_STYLE style;
    DWRITE_FONT_STRETCH stretch;

    Microsoft::WRL::ComPtr<IDWriteTextFormat> format = nullptr;

    bool operator==(const FontTextDesc& val)
    {
        bool b = true;
        b &= font == val.font;
        b &= fontSize == val.fontSize;
        b &= weight == val.weight;
        b &= style == val.style;
        b &= stretch == val.stretch;

        return b;
    }
};

class Texts : public Component
{
public:
    Texts(const std::string& name = "Texts") : Component(name) {}
    virtual ~Texts() = default;

    // ؽƮ ڿ  簢 ׸  ⺻ ̽.
    virtual void RenderText
    (
        const std::wstring& text,
        const RECT& rect,
        const float size = 25.0f,
        const DirectX::SimpleMath::Color& color = Values::White,
        const std::wstring& font = L"Pretendard JP",
        const DWRITE_FONT_WEIGHT& weight = DWRITE_FONT_WEIGHT_NORMAL,
        const DWRITE_FONT_STYLE& style = DWRITE_FONT_STYLE_NORMAL,
        const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL
    );

    virtual void Update() override {} // ؽƮ ִϸ̼ ʿϸ ̵Ѵ.
    virtual void Render() override {} // ؽƮ DrawText ȣ  ȴ.

public:
    // Ư Ʈ  ġ ڵ 󰡵 Ѵ.
    void AttachToObject(GameObject* object);
    //  Ʈ  Ѵ.
    void Detach();
        // true ϸ ȭ鿡 ؽƮ Ѵ.
	void SetTextOut(const bool textOut) { this->textOut = textOut; }
        //   ¸ ȮѴ.
	bool GetTextOut() const { return textOut; }

protected:
    std::vector<std::unique_ptr<FontBrushDesc>> fontBrush;
    std::vector<std::unique_ptr<FontTextDesc>> fontText;

    FontBrushDesc* GetOrCreateBrush(const DirectX::SimpleMath::Color& color);
    FontTextDesc* GetOrCreateTextFormat
    (
        const std::wstring& font,
        float size,
        const DWRITE_FONT_WEIGHT& weight,
        const DWRITE_FONT_STYLE& style,
        const DWRITE_FONT_STRETCH& stretch
    );

};
    DirectX::SimpleMath::Vector3 offset = Values::ZeroVector3;  // 객체 위치에 더할 오프셋

    bool textOut = false;

private:
    RECT CalculateRectFromObject(const RECT& rect);
    RECT ConvertToWindowCoordinates(const RECT& rect, float screenHeight);

};