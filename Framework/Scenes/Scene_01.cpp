#include "Framework.h"
#include "Scene_01.h"
#include "Objects/ObjectList.h"

using namespace DirectX;
using namespace SimpleMath;

void Scene_01::Awake()
{

}

void Scene_01::Start()
{
	isLoaded = false;

	objects.push_back(
		std::make_shared<ColorObject>(
			"SplashSceneBackground",
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
	const auto& _splashLogo = objects[1];
	
	// 스플래쉬 이펙트
	if (!_bSplashDone)
	{
		// 스플래쉬 이펙트 타이머
		_currentSplashTime -= TimeManager::Get()->GetDeltaTime();
		
		// 스플래쉬 로고에 타이머를 이용하여 알파값 제어
		_splashLogo->GetComponent<Texture>("Texture")->SetColor(Color(1.0f, 1.0f, 1.0f, _currentSplashTime));

		// 타이머가 모두 차면 효과 종료 후 타이틀 씬으로
		if (_currentSplashTime <= _targetSplashTime) {
			// 종료
			_bSplashDone = true;

			// 씬 로드
			SceneManager::Get()->LoadScene("TitleScene");
		}
	}

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

}
