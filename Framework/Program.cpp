#include "Framework.h"
#include "Program.h"

#include "WindowManager.h"

float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;
HWND gHandle;

void Program::Awake()
{
}

void Program::Start()
{
}

void Program::Update()
{
}

void Program::Render()
{
}

void Program::PostRender()
{
}

void Program::ImGUI()
{
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
	desc._appName = L"Final Fantasy H"; // 창 이름
	desc._instance = hInstance;
	desc._handle = nullptr;
	desc._width = gWinWidth;
	desc._height = gWinHeight;
	desc._icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FINALFANTASY));

	auto WindowManager = std::make_unique<WindowManager>(desc);
	WPARAM wParam = WindowManager->Run();

	return int(wParam);
}