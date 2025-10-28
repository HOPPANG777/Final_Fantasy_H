#include "Framework.h"
#include "GraphicsManager.h"

GraphicsManager::GraphicsManager()
{
    EnumerateAdapters();
    Init();
}

GraphicsManager::~GraphicsManager()
{
    UnloadFont();
    if (_swapChain) _swapChain->SetFullscreenState(FALSE, nullptr);
}

void GraphicsManager::Init()
{
    LoadFont();
    RecreateDeviceAndSwapChain(); // 통합 경로
    CreateDirectWrite();
}

void GraphicsManager::RecreateDeviceAndSwapChain()
{
    // 기존 CreateDirect3D 논리 포함
    UINT maxVRam = 0;
    for (UINT i = 0; i < _adapterInfos.size(); i++)
    {
        auto& info = _adapterInfos[i];
        UINT64 vram = info->adapterDesc.DedicatedVideoMemory;
        if (vram > maxVRam) { _selectedAdapterIndex = (int)i; maxVRam = (UINT)vram; }
    }

    // 출력해보기
    for (size_t i = 0; i < _adapterInfos.size(); i++)
    {
        std::wcout << L"GPU Name : " << _adapterInfos[i]->adapterDesc.Description << std::endl;
        std::cout << "VRAM : " << _adapterInfos[i]->adapterDesc.DedicatedVideoMemory << std::endl;
        std::cout << std::endl;
    }

    std::wcout << L"Selected GPU Name : " << _adapterInfos[_selectedAdapterIndex]->adapterDesc.Description << std::endl;

    Microsoft::WRL::ComPtr<IDXGIAdapter1> chosenAdapter = _adapterInfos[_selectedAdapterIndex]->adapter;

    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferDesc.Width = 0;
    desc.BufferDesc.Height = 0;
    if (_bVsync)
    {
        desc.BufferDesc.RefreshRate.Numerator = _adapterInfos[0]->outputInfo->numerator;
        desc.BufferDesc.RefreshRate.Denominator = _adapterInfos[0]->outputInfo->denominator;
    }
    else
    {
        desc.BufferDesc.RefreshRate.Numerator = 60;
        desc.BufferDesc.RefreshRate.Denominator = 1;
    }
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 2; // 더블 버퍼
    desc.OutputWindow = gHandle;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 기존 유지(필요시 FLIP으로 변경 가능)
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    std::vector<D3D_FEATURE_LEVEL> featureLevels
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        chosenAdapter.Get(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        createDeviceFlags,
        featureLevels.data(),
        (UINT)featureLevels.size(),
        D3D11_SDK_VERSION,
        &desc,
        _swapChain.ReleaseAndGetAddressOf(),
        _device.ReleaseAndGetAddressOf(),
        nullptr,
        _deviceContext.ReleaseAndGetAddressOf()
    );
    CHECK(hr);

    // D2D는 여기서 재구성
    CreateDirect2D();

    // RTV/D2D 타깃/뷰포트 재설정
    RecreateTargetsFromSwapChain();
}

void GraphicsManager::RecreateTargetsFromSwapChain()
{
    _deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    if (_d2DeviceContext) _d2DeviceContext->SetTarget(nullptr);

    _rtv.Reset();
    _d2Bitmap.Reset();
    _dxgiSurface.Reset();

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
    CHECK(hr);

    hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _rtv.ReleaseAndGetAddressOf());
    CHECK(hr);

    _viewport.TopLeftX = 0.0f;
    _viewport.TopLeftY = 0.0f;
    _viewport.Width = gWinWidth;
    _viewport.Height = gWinHeight;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;

    _deviceContext->RSSetViewports(1, &_viewport);

    hr = _swapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)_dxgiSurface.GetAddressOf());
    CHECK(hr);

    D2D1_BITMAP_PROPERTIES1 props = {};
    props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    props.dpiX = 96.0f;
    props.dpiY = 96.0f;
    props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

    hr = _d2DeviceContext->CreateBitmapFromDxgiSurface(_dxgiSurface.Get(), &props, _d2Bitmap.ReleaseAndGetAddressOf());
    CHECK(hr);

    _d2DeviceContext->SetTarget(_d2Bitmap.Get());
}

void GraphicsManager::CreateBackBuffer()
{
    // 호출 호환성 유지. 실제 작업은 RecreateTargetsFromSwapChain에서 수행.
    RecreateTargetsFromSwapChain();
}

void GraphicsManager::Begin()
{
    _deviceContext->RSSetViewports(1, &_viewport);
    _deviceContext->OMSetRenderTargets(1, _rtv.GetAddressOf(), nullptr);
    _deviceContext->ClearRenderTargetView(_rtv.Get(), Values::Gray);

    _d2DeviceContext->BeginDraw();
}

void GraphicsManager::End()
{
    HRESULT D2hr = _d2DeviceContext->EndDraw();
    CHECK(D2hr);

    HRESULT hr = _swapChain->Present(_bVsync ? 1 : 0, 0);
    CHECK(hr);
}

void GraphicsManager::GUI()
{
    ImGui::Begin("Graphics", &isOpen);

    ImGui::Checkbox("Vsync", &_bVsync);

    ImGui::End();
}

