#pragma once

class TestObject : public GameObject
{
public:
	TestObject(
		const std::string& name,
		const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector3& scale,
		float rotation,
		const DirectX::SimpleMath::Color& color = Values::Red,
		bool useRectTransformType = false
	);
	~TestObject();

public:
	void Update() override;
	void Render() override;

};

