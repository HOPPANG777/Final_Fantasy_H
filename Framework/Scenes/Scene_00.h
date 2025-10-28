#pragma once

#include <array>
#include <memory>
#include <string>

#include "../Interfaces/LoadableScene.h"

class TileMap;
class TileSet;

// 타일맵 편집 장면을 담당하는 구현 장면 클래스
class Scene_00 : public LoadableScene
{
public:
    // 장면이 생성될 때 호출되어 기본 상태를 준비한다.
    void Awake() override;
    // 장면 진입 시 1회 실행되는 초기화 로직을 처리한다.
    void Start() override;

    // 장면이 파괴될 때 자원을 반환한다.
    void Destroy() override;

    // 고정 주기 로직을 갱신한다.
    void FixedUpdate() override;
    // 매 프레임 갱신 로직을 수행한다.
    void Update() override;
    // 업데이트 이후 후처리 로직을 수행한다.
    void LateUpdate() override;

    // 장면의 메인 렌더링을 수행한다.
    void Render() override;
    // 렌더링 이후 후처리 작업을 수행한다.
    void PostRender() override;

    // ImGui 기반 편집 UI를 구성한다.
    void ImGUI() override;

protected:
    // 장면 로딩 단계 구성을 정의한다.
    void ConfigureLoadSteps() override;
    // 로딩 시작 시 기존 데이터를 초기화한다.
    void OnLoadBegin() override;

private:
    // 타일맵을 새로 생성하거나 재구성한다.
    void CreateTileMap();
    // 현재 타일맵을 파일로 저장한다.
    void SaveCurrentMap(const std::string& fileName);
    // 지정된 파일에서 타일맵을 불러온다.
    void LoadMapFromFile(const std::string& fileName);
    // 파일 경로를 확장자와 함께 구성한다.
    std::string BuildMapPath(const std::string& fileName, const std::string& extension) const;
    // 맵 저장 폴더가 존재하도록 보장한다.
    void EnsureMapDirectoryExists() const;

private:
    std::unique_ptr<TileMap> tileMap;                     // 편집 중인 타일맵 인스턴스
    std::shared_ptr<TileSet> tileSet;                     // 타일셋 리소스

    UINT spacing = 64;                                    // 타일 간 간격
    UINT mapWidth = 0;                                    // 현재 맵 가로 타일 수
    UINT mapHeight = 0;                                   // 현재 맵 세로 타일 수

    UINT resizeWidth = 0;                                 // 사용자가 지정한 가로 리사이즈 값
    UINT resizeHeight = 0;                                // 사용자가 지정한 세로 리사이즈 값

    UINT tileSetColumns = 0;                              // 타일셋의 열 개수
    UINT tileSetRows = 0;                                 // 타일셋의 행 개수

    std::array<char, 128> mapFileNameBuffer{};            // 맵 파일 이름 버퍼
    std::array<char, 260> tileSetRelativePathBuffer{};    // 타일셋 상대 경로 버퍼
};
