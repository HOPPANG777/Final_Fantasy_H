#include "Framework.h"
#include "Texture.h"

//, texture(std::make_shared<Texture2D>(path))
Texture::Texture(const Vector3& position, const Vector3& scale, const float rotation, const wstring path, const TransformType type, const wstring shaderPath)
	: Drawable("Texture", position, scale, rotation, type, shaderPath)
{
	//vertices
	{
		vertices.assign(4, VertexTexture());

		vertices[0].position = { -0.5f, -0.5f, 0 };
		vertices[0].uv = Vector2(0, 1);

		vertices[1].position = { -0.5f, 0.5f, 0 };
		vertices[1].uv = Vector2(0, 0);

		vertices[2].position = { 0.5f, -0.5f, 0 };
		vertices[2].uv = Vector2(1, 1);

		vertices[3].position = { 0.5f, 0.5f, 0 };
		vertices[3].uv = Vector2(1, 0);
	}

	//vertexBuffer
	{
		vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//indices
	{
		indices = { 0, 1, 2, 2, 1, 3 };
	}

	//indexBuffer
	{
		indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
	}

	//inputLayout
	{
		inputLayout->Create(VertexTexture::ieDescs.data(), VertexTexture::ieDescs.size(), vertexShader->GetBlob());
	}

	// Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

		desc.CullMode = D3D11_CULL_BACK;
		//desc.CullMode = D3D11_CULL_NONE;

		desc.FillMode = D3D11_FILL_SOLID;

		desc.FrontCounterClockwise = false;

		HRESULT hr = DEVICE->CreateRasterizerState(&desc, rs.GetAddressOf());
		CHECK(hr);
	}

	// Sampler
	{
		D3D11_SAMPLER_DESC desc;
		RenderStates::GetSamplerDesc(&desc);
		RenderStates::CreateSampler(&desc, sampler[0].GetAddressOf());

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		RenderStates::CreateSampler(&desc, sampler[1].GetAddressOf());
	}

	// Blend
	{
		D3D11_BLEND_DESC desc;
		RenderStates::GetBlendDesc(&desc);
		RenderStates::CreateBlend(&desc, blend[0].GetAddressOf());

		desc.RenderTarget[0].BlendEnable = true;
		RenderStates::CreateBlend(&desc, blend[1].GetAddressOf());
	}

	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			DEVICE,
			path.c_str(),
			nullptr,
			nullptr,
			srv.GetAddressOf(),
			nullptr
		);
		CHECK(hr);
	}

	// ColorBuffer 
	{
		colorBuffer = make_unique<ColorBuffer>();
	}

}

Texture::~Texture()
{
}

void Texture::Update()
{
	BASE::Update();
}

void Texture::Render()
{
	BASE::Render();

	colorBuffer->SetPSBuffer(2);

	DC->PSSetSamplers(0, 1, sampler[1].GetAddressOf());
	
	DC->PSSetShaderResources(0, 1, srv.GetAddressOf());
	DC->RSSetState(rs.Get());

	DC->OMSetBlendState(blend[1].Get(), nullptr, (UINT)0xFFFFFFFF);

	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
