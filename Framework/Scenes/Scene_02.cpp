#include "Framework.h"
#include "Scene_02.h"

void Scene_02::Awake()
{
	isLoad = false;





	isLoad = true;
}

void Scene_02::Start()
{
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
}

