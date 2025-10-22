@echo off
setlocal EnableDelayedExpansion

REM =====================================================
REM [BuildSetting.bat]
REM 목적:
REM   - 빌드 완료 후 리소스 파일을 Contents 폴더로 자동 복사
REM   - Debug: 셰이더 원본(.hlsl) 그대로 복사
REM   - Release: 캐시된 .cso를 우선 사용 (없으면 fxc로 컴파일)
REM 위치:
REM   - 솔루션 루트에 두고, 포스트 빌드 이벤트에서 호출
REM 경로 구조:
REM   $(SolutionDir)Build\bin\$(ProjectName)\$(PlatformTarget)\$(Configuration)\Contents
REM =====================================================

REM ====== [1] 빌드 환경 변수 설정 ======
REM 인자: 1=SolutionDir, 2=ProjectDir, 3=Platform, 4=Configuration, 5=ProjectName
set "SOLUTION_DIR=%~1"
set "PROJECT_DIR=%~2"
set "PLATFORM=%~3"
set "CONFIG=%~4"
set "PROJECT_NAME=%~5"

REM --- 기본값 보정 (필수 인자가 없을 경우 대비) ---
if "%SOLUTION_DIR%"=="" set "SOLUTION_DIR=%cd%\"
if "%PROJECT_DIR%"=="" set "PROJECT_DIR=%~dp0Framework\"
if "%PLATFORM%"=="" set "PLATFORM=Win32"
if "%CONFIG%"=="" set "CONFIG=Debug"
if "%PROJECT_NAME%"=="" set "PROJECT_NAME=Framework"

REM 경로 정규화: ProjectDir/SolutionDir에 항상 trailing backslash 보장
for %%P in ("%SOLUTION_DIR%") do set "SOLUTION_DIR=%%~fP\"
for %%P in ("%PROJECT_DIR%") do set "PROJECT_DIR=%%~fP\"

set "DEST_CONTENTS=%SOLUTION_DIR%Build\bin\%PROJECT_NAME%\%PLATFORM%\%CONFIG%\Contents"

REM ====== [2] 소스 폴더 경로 설정 ======
set "SRC_ROOT=%PROJECT_DIR%"
set "TEX_SRC=%SRC_ROOT%_Textures"
set "FNT_SRC=%SRC_ROOT%_Fonts"
set "SND_SRC=%SRC_ROOT%_Sounds"
REM set "TILE_SRC=%SRC_ROOT%_TileData"
set "SHD_SRC=%SRC_ROOT%_Shaders"
set "IMGUI_INI=%SRC_ROOT%imgui.ini"

REM 셰이더 출력 경로
set "SHD_DST=%DEST_CONTENTS%\_Shaders"

REM fxc.exe 경로 (PATH에 없을 경우 절대경로로 수정 가능)
set "FXC=fxc"

REM =====================================================
REM [3] Contents 폴더 생성
REM =====================================================
if not exist "%DEST_CONTENTS%" mkdir "%DEST_CONTENTS%"
if not exist "%SHD_DST%"      mkdir "%SHD_DST%"

REM =====================================================
REM [4] 공통 리소스 복사 (_Textures, _Fonts, _Sounds, _TileData, imgui.ini)
REM =====================================================
if exist "%TEX_SRC%"  robocopy "%TEX_SRC%"  "%DEST_CONTENTS%\_Textures" /E /NFL /NDL /NJH /NJS /NP >nul
if exist "%FNT_SRC%"  robocopy "%FNT_SRC%"  "%DEST_CONTENTS%\_Fonts"    /E /NFL /NDL /NJH /NJS /NP >nul
if exist "%SND_SRC%"  robocopy "%SND_SRC%"  "%DEST_CONTENTS%\_Sounds"   /E /NFL /NDL /NJH /NJS /NP >nul
REM if exist "%TILE_SRC%" robocopy "%TILE_SRC%" "%DEST_CONTENTS%\_TileData" /E /NFL /NDL /NJH /NJS /NP >nul
if exist "%IMGUI_INI%" copy /Y "%IMGUI_INI%" "%DEST_CONTENTS%" >nul

REM =====================================================
REM [5] 셰이더 처리
REM   - Debug  : 원본 .hlsl 복사
REM   - Release: 캐시된 .cso 복사 (없으면 fxc로 컴파일)
REM =====================================================
if /I "%CONFIG%"=="Debug" (
    REM --- Debug 빌드: 셰이더 폴더 그대로 복사 ---
    if exist "%SHD_SRC%" robocopy "%SHD_SRC%" "%SHD_DST%" /E /NFL /NDL /NJH /NJS /NP >nul
) else (
    REM --- Release 빌드: 셰이더 캐시 사용 ---
    if exist "%SHD_SRC%" (
        if exist "%SHD_SRC%\*.cso" (
            REM --- 캐시된 .cso 사용 ---
            robocopy "%SHD_SRC%" "%SHD_DST%" *.cso /NFL /NDL /NJH /NJS /NP >nul
        ) else (
            echo [BuildSetting] 캐시된 .cso가 없어 fxc로 컴파일합니다.
            REM Vertex Shader
            for %%F in ("%SHD_SRC%\*.vs.hlsl") do "%FXC%" /nologo /T vs_5_0 /E VSMain /O3 /Fo "%SHD_DST%\%%~nF.cso" "%%~fF"
            REM Pixel Shader
            for %%F in ("%SHD_SRC%\*.ps.hlsl") do "%FXC%" /nologo /T ps_5_0 /E PSMain /O3 /Fo "%SHD_DST%\%%~nF.cso" "%%~fF"
            REM Geometry Shader
            for %%F in ("%SHD_SRC%\*.gs.hlsl") do "%FXC%" /nologo /T gs_5_0 /E GSMain /O3 /Fo "%SHD_DST%\%%~nF.cso" "%%~fF"
            REM Compute Shader
            for %%F in ("%SHD_SRC%\*.cs.hlsl") do "%FXC%" /nologo /T cs_5_0 /E CSMain /O3 /Fo "%SHD_DST%\%%~nF.cso" "%%~fF"
            REM Hull Shader
            for %%F in ("%SHD_SRC%\*.hs.hlsl") do "%FXC%" /nologo /T hs_5_0 /E HSMain /O3 /Fo "%SHD_DST%\%%~nF.cso" "%%~fF"
            REM Domain Shader
            for %%F in ("%SHD_SRC%\*.ds.hlsl") do "%FXC%" /nologo /T ds_5_0 /E DSMain /O3 /Fo "%SHD_DST%\%%~nF.cso" "%%~fF"
        )

        REM --- 추가 확장자(hlsli, inc 등) 보조 파일도 복사 ---
        for %%X in (hlsli inc txt json) do (
            if exist "%SHD_SRC%\*.%%X" robocopy "%SHD_SRC%" "%SHD_DST%" *.%%X /NFL /NDL /NJH /NJS /NP >nul
        )
    )
)

REM =====================================================
REM [6] 완료 메시지
REM =====================================================
echo [BuildSetting] Contents 구성 완료.
endlocal
