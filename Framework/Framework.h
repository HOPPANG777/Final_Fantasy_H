// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

// 디버그 모드일 때
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") // 콘솔창을 띄어줌
#endif // #if나 #ifdef를 했다면 endif로 끝내야함

#include "resource.h"
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h> // lParam으로 부터 x, y를 분리 하기 위한 함수가 담긴 헤더 파일만 추가해줌 (위 define으로 사용 불가됨)
// C 런타임 헤더 파일입니다.
#include <cassert>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <commdlg.h>
#include <crtdbg.h>

// C++ 런타임 헤더 파일입니다.
#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <chrono>
#include <string>
#include <map>
#include <unordered_map>
#include <random>
#include <functional>
#include <fstream>
#include <ctime>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <future>
#include <sstream>
#include <exception>

//DirectX D3D11
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//Direct 2D, Direct Write
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <dwrite.h>
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//WRL(for ComPtr) // 스마트 포인트 같은 애며, 실제로 스마트 포인터 처럼 사용함
#include <wrl.h>

//DirectXTK
#include <directxtk/SimpleMath.h> // <>를 이용해서 폴더 찾아가서 사용 가능.

// DirectXTex
#include <DirectXTex.h>

//IMGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

//Macro
#define WIN_DEFAULT_WIDTH 1280
#define WIN_DEFAULT_HEIGHT 720

#define CENTER_X (WIN_DEFAULT_WIDTH / 2)
#define CENTER_Y (WIN_DEFAULT_HEIGHT / 2)
#define CENTER (Vector2(CENTER_X, CENTER_Y))

#define CHECK(hr) { if (FAILED(hr)) std::cout << "Error Code : 0x" << std::hex << hr << " - " << std::system_category().message(hr) << "\n"; assert(SUCCEEDED(hr)); }

#define BASE __super

#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] p; p = nullptr; } }

#define DECLARE_SINGLETON(CLASS_NAME)						\
private:													\
CLASS_NAME();												\
~CLASS_NAME();												\
public:														\
CLASS_NAME(const CLASS_NAME& other) = delete;				\
CLASS_NAME& operator=(const CLASS_NAME& other) = delete;	\
CLASS_NAME(const CLASS_NAME&& other) = delete;				\
CLASS_NAME& operator=(const CLASS_NAME&& other) = delete;	\
public:														\
static CLASS_NAME* Get()									\
{															\
	static CLASS_NAME instance;								\
	return &instance;										\
}

#define CLAMP(value, low, high) ((value) < (low) ? (low) : ((value) > (high) ? (high) : (value)))

//ExternGlobals
extern float gWinWidth;
extern float gWinHeight;
extern HWND gHandle;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Path
const std::wstring ShaderPath = L"_Shaders/";
const std::wstring TexturePath = L"_Textures/";
const std::wstring SoundPath = L"_Sounds/";
const std::wstring FontPath = L"_Fonts/";

//Interfaces
#include "IScene.h"
#include "IColorable.h"

//Utilities
#include "DirectHelper.h"
#include "Gizmo.h"
#include "Random.h"
#include "Gui.h"
#include "AnimationClip.h"
#include "Path.h"
#include "String.h"

//Systems
#include "KeyboardManager.h"
#include "MouseManager.h"
#include "TimeManager.h"
#include "GraphicsManager.h"

//Renders
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "VertexBuffer.h"

#include "ConstantBuffer.h"
#include "GlobalBuffers.h"
#include "Texture2D.h"
#include "VertexType.h"

#include "IShader.h"
#include "PixelShader.h"
#include "VertexShader.h"

#include "RenderStates.h"

//Components
#include "Transform.h"
#include "RectTransform.h"
#include "Animator.h"
#include "BoxCollider.h"
#include "Collider.h"
#include "TypingText.h"
#include "ShakingText.h"
#include "Texts.h"
#include "Component.h"

//Object
#include "GameObject.h"

//Camera
#include "CameraManager.h"
