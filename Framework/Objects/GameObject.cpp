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
        : GameObject(other.name, false)
{
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
        for (const std::pair<const std::string, std::shared_ptr<Component>>& element : components)
        {
                const std::shared_ptr<Component>& compPtr = element.second;

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
        worldBuffer->SetVSBuffer(0);

        for (const auto& element : components)
        {
                const std::shared_ptr<Component>& compPtr = element.second;

                compPtr->Render();
        }
}
