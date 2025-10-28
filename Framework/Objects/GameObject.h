#pragma once

class GameObject
{
public:
	// 기본 생성자 (Object 이름만 지정)
	GameObject(
		const std::string& name,
		bool RectTransformType = false			// false = Transform, true = RectTransform
	)
		: name(name), RectTransformType(RectTransformType)
	{
		worldBuffer = std::make_unique<WorldBuffer>();

		if (RectTransformType)
		{
			const std::shared_ptr<RectTransform>& rectTransform = std::make_shared<RectTransform>();
			AddComponent(rectTransform);
		}
		else
		{
			const std::shared_ptr<Transform>& transform = std::make_shared<Transform>();
			AddComponent(transform);
		}
	}

	// 위치/스케일/회전을 초기에 지정하는 생성자
	GameObject(
		const std::string& name,
		const DirectX::SimpleMath::Vector3& initPos,
		const DirectX::SimpleMath::Vector3& initScale,
		float initRotation,
		bool RectTransformType = false
	)
		: GameObject(name, RectTransformType)
	{
		if (RectTransformType)
		{
			const std::shared_ptr<RectTransform>& rectTransform = GetComponent<RectTransform>("RectTransform");
			if (rectTransform)
			{
				rectTransform->SetPosition(initPos);
				rectTransform->SetScale(initScale);
				rectTransform->SetRotation(initRotation);
			}

		}
		else
		{
			const std::shared_ptr<Transform>& transform = GetComponent<Transform>("Transform");
			if (transform)
			{
				transform->SetPosition(initPos);
				transform->SetScale(initScale);
				transform->SetRotation(initRotation);
			}
		}

	}

	// 복사 생성/대입 연산자
	GameObject(const GameObject& other)
		: GameObject(other.name, false) // 복사 생성자 로직은 필요에 따라 수정
	{
		// 복사 생성자 구현 (필요 시 components 복사)
	}

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
	virtual void Update()
	{
		// 등록된 모든 컴포넌트 업데이트
		// 여기서 each element의 타입은 std::pair<const std::string, std::shared_ptr<Component>>
		for (const std::pair<const std::string, std::shared_ptr<Component>>& element : components)
		{
			const std::string& compName = element.first;    // 컴포넌트 이름
			const std::shared_ptr<Component>& compPtr = element.second; // 컴포넌트 포인터

			compPtr->Update();
		}

		if (RectTransformType)
		{
			const std::shared_ptr<RectTransform>& rectTransform = GetComponent<RectTransform>("RectTransform");
			if (rectTransform)
			{
				worldBuffer->SetWorld(rectTransform->GetWorld());
			}
		}
		else
		{
			const std::shared_ptr<Transform>& transform = GetComponent<Transform>("Transform");
			if (transform)
			{
				worldBuffer->SetWorld(transform->GetWorld());
			}
		}

		// RectTransform이 있다면 우선적으로 사용합니다.
		const std::shared_ptr<RectTransform>& rectTransform = GetComponent<RectTransform>("RectTransform");
		if (rectTransform)
		{
			worldBuffer->SetWorld(rectTransform->GetWorld());
		}
		else
		{
			const std::shared_ptr<Transform>& transform = GetComponent<Transform>("Transform");
			if (transform)
			{
				worldBuffer->SetWorld(transform->GetWorld());
			}
		}
	}

	virtual void Render()
	{
		// WorldBuffer를 VS에 등록 (셰이더에서 월드 행렬 사용 가능)
		worldBuffer->SetVSBuffer(0);

		// 등록된 모든 컴포넌트 Render
		for (const auto& element : components)
		{
			const auto& compName = element.first;
			const auto& compPtr = element.second;

			compPtr->Render();
		}
	}

	virtual void GUI()
	{
		for (const auto& element : components)
		{
			const auto& compName = element.first;
			const auto& compPtr = element.second;

			compPtr->GUI();
		}
	}

public:
	void AddComponent(const std::shared_ptr<Component>& component)
	{
		if (components.count(component->GetName())) return;

		component->SetOwner(this);
		components.emplace(component->GetName(), component);
	}

	void RemoveComponent(const std::string& compName)
	{
		components.erase(compName);
	}


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
	std::shared_ptr<Transform> GetTransform() const
	{
		return GetComponent<Transform>("Transform");
	}

	std::shared_ptr<RectTransform> GetRectTransform() const
	{
		return GetComponent<RectTransform>("RectTransform");
	}

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

