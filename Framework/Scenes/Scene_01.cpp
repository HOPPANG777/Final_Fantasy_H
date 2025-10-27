#include "Framework.h"
#include "Scene_01.h"
#include "Objects/ObjectList.h"

void Scene_01::Awake()
{
	isLoaded = false;

	objects.push_back(
		std::make_shared<TestObject>(
			"Mirko2",
			Values::GetCenter(),
			DirectX::SimpleMath::Vector3(Values::GetWinWidth(), Values::GetWinHeight(), 0),
			0.0f,
			TexturePath + L"Mirko3.jpg"
		)
	);

	objects.push_back(
		std::make_shared<TestObject>(
			"Mirko",
			Values::GetCenter(),
			DirectX::SimpleMath::Vector3(159, 416, 0),
			0.0f,
			TexturePath + L"Mirko1.png"
		)
	);

	objects[1]->AddComponent(std::make_shared<MoveComponent>());

	isLoaded = true;
}

void Scene_01::Start()
{

}

void Scene_01::Destroy()
{
	for (auto& obj : objects)
		if (obj != nullptr)
			obj.reset();
}

void Scene_01::FixedUpdate()
{
}

void Scene_01::Update()
{
	for (const auto& obj : objects)
		if (obj != nullptr)
			obj->Update();
}

void Scene_01::LateUpdate()
{
}

void Scene_01::Render()
{
	for (const auto& obj : objects)
		if (obj != nullptr)
			obj->Render();
}

void Scene_01::PostRender()
{

}

void Scene_01::ImGUI()
{
	//GraphicsManager::Get()->GUI();
}
