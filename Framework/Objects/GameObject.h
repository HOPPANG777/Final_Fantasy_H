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

public:
	template<typename T>
	T* FindComponentByInterface() const
	{
		try
		{
			// 컨테이너가 비어있는지 먼저 확인
			if (components.empty())
			{
				return nullptr;
			}

			// components의 타입: std::unordered_map<std::string, std::unique_ptr<Component>>
			for (const auto& pair : components)
			{
				T* comp = dynamic_cast<T*>(pair.second.get());
				if (comp != nullptr)
				{
					return comp;
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "[FindComponentByInterface] 예외 발생: " << e.what() << std::endl;
			// 필요한 경우 추가적인 복구 또는 로그 처리
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetAllComponentsByInterface() const
	{
		std::vector<T*> result;
		try
		{
			if (components.empty())
			{
				return result;
			}

			// components의 타입: std::unordered_map<std::string, std::unique_ptr<Component>>
			for (const auto& pair : components)
			{
				T* comp = dynamic_cast<T*>(pair.second.get());
				if (comp != nullptr)
				{
					result.push_back(comp);
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "[GetAllComponentsByInterface] 예외 발생: " << e.what() << std::endl;
		}
		return result;
	}

protected:
	std::string name;
	std::map<std::string, std::shared_ptr<Component>> components;

	std::unique_ptr<WorldBuffer> worldBuffer;

	bool RectTransformType = false;
};

