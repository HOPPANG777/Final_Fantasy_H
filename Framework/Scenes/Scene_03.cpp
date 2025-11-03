#include "Framework.h"
#include "Scene_03.h"
#include "Objects/ObjectList.h"

// 타이틀 장면은 Awake 단계에서 추가 초기화가 필요 없다.
void Scene_03::Awake()
{
}

// 로딩 단계에서 오브젝트를 구성하므로 Start에서는 메뉴 구성을 초기화한다.
void Scene_03::Start()
{
	InitializeMenu();
}

// 장면 종료 시 생성한 오브젝트를 정리한다.
void Scene_03::Destroy()
{
	//pressAnySpriteText.reset();
	bPressButton = false;

	menuOptions.clear();
	currentMenuIndex = 0;
	menuCursorObject.reset();

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
	if (!bPressButton && GetForegroundWindow() == gHandle) // 창 활성 시에만
	{
		if (AnyMouseDown() || AnyKeyboardDown())
			bPressButton = true;
	}

	if (bPressButton && menuOptions.empty())
	{
		InitializeMenu();
	}

	if (bPressButton && !menuOptions.empty())
	{
		if (KeyboardManager::Get()->Down(VK_UP))
		{
			SelectMenu(-1);
		}
		else if (KeyboardManager::Get()->Down(VK_DOWN))
		{
			SelectMenu(1);
		}

		UpdateMenuCursor();
	}

	//if (auto spriteText = pressAnySpriteText.lock())
//{
//	if (bPressButton)
//	{
//		auto transparentColor = pressAnyBaseColor;
//		transparentColor.w = 0.0f;
//		spriteText->SetColor(transparentColor);
//	}
//	else
//	{
//		const float period = (pressAnyBlinkPeriod <= 0.0f) ? 1.0f : pressAnyBlinkPeriod;
//		const float elapsed = static_cast<float>(TimeManager::Get()->GetWorldTime());
//		const float phase = (elapsed / period) * DirectX::XM_2PI;
//		const float intensity = 0.5f * (std::sin(phase) + 1.0f);

//		auto blinkColor = pressAnyBaseColor;
//		blinkColor.w = CLAMP(intensity, 0.0f, 1.0f);
//		spriteText->SetColor(blinkColor);
//	}
//}

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

	if (bPressButton == true)
	{
		const DirectX::SimpleMath::Color disabledColor(0.0f, 0.0f, 0.0f, 0.5f);
		const DirectX::SimpleMath::Color highlightColor = Values::Green;

		for (size_t index = 0; index < menuOptions.size(); ++index)
		{
			const auto& option = menuOptions[index];
			if (option.objectIndex >= objects.size())
			{
				continue;
			}

			const auto& menuObject = objects[option.objectIndex];
			if (menuObject == nullptr)
			{
				continue;
			}

			const auto& textComponent = menuObject->GetComponent<Texts>("Texts");
			if (textComponent == nullptr)
			{
				continue;
			}

			DirectX::SimpleMath::Color textColor = option.disabled ? disabledColor : Values::Black;
			if (!option.disabled && index == currentMenuIndex)
			{
				textColor = highlightColor;
			}

			textComponent->RenderText(
				option.label,
				menuObject->GetRectTransform()->GetRect(),
				28.0f,
				textColor,
				L"Pretendard JP",
				DWRITE_TEXT_ALIGNMENT_CENTER,
				DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
				DWRITE_FONT_WEIGHT_BOLD
			);
		}

		if (const auto cursorObject = menuCursorObject.lock())
		{
			if (const auto& cursorText = cursorObject->GetComponent<Texts>("Texts"))
			{
				cursorText->RenderText(
					L"▶",
					cursorObject->GetRectTransform()->GetRect(),
					32.0f,
					highlightColor,
					L"Pretendard JP",
					DWRITE_TEXT_ALIGNMENT_CENTER,
					DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
					DWRITE_FONT_WEIGHT_BOLD
				);
			}
		}
	}

	if (bPressButton == false)
	{
		objects[3]->GetComponent<BlinkingText>("BlinkingText")->RenderText(
			L"PRESS ANY BUTTON",
			objects[3]->GetRectTransform()->GetRect(),
			30.0f,
			Values::Cyan,
			L"Pretendard JP",
			DWRITE_TEXT_ALIGNMENT_CENTER,
			DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
			DWRITE_FONT_WEIGHT_MEDIUM
		);
	}
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
					"Copyright",
					DirectX::SimpleMath::Vector3(0, -280, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			const auto& copyrightObject = objects.back();
			copyrightObject->AddComponent(std::make_shared<Texts>());
			copyrightObject->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(1000, 0));
		});

	//AddLoadStep([this]()
	//	{
	//		objects.push_back(
	//			std::make_shared<GameObject>(
	//				"PRESSANYBUTTON",
	//				DirectX::SimpleMath::Vector3(0, -120, 0),
	//				Values::ZeroVector3,
	//				0.0f,
	//				true
	//			)
	//		);

	//		const auto& spriteText = std::make_shared<SpriteText>(
	//			Values::ZeroVector3,
	//			Values::OneVector3,
	//			0.0f,
	//			TexturePath + L"FFPixelFont.png",
	//			true,
	//			DirectX::SimpleMath::Vector2(8.0f, 8.0f),
	//			16,
	//			L' '
	//		);

	//		std::wstring glyphOrder;
	//		glyphOrder.reserve(95);
	//		for (wchar_t character = L' '; character <= L'~'; ++character)
	//		{
	//			glyphOrder.push_back(character);
	//		}

	//		spriteText->SetGlyphOrder(glyphOrder);
	//		spriteText->SetText(L"PRESS ANY BUTTON");
	//		spriteText->SetSpacing(1.0f, 2.0f);
	//		spriteText->SetColor(pressAnyBaseColor);

	//		//const auto& pressAnyObject = objects.back();
	//		objects[3]->AddComponent(spriteText);
	//		objects[3]->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(1000, 0));
	//		pressAnySpriteText = spriteText;
	//	});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"PRESSANYBUTTON",
					DirectX::SimpleMath::Vector3(0, -120, 0),
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
					"Countinue",
					DirectX::SimpleMath::Vector3(0, -40, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[4]->AddComponent(std::make_shared<Texts>());
			objects[4]->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(300, 0));
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"Start",
					DirectX::SimpleMath::Vector3(0, -80, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[5]->AddComponent(std::make_shared<Texts>());
			objects[5]->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(300, 0));
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"Extra",
					DirectX::SimpleMath::Vector3(0, -120, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[6]->AddComponent(std::make_shared<Texts>());
			objects[6]->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(300, 0));
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"Option",
					DirectX::SimpleMath::Vector3(0, -160, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[7]->AddComponent(std::make_shared<Texts>());
			objects[7]->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(300, 0));
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"Quit",
					DirectX::SimpleMath::Vector3(0, -200, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects[8]->AddComponent(std::make_shared<Texts>());
			objects[8]->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(300, 0));
		});

	AddLoadStep([this]()
		{
			objects.push_back(
				std::make_shared<GameObject>(
					"MenuCursor",
					DirectX::SimpleMath::Vector3(-100, -80, 0),
					Values::ZeroVector3,
					0.0f,
					true
				)
			);

			objects.back()->AddComponent(std::make_shared<Texts>());
			objects.back()->GetRectTransform()->SetSize(DirectX::SimpleMath::Vector2(120, 0));
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
	menuOptions.clear();
	currentMenuIndex = 0;
	menuCursorObject.reset();
}

void Scene_03::InitializeMenu()
{
	menuOptions.clear();
	currentMenuIndex = 0;
	menuCursorObject.reset();

	if (objects.size() <= 9)
	{
		return;
	}

	menuOptions.push_back({ 4, L"불러오기", true });
	menuOptions.push_back({ 5, L"새 게임", false });
	menuOptions.push_back({ 6, L"엑스트라", false });
	menuOptions.push_back({ 7, L"옵션", false });
	menuOptions.push_back({ 8, L"종료", false });

	for (size_t index = 0; index < menuOptions.size(); ++index)
	{
		if (!menuOptions[index].disabled)
		{
			currentMenuIndex = index;
			break;
		}
	}

	for (const auto& object : objects)
	{
		if (object != nullptr && object->GetName() == "MenuCursor")
		{
			menuCursorObject = object;
			break;
		}
	}

	UpdateMenuCursor();
}

void Scene_03::SelectMenu(int direction)
{
	if (menuOptions.empty())
	{
		return;
	}

	const int count = static_cast<int>(menuOptions.size());
	int nextIndex = static_cast<int>(currentMenuIndex);

	for (int iteration = 0; iteration < count; ++iteration)
	{
		nextIndex += direction;

		if (nextIndex < 0)
		{
			nextIndex = count - 1;
		}
		else if (nextIndex >= count)
		{
			nextIndex = 0;
		}

		if (!menuOptions[static_cast<size_t>(nextIndex)].disabled)
		{
			currentMenuIndex = static_cast<size_t>(nextIndex);
			UpdateMenuCursor();
			break;
		}
	}
}

void Scene_03::UpdateMenuCursor()
{
	if (menuOptions.empty())
	{
		return;
	}

	if (currentMenuIndex >= menuOptions.size())
	{
		currentMenuIndex = 0;
	}

	const auto optionIndex = menuOptions[currentMenuIndex].objectIndex;
	if (optionIndex >= objects.size())
	{
		return;
	}

	const auto cursorObject = menuCursorObject.lock();
	if (!cursorObject)
	{
		return;
	}

	const auto& targetObject = objects[optionIndex];
	if (targetObject == nullptr)
	{
		return;
	}

	const auto& targetRect = targetObject->GetRectTransform();
	const auto& cursorRect = cursorObject->GetRectTransform();
	if (targetRect == nullptr || cursorRect == nullptr)
	{
		return;
	}

	DirectX::SimpleMath::Vector3 newPosition = targetRect->GetPosition() + menuCursorOffset;
	cursorRect->SetPosition(newPosition);
}
