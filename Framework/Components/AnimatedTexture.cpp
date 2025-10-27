#include "Framework.h"
#include "AnimatedTexture.h"

#include "../_Misc/Animator.h"

AnimatedTexture::AnimatedTexture(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& scale,
    float rotation,
    const std::wstring& texturePath,
    bool useRectTransform,
    bool autoCreateAnimator)
    : Texture(position, scale, rotation, texturePath, useRectTransform, "AnimatedTexture"),
      _autoCreateAnimator(autoCreateAnimator)
{
}

void AnimatedTexture::Update()
{
    Texture::Update();

    InitializeAnimator();

    const std::shared_ptr<Animator> animator = GetAnimator();
    if (!animator)
    {
        return;
    }

    animator->Update();

    const DirectX::SimpleMath::Vector2 framePosition = animator->GetCurrentFrame();
    const DirectX::SimpleMath::Vector2 frameSize = animator->GetTexelFrameSize();

    UpdateFrameUVs(framePosition, frameSize);

    const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> currentSrv = animator->GetCurrentSRV();
    SetSRV(currentSrv.Get());
}

std::shared_ptr<Animator> AnimatedTexture::GetAnimator() const
{
    GameObject* owner = GetOwner();
    if (!owner)
    {
        return nullptr;
    }

    return owner->GetComponent<Animator>("Animator");
}

void AnimatedTexture::InitializeAnimator()
{
    if (_animatorInitialized)
    {
        return;
    }

    GameObject* owner = GetOwner();
    if (!owner)
    {
        return;
    }

    if (_autoCreateAnimator)
    {
        std::shared_ptr<Animator> animator = owner->GetComponent<Animator>("Animator");
        if (!animator)
        {
            animator = std::make_shared<Animator>("Animator");
            owner->AddComponent(animator);
        }
    }

    _animatorInitialized = true;
}

void AnimatedTexture::UpdateFrameUVs(
    const DirectX::SimpleMath::Vector2& framePosition,
    const DirectX::SimpleMath::Vector2& frameSize)
{
    std::vector<VertexTexture>& vertices = AccessVertices();
    if (vertices.size() < 4)
    {
        return;
    }

    vertices[0].uv.x = framePosition.x;
    vertices[0].uv.y = framePosition.y + frameSize.y;

    vertices[1].uv.x = framePosition.x;
    vertices[1].uv.y = framePosition.y;

    vertices[2].uv.x = framePosition.x + frameSize.x;
    vertices[2].uv.y = framePosition.y + frameSize.y;

    vertices[3].uv.x = framePosition.x + frameSize.x;
    vertices[3].uv.y = framePosition.y;

    CommitVertices();
}

