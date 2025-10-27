#pragma once

class AnimationClip
{
public:
    AnimationClip(
        const std::wstring& clipName,
        Texture2D* sourceTexture,
        UINT columns,
        UINT rows,
        const DirectX::SimpleMath::Vector2& startPos,
        const DirectX::SimpleMath::Vector2& endPos,
        bool reversed = false
    );

    const std::wstring& GetName() const;
    UINT GetFrameCount() const;
    bool IsReversed() const;
    const DirectX::SimpleMath::Vector2& GetFrameSize() const;
    const std::vector<DirectX::SimpleMath::Vector2>& GetFrames() const;
    DirectX::SimpleMath::Vector2 GetFrame(size_t frameIndex) const;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;

private:
    void BuildKeyFrames(
        UINT columns,
        UINT rows,
        const DirectX::SimpleMath::Vector2& startPos,
        const DirectX::SimpleMath::Vector2& endPos,
        float textureWidth,
        float textureHeight
    );

private:
    std::wstring _name;
    std::vector<DirectX::SimpleMath::Vector2> _keyFrames;
    UINT _frameCount = 0;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
    DirectX::SimpleMath::Vector2 _texelFrameSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    bool _reversed = false;
};
