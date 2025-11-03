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
	const DWRITE_FONT_STRETCH& stretch
)
{
	// 전체 텍스트(fullText) 대신, visibleCharacters만큼 자른 텍스트를 렌더링
	std::wstring visibleText = text.substr(0, visibleCharacters);
	fullText = text;
	Texts::RenderText(visibleText, rect, size, color, font, textalignment, paragraphalignment, weight, style, stretch);
}

void TypingText::Update()
{
	// 여기서 기본 Texts::Update()가 특별한 로직이 없다면 생략하고 타이핑 업데이트만 수행
    elapsedTime += TimeManager::Get()->GetDeltaTime();

    if (elapsedTime >= typingSpeed)
    {
        elapsedTime = 0.0f;

        // 아직 완성 전이면 한 글자씩 증가
        if (!complete)
        {
            visibleCharacters++;
            if (visibleCharacters >= fullText.size())
            {
                visibleCharacters = fullText.size();
                complete = true;
            }
        }
        // 완성 후 루프 모드일 경우 초기화
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
