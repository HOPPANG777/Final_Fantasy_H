#include "Framework.h"
#include "Scene_04.h"

void Scene_04::Awake()
{
}

void Scene_04::Start()
{
}

void Scene_04::Destroy()
{
	for (auto& obj : objects)
	{
		if (obj != nullptr)
		{
			obj.reset();
		}
	}
	objects.clear();
}

void Scene_04::FixedUpdate()
{
}

void Scene_04::Update()
{
	for (const auto& obj : objects)
	{
		if (obj != nullptr)
		{
			obj->Update();
		}
	}
}

void Scene_04::LateUpdate()
{
}

void Scene_04::Render()
{
	for (const auto& obj : objects)
	{
		if (obj != nullptr)
		{
			obj->Render();
		}
	}
}

void Scene_04::PostRender()
{
}

void Scene_04::ImGUI()
{
}

void Scene_04::ConfigureLoadSteps()
{
}

void Scene_04::OnLoadBegin()
{
	LoadableScene::OnLoadBegin();
	objects.clear();
}
