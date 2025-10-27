#pragma once

// Animator Ʈ AnimationClip ϸ ȭ ð   Ѵ.
// , ӵ,  ġ  AnimatedTexture     Ѵ.


class AnimationClip;

class Animator : public Component
{
public:
    explicit Animator(const std::string& componentName = "Animator");
    ~Animator() override = default;

    void Update() override;
    void Render() override;

    // ִϸ̼ Ŭ   
    void AddAnimClip(const std::shared_ptr<AnimationClip>& animClip);
    void SetCurrentAnimClip(const std::wstring& clipName);

    //      ȸ
    DirectX::SimpleMath::Vector2 GetCurrentFrame() const;
    DirectX::SimpleMath::Vector2 GetTexelFrameSize() const;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetCurrentSRV() const;

    //  ð  ɼ 
    void SetAnimationTime(float animationTime);
    std::wstring GetCurrentClipName() const;

    //    ӵ 
    void SetLoop(bool loop);
    bool IsFinished() const;

    void SetPlaybackSpeed(float speed);
    float GetPlaybackSpeed() const { return _playbackSpeed; }

    // ܺο ð ġ ϰų Ȯ
    void SetNormalizedTime(float normalizedTime);
    float GetNormalizedTime() const;

    bool IsAnimationFinished() const;

private:
    //    ε Ѵ.
    void ApplyCurrentFrame();

private:
    // ִϸ̼  ¸ ϴ  
    std::unordered_map<std::wstring, std::shared_ptr<AnimationClip>> _animationClips;
    std::shared_ptr<AnimationClip> _currentClip = nullptr;

    UINT _currentFrameIndex = 0;
    DirectX::SimpleMath::Vector2 _currentFrame = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    float _animationTime = 1.0f / 15.0f;

    bool _loop = true;
    bool _finished = false;

    float _playbackSpeed = 1.0f;
    float _normalizedTime = 0.0f;
};
