#include "Framework.h"
#include "Scene_02.h"
#include "Objects/ObjectList.h"

using namespace DirectX;
using namespace SimpleMath;

void Scene_02::Awake()
{

}

void Scene_02::Start()
{
	isLoaded = false;

	objects.push_back(
		std::make_shared<ColorObject>(
			"LoadingSceneBackground",
			Values::ZeroVector3,
			Vector3(Values::GetWinWidth(), Values::GetWinHeight(), 0),
			0.0f,
			Values::Black,
			true
		)
	);

	CameraManager::Get()->Reset();

	isLoaded = true;
}

void Scene_02::Destroy()
{
	for (auto& obj : objects)
		if (obj != nullptr)
			obj.reset();
}

void Scene_02::FixedUpdate()
{
}

void Scene_02::Update()
{
	for (const auto& obj : objects)
		if (obj != nullptr)
			obj->Update();
}

void Scene_02::LateUpdate()
{
}

void Scene_02::Render()
{
	for (const auto& obj : objects)
		if (obj != nullptr)
			obj->Render();
}

void Scene_02::PostRender()
{
}

void Scene_02::ImGUI()
{
	objects[0]->GUI();
}

