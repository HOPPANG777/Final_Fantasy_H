#pragma once

class GameObject
{
public:
	GameObject(const std::string& name, const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& scale, const float rotation)
		: name(name), position(position), scale(scale), rotation(DirectX::XMConvertToRadians(rotation))
	{
		worldBuffer = std::make_unique<WorldBuffer>();
	}

	GameObject(const GameObject& other)
		: GameObject(other.name, other.position, other.scale, other.rotation)
	{

	}

	GameObject operator=(const GameObject& other)
	{
		GameObject obj(other);
		return obj;
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

		return dynamic_pointer_cast<T>(components.at(compName));
	}

public:
	void Move(const DirectX::SimpleMath::Vector2& speed) { position += speed * TimeManager::Get()->GetDeltaTime(); }
	void Rotate(const float rotation) { this->rotation += DirectX::XMConvertToRadians(rotation) * TimeManager::Get()->GetDeltaTime(); }
	void Scale(const DirectX::SimpleMath::Vector2& scale) { this->scale += scale * TimeManager::Get()->GetDeltaTime(); }

protected:
	std::string name;
	std::map<std::string, std::shared_ptr<Component>> components;

	DirectX::SimpleMath::Vector2 position, scale;
	float rotation = 0.0f;

	DirectX::SimpleMath::Matrix world, S, R, T;
	std::unique_ptr<WorldBuffer> worldBuffer;

};

