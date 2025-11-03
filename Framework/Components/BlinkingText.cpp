#include "Framework.h"
#include "BlinkingText.h"

using namespace DirectX;
using namespace SimpleMath;

void BlinkingText::RenderText(
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
	Color newColor = color;
	timer += TimeManager::Get()->GetDeltaTime();

	if (timer >= interval)
		timer -= interval; // 주기 반복

	// 0~interval 구간에서 0→1→0 변화
	float t = timer / interval;
	float alpha = t < 0.5f ? t * 2.0f : (1.0f - (t - 0.5f) * 2.0f);

	newColor.A(alpha);

	Texts::RenderText(text, rect, size, newColor, font, textalignment, paragraphalignment, weight, style, stretch);
}

void BlinkingText::Update()
{
}

void BlinkingText::Render()
{
}

void BlinkingText::GUI()
{
}
