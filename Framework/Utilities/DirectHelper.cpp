#include "Framework.h"
#include "DirectHelper.h"

const float DirectHelper::PI = 3.141592f;
const float DirectHelper::Deg2Rad = PI / 180;

const DirectX::SimpleMath::Vector3 DirectHelper::ZeroVector3 = DirectX::SimpleMath::Vector3(0, 0, 0);
const DirectX::SimpleMath::Vector3 DirectHelper::OneVector3 = DirectX::SimpleMath::Vector3(1, 1, 1);
const DirectX::SimpleMath::Vector3 DirectHelper::LeftVector3 = DirectX::SimpleMath::Vector3(-1, 0, 0);
const DirectX::SimpleMath::Vector3 DirectHelper::RightVector3 = DirectX::SimpleMath::Vector3(1, 0, 0);
const DirectX::SimpleMath::Vector3 DirectHelper::UpVector3 = DirectX::SimpleMath::Vector3(0, 1, 0);
const DirectX::SimpleMath::Vector3 DirectHelper::DownVector3 = DirectX::SimpleMath::Vector3(0, -1, 0);
const DirectX::SimpleMath::Vector3 DirectHelper::ForwardVector3 = DirectX::SimpleMath::Vector3(0, 0, 1);

const DirectX::SimpleMath::Vector2 DirectHelper::ZeroVector2 = DirectX::SimpleMath::Vector2(0, 0);
const DirectX::SimpleMath::Vector2 DirectHelper::OneVector2 = DirectX::SimpleMath::Vector2(1, 1);
const DirectX::SimpleMath::Vector2 DirectHelper::uvLT = DirectX::SimpleMath::Vector2(0, 0);
const DirectX::SimpleMath::Vector2 DirectHelper::uvLB = DirectX::SimpleMath::Vector2(0, 1);
const DirectX::SimpleMath::Vector2 DirectHelper::uvRT = DirectX::SimpleMath::Vector2(1, 0);
const DirectX::SimpleMath::Vector2 DirectHelper::uvRB = DirectX::SimpleMath::Vector2(1, 1);

const DirectX::SimpleMath::Color DirectHelper::Red = DirectX::SimpleMath::Color(1, 0, 0, 1);
const DirectX::SimpleMath::Color DirectHelper::Green = DirectX::SimpleMath::Color(0, 1, 0, 1);
const DirectX::SimpleMath::Color DirectHelper::Blue = DirectX::SimpleMath::Color(0, 0, 1, 1);
const DirectX::SimpleMath::Color DirectHelper::Yellow = DirectX::SimpleMath::Color(1, 1, 0, 1);
const DirectX::SimpleMath::Color DirectHelper::Magenta = DirectX::SimpleMath::Color(1, 0, 1, 1);
const DirectX::SimpleMath::Color DirectHelper::Cyan = DirectX::SimpleMath::Color(0, 1, 1, 1);
const DirectX::SimpleMath::Color DirectHelper::Black = DirectX::SimpleMath::Color(0, 0, 0, 1);
const DirectX::SimpleMath::Color DirectHelper::White = DirectX::SimpleMath::Color(1, 1, 1, 1);
const DirectX::SimpleMath::Color DirectHelper::Gray = DirectX::SimpleMath::Color(0.4f, 0.4f, 0.4f, 1);
const DirectX::SimpleMath::Color DirectHelper::Transparent = DirectX::SimpleMath::Color(0, 0, 0, 0);
const DirectX::SimpleMath::Color DirectHelper::ClearColor = DirectX::SimpleMath::Color(0xFF555566);

const DirectX::SimpleMath::Matrix DirectHelper::Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};