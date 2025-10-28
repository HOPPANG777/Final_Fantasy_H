#include "Framework.h"
#include "Scene_01.h"
#include "Objects/ObjectList.h"

using namespace DirectX;
using namespace SimpleMath;

// 장면 준비 단계에서 기본 상태를 초기화한다.
void Scene_01::Awake()
{
        _bSplashDone = false;
        _currentSplashTime = 2.0f;
}

// 로딩 단계에서 오브젝트를 구성하므로 별도 처리가 필요 없다.
void Scene_01::Start()
{
}

// 장면 종료 시 생성한 오브젝트를 정리한다.
void Scene_01::Destroy()
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

// 스플래시 장면은 고정 갱신 로직을 사용하지 않는다.
void Scene_01::FixedUpdate()
{
}

// 매 프레임마다 페이드 아웃 타이머와 오브젝트를 갱신한다.
void Scene_01::Update()
{
        if (objects.size() >= 2)
        {
                const auto& splashLogo = objects[1];

                // 스플래시 연출이 끝나지 않았다면 알파 값을 줄여준다.
                if (!_bSplashDone)
                {
                        _currentSplashTime -= TimeManager::Get()->GetDeltaTime();

                        splashLogo->GetComponent<Texture>("Texture")->SetColor(Color(1.0f, 1.0f, 1.0f, _currentSplashTime));

                        // 목표 시간에 도달하면 다음 장면을 요청한다.
                        if (_currentSplashTime <= _targetSplashTime)
                        {
                                _bSplashDone = true;
                                SceneManager::Get()->LoadScene("TitleScene");
                        }
                }
        }

        for (const auto& obj : objects)
        {
                if (obj != nullptr)
                {
                        obj->Update();
                }
        }
}

// 후처리 갱신 로직은 사용하지 않는다.
void Scene_01::LateUpdate()
{
}

// 스플래시 장면의 렌더링을 수행한다.
void Scene_01::Render()
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
void Scene_01::PostRender()
{
}

// 디버그 UI는 현재 사용하지 않는다.
void Scene_01::ImGUI()
{
}

// 로딩 단계에서 실행할 작업을 정의한다.
void Scene_01::ConfigureLoadSteps()
{
        AddLoadStep([this]()
        {
                objects.push_back(
                        std::make_shared<ColorObject>(
                                "SplashSceneBackground",
                                Values::ZeroVector3,
                                Vector3(Values::GetWinWidth(), Values::GetWinHeight(), 0),
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
                                "Logo",
                                Values::ZeroVector3,
                                Vector3(640, 67, 0),
                                0.0f,
                                TexturePath + L"Square_Enix_logo.png",
                                true
                        )
                );
        });

        AddLoadStep([]()
        {
                CameraManager::Get()->Reset();
        });
}

// 로딩이 시작될 때 상태와 오브젝트를 초기화한다.
void Scene_01::OnLoadBegin()
{
        LoadableScene::OnLoadBegin();

        objects.clear();
        _bSplashDone = false;
        _currentSplashTime = 2.0f;
}
