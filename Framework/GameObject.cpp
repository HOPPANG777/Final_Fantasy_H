#include "Framework.h"
#include "GameObject.h"

GameObject::GameObject(const std::string& name, bool RectTransformType)
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

GameObject::GameObject(
	const std::string& name,
	const DirectX::SimpleMath::Vector3& initPos,
	const DirectX::SimpleMath::Vector3& initScale,
	float initRotation,
	bool RectTransformType
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

GameObject::GameObject(const GameObject& other)
	: GameObject(other.name, false) // 복사 생성자 로직은 필요에 따라 수정
{
	// 복사 생성자 구현 (필요 시 components 복사)
}


void GameObject::AddComponent(const std::shared_ptr<Component>& component)
{
	if (components.count(component->GetName())) return;

	component->SetOwner(this);
	components.emplace(component->GetName(), component);
}

void GameObject::RemoveComponent(const std::string& compName)
{
	components.erase(compName);
}

std::shared_ptr<Transform> GameObject::GetTransform() const
{
	return GetComponent<Transform>("Transform");
}

std::shared_ptr<RectTransform> GameObject::GetRectTransform() const
{
	return GetComponent<RectTransform>("RectTransform");
}

void GameObject::Update()
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

void GameObject::Render()
{
	// 등록된 모든 컴포넌트 Render
	for (const auto& element : components)
	{
		const auto& compName = element.first;
		const auto& compPtr = element.second;

		compPtr->Render();
	}

	// WorldBuffer를 VS에 등록 (셰이더에서 월드 행렬 사용 가능)
	worldBuffer->SetVSBuffer(0);
}
