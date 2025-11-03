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

// 렌더링 후처리는 폰트 출력등에 사용한다.
void Scene_02::PostRender()
{
	objects[2]->GetComponent<Texts>("Texts")->RenderText(
		L"불러오는중",
		objects[2]->GetRectTransform()->GetRect(),
		25.0f
	);

	objects[3]->GetComponent<TypingText>("TypingText")->RenderText(
		L"...",
		objects[3]->GetRectTransform()->GetRect(),
		35.0f,
		Values::White,
		L"Pretendard JP",
		DWRITE_TEXT_ALIGNMENT_LEADING
	);
}

// 로딩 장면의 디버그 UI를 제공한다.
void Scene_02::ImGUI()
{
	//if (!objects.empty() && objects[0] != nullptr)
	//{
	//	objects[0]->GUI();
	//}
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
				std::make_shared<LoadingObject>(
					"LoadingIcon",
					DirectX::SimpleMath::Vector3(130, 18, 0),
					Vector3(64, 64, 0),
					0.0f,
					TexturePath + L"Loading_Icon.png",
					true
				)
			);
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"LoadingText1",
					Values::ZeroVector3,
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[2]->AddComponent(std::make_shared<Texts>());
			objects[2]->GetRectTransform()->SetSize(Vector2(150, 0));
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"LoadingTypingText",
					Vector3(140, 0, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[3]->AddComponent(std::make_shared<TypingText>("TypingText", 0.5f));
			objects[3]->GetRectTransform()->SetSize(Vector2(150, 0));
			objects[3]->GetComponent<TypingText>("TypingText")->SetLoop(true);
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
