#include "Framework.h"
#include "WindowManager.h"
#include "Program/Program.h"

// Program ̱ ü ʱȭ.
std::unique_ptr<Program> WindowManager::program = nullptr;

// :  Ŭ ϰ â /ʱȭѴ.
WindowManager::WindowManager(const WinDesc& initDesc)
        : desc(initDesc)
{
        //  Ŭ Ѵ.
        WORD wHr = MyRegisterClass(desc);
        assert(wHr != 0);

        //  츦 Ѵ.
        desc.handle = CreateWindowExW
        (
                WS_EX_APPWINDOW,
                desc.appName.c_str(),
                desc.appName.c_str(),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                nullptr,
                HMENU(nullptr),
                desc.instance,
                nullptr
        );

        // Ŭ̾Ʈ ũⰡ  ػ󵵿 ġϵ Ѵ.
        RECT rect = { 0, 0, LONG(desc.width), LONG(desc.height) };

        AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, 0);

        // ȭ ߾ӿ â  ġ Ѵ.
        UINT centerLeft = (GetSystemMetrics(SM_CXSCREEN) - UINT(desc.width)) / 2 + rect.left;
        UINT centerTop = (GetSystemMetrics(SM_CYSCREEN) - UINT(desc.height)) / 2 + rect.top;

        MoveWindow
        (
                desc.handle,
                centerLeft,
                centerTop,
                rect.right - rect.left,
                rect.bottom - rect.top,
                true
        );

        // â ȭ鿡 ǥϰ ޽ Ѵ.
        ShowWindow(desc.handle, SW_SHOWNORMAL);

        UpdateWindow(desc.handle);

        // 콺 Ŀ ̵ Ѵ.
        ShowCursor(true);
}

// Ҹ:     Ѵ.
WindowManager::~WindowManager()
{
        DestroyWindow(desc.handle);
        UnregisterClassW(desc.appName.c_str(), desc.instance);
}

//  Ŭ Ͽ ޽ ó ϵ Ѵ.
ATOM WindowManager::MyRegisterClass(const WinDesc& desc)
{
        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
        wcex.lpfnWndProc = &WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = desc.instance;
        wcex.hIcon = desc.icon;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = HBRUSH(GetStockObject(WHITE_BRUSH));
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = desc.appName.c_str();
        wcex.hIconSm = wcex.hIcon;

        return RegisterClassExW(&wcex);
}

// ޽  鼭 Program νϽ Ѵ.
WPARAM WindowManager::Run()
{
        program = std::make_unique<Program>();
        program->Awake();
        program->Start();

        MSG msg = { 0 };

        // ⺻ ޽ 
        while (true)
        {
                if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
                {
                        // WM_QUIT ޽   Ѵ.
                        if (msg.message == WM_QUIT) break;

                        // Ű  Է ޽  óѴ.
                        TranslateMessage(&msg);

                        // 콺 Է  Ѵ.
                        MouseManager::Get()->MouseProc(msg.message, msg.wParam, msg.lParam);

                        // ޽   ν Ѵ.
                        DispatchMessage(&msg);
                }
                else
                {
                        // ó ޽    Ѵ.
                        MainRender();
                }
        }

        return msg.wParam;
}

//   ȣǾ Է, Ʈ,  Ѵ.
void WindowManager::MainRender()
{
        KeyboardManager::Get()->Update();
        MouseManager::Get()->Update();
        TimeManager::Get()->Update();
        ImGuiManager::Get()->Update();

        program->Update();

        GraphicsManager::Get()->Begin();
        {
                program->Render();
                program->PostRender();

                program->ImGUI();
                ImGuiManager::Get()->Render();
        }
        GraphicsManager::Get()->End();
}

//  ν:   ޽ óѴ.
LRESULT WindowManager::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
        switch (message)
        {
        case WM_CREATE:
                gHandle = handle;
                break;
        case WM_SIZE:
                gWinWidth = float(GET_X_LPARAM(lParam));
                gWinHeight = float(GET_Y_LPARAM(lParam));
                break;
        case WM_DESTROY:
        case WM_CLOSE:
                //if (MessageBox(handle, L" Ͻðڽϱ?", L"WinAPI ", MB_OKCANCEL) == IDOK)
                PostQuitMessage(0);

                return 0;
        }
        return DefWindowProc(handle, message, wParam, lParam);
}
