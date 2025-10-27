#pragma once

class GameObject;

class SceneManager
{
    DECLARE_SINGLETON(SceneManager);    
public:
    void Start();

    void Update();

    void Render();
    void PostRender();

    void ImGUI();

public:
    void AddScene(const std::string& sceneName, std::shared_ptr<Scene> newScene);
    void LoadScene(const std::string& sceneName);

public:
    void SetLoadingScene(const std::string& sceneName);

    void SetSplashScene(const std::string& sceneName);
    void StartGame(const std::string& titleSceneName);

public:
	void SetPlayer(std::shared_ptr<GameObject> player) { this->player = player; }
	std::shared_ptr<GameObject> GetPlayer() { return player; }
	void ResetPlayer() { this->player = nullptr; }

private:
    void ProcessSceneChange();

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> sceneList;
    std::shared_ptr<Scene> currentScene;
    std::shared_ptr<Scene> nextScene;


    std::shared_ptr<Scene> loadingScene;
    bool isLoading = false;
    float loadingProgress = 0.0f;

    std::shared_ptr<Scene> splashScene;
    bool isFirstLaunch = true;

    bool sceneChangeRequested = false;

    std::shared_ptr<GameObject> player;

};

