#pragma once

// ShakingText ؽƮ    ִ  ϴ Ʈ.
//  شϴ intensity   鸲 ⸦ Ѵ.

class ShakingText : public Texts
{
public:
    ShakingText(const std::string& name = "ShakingText", float intensity = 2.0f)
        : Texts(name), intensity(intensity) 
    {

    }

    //      鸮 ȿ .
    void RenderText
    (
        const std::wstring& text,
        const RECT& rect,
        const float size = 25.0f,
        const DirectX::SimpleMath::Color& color = Values::White,
        const std::wstring& font = L"Pretendard JP",
        const DWRITE_FONT_WEIGHT& weight = DWRITE_FONT_WEIGHT_NORMAL,
        const DWRITE_FONT_STYLE& style = DWRITE_FONT_STYLE_NORMAL,
        const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL
    ) override;


    // 鸲  ܰ迡 óϹǷ Update  д.
    void Update() override;
    void Render() override;

private:
    float intensity;

    // intensity   -intensity ~ +intensity   Ѵ.
    int RandomOffset()
    {
        return int((rand() / float(RAND_MAX) - 0.5f) * 2 * intensity);
    }
};
