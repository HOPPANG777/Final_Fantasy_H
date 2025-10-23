struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

cbuffer WorldBuffer : register(b0)
{
    matrix _world;
}

cbuffer ViewProjectionBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
}

// 색상 변환 상수 버퍼 (슬롯 b2)
cbuffer ColorBuffer : register(b2)
{
    float4 Color; // 예를 들어, (1.0, 0.5, 0.5, 1.0) 등으로 지정
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    
    output.uv = input.uv;
    
    return output;
}

Texture2D sourceTex : register(t0);

SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 color = sourceTex.Sample(samp, (float2) input.uv);
	
    if (color.r >= 0.99f && color.g <= 0.01f && color.b >= 0.99f) discard;
	// discard는 색을 그리지 않고 넘기는 키워드
	// 그리지 않은 색은 투명으로 처리된다.
	
	// 텍스처의 RGBA 값에 변환 계수를 곱하여 색상 변경
    color *= Color;
	
    return color;
}