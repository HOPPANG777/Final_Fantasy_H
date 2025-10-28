#pragma once

class AnimationClip;

class Animator : public Component
{
public:
    explicit Animator(const std::string& componentName = "Animator");
    ~Animator() override = default;

    void Update() override;
    void Render() override;

    void GUI() override;

public:
    void AddAnimClip(const std::shared_ptr<AnimationClip>& animClip);
    void SetCurrentAnimClip(const std::wstring& clipName);

    DirectX::SimpleMath::Vector2 GetCurrentFrame() const;
    DirectX::SimpleMath::Vector2 GetTexelFrameSize() const;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetCurrentSRV() const;

    void SetAnimationTime(float animationTime);
    std::wstring GetCurrentClipName() const;

    void SetLoop(bool loop);
    bool IsFinished() const;

    void SetPlaybackSpeed(float speed);
    float GetPlaybackSpeed() const { return _playbackSpeed; }

    void SetNormalizedTime(float normalizedTime);
    float GetNormalizedTime() const;

    bool IsAnimationFinished() const;

private:
    void ApplyCurrentFrame();

private:
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
