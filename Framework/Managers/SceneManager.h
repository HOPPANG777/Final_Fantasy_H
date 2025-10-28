#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Scene;
class GameObject;

// 장면 전환과 로딩을 총괄하는 싱글톤 관리자
class SceneManager
{
    DECLARE_SINGLETON(SceneManager);

public:
    // 현재 활성 장면의 Start를 호출하여 실행을 시작한다.
    void Start();

    // 매 프레임마다 장면 갱신 및 로딩 상태를 처리한다.
    void Update();

    // 렌더링 단계를 현재 상태에 맞게 호출한다.
    void Render();
    void PostRender();

    // ImGui 디버그 인터페이스를 업데이트한다.
    void ImGUI();

public:
    // 장면 이름과 인스턴스를 등록한다.
    void AddScene(const std::string& sceneName, std::shared_ptr<Scene> newScene);
    // 지정한 장면으로 전환을 요청한다.
    void LoadScene(const std::string& sceneName);

    // 로딩 화면으로 사용할 장면을 설정한다.
    void SetLoadingScene(const std::string& sceneName);
    // 초기 실행 시 표시할 스플래시 장면을 설정한다.
    void SetSplashScene(const std::string& sceneName);
    // 게임을 시작하면서 첫 장면을 준비한다.
    void StartGame(const std::string& titleSceneName);

    // 로딩 중인지 여부를 반환한다.
    bool IsLoading() const { return isLoading; }
    // 현재 로딩 진행률(0~1)을 반환한다.
    float GetLoadingProgress() const { return loadingProgress; }

    // 플레이어 오브젝트를 등록 및 조회한다.
    void SetPlayer(std::shared_ptr<GameObject> player) { this->player = player; }
    std::shared_ptr<GameObject> GetPlayer() { return player; }
    void ResetPlayer() { this->player = nullptr; }

private:
    // 지연된 장면 전환 요청을 처리한다.
    void ProcessSceneChange();
    // 지정한 장면에 대해 Awake와 BeginLoad를 호출한다.
    void BeginSceneLoad(const std::shared_ptr<Scene>& scene);
    // 로딩 장면을 즉시 사용할 수 있도록 준비한다.
    void PrepareLoadingScene();
    // 로딩 장면 없이 즉시 장면을 전환한다.
    void CompleteSceneWithoutLoading();

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> sceneList; // 등록된 장면 목록
    std::shared_ptr<Scene> currentScene;                               // 현재 활성 장면
    std::shared_ptr<Scene> nextScene;                                  // 전환 대상 장면

    std::shared_ptr<Scene> loadingScene;                               // 로딩 화면 장면
    bool isLoading = false;                                            // 로딩 진행 여부
    float loadingProgress = 1.0f;                                      // 로딩 진행률

    std::shared_ptr<Scene> splashScene;                                // 초기 스플래시 장면

    bool sceneChangeRequested = false;                                 // 장면 전환 요청 상태

    std::shared_ptr<GameObject> player;                                // 현재 플레이어 오브젝트
};
