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
                HWND handle,
                UINT message,
                WPARAM wParam,
                LPARAM lParam
        );

        WinDesc desc;

        static std::unique_ptr<class Program> program;
};

