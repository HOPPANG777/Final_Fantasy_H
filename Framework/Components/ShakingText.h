#pragma once

class ShakingText : public Texts
{
public:
    ShakingText(const std::string& name = "ShakingText", float intensity = 2.0f)
        : Texts(name), intensity(intensity) 
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
        const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL,
        bool useOutline = false,
        const DirectX::SimpleMath::Color& outlineColor = Values::Black,
        float outlineThickness = 1.0f
    ) override;


    void Update() override;
    void Render() override;

    void GUI() override;

private:
    float intensity;

    int RandomOffset()
    {
        return int((rand() / float(RAND_MAX) - 0.5f) * 2 * intensity);
    }
};
