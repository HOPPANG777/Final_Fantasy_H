#pragma once

class GameObject
{
public:
	// 기본 생성자 (Object 이름만 지정)
	GameObject(
		const std::string& name,
		bool RectTransformType = false			// false = Transform, true = RectTransform
	);

	// 위치/스케일/회전을 초기에 지정하는 생성자
	GameObject(
		const std::string& name, 
		const DirectX::SimpleMath::Vector3& initPos, 
		const DirectX::SimpleMath::Vector3& initScale, 
		float initRotation,
		bool RectTransformType = false
	);

	// 복사 생성/대입 연산자
	GameObject(const GameObject& other);
	GameObject& operator=(const GameObject& other)
	{
		if (this != &other)
		{
			name = other.name;
			// 필요한 경우 components를 복제
		}
		return *this;
	}

	virtual ~GameObject() = default;

public:
	virtual void Update();
	virtual void Render();

public:
	void AddComponent(const std::shared_ptr<Component>& component);
	void RemoveComponent(const std::string& compName);

	template<typename T>
	std::shared_ptr<T> GetComponent(const std::string& compName) const
	{
		if (!components.count(compName)) return nullptr;

		return std::dynamic_pointer_cast<T>(components.at(compName));
	}

public:
	std::string GetName() const { return name; }
	void SetName(const std::string& newName) { name = newName; }

public:
	std::shared_ptr<Transform> GetTransform() const;
	std::shared_ptr<RectTransform> GetRectTransform() const;

protected:
	std::string name;
	std::map<std::string, std::shared_ptr<Component>> components;

	std::unique_ptr<WorldBuffer> worldBuffer;

	bool RectTransformType = false;
};

