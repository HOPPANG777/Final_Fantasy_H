#include "Framework.h"
#include "GameObject.h"

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

void GameObject::Update()
{
	for (const auto& comp : components) comp.second->Update();

	//SRT
	{
		S = DirectX::XMMatrixScalingFromVector(scale);
		R = DirectX::XMMatrixRotationZ(-rotation);
		T = DirectX::XMMatrixTranslationFromVector(position);
	}

	//world
	{
		world = S * R * T;
	}

	//worldBuffer
	{
		worldBuffer->SetWorld(world);
	}
}

void GameObject::Render()
{
	for (const auto& comp : components) comp.second->Render();

	//worldBuffer
	{
		worldBuffer->SetVSBuffer(0);
	}
}
