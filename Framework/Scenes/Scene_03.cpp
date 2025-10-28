#include "Framework.h"
#include "Scene_03.h"

void Scene_03::Awake()
{
	isLoaded = false;

	objects.push_back(
		std::make_shared<ColorObject>(
			"TitileSceneBackground",
			Values::ZeroVector3,
			DirectX::SimpleMath::Vector3(Values::GetWinWidth(), Values::GetWinHeight(), 0),
			0.0f,
			Values::White,
			true
		)
	);

	isLoaded = true;
}

void Scene_03::Start()
{
}

void Scene_03::Destroy()
{
	for (auto& obj : objects)
		if (obj != nullptr)
			obj.reset();
}

void Scene_03::FixedUpdate()
{
}

void Scene_03::Update()
{
	for (const auto& obj : objects)
		if (obj != nullptr)
			obj->Update();
}

void Scene_03::LateUpdate()
{
}

void Scene_03::Render()
{
	for (const auto& obj : objects)
		if (obj != nullptr)
			obj->Render();
}

void Scene_03::PostRender()
{
}

void Scene_03::ImGUI()
{
}
