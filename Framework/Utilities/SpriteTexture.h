#pragma once

#include <memory>
#include <string>

#include <wrl/client.h>
#include <directxtk/SimpleMath.h>

class AnimationClip;

// 스프라이트 시트 정보를 외부에 제공하기 위한 인터페이스
class ISpriteTexture
{
public:
    virtual ~ISpriteTexture() = default;

public:
    // 스프라이트 시트의 표시 이름을 반환
    virtual const std::wstring& GetName() const = 0;
    // 원본 이미지 파일 경로를 반환
    virtual const std::wstring& GetFilePath() const = 0;
    // 텍스처의 실제 픽셀 크기를 반환
    virtual DirectX::SimpleMath::Vector2 GetTextureSize() const = 0;
    // 시트에서 사용할 영역의 시작 픽셀 좌표를 반환
    virtual DirectX::SimpleMath::Vector2 GetSheetStart() const = 0;
    // 시트에서 사용할 영역의 끝 픽셀 좌표를 반환
    virtual DirectX::SimpleMath::Vector2 GetSheetEnd() const = 0;
    // 한 프레임의 픽셀 크기를 반환
    virtual DirectX::SimpleMath::Vector2 GetFramePixelSize() const = 0;
    // 한 프레임의 텍셀 크기를 반환
    virtual DirectX::SimpleMath::Vector2 GetFrameUVSize() const = 0;
    // 열, 행 기준으로 시작 텍셀 좌표를 반환
    virtual DirectX::SimpleMath::Vector2 GetFrameStartUV(UINT column, UINT row) const = 0;
    // 열, 행 기준으로 끝 텍셀 좌표를 반환
    virtual DirectX::SimpleMath::Vector2 GetFrameEndUV(UINT column, UINT row) const = 0;
    // 프레임이 유효한 인덱스인지 확인
    virtual bool IsValidFrame(UINT column, UINT row) const = 0;
    // 쉐이더 리소스 뷰를 반환
    virtual Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const = 0;
};

