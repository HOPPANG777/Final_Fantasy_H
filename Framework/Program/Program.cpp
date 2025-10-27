#include "Framework.h"
#include "Program.h"
#include "Scenes/SceneList.h"


float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;
HWND gHandle;

void Program::Awake()
{
	RenderStates::Init();

	CameraManager::Get()->Init();

	// Scene 
	{
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
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow
)
{
	WinDesc desc;
	desc.appName = L"Final Fantasy H";
	desc.instance = hInstance;
	desc.handle = nullptr;
	desc.width = gWinWidth;
	desc.height = gWinHeight;
	desc.icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FINALFANTASY));

	auto window = std::make_unique<WindowManager>(desc);
	WPARAM wParam = window->Run();

	return int(wParam);
}