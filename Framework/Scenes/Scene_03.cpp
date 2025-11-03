#include "Framework.h"
#include "Scene_03.h"
#include "Objects/ObjectList.h"

// 타이틀 장면은 Awake 단계에서 추가 초기화가 필요 없다.
void Scene_03::Awake()
{
}

// 로딩 단계에서 오브젝트를 구성하므로 Start에서는 별도 작업을 하지 않는다.
void Scene_03::Start()
{
}

// 장면 종료 시 생성한 오브젝트를 정리한다.
void Scene_03::Destroy()
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

// 고정 갱신 로직은 사용하지 않는다.
void Scene_03::FixedUpdate()
{
}

// 매 프레임 타이틀 장면의 오브젝트를 갱신한다.
void Scene_03::Update()
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
void Scene_03::LateUpdate()
{
}

// 타이틀 장면을 렌더링한다.
void Scene_03::Render()
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
void Scene_03::PostRender()
{
	objects[2]->GetComponent<Texts>("Texts")->RenderText(
		L"ⓒSQUARE ENIX Co., LTD. ALL Rights Reserved.\nLOGO & IMAGE ILLUSTRATION : ⓒYOSHITAKA AMANO",
		objects[2]->GetRectTransform()->GetRect(),
		20.0f,
		Values::Black,
		L"Pretendard JP"
	);

	objects[4]->GetComponent<BlinkingText>("BlinkingText")->RenderText(
		L"PRESS ANY BUTTON",
		objects[4]->GetRectTransform()->GetRect(),
		30.0f,
		Values::Black,
		L"Pretendard JP",
		DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
		DWRITE_FONT_WEIGHT_MEDIUM
	);

	objects[3]->GetComponent<BlinkingText>("BlinkingText")->RenderText(
		L"PRESS ANY BUTTON",
		objects[3]->GetRectTransform()->GetRect(),
		30.0f,
		DirectX::SimpleMath::Color(0x00ffff),
		L"Pretendard JP",
		DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
		DWRITE_FONT_WEIGHT_MEDIUM
	);


}

// 디버그 UI는 현재 사용하지 않는다.
void Scene_03::ImGUI()
{
	
}

// 타이틀 장면에서 사용할 오브젝트를 구성한다.
void Scene_03::ConfigureLoadSteps()
{
	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<ColorObject>(
					"TitleSceneBackground",
					Values::ZeroVector3,
					DirectX::SimpleMath::Vector3(Values::GetWinWidth(), Values::GetWinHeight(), 0),
					0.0f,
					Values::White,
					true
				)
			);
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<TextureObject>(
					"Title",
					DirectX::SimpleMath::Vector3(0, 150, 0),
					DirectX::SimpleMath::Vector3(706, 520, 0),
					0.0f,
					TexturePath + L"Final_Fantasy_5.png",
					true
				)
			);
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"CopyrightsText",
					DirectX::SimpleMath::Vector3(0, -305, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[2]->AddComponent(std::make_shared<Texts>());
			objects[2]->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(1000, 0));
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"PRESSANYBUTTON",
					DirectX::SimpleMath::Vector3(0, -150, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[3]->AddComponent(std::make_shared<BlinkingText>("BlinkingText", 1.5f));
			objects[3]->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(1000, 0));
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"PRESSANYBUTTONBackground",
					DirectX::SimpleMath::Vector3(2, -152, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[4]->AddComponent(std::make_shared<BlinkingText>("BlinkingText", 1.5f));
			objects[4]->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(1000, 0));
		});

	AddLoadStep([]()
		{
			CameraManager::Get()->Reset();
		});
}

// 로딩 시작 시 오브젝트를 초기화한다.
void Scene_03::OnLoadBegin()
{
	LoadableScene::OnLoadBegin();
	objects.clear();
}
