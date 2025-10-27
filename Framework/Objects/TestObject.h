#pragma once

class TestObject : public GameObject
{
public:
        //  ׽Ʈ Ʈ ϴ Դϴ.
        // name: Ϳ Ȯ Ʈ ̸Դϴ.
        // pos, scale, rotation: ġ ġ ũ, ȸ  մϴ.
        // texturePath:  ؽó Դϴ.   ⺻ ؽó մϴ.
        // useRectTransformType: true UI ǥ踦 ϵ RectTransform մϴ.
        TestObject(
                const std::string& name,
                const DirectX::SimpleMath::Vector3& pos,
                const DirectX::SimpleMath::Vector3& scale,
                float rotation,
                const std::wstring& texturePath = L"",
                bool useRectTransformType = false
        )
                : GameObject(name, pos, scale, rotation, useRectTransformType)
        {
                // ؽó  ϴ Texture Ʈ մϴ.
                this->AddComponent(std::make_shared<Texture>(pos, scale, rotation, texturePath, useRectTransformType));
        }

        ~TestObject() = default;

public:
        // GameObject ⺻ ۿ ߰  ʿϸ ⿡ մϴ.
        void Update() override;
        void Render() override;

};
