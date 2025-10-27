#pragma once

class RenderStates final
{
public:
    static void Init();

    static const D3D11_RASTERIZER_DESC& GetRasterizerDesc()         noexcept { return _rasterizerDesc; }
    static const D3D11_DEPTH_STENCIL_DESC& GetDepthStencilDesc()    noexcept { return _depthStencilDesc; }
    static const D3D11_SAMPLER_DESC& GetSamplerDesc()               noexcept { return _samplerDesc; }
    static const D3D11_BLEND_DESC& GetBlendDesc()                   noexcept { return _blendDesc; }

    static Microsoft::WRL::ComPtr<ID3D11RasterizerState>
        CreateRasterizer(ID3D11Device* device, const D3D11_RASTERIZER_DESC& desc);

    static Microsoft::WRL::ComPtr<ID3D11DepthStencilState>
        CreateDepthStencil(ID3D11Device* device, const D3D11_DEPTH_STENCIL_DESC& desc);

    static Microsoft::WRL::ComPtr<ID3D11SamplerState>
        CreateSampler(ID3D11Device* device, const D3D11_SAMPLER_DESC& desc);

    static Microsoft::WRL::ComPtr<ID3D11BlendState>
        CreateBlend(ID3D11Device* device, const D3D11_BLEND_DESC& desc);

    static Microsoft::WRL::ComPtr<ID3D11RasterizerState>
        CreateDefaultRasterizer(ID3D11Device* device) { return CreateRasterizer(device, _rasterizerDesc); }

    static Microsoft::WRL::ComPtr<ID3D11DepthStencilState>
        CreateDefaultDepthStencil(ID3D11Device* device) { return CreateDepthStencil(device, _depthStencilDesc); }

    static Microsoft::WRL::ComPtr<ID3D11SamplerState>
        CreateDefaultSampler(ID3D11Device* device) { return CreateSampler(device, _samplerDesc); }

    static Microsoft::WRL::ComPtr<ID3D11BlendState>
        CreateDefaultBlend(ID3D11Device* device) { return CreateBlend(device, _blendDesc); }

private:
    static void BuildRasterizerDesc()    noexcept;
    static void BuildDepthStencilDesc()  noexcept;
    static void BuildSamplerDesc()       noexcept;
    static void BuildBlendDesc()         noexcept;

private:
    static D3D11_RASTERIZER_DESC    _rasterizerDesc;
    static D3D11_DEPTH_STENCIL_DESC _depthStencilDesc;
    static D3D11_SAMPLER_DESC       _samplerDesc;
    static D3D11_BLEND_DESC         _blendDesc;
};

