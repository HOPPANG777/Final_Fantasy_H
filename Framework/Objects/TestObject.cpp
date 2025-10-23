#include "Framework.h"
#include "TestObject.h"

/*
    Texture(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& scale,
        float rotation,
        const std::wstring& texturePath = L"",
        bool useRectTransform = false,
        const std::wstring& shaderPath = ShaderPath + L"VertexTexture.hlsl"
    );
    ~Texture() override = default;
*/

void TestObject::Update()
{
	__super::Update();
}

void TestObject::Render()
{
	__super::Render();
}
