#pragma once

class TestObject : public GameObject
{
public:
	//TestObject(
	//	const std::string& name,
	//	const DirectX::SimpleMath::Vector3& pos,
	//	const DirectX::SimpleMath::Vector3& scale,
	//	float rotation,
	//	const DirectX::SimpleMath::Color& color = Values::Red,
	//	bool useRectTransformType = false
	//)
	//	: GameObject(name, pos, scale, rotation, useRectTransformType)
	//{
	//	this->AddComponent(std::make_shared<ColorRect>(pos, scale, rotation, color, useRectTransformType));
	//}

	TestObject(
		const std::string& name,
		const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector3& scale,
		float rotation,
		const std::wstring& texturePath = L"",
		bool useRectTransformType = false
	)
		: GameObject(name, pos, scale, rotation, useRectTransformType)
	{
		this->AddComponent(std::make_shared<Texture>(pos, scale, rotation, texturePath, useRectTransformType));
	}

	~TestObject() = default;

public:
	void Update() override
	{
		__super::Update();
	}

	void Render() override
	{
		__super::Render();
	}

	void GUI() override
	{
		__super::GUI();
	}

};

