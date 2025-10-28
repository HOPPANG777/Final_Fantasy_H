#pragma once

#include <string>
#include <vector>

#include <wrl/client.h>
#include <DirectXTex.h>

class Texture2D;

// 텍스처 캐시에서 관리하는 텍스처 정보 구조체
struct Texture2DCacheDesc
{
    std::wstring filePath;                                                        // 텍스처 파일 경로
    UINT width = 0;                                                               // 텍스처 가로 해상도
    UINT height = 0;                                                              // 텍스처 세로 해상도
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr; // 생성된 SRV 포인터
    DirectX::TexMetadata metadata = {};                                           // 텍스처 메타데이터(포맷, 밉맵 등)

    bool operator==(const Texture2DCacheDesc& other) const;                        // 다른 구조체와 동일한지 비교
};

// 텍스처 캐시 공통 동작을 정의하는 순수가상 인터페이스
class ITexture2DCache
{
public:
    virtual ~ITexture2DCache() = default;

    virtual void Load(Texture2D& texture) = 0; // 텍스처 로드 및 캐시 재사용 동작
    virtual void Clear() = 0;                  // 캐시에 저장된 모든 데이터 삭제
};

// 기본 텍스처 캐시 구현체
class Texture2DCache final : public ITexture2DCache
{
public:
    Texture2DCache() = default;            // 기본 생성자
    ~Texture2DCache() override = default;  // 기본 소멸자

    void Load(Texture2D& texture) override; // 텍스처 로드 구현
    void Clear() override;                  // 캐시 초기화 구현

private:
    const Texture2DCacheDesc* FindCachedDesc(const Texture2DCacheDesc& desc) const; // 캐시에 동일 텍스처가 있는지 탐색

private:
    std::vector<Texture2DCacheDesc> _cache; // 캐시에 저장된 텍스처 목록
};

