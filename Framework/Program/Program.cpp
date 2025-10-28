#include "Framework.h"
#include "Program.h"
#include "Scenes/SceneList.h"

float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;
HWND gHandle;

void Program::Awake()
{
        RenderStates::Init();

        Values::SetReferenceResolution(gWinWidth, gWinHeight);

        CameraManager::Get()->Init();

	// Scene 세팅
	{
		SceneManager::Get()->AddScene("TileMapTool", std::make_shared<Scene_MapTool>());
		SceneManager::Get()->AddScene("SplashScene", std::make_shared<Scene_01>());

		SceneManager::Get()->LoadScene("SplashScene");
	}
}

void Program::Start()
{
	SceneManager::Get()->Start();
}

void Program::Update()
{
	CameraManager::Get()->Update();

	SceneManager::Get()->Update();
}

void Program::Render()
{
	CameraManager::Get()->Render();

	SceneManager::Get()->Render();
}

void Program::PostRender()
{
	SceneManager::Get()->PostRender();
}

void Program::ImGUI()
{
	SceneManager::Get()->ImGUI();
}

int APIENTRY wWinMain
(
	_In_ HINSTANCE hInstance,		// 윈도우 실행을 위한 고유값
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,		// 커맨드 전달인자
	_In_ int       nCmdShow
)
{
	WinDesc desc;
	desc.appName = L"Final Fantasy H"; // 창 이름
	desc.instance = hInstance;
	desc.handle = nullptr;
	desc.width = gWinWidth;
	desc.height = gWinHeight;
	desc.icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FINALFANTASY));

	auto window = std::make_unique<WindowManager>(desc);
	WPARAM wParam = window->Run();

	return int(wParam);
}