#pragma once

#include <memory>
#include <vector>

#include "../Interfaces/LoadableScene.h"

class GameObject;

// 스플래시 장면을 관리하는 구현 장면 클래스
class Scene_01 : public LoadableScene
{
public:
        // 장면이 생성될 때 초기 상태를 설정한다.
        void Awake() override;
        // 장면 입장 시 1회 실행되는 초기화 로직을 수행한다.
        void Start() override;

        // 장면 종료 시 자원을 정리한다.
        void Destroy() override;

        // 고정 주기 갱신을 처리한다.
        void FixedUpdate() override;
        // 매 프레임 갱신을 처리한다.
        void Update() override;
        // 후처리 갱신을 처리한다.
        void LateUpdate() override;

        // 렌더링을 수행한다.
        void Render() override;
        // 렌더링 후처리를 수행한다.
        void PostRender() override;

        // ImGui 기반 디버그 UI를 구성한다.
        void ImGUI() override;

protected:
        // 스플래시 장면의 로딩 단계를 구성한다.
        void ConfigureLoadSteps() override;
        // 로딩 시작 시 상태를 초기화한다.
        void OnLoadBegin() override;

private:
        std::vector<std::shared_ptr<GameObject>> objects; // 스플래시 장면에서 사용하는 오브젝트 목록

        bool _bSplashDone = false;                        // 스플래시 표시 완료 여부

        const float _targetSplashTime = 0.0f;             // 스플래시 종료 기준 시간
        float _currentSplashTime = 2.0f;                  // 남은 스플래시 표시 시간
};
