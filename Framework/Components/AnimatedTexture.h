#pragma once



#include "Texture.h"

class Animator;

class AnimatedTexture : public Texture
{
public:
    AnimatedTexture(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& scale,
        float rotation,
        const std::wstring& texturePath = L"",
        bool useRectTransform = false,
        bool autoCreateAnimator = true
    );

    ~AnimatedTexture() override = default;

    void Update() override;

    std::shared_ptr<Animator> GetAnimator() const;

private:
    void InitializeAnimator();
    void UpdateFrameUVs(const DirectX::SimpleMath::Vector2& framePosition, const DirectX::SimpleMath::Vector2& frameSize);

private:
    bool _autoCreateAnimator = true;
    bool _animatorInitialized = false;
};

