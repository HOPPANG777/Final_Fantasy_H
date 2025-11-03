#pragma once

class LoadingObject : public AnimationObject
{
public:
	LoadingObject(
		const std::string& name,
		const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector3& scale,
		float rotation,
		const std::wstring& texturePath = L"",
		bool useRectTransformType = false
	)
		: AnimationObject(name, pos, scale, rotation, texturePath, useRectTransformType)
	{
                const auto& animatedTexture = this->GetComponent<AnimatedTexture>("AnimatedTexture");
                if (!animatedTexture)
                {
                        return;
                }

                const auto& animator = animatedTexture->GetAnimator();
                if (!animator)
                {
                        return;
                }

                const auto& _loadingIconSprite = std::make_shared<Texture2D>(TexturePath + L"Loading_Icon.png");
                const auto& _loadingIcon = std::make_shared<AnimationClip>(
                        L"_LoadingIcon",
                        _loadingIconSprite,
                        5,
                        1,
                        Values::ZeroVector2,
                        DirectX::SimpleMath::Vector2((float)_loadingIconSprite->GetWidth(), (float)_loadingIconSprite->GetHeight()),
                        true
                );

                animator->AddAnimClip(_loadingIcon);
                animator->SetAnimationTime(1.0f / 1.3f);
                animator->SetCurrentAnimClip(L"_LoadingIcon");
        }

	~LoadingObject() = default;

public:
	void Update() override
	{
		__super::Update();
	}

	void Render() override
	{
		__super::Render();
	}

	void GUI() override
	{
		__super::GUI();
	}

};

