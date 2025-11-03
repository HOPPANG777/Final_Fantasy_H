#pragma once

#include <memory>
#include <vector>

#include "../Interfaces/LoadableScene.h"

class GameObject;
class SpriteText;

// 타이틀 장면을 담당하는 구현 장면 클래스
class Scene_03 : public LoadableScene
{
public:
	// 장면이 생성될 때 기본 상태를 준비한다.
	void Awake() override;
	// 장면 입장 시 초기화를 수행한다.
	void Start() override;

	// 장면이 종료될 때 자원을 반환한다.
	void Destroy() override;

	// 고정 주기 갱신을 처리한다.
	void FixedUpdate() override;
	// 매 프레임 갱신을 처리한다.
	void Update() override;
	// 업데이트 이후 후처리 로직을 수행한다.
	void LateUpdate() override;

	// 장면 렌더링을 수행한다.
	void Render() override;
	// 렌더링 후처리를 수행한다.
	void PostRender() override;

	// ImGui 디버그 UI를 구성한다.
	void ImGUI() override;

protected:
	// 타이틀 장면에서 사용할 로딩 단계를 구성한다.
	void ConfigureLoadSteps() override;
	// 로딩 시작 시 오브젝트를 초기화한다.
	void OnLoadBegin() override;

private:
	inline bool AnyMouseDown()
	{
		return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ||
			(GetAsyncKeyState(VK_RBUTTON) & 0x8000) ||
			(GetAsyncKeyState(VK_MBUTTON) & 0x8000) ||
			(GetAsyncKeyState(VK_XBUTTON1) & 0x8000) ||
			(GetAsyncKeyState(VK_XBUTTON2) & 0x8000);
	}

	inline bool AnyKeyboardDown()
	{
		for (int vk = 0; vk < 256; ++vk)
			if (GetAsyncKeyState(vk) & 0x8000) return true;
		return false;
	}

private:
	std::vector<std::shared_ptr<GameObject>> objects; // 타이틀 장면에서 사용하는 오브젝트 목록
	bool bPressButton = false;

	std::weak_ptr<SpriteText> pressAnySpriteText;
	DirectX::SimpleMath::Color pressAnyBaseColor = DirectX::SimpleMath::Color(0.0f, 1.0f, 1.0f, 1.0f);
	float pressAnyBlinkPeriod = 1.5f;

};
