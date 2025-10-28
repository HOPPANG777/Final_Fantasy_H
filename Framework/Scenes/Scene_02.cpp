#include "Framework.h"
#include "Scene_02.h"
#include "Objects/ObjectList.h"

using namespace DirectX;
using namespace SimpleMath;

// 로딩 장면은 Awake 단계에서 별도 처리가 필요 없다.
void Scene_02::Awake()
{
}

// 로딩 단계에서 오브젝트를 구성하므로 여기서는 처리할 내용이 없다.
void Scene_02::Start()
{
}

// 장면 종료 시 생성한 오브젝트를 정리한다.
void Scene_02::Destroy()
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

// 로딩 장면은 고정 갱신을 사용하지 않는다.
void Scene_02::FixedUpdate()
{
}

// 매 프레임마다 로딩 UI 오브젝트를 갱신한다.
void Scene_02::Update()
{
	for (const auto& obj : objects)
	{
		if (obj != nullptr)
		{
			obj->Update();
		}
	}
}

// 후처리 갱신은 사용하지 않는다.
void Scene_02::LateUpdate()
{
}

// 로딩 화면을 렌더링한다.
void Scene_02::Render()
{
	for (const auto& obj : objects)
	{
		if (obj != nullptr)
		{
			obj->Render();
		}
	}
}

// 렌더링 후처리는 사용하지 않는다.
void Scene_02::PostRender()
{
}

// 로딩 장면의 디버그 UI를 제공한다.
void Scene_02::ImGUI()
{
	if (!objects.empty() && objects[0] != nullptr)
	{
		objects[0]->GUI();
	}
}

// 로딩 장면에서 사용할 오브젝트를 구성한다.
void Scene_02::ConfigureLoadSteps()
{
	AddLoadStep([this]()
		{
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
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<AnimationObject>(
					"LoadingIcon",
					Values::ZeroVector3,
					Vector3(128, 128, 0),
					0.0f,
					TexturePath + L"Loading_Icon.png",
					true
				)
			);


			const auto& _loadingIconSprite = std::make_shared<Texture2D>(TexturePath + L"Loading_Icon.png");
			const auto& _loadingIcon = std::make_shared<AnimationClip>(
				L"_LoadingIcon",
				_loadingIconSprite.get(),
				5, 1,
				Values::ZeroVector2,
				Vector2((float)_loadingIconSprite->GetWidth(), (float)_loadingIconSprite->GetHeight()),
				false
			);

			objects[1]->GetComponent<AnimatedTexture>("AnimatedTexture")->GetAnimator()->AddAnimClip(_loadingIcon);
			objects[1]->GetComponent<AnimatedTexture>("AnimatedTexture")->GetAnimator()->SetAnimationTime(1.0f / 2.8f);
			objects[1]->GetComponent<AnimatedTexture>("AnimatedTexture")->GetAnimator()->SetCurrentAnimClip(L"_LoadingIcon");

		});

	AddLoadStep([]()
		{
			CameraManager::Get()->Reset();
		});
}

// 로딩 시작 시 기존 오브젝트를 정리한다.
void Scene_02::OnLoadBegin()
{
	LoadableScene::OnLoadBegin();
	objects.clear();
}
