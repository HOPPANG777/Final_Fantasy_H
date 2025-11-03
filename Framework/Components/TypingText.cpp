#include "Framework.h"
#include "TypingText.h"

void TypingText::RenderText
(
	const std::wstring& text,
	const RECT& rect,
	const float size,
	const DirectX::SimpleMath::Color& color,
	const std::wstring& font,
    const DWRITE_TEXT_ALIGNMENT& textalignment,
    const DWRITE_PARAGRAPH_ALIGNMENT& paragraphalignment,
	const DWRITE_FONT_WEIGHT& weight,
	const DWRITE_FONT_STYLE& style,
	const DWRITE_FONT_STRETCH& stretch,
	bool useOutline,
	const DirectX::SimpleMath::Color& outlineColor,
	float outlineThickness
)
{
	
	std::wstring visibleText = text.substr(0, visibleCharacters);
	fullText = text;
	Texts::RenderText(visibleText, rect, size, color, font, textalignment, paragraphalignment, weight, style, stretch, useOutline, outlineColor, outlineThickness);
}

void TypingText::Update()
{
	
    elapsedTime += TimeManager::Get()->GetDeltaTime();

    if (elapsedTime >= typingSpeed)
    {
        elapsedTime = 0.0f;

       
        if (!complete)
        {
            visibleCharacters++;
            if (visibleCharacters >= fullText.size())
            {
                visibleCharacters = fullText.size();
                complete = true;
            }
        }
        
        else if (bLoop)
        {
            visibleCharacters = 0;
            complete = false;
        }
    }
}

void TypingText::Render()
{

}

void TypingText::GUI()
{
}
