// Texts δ ۲ ҽ غϰ ڿ ο ȣ Ѵ.

#include "Framework.h"
#include "Texts.h"

void Texts::RenderText
(
	const std::wstring& text,
	const RECT& rect,
	const float size,
	const DirectX::SimpleMath::Color& color,
	const std::wstring& font,
	const DWRITE_FONT_WEIGHT& weight,
	const DWRITE_FONT_STYLE& style,
	const DWRITE_FONT_STRETCH& stretch
)
{
        //   귯ø    δ.
	FontBrushDesc* brush = GetOrCreateBrush(color);
        // ۲ð ũ յ ĳ    ּȭѴ.
	FontTextDesc* textDesc = GetOrCreateTextFormat(font, size, weight, style, stretch);

        //  Ʈ   ġ  簢 Ѵ.
	RECT finalRect = CalculateRectFromObject(rect);

	D2D1_RECT_F rectF =
	{
	static_cast<float>(finalRect.left),
	static_cast<float>(gWinHeight - finalRect.bottom),
	static_cast<float>(finalRect.right),
	static_cast<float>(gWinHeight - finalRect.top)
	};

        // Direct2D ؽƮ   ؽƮ ׸.
	GraphicsManager::Get()->GetD2DC()->DrawTextW
	(
		text.c_str(),
		static_cast<UINT32>(text.size()),
		textDesc->format.Get(),
		rectF,
		brush->brush.Get()
	);

        //  CalculateRectFromObject ش Ʈ ġ ȰѴ.
        // ̹   귯ð    ʴ´.
        //   귯ø ǰ Բ ̳ʿ Ѵ.
        //   ϸ   Ѵ.
        //  ˵ ĳÿ   ȣ⿡ Ѵ.
        //  簢 ϱ  ӽ .
                // RectTransform ִٸ ش Rect  ״ Ѵ.
                //  ⺻ Transform ġ offset  簢 ̵Ų.
}
}

void Texts::AttachToObject(GameObject* object)
{
	this->attachedObject = object;
}

void Texts::Detach()
{
	this->attachedObject = nullptr;
}

FontBrushDesc* Texts::GetOrCreateBrush(const DirectX::SimpleMath::Color& color)
{
	auto newBrushDesc = std::make_unique<FontBrushDesc>();
	newBrushDesc->color = color;

	const auto& brushIt = std::find_if(fontBrush.begin(), fontBrush.end(),
		[&newBrushDesc](const std::unique_ptr<FontBrushDesc>& ptr)
		{
			return *ptr == *newBrushDesc;
		});

	if (brushIt != fontBrush.end()) return brushIt->get();

	D2D1::ColorF colorF(color.R(), color.G(), color.B(), color.A());
	HRESULT hr = GraphicsManager::Get()->GetD2DC()->CreateSolidColorBrush(colorF, &newBrushDesc->brush);
	CHECK(hr);

	FontBrushDesc* createdBrush = newBrushDesc.get();
	fontBrush.push_back(std::move(newBrushDesc));
	return createdBrush;
}

FontTextDesc* Texts::GetOrCreateTextFormat
(
	const std::wstring& font,
	float size,
	const DWRITE_FONT_WEIGHT& weight,
	const DWRITE_FONT_STYLE& style,
	const DWRITE_FONT_STRETCH& stretch
)
{
	auto newTextDesc = std::make_unique<FontTextDesc>();
	newTextDesc->font = font;
	newTextDesc->fontSize = size;
	newTextDesc->weight = weight;
	newTextDesc->style = style;
	newTextDesc->stretch = stretch;

	const auto& textIt = std::find_if(fontText.begin(), fontText.end(),
		[&newTextDesc](const std::unique_ptr<FontTextDesc>& ptr)
		{
			return *ptr == *newTextDesc;
		});

	if (textIt != fontText.end()) return textIt->get();

	HRESULT hr = GraphicsManager::Get()->GetWrite()->CreateTextFormat
	(
		newTextDesc->font.c_str(),
		nullptr,
		newTextDesc->weight,
		newTextDesc->style,
		newTextDesc->stretch,
		newTextDesc->fontSize,
		L"ko-kr",
		&newTextDesc->format
	);
	CHECK(hr);

	FontTextDesc* createdTextDesc = newTextDesc.get();
	fontText.push_back(std::move(newTextDesc));
	return createdTextDesc;
}

RECT Texts::CalculateRectFromObject(const RECT& rect)
{
	RECT adjustedRect;
	if (attachedObject)
	{
		// RectTransform이 있다면 그 값을 사용
		const auto& rectTransform = attachedObject->GetComponent<RectTransform>("RectTransform");
		if (rectTransform)
		{
			adjustedRect = rectTransform->GetRect();
		}
		else
		{
			// 없으면 기본적으로 객체의 위치와 offset을 더함
			DirectX::SimpleMath::Vector3 objectPos = attachedObject->GetComponent<RectTransform>("RectTransform")->GetPosition();
			adjustedRect = rect;
			adjustedRect.left += static_cast<LONG>(objectPos.x + offset.x);
			adjustedRect.right += static_cast<LONG>(objectPos.x + offset.x);
			adjustedRect.top += static_cast<LONG>(objectPos.y + offset.y);
			adjustedRect.bottom += static_cast<LONG>(objectPos.y + offset.y);
		}
		// 보정 함수 호출: DX3D 좌표계 -> 윈도우 좌표계로 변환
		return ConvertToWindowCoordinates(adjustedRect, gWinHeight);
	}
	return rect;
}

// 좌표계를 보정하는 헬퍼 함수: DX3D(원점: 좌하단) -> DX2D(원점: 좌상단)
RECT Texts::ConvertToWindowCoordinates(const RECT& rect, float screenHeight)
{
	RECT windowRect;
	windowRect.left = rect.left;
	windowRect.right = rect.right;
	windowRect.top = static_cast<LONG>(screenHeight) - rect.bottom; // bottom 값에서 보정
	windowRect.bottom = static_cast<LONG>(screenHeight) - rect.top;
	return windowRect;
}