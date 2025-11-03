#pragma once

class TypingText : public Texts
{
public:
    TypingText(const std::string& name = "TypingText", float typingSpeed = 0.075f)
        : Texts(name), typingSpeed(typingSpeed), elapsedTime(0.0f), visibleCharacters(0)
    {

    }

    
    virtual void RenderText
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

public:
    virtual void Update() override;
    virtual void Render() override;

    void GUI() override;

public:
    
    bool IsComplete() const { return complete; }
    
	void SkipToEnd() { visibleCharacters = fullText.size(); complete = true; }

    void SetLoop(bool loop) { bLoop = loop; }

private:
    float typingSpeed;       
    float elapsedTime;       
    size_t visibleCharacters;  
    std::wstring fullText;         
    bool complete = false;    
    bool bLoop = false;
};
