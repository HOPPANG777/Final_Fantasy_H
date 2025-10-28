#pragma once

#include "IScene.h"

#include <algorithm>
#include <functional>
#include <vector>

// 로딩 단계를 순차적으로 실행하며 진행률을 계산하는 기본 장면 클래스
class LoadableScene : public Scene
{
public:
    // 기본 생성자: 로딩 상태를 초기화한다.
    LoadableScene();
    ~LoadableScene() override = default;

    // 장면 로딩을 시작하며 단계 구성을 초기화한다.
    void BeginLoad() override;
    // 한 프레임의 로딩을 진행하여 다음 단계 실행 여부를 판단한다.
    void UpdateLoad(float deltaTime) override;
    // 누적된 로딩 진행률(0~1)을 반환한다.
    float GetLoadProgress() const override { return loadingProgress; }
    // 장면의 모든 로딩 단계가 완료되었는지 여부를 반환한다.
    bool IsLoaded() const override { return isLoaded; }

protected:
    // 로딩 단계에 사용할 함수 타입 정의.
    using LoadAction = std::function<void()>;

    // 로딩 단계 정보를 저장하는 구조체.
    struct LoadStep
    {
        LoadAction action;   // 실행할 작업 함수
        float weight = 1.0f; // 전체 진행률에서 차지하는 가중치
        bool executed = false; // 이미 실행되었는지 여부
    };

    // 로딩 단계를 추가하여 실행 목록에 등록한다.
    void AddLoadStep(const LoadAction& action, float weight = 1.0f);
    // 기존에 구성된 로딩 단계 목록을 비운다.
    void ClearLoadSteps();

    // 파생 클래스에서 로딩 단계를 구성하기 위해 반드시 구현해야 하는 함수.
    virtual void ConfigureLoadSteps() = 0;
    // 로딩이 시작될 때 파생 클래스에서 상태를 초기화하기 위해 선택적으로 재정의한다.
    virtual void OnLoadBegin();
    // 모든 로딩 단계가 완료되었을 때 파생 클래스에서 후처리를 위해 선택적으로 재정의한다.
    virtual void OnLoadCompleted();

private:
    // 등록된 로딩 단계 목록.
    std::vector<LoadStep> loadSteps;
    // 현재 실행 중인 단계의 인덱스.
    size_t currentStepIndex = 0;
    // 모든 단계의 가중치 합.
    float totalWeight = 0.0f;
    // 완료된 단계의 가중치 합.
    float completedWeight = 0.0f;
    // 0~1 범위의 로딩 진행률.
    float loadingProgress = 0.0f;
    // BeginLoad가 호출되어 단계 구성이 완료되었는지 여부.
    bool loadInitialized = false;
    // 장면이 완전히 로드되었는지 여부.
    bool isLoaded = false;
};

// 이하 구현부는 헤더에 정의하여 템플릿처럼 재사용한다.
inline LoadableScene::LoadableScene() = default;

inline void LoadableScene::BeginLoad()
{
    isLoaded = false;
    loadingProgress = 0.0f;
    completedWeight = 0.0f;
    currentStepIndex = 0;

    ClearLoadSteps();
    totalWeight = 0.0f;

    loadInitialized = true;

    OnLoadBegin();
    ConfigureLoadSteps();

    if (loadSteps.empty() || totalWeight <= 0.0f)
    {
        loadingProgress = 1.0f;
        isLoaded = true;
        OnLoadCompleted();
    }
}

inline void LoadableScene::UpdateLoad(float /*deltaTime*/)
{
    if (!loadInitialized || isLoaded || loadSteps.empty())
    {
        return;
    }

    LoadStep& currentStep = loadSteps[currentStepIndex];
    if (currentStep.executed)
    {
        return;
    }

    currentStep.action();
    currentStep.executed = true;

    completedWeight += currentStep.weight;

    if (totalWeight > 0.0f)
    {
        loadingProgress = CLAMP(completedWeight / totalWeight, 0.0f, 1.0f);
    }
    else
    {
        loadingProgress = 1.0f;
    }

    ++currentStepIndex;
    if (currentStepIndex >= loadSteps.size())
    {
        loadingProgress = 1.0f;
        isLoaded = true;
        OnLoadCompleted();
    }
}

inline void LoadableScene::AddLoadStep(const LoadAction& action, float weight)
{
    if (!action)
    {
        return;
    }

    const float clampedWeight = max(weight, 0.0f);
    if (clampedWeight <= 0.0f)
    {
        return;
    }

    loadSteps.push_back({ action, clampedWeight, false });
    totalWeight += clampedWeight;
}

inline void LoadableScene::ClearLoadSteps()
{
    loadSteps.clear();
}

inline void LoadableScene::OnLoadBegin()
{
}

inline void LoadableScene::OnLoadCompleted()
{
}
