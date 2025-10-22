#pragma once


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

    virtual void RenderText
    (
        const std::wstring& text,
        const RECT& rect,
        const float size = 25.0f,
        const DirectX::SimpleMath::Color& color = DirectHelper::White,
        const std::wstring& font = L"Pretendard JP",
        const DWRITE_FONT_WEIGHT& weight = DWRITE_FONT_WEIGHT_NORMAL,
        const DWRITE_FONT_STYLE& style = DWRITE_FONT_STYLE_NORMAL,
        const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL
    );

    virtual void Update() override {} // 애니메이션을 구현하려면 업데이트 메서드 사용
    virtual void Render() override {}

public:
    void AttachToObject(GameObject* object);
    void Detach();
	void SetTextOut(const bool textOut) { this->textOut = textOut; }
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

    GameObject* attachedObject = nullptr; // 텍스트가 따라다닐 객체
    DirectX::SimpleMath::Vector3 offset = DirectHelper::ZeroVector3;  // 객체 위치에 더할 오프셋

    bool textOut = false;

private:
    RECT CalculateRectFromObject(const RECT& rect);
    RECT ConvertToWindowCoordinates(const RECT& rect, float screenHeight);

};