#include "Framework.h"
#include "AnimatedTexture.h"

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
    __super::Update();

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

void AnimatedTexture::Render()
{
    __super::Render();
}

void AnimatedTexture::GUI()
{
    __super::GUI();
}

std::shared_ptr<Animator> AnimatedTexture::GetAnimator()
{
    InitializeAnimator();

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

    vertices[0]._uv.x = framePosition.x;
    vertices[0]._uv.y = framePosition.y + frameSize.y;

    vertices[1]._uv.x = framePosition.x;
    vertices[1]._uv.y = framePosition.y;

    vertices[2]._uv.x = framePosition.x + frameSize.x;
    vertices[2]._uv.y = framePosition.y + frameSize.y;

    vertices[3]._uv.x = framePosition.x + frameSize.x;
    vertices[3]._uv.y = framePosition.y;

    CommitVertices();
}

