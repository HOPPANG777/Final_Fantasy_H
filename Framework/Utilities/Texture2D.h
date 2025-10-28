#pragma once

#include <memory>
#include <string>
#include <vector>

#include <wrl/client.h>
#include <DirectXTex.h>
#include <directxtk/SimpleMath.h>

class ITexture2DCache;
class Texture2DCache;

// 2D 텍스처 리소스를 표현하는 래퍼 클래스
class Texture2D
{
public:
    using Color = DirectX::SimpleMath::Color; // 픽셀 정보를 표현할 색상 타입

public:
    explicit Texture2D(const std::wstring& filePath);               // 기본 캐시를 사용하는 생성자
    Texture2D(const std::wstring& filePath, ITexture2DCache& cache); // 사용자 지정 캐시를 사용하는 생성자

public:
    void ReadPixel(std::vector<Color>* pixels);                                          // 텍스처 픽셀 데이터를 CPU로 읽어오기
    static void ReadPixel(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, std::vector<Color>* pixels); // 임의의 텍스처에서 픽셀 읽기

public:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV() const { return _srv; } // 쉐이더 리소스 뷰 반환

    const std::wstring& GetFilePath() const { return _filePath; } // 텍스처 파일 경로 반환
    float GetWidth() const { return static_cast<float>(_metaData.width); }   // 텍스처 가로 해상도 반환
    float GetHeight() const { return static_cast<float>(_metaData.height); } // 텍스처 세로 해상도 반환

public:
    static void SetDefaultCache(std::shared_ptr<ITexture2DCache> cache); // 전역 기본 캐시 설정
    static std::shared_ptr<ITexture2DCache> GetDefaultCache();           // 전역 기본 캐시 조회

private:
    void LoadTexture(ITexture2DCache& cache); // 캐시를 이용한 텍스처 로드 수행

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srv = nullptr; // 텍스처 SRV 핸들
    DirectX::TexMetadata _metaData = {};                             // 텍스처 메타데이터 저장 공간
    std::wstring _filePath = L"";                                   // 텍스처 원본 파일 경로

private:
    static std::shared_ptr<ITexture2DCache> _defaultCache; // 전역 기본 캐시 인스턴스

private:
    friend class Texture2DCache; // 캐시가 내부 데이터에 접근할 수 있도록 친구 선언
};