// AnimationClip과 Tile에서 재사용 가능한 스프라이트 시트 도우미
class SpriteTexture final : public ISpriteTexture
{
public:
    // 파일 경로와 시트 구성 정보를 이용하여 스프라이트 시트를 초기화
    SpriteTexture(
        const std::wstring& name,
        const std::wstring& filePath,
        UINT columns,
        UINT rows,
        const DirectX::SimpleMath::Vector2& sheetStart = DirectX::SimpleMath::Vector2(0.0f, 0.0f),
        const DirectX::SimpleMath::Vector2& sheetEnd = DirectX::SimpleMath::Vector2(-1.0f, -1.0f)
    );

public:
    // 인터페이스 구현: 이름 반환
    const std::wstring& GetName() const override { return _name; }
    // 인터페이스 구현: 파일 경로 반환
    const std::wstring& GetFilePath() const override { return _filePath; }
    // 인터페이스 구현: 텍스처 크기 반환
    DirectX::SimpleMath::Vector2 GetTextureSize() const override { return _textureSize; }
    // 인터페이스 구현: 시트 시작 위치 반환
    DirectX::SimpleMath::Vector2 GetSheetStart() const override { return _sheetStart; }
    // 인터페이스 구현: 시트 끝 위치 반환
    DirectX::SimpleMath::Vector2 GetSheetEnd() const override { return _sheetEnd; }
    // 인터페이스 구현: 프레임 픽셀 크기 반환
    DirectX::SimpleMath::Vector2 GetFramePixelSize() const override { return _framePixelSize; }
    // 인터페이스 구현: 프레임 텍셀 크기 반환
    DirectX::SimpleMath::Vector2 GetFrameUVSize() const override { return _frameUVSize; }
    // 인터페이스 구현: 시작 텍셀 좌표 반환
    DirectX::SimpleMath::Vector2 GetFrameStartUV(UINT column, UINT row) const override;
    // 인터페이스 구현: 끝 텍셀 좌표 반환
    DirectX::SimpleMath::Vector2 GetFrameEndUV(UINT column, UINT row) const override;
    // 인터페이스 구현: 프레임 유효성 검사
    bool IsValidFrame(UINT column, UINT row) const override;
    // 인터페이스 구현: SRV 반환
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const override { return _shaderResourceView; }

public:
    // 프레임 인덱스를 이용하여 시작 텍셀 좌표 계산
    DirectX::SimpleMath::Vector2 GetFrameStartUV(UINT frameIndex) const
    {
        UINT column = 0;
        UINT row = 0;
        if (!GetFrameCoordinate(frameIndex, column, row))
        {
            return DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        }
        return GetFrameStartUV(column, row);
    }
    // 프레임 인덱스를 이용하여 끝 텍셀 좌표 계산
    DirectX::SimpleMath::Vector2 GetFrameEndUV(UINT frameIndex) const
    {
        UINT column = 0;
        UINT row = 0;
        if (!GetFrameCoordinate(frameIndex, column, row))
        {
            return DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        }
        return GetFrameEndUV(column, row);
    }
    // 프레임 인덱스로 열과 행 계산
    bool GetFrameCoordinate(UINT frameIndex, UINT& outColumn, UINT& outRow) const
    {
        if (_columns == 0 || _rows == 0)
        {
            return false;
        }
        const UINT maxIndex = _columns * _rows;
        if (frameIndex >= maxIndex)
        {
            return false;
        }
        outColumn = frameIndex % _columns;
        outRow = frameIndex / _columns;
        return true;
    }
    // AnimationClip 생성을 돕는 헬퍼 함수
    std::shared_ptr<AnimationClip> CreateAnimationClip(
        const std::wstring& clipName,
        UINT startColumn,
        UINT startRow,
        UINT clipColumns,
        UINT clipRows,
        bool reversed
    ) const;
    // 타일에 사용할 시작/끝 텍셀 좌표를 계산
    bool TryGetTileUV(UINT tileIndex, DirectX::SimpleMath::Vector2& outStartUV, DirectX::SimpleMath::Vector2& outEndUV) const
    {
        UINT column = 0;
        UINT row = 0;
        if (!GetFrameCoordinate(tileIndex, column, row))
        {
            outStartUV = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
            outEndUV = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
            return false;
        }
        outStartUV = GetFrameStartUV(column, row);
        outEndUV = GetFrameEndUV(column, row);
        return true;
    }

private:
    // 텍스처를 로드하고 메타데이터를 설정
    void LoadTexture(const std::wstring& filePath);
    // 시트 범위와 프레임 크기를 갱신
    void UpdateSheetInfo(const DirectX::SimpleMath::Vector2& sheetEnd);
    // 열과 행을 이용하여 시작 픽셀 좌표를 계산
    DirectX::SimpleMath::Vector2 CalculateFrameStartPixel(UINT column, UINT row) const;

private:
    // 스프라이트 시트 이름
    std::wstring _name;
    // 원본 이미지 파일 경로
    std::wstring _filePath;
    // 시트 열 개수
    UINT _columns = 0;
    // 시트 행 개수
    UINT _rows = 0;
    // 시트에서 사용할 시작 픽셀 좌표
    DirectX::SimpleMath::Vector2 _sheetStart = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    // 시트에서 사용할 끝 픽셀 좌표
    DirectX::SimpleMath::Vector2 _sheetEnd = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    // 텍스처의 전체 픽셀 크기
    DirectX::SimpleMath::Vector2 _textureSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    // 한 프레임의 픽셀 크기
    DirectX::SimpleMath::Vector2 _framePixelSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    // 한 프레임의 텍셀 크기
    DirectX::SimpleMath::Vector2 _frameUVSize = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    // 쉐이더 리소스 뷰
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
};