void GraphicsManager::EnumerateAdapters()
{
    Microsoft::WRL::ComPtr<IDXGIFactory1> factory;
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory))) return;

    UINT index = 0;
    while (true)
    {
        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter = nullptr;
        HRESULT hr = factory->EnumAdapters1(index, &adapter);
        if (hr == DXGI_ERROR_NOT_FOUND) break;
        assert(SUCCEEDED(hr));

        std::unique_ptr<AdapterInfo> adapterInfo = std::make_unique<AdapterInfo>();
        adapterInfo->adapterOrdinal = index;
        adapter->GetDesc1(&adapterInfo->adapterDesc);
        adapterInfo->adapter = adapter;

        EnumerateAdapterOutput(adapterInfo.get());
        _adapterInfos.push_back(move(adapterInfo));
        index++;
    }
}

bool GraphicsManager::EnumerateAdapterOutput(AdapterInfo* adapterInfo)
{
    Microsoft::WRL::ComPtr<IDXGIOutput> output;
    HRESULT hr = adapterInfo->adapter->EnumOutputs(0, &output);
    if (DXGI_ERROR_NOT_FOUND == hr) return false;
    CHECK(hr);

    std::shared_ptr<OutputInfo> outputInfo = std::make_shared<OutputInfo>();
    output->GetDesc(&outputInfo->outputDesc);
    outputInfo->output = output;

    UINT numModes = 0;
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

    hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
    CHECK(hr);

    std::unique_ptr<DXGI_MODE_DESC[]> displayModes = std::make_unique<DXGI_MODE_DESC[]>(numModes);
    hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes.get());
    CHECK(hr);

    for (UINT i = 0; i < numModes; i++)
    {
        bool bCheck = true;
        bCheck &= displayModes[i].Width == Values::GetWinWidth();
        bCheck &= displayModes[i].Height == Values::GetWinHeight();
        if (bCheck)
        {
            outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
            outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
        }
    }
    adapterInfo->outputInfo = outputInfo;
    return true;
}

void GraphicsManager::CreateDirect3D() { /* 통합됨: RecreateDeviceAndSwapChain */ }

void GraphicsManager::CreateDirect2D()
{
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory1), nullptr, &_d2Factory);
    CHECK(hr);

    hr = _device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(_dxgiDevice.GetAddressOf()));
    CHECK(hr);

    hr = _d2Factory->CreateDevice(_dxgiDevice.Get(), &_d2Device);
    CHECK(hr);

    hr = _d2Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &_d2DeviceContext);
    CHECK(hr);
}

void GraphicsManager::CreateDirectWrite()
{
    HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(_writeFactory.GetAddressOf()));
    CHECK(hr);
}

void GraphicsManager::SetFullScreen(bool fullscreen)
{
    HRESULT hr = _swapChain->SetFullscreenState(fullscreen, nullptr);
    CHECK(hr);
}

void GraphicsManager::SetWindowManagerMode(WindowManagerMode mode)
{
    _WindowManagerMode = mode;
    
    if (_WindowManagerMode == WindowManagerMode::WINDOW_WINDOW)
    {
        _swapChain->SetFullscreenState(FALSE, nullptr);
    }
    else if (_WindowManagerMode == WindowManagerMode::WINDOW_BORDERLESS)
    {
        _swapChain->SetFullscreenState(FALSE, nullptr);
        // 보더리스는 윈도우 스타일/MoveWindowManager는 상위 레이어에서 처리
    }
    else // Exclusive
    {
        HRESULT hr = _swapChain->SetFullscreenState(TRUE, nullptr);
        CHECK(hr);
    }
}

void GraphicsManager::SetResolution(UINT width, UINT height)
{
    if (width == 0 || height == 0) return;

    gWinWidth = float(width);
    gWinHeight = float(height);
    CameraManager::Get()->SetProjection(gWinWidth, gWinHeight);

    RECT rect = { 0,0,(LONG)width,(LONG)height };
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
    UINT left = (GetSystemMetrics(SM_CXSCREEN) - width) / 2 + rect.left;
    UINT top = (GetSystemMetrics(SM_CYSCREEN) - height) / 2 + rect.top;
    MoveWindow(gHandle, left, top, rect.right - rect.left, rect.bottom - rect.top, TRUE);

    // 스왑체인 리사이즈 → 공통 재생성
    HRESULT hr = _swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    CHECK(hr);
    RecreateTargetsFromSwapChain();
}

bool GraphicsManager::LoadCustomFont(const std::wstring& fontFilePath)
{
    int ret = AddFontResourceExW(fontFilePath.c_str(), FR_PRIVATE, 0);

    if (ret == 0) return false;
    SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    return true;
}

bool GraphicsManager::UnloadCustomFont(const std::wstring& fontFilePath)
{
    int ret = RemoveFontResourceExW(fontFilePath.c_str(), FR_PRIVATE, 0);

    if (ret == 0) return false;
    SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    return true;
}

void GraphicsManager::LoadFont()
{
    LoadCustomFont(FontPath + L"PretendardJP-Medium.otf");
}

void GraphicsManager::UnloadFont()
{
    UnloadCustomFont(FontPath + L"PretendardJP-Medium.otf");
}
