// Animator δ  Ŭ ð  Ű ʿ  ĳѴ.
// ȭ ð Ͽ ApplyCurrentFrame  ε Ѵ.

#include "Framework.h"
#include "Animator.h"

Animator::Animator(const std::string& componentName)
    : Component(componentName)
{
}

void Animator::Update()
{
    //   Ŭ   ʿ䰡 .
    if (_currentClip == nullptr)
    {
        return;
    }

    //  ¿ ̹ ƴٸ  ̻  ʴ´.
    if (_finished && !_loop)
    {
        return;
    }

    //  ӵ 0̸ Ͻ  · Ѵ.
    if (_playbackSpeed == 0.0f)
    {
        return;
    }

    //  Ŭ   ȮѴ.
    //   0̸ ʱȭ  Ѵ.
    const UINT frameCount = _currentClip->GetFrameCount();
    if (frameCount == 0)
    {
        return;
    }

    //  Ӵ  ð    ü  ð Ѵ.
    const float clipDuration = _animationTime * static_cast<float>(frameCount);
    if (clipDuration <= 0.0f)
    {
        return;
    }

    const float deltaTime = TimeManager::Get()->GetDeltaTime();
    //  ð ȭ 0~1  ð ȯѴ.
    _normalizedTime += (_playbackSpeed * deltaTime) / clipDuration;

    //     ߴ Ȯϰ óѴ.
    if (_playbackSpeed > 0.0f && _normalizedTime >= 1.0f)
    {
        if (_loop)
        {
            _normalizedTime = 0.0f;
            _finished = false;
        }
        else
        {
            _normalizedTime = 1.0f;
            _finished = true;
            _playbackSpeed = 0.0f;
        }
    }
    //      ߴ ȮѴ.
    else if (_playbackSpeed < 0.0f && _normalizedTime <= 0.0f)
    {
        if (_loop)
        {
            _normalizedTime = 1.0f;
            _finished = false;
        }
        else
        {
            _normalizedTime = 0.0f;
            _finished = true;
            _playbackSpeed = 0.0f;
        }
    }

    // ȭ ð   ʵ Ѵ.
    if (_normalizedTime < 0.0f)
    {
        _normalizedTime = 0.0f;
    }
    else if (_normalizedTime > 1.0f)
    {
        _normalizedTime = 1.0f;
    }

    //  ȭ ð    Ѵ.
    ApplyCurrentFrame();
}

void Animator::Render()
{
}

void Animator::AddAnimClip(const std::shared_ptr<AnimationClip>& animClip)
{
    // null Ͱ ޵Ǹ Ѵ.
    if (!animClip)
    {
        return;
    }

    _animationClips[animClip->GetName()] = animClip;
}

void Animator::SetCurrentAnimClip(const std::wstring& clipName)
{
    // ̹  Ŭ  ̸ ʱȭ ʴ´.
    if (_currentClip && _currentClip->GetName() == clipName)
    {
        return;
    }

    // ϵ Ŭ Ͽ ̸ ˻Ѵ.
    const auto clipIt = _animationClips.find(clipName);
    if (clipIt == _animationClips.end())
    {
        assert(false && "Animator::SetCurrentAnimClip: Clip not found");
        return;
    }

    // ˻ ϸ  Ŭ üѴ.
    _currentClip = clipIt->second;

    _normalizedTime = 0.0f;
    _finished = false;
    _playbackSpeed = 1.0f;

    //   ʿ Ŭ̸ ʱ ¿   ´.
    if (_currentClip->IsReversed())
    {
        _normalizedTime = 1.0f;
        _playbackSpeed = -1.0f;
    }

    ApplyCurrentFrame();
}

DirectX::SimpleMath::Vector2 Animator::GetCurrentFrame() const
{
    return _currentFrame;
}

DirectX::SimpleMath::Vector2 Animator::GetTexelFrameSize() const
{
    //  Ŭ   ȿ  ȯѴ.
    if (_currentClip)
    {
        return _currentClip->GetFrameSize();
    }

    // Ŭ  ⺻ ȯѴ.
    return DirectX::SimpleMath::Vector2(0.0f, 0.0f);
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Animator::GetCurrentSRV() const
{
    if (_currentClip)
    {
        return _currentClip->GetShaderResourceView();
    }

    // Ŭ  ؽó ҽ .
    return nullptr;
}

void Animator::SetAnimationTime(float animationTime)
{
    _animationTime = animationTime;
}

std::wstring Animator::GetCurrentClipName() const
{
    if (_currentClip)
    {
        return _currentClip->GetName();
    }

    return L"";
}

void Animator::SetLoop(bool loop)
{
    _loop = loop;
    //  °   ¸ ʱȭѴ.

    if (_loop)
    {
        _finished = false;
    }
}

bool Animator::IsFinished() const
{
    // ⺻δ ο   ¸ ȯѴ.
    return _finished;
}

void Animator::SetPlaybackSpeed(float speed)
{
    _playbackSpeed = speed;
    // ӵ ٽ 0 ƴ  Ǹ  ÷׸ Ѵ.

    if (_playbackSpeed != 0.0f)
    {
        _finished = false;
    }
}

void Animator::SetNormalizedTime(float normalizedTime)
{
    // ܺο  ȭ ð   Ѵ.
    _normalizedTime = normalizedTime;

    if (_normalizedTime < 0.0f)
    {
        _normalizedTime = 0.0f;
    }
    else if (_normalizedTime > 1.0f)
    {
        _normalizedTime = 1.0f;
    }

    _finished = false;

    ApplyCurrentFrame();
}

float Animator::GetNormalizedTime() const
{
    return _normalizedTime;
}

bool Animator::IsAnimationFinished() const
{
    //  Ŭ  ڵ  · Ѵ.
    if (_currentClip == nullptr)
    {
        return true;
    }

    if (_playbackSpeed > 0.0f)
    {
        return (_normalizedTime >= 1.0f);
    }

    if (_playbackSpeed < 0.0f)
    {
        return (_normalizedTime <= 0.0f);
    }

    return _finished;
}

void Animator::ApplyCurrentFrame()
{
    if (_currentClip == nullptr)
    {
        _currentFrameIndex = 0;
        _currentFrame = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        return;
    }

    const UINT frameCount = _currentClip->GetFrameCount();
    if (frameCount == 0)
    {
        _currentFrameIndex = 0;
        _currentFrame = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        return;
    }

    // ȭ ð   ε ȯѴ.
    const float rawIndex = _normalizedTime * static_cast<float>(frameCount - 1);
    UINT frameIndex = static_cast<UINT>(rawIndex + 0.5f);

    // 迭   ʵ   Ѵ.
    if (frameIndex >= frameCount)
    {
        frameIndex = frameCount - 1;
    }

    _currentFrameIndex = frameIndex;
    _currentFrame = _currentClip->GetFrame(static_cast<size_t>(_currentFrameIndex));
}
