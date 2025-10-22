#pragma once

struct WinDesc
{
	std::wstring appName = L"";
	HINSTANCE instance = nullptr;
	HWND handle = nullptr;
	UINT width = 0;
	UINT height = 0;
	HICON icon = nullptr;
};

class WindowManager
{
public:
	WindowManager(const WinDesc& initDesc);
	~WindowManager();

	ATOM MyRegisterClass(const WinDesc& desc);

	WPARAM Run();

	void MainRender();

private:
	static LRESULT CALLBACK WndProc
	(
		// WindowManager handle 
		HWND handle,
		//Message Queue에서 받은 메시지
		UINT message,
		// 워드 파라미터(정수값)
		WPARAM wParam,
		//롱 파라미터(포인터)
		LPARAM lParam
	);

	WinDesc desc;

	static std::unique_ptr<class Program> program;
};

