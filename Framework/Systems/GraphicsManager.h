#pragma once

enum WindowManagerMode 
{ 
    WINDOW_WINDOW,
    WINDOW_BORDERLESS,
    WINDOW_EXCLUSIVE
};

class GraphicsManager
{
    DECLARE_SINGLETON(GraphicsManager);

    struct OutputInfo 
    {
        Microsoft::WRL::ComPtr<IDXGIOutput> output = nullptr;
        DXGI_OUTPUT_DESC outputDesc = {};
        UINT numerator = 0;
        UINT denominator = 1;
    };
    struct AdapterInfo 
    {
        UINT adapterOrdinal = 0;
        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter = nullptr;
        DXGI_ADAPTER_DESC1 adapterDesc = {};
        std::shared_ptr<OutputInfo> outputInfo = nullptr;
    };

public:
    void Init();
    void CreateBackBuffer();

public:
    ID3D11Device* GetDevice() const { return _device.Get(); }
    ID3D11DeviceContext* GetDC() const { return _deviceContext.Get(); }
    IDXGISwapChain* GetSwapChain() const { return _swapChain.Get(); }
    ID2D1DeviceContext* GetD2DC() const { return _d2DeviceContext.Get(); }
    IDWriteFactory* GetWrite() const { return _writeFactory.Get(); }

public:
    void Begin();
    void End();

    void GUI();

public:
    void SetFullScreen(bool fullscreen);                 //  (ͽŬú )
    void SetWindowManagerMode(WindowManagerMode mode);                 // / ȯ
    void SetResolution(UINT width, UINT height);         // Ÿ
    void SetVSync(bool on) { _bVsync = on; }

public:
    bool LoadCustomFont(const std::wstring& fontFilePath);
    bool UnloadCustomFont(const std::wstring& fontFilePath);

public:
    void LoadFont();
    void UnloadFont();

private:
    void EnumerateAdapters();
    bool EnumerateAdapterOutput(AdapterInfo* adapterInfo);

private:
    //  ܰ и
    void CreateDirect3D();
    void CreateDirect2D();
    void CreateDirectWrite();

public:
    //
    void RecreateDeviceAndSwapChain();   //   + ̽ + ü
    void RecreateTargetsFromSwapChain(); // RTV + D2D Target + Viewport

private:
    void ApplyFullScreen(bool fullscreen);
    void ApplyWindowManagerMode(WindowManagerMode mode);
    void ApplyResolution(UINT width, UINT height);
    void ProcessPendingStateChanges();

private:
    Microsoft::WRL::ComPtr<ID3D11Device> _device = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain = nullptr;

private:
    Microsoft::WRL::ComPtr<ID2D1Device> _d2Device = nullptr;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> _d2DeviceContext = nullptr;

    Microsoft::WRL::ComPtr<ID2D1Factory1> _d2Factory = nullptr;
    Microsoft::WRL::ComPtr<IDWriteFactory> _writeFactory = nullptr;

    Microsoft::WRL::ComPtr<ID2D1Bitmap1> _d2Bitmap = nullptr;

private:
    Microsoft::WRL::ComPtr<IDXGIDevice> _dxgiDevice = nullptr;
    Microsoft::WRL::ComPtr<IDXGISurface> _dxgiSurface = nullptr;

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _rtv = nullptr;
    D3D11_VIEWPORT _viewport = { 0 };
    
private:
    UINT _numerator = 0;
    UINT _denominator = 1;

    UINT _gpuMemorySize = 0;
    std::wstring _gpuName;

    std::vector<std::unique_ptr<AdapterInfo>> _adapterInfos;
    int _selectedAdapterIndex = 0;

    bool _bVsync = true;
    WindowManagerMode _WindowManagerMode = WindowManagerMode::WINDOW_WINDOW;

    bool isOpen = false;

    bool _pendingResolutionChange = false;
    UINT _pendingResolutionWidth = 0;
    UINT _pendingResolutionHeight = 0;

    bool _pendingFullscreenChange = false;
    bool _pendingFullscreenState = false;

    bool _pendingWindowModeChange = false;
    WindowManagerMode _pendingWindowMode = WindowManagerMode::WINDOW_WINDOW;
};
