#pragma once

//   ࿡ ʿ   ü.
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
        // : â ϰ  츦 Ѵ.
        WindowManager(const WinDesc& initDesc);
        // Ҹ:  ϵ Ŭ Ѵ.
        ~WindowManager();

        //  Ŭ Ѵ.   0 ȯѴ.
        ATOM MyRegisterClass(const WinDesc& desc);

        // ޽  鼭 α׷ Ѵ.
        WPARAM Run();

        // Է°     ƾ óѴ.
        void MainRender();

private:
        static LRESULT CALLBACK WndProc
        (
                // WindowManager ϴ  ڵ
                HWND handle,
                // ޽ ť ޵ ޽ ڵ
                UINT message,
                //  ߰ (Ű  )
                WPARAM wParam,
                //  Ǵ ǥ   ΰ Ű
                LPARAM lParam
        );

        WinDesc desc;

        //    ϴ Program νϽ.
        static std::unique_ptr<class Program> program;
};

