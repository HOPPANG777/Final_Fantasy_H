#include "Framework.h"
#include "Texts.h"

void Texts::RenderText
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
	FontBrushDesc* brush = GetOrCreateBrush(color);
	FontTextDesc* textDesc = GetOrCreateTextFormat(font, size, weight, style, stretch);

	textDesc->format->SetTextAlignment(textalignment);
	textDesc->format->SetParagraphAlignment(paragraphalignment);

	RECT finalRect = CalculateRectFromObject(rect);

	D2D1_RECT_F rectF =
	{
	static_cast<float>(finalRect.left),
	static_cast<float>(gWinHeight - finalRect.bottom),
	static_cast<float>(finalRect.right),
	static_cast<float>(gWinHeight - finalRect.top)
	};

	if (useOutline && outlineThickness > 0.0f)
	{
		FontBrushDesc* outlineBrush = GetOrCreateBrush(outlineColor);
		const float outlineOffsets[8][2] =
		{
			{-1.0f, 0.0f},
			{1.0f, 0.0f},
			{0.0f, -1.0f},
			{0.0f, 1.0f},
			{-1.0f, -1.0f},
			{-1.0f, 1.0f},
			{1.0f, -1.0f},
			{1.0f, 1.0f}
		};

		for (const auto& offsetPair : outlineOffsets)
		{
			D2D1_RECT_F outlineRectF = rectF;
			const float offsetX = offsetPair[0] * outlineThickness;
			const float offsetY = offsetPair[1] * outlineThickness;

			outlineRectF.left += offsetX;
			outlineRectF.right += offsetX;
			outlineRectF.top += offsetY;
			outlineRectF.bottom += offsetY;

			GraphicsManager::Get()->GetD2DC()->DrawTextW
			(
				text.c_str(),
				static_cast<UINT32>(text.size()),
				textDesc->format.Get(),
				outlineRectF,
				outlineBrush->brush.Get()
			);
		}
	}

	GraphicsManager::Get()->GetD2DC()->DrawTextW
	(
		text.c_str(),
		static_cast<UINT32>(text.size()),
		textDesc->format.Get(),
		rectF,
		brush->brush.Get()
	);
}

void Texts::GUI()
{
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
		// RectTransform ִٸ   
		const auto& rectTransform = attachedObject->GetComponent<RectTransform>("RectTransform");
		if (rectTransform)
		{
			adjustedRect = rectTransform->GetRect();
		}
		else
		{
			//  ⺻ ü ġ offset 
			DirectX::SimpleMath::Vector3 objectPos = attachedObject->GetComponent<RectTransform>("RectTransform")->GetPosition();
			adjustedRect = rect;
			adjustedRect.left += static_cast<LONG>(objectPos.x + offset.x);
			adjustedRect.right += static_cast<LONG>(objectPos.x + offset.x);
			adjustedRect.top += static_cast<LONG>(objectPos.y + offset.y);
			adjustedRect.bottom += static_cast<LONG>(objectPos.y + offset.y);
		}
		//  Լ ȣ: DX3D ǥ ->  ǥ ȯ
		return ConvertToWindowCoordinates(adjustedRect, gWinHeight);
	}
	return rect;
}

// ǥ踦 ϴ  Լ: DX3D(: ϴ) -> DX2D(: »)
RECT Texts::ConvertToWindowCoordinates(const RECT& rect, float screenHeight)
{
	RECT windowRect;
	windowRect.left = rect.left;
	windowRect.right = rect.right;
	windowRect.top = static_cast<LONG>(screenHeight) - rect.bottom; // bottom  
	windowRect.bottom = static_cast<LONG>(screenHeight) - rect.top;
	return windowRect;
}
