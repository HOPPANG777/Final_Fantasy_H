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
