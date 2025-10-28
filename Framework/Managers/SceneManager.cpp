#include "Framework.h"
#include "SceneManager.h"

#include <algorithm>

SceneManager::SceneManager()
{
        loadingProgress = 1.0f;
}

SceneManager::~SceneManager()
{
        for (auto& scene : sceneList)
        {
                if (scene.second != nullptr)
                {
                        scene.second->Destroy();
                }
        }
        sceneList.clear();
}

// 새로운 장면을 등록한다.
void SceneManager::AddScene(const std::string& sceneName, std::shared_ptr<Scene> newScene)
{
        if (sceneList.find(sceneName) != sceneList.end())
        {
                printf("Scene '%s' already exists.\n", sceneName.c_str());
                return;
        }

        sceneList[sceneName] = newScene;
}

// 장면 전환을 요청한다.
void SceneManager::LoadScene(const std::string& sceneName)
{
        const auto it = sceneList.find(sceneName);
        if (it == sceneList.end())
        {
                printf("Scene '%s' not found.\n", sceneName.c_str());
                return;
        }

        if (isLoading)
        {
                return;
        }

        nextScene = it->second;
        sceneChangeRequested = true;
        loadingProgress = 0.0f;
}

// 로딩 화면으로 사용할 장면을 지정한다.
void SceneManager::SetLoadingScene(const std::string& sceneName)
{
        const auto it = sceneList.find(sceneName);
        if (it != sceneList.end())
        {
                loadingScene = it->second;
        }
        else
        {
                printf("Loading Scene '%s' not found.\n", sceneName.c_str());
        }
}

// 스플래시 장면을 지정한다.
void SceneManager::SetSplashScene(const std::string& sceneName)
{
        const auto it = sceneList.find(sceneName);
        if (it != sceneList.end())
        {
                splashScene = it->second;
        }
        else
        {
                printf("Splash Scene '%s' not found.\n", sceneName.c_str());
        }
}

// 게임을 시작하면서 첫 장면을 준비한다.
void SceneManager::StartGame(const std::string& titleSceneName)
{
        if (!splashScene)
        {
                LoadScene(titleSceneName);
                return;
        }

        if (currentScene)
        {
                currentScene->Destroy();
                CameraManager::Get()->Reset();
        }

        currentScene = splashScene;
        BeginSceneLoad(currentScene);

        while (currentScene && !currentScene->IsLoaded())
        {
                currentScene->UpdateLoad(0.0f);
        }

        loadingProgress = 1.0f;
        sceneChangeRequested = false;
        nextScene.reset();
        isLoading = false;
}

// 현재 활성 장면의 Start를 호출한다.
void SceneManager::Start()
{
        if (isLoading)
        {
                return;
        }

        if (currentScene)
        {
                currentScene->Start();
        }
}

// 매 프레임마다 장면을 갱신한다.
void SceneManager::Update()
{
        if (isLoading)
        {
                if (loadingScene)
                {
                        loadingScene->Update();
                }

                if (nextScene)
                {
                        nextScene->UpdateLoad(TimeManager::Get()->GetDeltaTime());
                        loadingProgress = std::clamp(nextScene->GetLoadProgress(), 0.0f, 1.0f);

                        if (nextScene->IsLoaded())
                        {
                                if (loadingScene)
                                {
                                        loadingScene->Destroy();
                                }

                                if (currentScene)
                                {
                                        currentScene->Start();
                                }

                                nextScene.reset();
                                isLoading = false;
                                loadingProgress = 1.0f;
                        }
                }

                return;
        }

        if (currentScene)
        {
                currentScene->FixedUpdate();
                currentScene->Update();
                currentScene->LateUpdate();
        }

        ProcessSceneChange();
}

// 현재 상태에 맞게 렌더링한다.
void SceneManager::Render()
{
        if (isLoading)
        {
                if (loadingScene)
                {
                        loadingScene->Render();
                }
                return;
        }

        if (currentScene)
        {
                currentScene->Render();
        }
}

// 포스트 렌더링을 처리한다.
void SceneManager::PostRender()
{
        if (isLoading)
        {
                if (loadingScene)
                {
                        loadingScene->PostRender();
                }
                return;
        }

        if (currentScene)
        {
                currentScene->PostRender();
        }
}

// ImGui 디버그 UI를 렌더링한다.
void SceneManager::ImGUI()
{
        if (isLoading)
        {
                if (loadingScene)
                {
                        loadingScene->ImGUI();
                }
                return;
        }

        if (currentScene)
        {
                currentScene->ImGUI();
        }
}

// 장면 로딩을 시작하기 위한 공통 초기화 함수.
void SceneManager::BeginSceneLoad(const std::shared_ptr<Scene>& scene)
{
        if (!scene)
        {
                return;
        }

        scene->Awake();
        scene->BeginLoad();
}

// 로딩 화면을 준비하여 즉시 사용할 수 있도록 한다.
void SceneManager::PrepareLoadingScene()
{
        if (!loadingScene)
        {
                return;
        }

        loadingScene->Awake();
        loadingScene->BeginLoad();

        while (!loadingScene->IsLoaded())
        {
                loadingScene->UpdateLoad(0.0f);
        }

        loadingScene->Start();
}

// 로딩 장면 없이 즉시 전환을 완료한다.
void SceneManager::CompleteSceneWithoutLoading()
{
        if (!nextScene)
        {
                return;
        }

        if (currentScene)
        {
                currentScene->Destroy();
                CameraManager::Get()->Reset();
        }

        currentScene = nextScene;
        BeginSceneLoad(currentScene);

        while (currentScene && !currentScene->IsLoaded())
        {
                currentScene->UpdateLoad(0.0f);
        }

        currentScene->Start();

        nextScene.reset();
        sceneChangeRequested = false;
        loadingProgress = 1.0f;
}

// 지연된 장면 전환 요청을 처리한다.
void SceneManager::ProcessSceneChange()
{
        if (!sceneChangeRequested || !nextScene || isLoading)
        {
                return;
        }

        if (!loadingScene)
        {
                CompleteSceneWithoutLoading();
                return;
        }

        if (currentScene)
        {
                currentScene->Destroy();
                CameraManager::Get()->Reset();
        }

        BeginSceneLoad(nextScene);
        currentScene = nextScene;

        PrepareLoadingScene();

        isLoading = true;
        loadingProgress = 0.0f;
        sceneChangeRequested = false;
}
