// RenderStates.cpp
#include "Framework.h"
#include "RenderStates.h"

using Microsoft::WRL::ComPtr;

D3D11_RASTERIZER_DESC    RenderStates::_rasterizerDesc = {};
D3D11_DEPTH_STENCIL_DESC RenderStates::_depthStencilDesc = {};
D3D11_SAMPLER_DESC       RenderStates::_samplerDesc = {};
D3D11_BLEND_DESC         RenderStates::_blendDesc = {};

void RenderStates::Init()
{
    BuildRasterizerDesc();
    BuildDepthStencilDesc();
    BuildSamplerDesc();
    BuildBlendDesc();
}

// ---------- Creators ----------
ComPtr<ID3D11RasterizerState>
RenderStates::CreateRasterizer(ID3D11Device* device, const D3D11_RASTERIZER_DESC& desc)
{
    ComPtr<ID3D11RasterizerState> state;
    const HRESULT hr = device->CreateRasterizerState(&desc, state.GetAddressOf());
    CHECK(hr);
    return state;
}

ComPtr<ID3D11DepthStencilState>
RenderStates::CreateDepthStencil(ID3D11Device* device, const D3D11_DEPTH_STENCIL_DESC& desc)
{
    ComPtr<ID3D11DepthStencilState> state;
    const HRESULT hr = device->CreateDepthStencilState(&desc, state.GetAddressOf());
    CHECK(hr);
    return state;
}

ComPtr<ID3D11SamplerState>
RenderStates::CreateSampler(ID3D11Device* device, const D3D11_SAMPLER_DESC& desc)
{
    ComPtr<ID3D11SamplerState> state;
    const HRESULT hr = device->CreateSamplerState(&desc, state.GetAddressOf());
    CHECK(hr);
    return state;
}

ComPtr<ID3D11BlendState>
RenderStates::CreateBlend(ID3D11Device* device, const D3D11_BLEND_DESC& desc)
{
    ComPtr<ID3D11BlendState> state;
    const HRESULT hr = device->CreateBlendState(&desc, state.GetAddressOf());
    CHECK(hr);
    return state;
}

// ---------- Defaults ----------
void RenderStates::BuildRasterizerDesc() noexcept
{
    _rasterizerDesc = {
        /* .FillMode                 */ D3D11_FILL_SOLID,
        /* .CullMode                 */ D3D11_CULL_BACK,
        /* .FrontCounterClockwise    */ FALSE,
        /* .DepthBias                */ 0,
        /* .DepthBiasClamp           */ 0.0f,
        /* .SlopeScaledDepthBias     */ 0.0f,
        /* .DepthClipEnable          */ TRUE,
        /* .ScissorEnable            */ FALSE,
        /* .MultisampleEnable        */ FALSE,
        /* .AntialiasedLineEnable    */ FALSE
    };
}

void RenderStates::BuildDepthStencilDesc() noexcept
{
    _depthStencilDesc = {};
    _depthStencilDesc.DepthEnable = TRUE;
    _depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    _depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    _depthStencilDesc.StencilEnable = TRUE;
    _depthStencilDesc.StencilReadMask = 0xFF;
    _depthStencilDesc.StencilWriteMask = 0xFF;

    _depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    _depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    _depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    _depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    _depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    _depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    _depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    _depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}

void RenderStates::BuildSamplerDesc() noexcept
{
    _samplerDesc = {};
    _samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    _samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    _samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    _samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    _samplerDesc.MipLODBias = 0.0f;
    _samplerDesc.MaxAnisotropy = 1;
    _samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    _samplerDesc.MinLOD = 0.0f;   // FLT_MIN  0.0f 
    _samplerDesc.MaxLOD = FLT_MAX;
}

void RenderStates::BuildBlendDesc() noexcept
{
    _blendDesc = {};
    _blendDesc.AlphaToCoverageEnable = FALSE;
    _blendDesc.IndependentBlendEnable = FALSE;

    auto& rt0 = _blendDesc.RenderTarget[0];
    rt0.BlendEnable = FALSE;
    rt0.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rt0.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rt0.BlendOp = D3D11_BLEND_OP_ADD;
    rt0.SrcBlendAlpha = D3D11_BLEND_ZERO;
    rt0.DestBlendAlpha = D3D11_BLEND_ONE;
    rt0.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rt0.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

