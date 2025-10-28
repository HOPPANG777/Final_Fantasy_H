#include "Framework.h"
#include "SpriteTexture.h"

namespace
{
    // 파일 확장자를 소문자로 정규화
    std::wstring NormalizeExtension(const std::wstring& extension)
    {
        std::wstring normalized = extension;
        std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](wchar_t character)
        {
            return static_cast<wchar_t>(std::towlower(character));
        });
        return normalized;
    }

    // 메타데이터에서 텍스처 크기를 추출
    DirectX::SimpleMath::Vector2 ExtractTextureSize(const DirectX::TexMetadata& metadata)
    {
        return DirectX::SimpleMath::Vector2(
            static_cast<float>(metadata.width),
            static_cast<float>(metadata.height));
    }
}

SpriteTexture::SpriteTexture( // 스프라이트 시트 정보를 초기화하는 생성자 구현
    const std::wstring& name,
    const std::wstring& filePath,
    UINT columns,
    UINT rows,
    const DirectX::SimpleMath::Vector2& sheetStart,
    const DirectX::SimpleMath::Vector2& sheetEnd)
    : _name(name)
    , _filePath(filePath)
    , _columns(columns)
    , _rows(rows)
    , _sheetStart(sheetStart)
{
    LoadTexture(filePath);
    UpdateSheetInfo(sheetEnd);
}

DirectX::SimpleMath::Vector2 SpriteTexture::GetFrameStartUV(UINT column, UINT row) const // 열과 행으로부터 시작 텍셀 좌표 계산
{
    if (!IsValidFrame(column, row) || _textureSize.x <= 0.0f || _textureSize.y <= 0.0f)
    {
        return DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    }
    const DirectX::SimpleMath::Vector2 startPixel = CalculateFrameStartPixel(column, row);
    return DirectX::SimpleMath::Vector2(startPixel.x / _textureSize.x, startPixel.y / _textureSize.y);
}

DirectX::SimpleMath::Vector2 SpriteTexture::GetFrameEndUV(UINT column, UINT row) const // 열과 행으로부터 끝 텍셀 좌표 계산
{
    if (!IsValidFrame(column, row) || _textureSize.x <= 0.0f || _textureSize.y <= 0.0f)
    {
        return DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    }
    const DirectX::SimpleMath::Vector2 endPixel = CalculateFrameStartPixel(column, row) + _framePixelSize;
    return DirectX::SimpleMath::Vector2(endPixel.x / _textureSize.x, endPixel.y / _textureSize.y);
}

bool SpriteTexture::IsValidFrame(UINT column, UINT row) const // 프레임 범위 검사
{
    if (_columns == 0 || _rows == 0)
    {
        return false;
    }
    if (column >= _columns || row >= _rows)
    {
        return false;
    }
    if (_framePixelSize.x <= 0.0f || _framePixelSize.y <= 0.0f)
    {
        return false;
    }
    return true;
}

std::shared_ptr<AnimationClip> SpriteTexture::CreateAnimationClip( // AnimationClip 생성을 지원하는 함수 구현
    const std::wstring& clipName,
    UINT startColumn,
    UINT startRow,
    UINT clipColumns,
    UINT clipRows,
    bool reversed) const
{
    if (_shaderResourceView == nullptr || clipColumns == 0 || clipRows == 0)
    {
        return nullptr;
    }
    if (!IsValidFrame(startColumn, startRow))
    {
        return nullptr;
    }
    const UINT endColumn = startColumn + clipColumns - 1;
    const UINT endRow = startRow + clipRows - 1;
    if (!IsValidFrame(endColumn, endRow))
    {
        return nullptr;
    }
    const DirectX::SimpleMath::Vector2 startPixel = CalculateFrameStartPixel(startColumn, startRow);
    const DirectX::SimpleMath::Vector2 endPixel(
        startPixel.x + _framePixelSize.x * static_cast<float>(clipColumns),
        startPixel.y + _framePixelSize.y * static_cast<float>(clipRows));
    return std::make_shared<AnimationClip>(
        clipName,
        _shaderResourceView,
        clipColumns,
        clipRows,
        startPixel,
        endPixel,
        reversed);
}

void SpriteTexture::LoadTexture(const std::wstring& filePath) // 텍스처 로드 및 리소스 생성
{
    _shaderResourceView.Reset();
    _textureSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    if (filePath.empty())
    {
        return;
    }
    const std::wstring extension = NormalizeExtension(Path::GetExtension(filePath));
    DirectX::ScratchImage scratchImage;
    DirectX::TexMetadata metadata;
    HRESULT result = S_OK;
    if (extension == L"dds")
    {
        result = DirectX::LoadFromDDSFile(filePath.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, scratchImage);
    }
    else if (extension == L"tga")
    {
        result = DirectX::LoadFromTGAFile(filePath.c_str(), &metadata, scratchImage);
    }
    else
    {
        result = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
    }
    if (FAILED(result))
    {
        return;
    }
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    result = DirectX::CreateShaderResourceView(
        GraphicsManager::Get()->GetDevice(),
        scratchImage.GetImages(),
        scratchImage.GetImageCount(),
        metadata,
        shaderResourceView.GetAddressOf());
    CHECK(result);
    _shaderResourceView = shaderResourceView;
    _textureSize = ExtractTextureSize(metadata);
}

void SpriteTexture::UpdateSheetInfo(const DirectX::SimpleMath::Vector2& desiredSheetEnd) // 시트 범위 및 프레임 정보 갱신
{
    _sheetEnd = desiredSheetEnd;
    if (_textureSize.x <= 0.0f || _textureSize.y <= 0.0f)
    {
        _sheetStart = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        _sheetEnd = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        _framePixelSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        _frameUVSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        return;
    }
    _sheetStart.x = std::clamp(_sheetStart.x, 0.0f, _textureSize.x);
    _sheetStart.y = std::clamp(_sheetStart.y, 0.0f, _textureSize.y);
    if (desiredSheetEnd.x <= _sheetStart.x || desiredSheetEnd.y <= _sheetStart.y)
    {
        _sheetEnd = _textureSize;
    }
    else
    {
        _sheetEnd.x = std::clamp(desiredSheetEnd.x, _sheetStart.x, _textureSize.x);
        _sheetEnd.y = std::clamp(desiredSheetEnd.y, _sheetStart.y, _textureSize.y);
    }
    const DirectX::SimpleMath::Vector2 sheetSize = _sheetEnd - _sheetStart;
    if (_columns == 0 || _rows == 0 || sheetSize.x <= 0.0f || sheetSize.y <= 0.0f)
    {
        _framePixelSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        _frameUVSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        return;
    }
    _framePixelSize.x = sheetSize.x / static_cast<float>(_columns);
    _framePixelSize.y = sheetSize.y / static_cast<float>(_rows);
    _frameUVSize.x = _framePixelSize.x / _textureSize.x;
    _frameUVSize.y = _framePixelSize.y / _textureSize.y;
}

DirectX::SimpleMath::Vector2 SpriteTexture::CalculateFrameStartPixel(UINT column, UINT row) const // 시작 픽셀 좌표 계산
{
    return DirectX::SimpleMath::Vector2(
        _sheetStart.x + _framePixelSize.x * static_cast<float>(column),
        _sheetStart.y + _framePixelSize.y * static_cast<float>(row));
}
