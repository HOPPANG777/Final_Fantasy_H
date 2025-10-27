#pragma once

// AnimatedTexture Ʈ Texture ȮϿ Ʈ Ʈ  ִϸ̼ Ѵ.
// Animator    UV ҽ 並 ڵ üѴ.


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
    // Animator Ʈ غƴ Ȯϰ ʿ  Ѵ.
    void InitializeAnimator();
    //   ǥ  簢 UV ٽ Ѵ.
    void UpdateFrameUVs(const DirectX::SimpleMath::Vector2& framePosition, const DirectX::SimpleMath::Vector2& frameSize);

private:
    // true̸ Animator   ڵ ߰Ѵ.
    bool _autoCreateAnimator = true;
    // ߺ ʱȭ   ÷.
    bool _animatorInitialized = false;
};

