#pragma once

class AnimationObject : public GameObject
{
	AnimationObject(
		const std::string& name,
		const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector3& scale,
		float rotation,
		const std::wstring& texturePath = L"",
		bool useRectTransformType = false
	)
		: GameObject(name, pos, scale, rotation, useRectTransformType)
	{
		this->AddComponent(std::make_shared<AnimatedTexture>(pos, scale, rotation, texturePath, useRectTransformType));
	}

	~AnimationObject() = default;

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

