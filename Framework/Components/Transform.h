#pragma once

class Transform : public Component
{
public:
	Transform(const std::string& name = "Transform") : Component(name) {}
	virtual ~Transform() override = default;

public:
	// Component을(를) 통해 상속됨
	void Update() override;
	void Render() override;

	const DirectX::SimpleMath::Matrix& GetWorld() const;

public:
	// Getter
	DirectX::SimpleMath::Vector3 GetPosition() const;
	DirectX::SimpleMath::Vector3 GetScale() const;
	float GetRotation() const;

public:
	// Setter
	void SetPosition(const DirectX::SimpleMath::Vector3& pos);
	void SetScale(const DirectX::SimpleMath::Vector3& scale);
	void SetRotation(float degrees);

public:
	// 이동/회전/스케일 변화를 DELTA 기반으로 적용할 수 있는 함수들
	void Move(const DirectX::SimpleMath::Vector3& speed);
	void Scale(const DirectX::SimpleMath::Vector3& scale);
	void Rotate(float degrees);
		 
private:
	DirectX::SimpleMath::Vector3 position = Values::ZeroVector3;
	DirectX::SimpleMath::Vector3 scale = Values::ZeroVector3;
	float rotation = 0.0f;	// 라디안(radian)으로 보관

	DirectX::SimpleMath::Matrix world = DirectX::XMMatrixIdentity();	// S * R * T 결과
	
};

