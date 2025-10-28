#include "Framework.h"
#include "Scene_01.h"
#include "Objects/ObjectList.h"

void Scene_01::Awake()
{
	isLoaded = false;

	objects.push_back(
		std::make_shared<ColorObject>(
			"Background",
			Values::ZeroVector3,
			DirectX::SimpleMath::Vector3(Values::GetWinWidth(), Values::GetWinHeight(), 0),
			0.0f,
			Values::White,
			true
		)
	);

	objects.push_back(
		std::make_shared<TextureObject>(
			"Logo",
			Values::ZeroVector3,
			DirectX::SimpleMath::Vector3(640, 67, 0),
			0.0f,
			TexturePath + L"Square_Enix_logo.png",
			true
		)
	);

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
	objects[1]->GUI();
	GraphicsManager::Get()->GUI();
}
