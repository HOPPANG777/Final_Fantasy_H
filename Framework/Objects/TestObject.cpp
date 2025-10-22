#include "Framework.h"
#include "TestObject.h"

/*
ColorRect::ColorRect(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& scale,
    float rotation,
    const DirectX::SimpleMath::Color& color,
    bool useRectTransform
*/

TestObject::TestObject(const std::string& name,const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& scale, float rotation, const DirectX::SimpleMath::Color& color, bool useRectTransformType)
	: GameObject(name, pos, scale, rotation, useRectTransformType)
{
	this->AddComponent(std::make_shared<ColorRect>(pos, scale, rotation, color, useRectTransformType));
}

TestObject::~TestObject()
{
}

void TestObject::Update()
{
	__super::Update();
}

void TestObject::Render()
{
	__super::Render();
}
