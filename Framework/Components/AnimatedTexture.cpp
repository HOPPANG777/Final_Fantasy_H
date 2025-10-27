// AnimatedTexture δ   Animator ¸ ݿ UV ǥ SRV Ѵ.
// ڵ  ɼ    Ʈ Animator Ʈ δ.

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
    Texture::Update();
    // ⺻ Texture     ִϸ͸ غѴ.

    InitializeAnimator();
    // ִϸͰ  غ ʾҴٸ     ʱȭѴ.

    // Animator    ͸ ޾ƿ´.
    const std::shared_ptr<Animator> animator = GetAnimator();
    if (!animator)
    {
        return;
    }

    // ð   Animator Ʈ   Ѵ.
    animator->Update();

    // Animator    ǥ ũ⸦ ޾ UV 缳Ѵ.
    const DirectX::SimpleMath::Vector2 framePosition = animator->GetCurrentFrame();
    const DirectX::SimpleMath::Vector2 frameSize = animator->GetTexelFrameSize();

    // Ʈ Ʈ  ش  ǵ UV Ѵ.
    UpdateFrameUVs(framePosition, frameSize);

    // Animator ϴ SRV ͼ   ؽó üѴ.
    const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> currentSrv = animator->GetCurrentSRV();
    SetSRV(currentSrv.Get());
}

std::shared_ptr<Animator> AnimatedTexture::GetAnimator() const
{
    // Animator  Ʈ پ ־ ϹǷ Owner  Ѵ.
    GameObject* owner = GetOwner();
    if (!owner)
    {
        return nullptr;
    }

    return owner->GetComponent<Animator>("Animator");
}

void AnimatedTexture::InitializeAnimator()
{
    //   ʱȭϵ ÷׸ ȮѴ.
    if (_animatorInitialized)
    {
        return;
    }

    GameObject* owner = GetOwner();
    if (!owner)
    {
        return;
    }

    // ڵ  ɼ   Animator Ʈ  ߰Ѵ.
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
    // ؽó ؽ ͸     UV ݿѴ.
    std::vector<VertexTexture>& vertices = AccessVertices();
    // 簢 ƴ  Ƿ ϰ ؽ  ȮѴ.
    if (vertices.size() < 4)
    {
        return;
    }

    // ϴ, », ϴ,   UV ǥ 缳Ѵ.
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

