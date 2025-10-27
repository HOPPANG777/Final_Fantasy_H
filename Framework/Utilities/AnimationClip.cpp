#include "Framework.h"
#include "AnimationClip.h"

#include "../_Misc/Texture2D.h"

AnimationClip::AnimationClip(
    const std::wstring& clipName,
    Texture2D* sourceTexture,
    UINT columns,
    UINT rows,
    const DirectX::SimpleMath::Vector2& startPos,
    const DirectX::SimpleMath::Vector2& endPos,
    bool reversed)
    : _name(clipName),
      _reversed(reversed)
{
    if (sourceTexture == nullptr)
    {
        return;
    }

    _shaderResourceView = sourceTexture->GetSRV();

    const float textureWidth = sourceTexture->GetWidth();
    const float textureHeight = sourceTexture->GetHeight();

    if (textureWidth <= 0.0f || textureHeight <= 0.0f)
    {
        return;
    }

    BuildKeyFrames(columns, rows, startPos, endPos, textureWidth, textureHeight);
}

const std::wstring& AnimationClip::GetName() const
{
    return _name;
}

UINT AnimationClip::GetFrameCount() const
{
    return _frameCount;
}

bool AnimationClip::IsReversed() const
{
    return _reversed;
}

const DirectX::SimpleMath::Vector2& AnimationClip::GetFrameSize() const
{
    return _texelFrameSize;
}

const std::vector<DirectX::SimpleMath::Vector2>& AnimationClip::GetFrames() const
{
    return _keyFrames;
}

DirectX::SimpleMath::Vector2 AnimationClip::GetFrame(size_t frameIndex) const
{
    if (_keyFrames.empty())
    {
        return DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    }

    if (frameIndex >= _keyFrames.size())
    {
        frameIndex = _keyFrames.size() - 1;
    }

    return _keyFrames[frameIndex];
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> AnimationClip::GetShaderResourceView() const
{
    return _shaderResourceView;
}

void AnimationClip::BuildKeyFrames(
    UINT columns,
    UINT rows,
    const DirectX::SimpleMath::Vector2& startPos,
    const DirectX::SimpleMath::Vector2& endPos,
    float textureWidth,
    float textureHeight)
{
    _keyFrames.clear();
    _frameCount = 0;
    _texelFrameSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);

    if (columns == 0 || rows == 0)
    {
        return;
    }

    const DirectX::SimpleMath::Vector2 clipSize = endPos - startPos;
    const float frameWidth = clipSize.x / static_cast<float>(columns);
    const float frameHeight = clipSize.y / static_cast<float>(rows);

    _texelFrameSize.x = frameWidth / textureWidth;
    _texelFrameSize.y = frameHeight / textureHeight;

    DirectX::SimpleMath::Vector2 texelStartPos;
    texelStartPos.x = startPos.x / textureWidth;
    texelStartPos.y = startPos.y / textureHeight;

    _keyFrames.reserve(static_cast<size_t>(columns) * static_cast<size_t>(rows));

    for (UINT row = 0; row < rows; ++row)
    {
        for (UINT column = 0; column < columns; ++column)
        {
            DirectX::SimpleMath::Vector2 frame;
            frame.x = texelStartPos.x + _texelFrameSize.x * static_cast<float>(column);
            frame.y = texelStartPos.y + _texelFrameSize.y * static_cast<float>(row);

            _keyFrames.emplace_back(frame);
        }
    }

    _frameCount = static_cast<UINT>(_keyFrames.size());
}
