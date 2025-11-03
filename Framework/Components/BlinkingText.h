#pragma once

class BlinkingText : public Texts
{
public:
    BlinkingText(const std::string& name = "BlinkingText", float interval = 0.5f)
        : Texts(name), interval(interval)
    {

    }

    void RenderText
    (
        const std::wstring& text,
        const RECT& rect,
        const float size = 25.0f,
        const DirectX::SimpleMath::Color& color = Values::White,
        const std::wstring& font = L"Pretendard JP",
        const DWRITE_TEXT_ALIGNMENT& textalignment = DWRITE_TEXT_ALIGNMENT_CENTER,
        const DWRITE_PARAGRAPH_ALIGNMENT& paragraphalignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
        const DWRITE_FONT_WEIGHT& weight = DWRITE_FONT_WEIGHT_NORMAL,
        const DWRITE_FONT_STYLE& style = DWRITE_FONT_STYLE_NORMAL,
        const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL
    ) override;


    void Update() override;
    void Render() override;

    void GUI() override;

private:
    float interval = 0.0f;
    float timer = 0.0f;

};
