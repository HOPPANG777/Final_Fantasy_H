#include "Framework.h"
#include "DirectHelper.h"

float Values::referenceWidth = WIN_DEFAULT_WIDTH;
float Values::referenceHeight = WIN_DEFAULT_HEIGHT;

void Values::SetReferenceResolution(float width, float height)
{
        referenceWidth = width > 0.0f ? width : 1.0f;
        referenceHeight = height > 0.0f ? height : 1.0f;
}

float Values::GetReferenceWidth()
{
        return referenceWidth;
}

float Values::GetReferenceHeight()
{
        return referenceHeight;
}

DirectX::SimpleMath::Vector2 Values::GetReferenceResolution()
{
        return DirectX::SimpleMath::Vector2(referenceWidth, referenceHeight);
}

DirectX::SimpleMath::Vector2 Values::GetUIScale()
{
        const float safeWidth = referenceWidth != 0.0f ? referenceWidth : 1.0f;
        const float safeHeight = referenceHeight != 0.0f ? referenceHeight : 1.0f;

        return DirectX::SimpleMath::Vector2(gWinWidth / safeWidth, gWinHeight / safeHeight);
}

float Values::GetUIScaleX()
{
        return GetUIScale().x;
}

float Values::GetUIScaleY()
{
        return GetUIScale().y;
}

float Values::ApplyUIScaleX(float value)
{
        return value * GetUIScaleX();
}

float Values::ApplyUIScaleY(float value)
{
        return value * GetUIScaleY();
}

DirectX::SimpleMath::Vector2 Values::ApplyUIScale(const DirectX::SimpleMath::Vector2& value)
{
        const DirectX::SimpleMath::Vector2 scale = GetUIScale();
        return DirectX::SimpleMath::Vector2(value.x * scale.x, value.y * scale.y);
}

const float Values::PI = 3.141592f;
const float Values::Deg2Rad = PI / 180;

const DirectX::SimpleMath::Vector3 Values::ZeroVector3 = DirectX::SimpleMath::Vector3(0, 0, 0);
const DirectX::SimpleMath::Vector3 Values::OneVector3 = DirectX::SimpleMath::Vector3(1, 1, 1);
const DirectX::SimpleMath::Vector3 Values::LeftVector3 = DirectX::SimpleMath::Vector3(-1, 0, 0);
const DirectX::SimpleMath::Vector3 Values::RightVector3 = DirectX::SimpleMath::Vector3(1, 0, 0);
const DirectX::SimpleMath::Vector3 Values::UpVector3 = DirectX::SimpleMath::Vector3(0, 1, 0);
const DirectX::SimpleMath::Vector3 Values::DownVector3 = DirectX::SimpleMath::Vector3(0, -1, 0);
const DirectX::SimpleMath::Vector3 Values::ForwardVector3 = DirectX::SimpleMath::Vector3(0, 0, 1);

const DirectX::SimpleMath::Vector2 Values::ZeroVector2 = DirectX::SimpleMath::Vector2(0, 0);
const DirectX::SimpleMath::Vector2 Values::OneVector2 = DirectX::SimpleMath::Vector2(1, 1);
const DirectX::SimpleMath::Vector2 Values::uvLT = DirectX::SimpleMath::Vector2(0, 0);
const DirectX::SimpleMath::Vector2 Values::uvLB = DirectX::SimpleMath::Vector2(0, 1);
const DirectX::SimpleMath::Vector2 Values::uvRT = DirectX::SimpleMath::Vector2(1, 0);
const DirectX::SimpleMath::Vector2 Values::uvRB = DirectX::SimpleMath::Vector2(1, 1);

const DirectX::SimpleMath::Color Values::Red = DirectX::SimpleMath::Color(1, 0, 0, 1);
const DirectX::SimpleMath::Color Values::Green = DirectX::SimpleMath::Color(0, 1, 0, 1);
const DirectX::SimpleMath::Color Values::Blue = DirectX::SimpleMath::Color(0, 0, 1, 1);
const DirectX::SimpleMath::Color Values::Yellow = DirectX::SimpleMath::Color(1, 1, 0, 1);
const DirectX::SimpleMath::Color Values::Magenta = DirectX::SimpleMath::Color(1, 0, 1, 1);
const DirectX::SimpleMath::Color Values::Cyan = DirectX::SimpleMath::Color(0, 1, 1, 1);
const DirectX::SimpleMath::Color Values::Black = DirectX::SimpleMath::Color(0, 0, 0, 1);
const DirectX::SimpleMath::Color Values::White = DirectX::SimpleMath::Color(1, 1, 1, 1);
const DirectX::SimpleMath::Color Values::Gray = DirectX::SimpleMath::Color(0.4f, 0.4f, 0.4f, 1);
const DirectX::SimpleMath::Color Values::Transparent = DirectX::SimpleMath::Color(0, 0, 0, 0);
const DirectX::SimpleMath::Color Values::ClearColor = DirectX::SimpleMath::Color(0xFF555566);

const DirectX::SimpleMath::Matrix Values::Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};