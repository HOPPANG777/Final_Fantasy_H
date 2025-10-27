#pragma once

// Transform ġ, ȸ, ũ  ϴ ⺻ Ʈ.
//    ٸ Ʈ   ֵ Ѵ.

class Transform : public Component
{
public:
	Transform(const std::string& name = "Transform") : Component(name) {}
	virtual ~Transform() override = default;

public:
        // Component ֱ⿡    Ѵ.
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
        DirectX::SimpleMath::Vector3 scale = Values::ZeroVector3; //   
	void Move(const DirectX::SimpleMath::Vector3& speed);
	void Scale(const DirectX::SimpleMath::Vector3& scale);
	void Rotate(float degrees);
		 
private:
	DirectX::SimpleMath::Vector3 position = Values::ZeroVector3;
	DirectX::SimpleMath::Vector3 scale = Values::ZeroVector3;
	float rotation = 0.0f;	// 라디안(radian)으로 보관

	DirectX::SimpleMath::Matrix world = DirectX::XMMatrixIdentity();	// S * R * T 결과
	
};

