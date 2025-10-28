#include "Framework.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip(
    const std::wstring& clipName,
    const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView,
    UINT columns,
    UINT rows,
    const DirectX::SimpleMath::Vector2& startPos,
    const DirectX::SimpleMath::Vector2& endPos,
    bool reversed)
    : _name(clipName), _shaderResourceView(shaderResourceView), _reversed(reversed)
{
    if (_shaderResourceView == nullptr)
    {
        return;
    }

    Microsoft::WRL::ComPtr<ID3D11Resource> resource;
    _shaderResourceView->GetResource(&resource);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
    const HRESULT conversionResult = resource.As(&texture2D);
    if (FAILED(conversionResult) || texture2D == nullptr)
    {
        return;
    }

    D3D11_TEXTURE2D_DESC textureDesc = {};
    texture2D->GetDesc(&textureDesc);

    const float textureWidth = static_cast<float>(textureDesc.Width);
    const float textureHeight = static_cast<float>(textureDesc.Height);

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
