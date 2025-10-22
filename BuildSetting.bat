@echo off
setlocal EnableExtensions

REM 사용법: SettingBuild.bat "<OUT_ROOT>"
REM 예: call "$(SolutionDir)SettingBuild.bat" "$(SolutionDir)Build\bin\$(ProjectName)\$(PlatformTarget)\$(Configuration)"

if "%~1"=="" (
  echo 대상 경로가 필요합니다.
  exit /b 1
)

set "OUT_ROOT=%~1"
set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%Framework"
set "CONTENTS=%OUT_ROOT%\Contents"

set "SRC_FONTS=%PROJECT_ROOT%\_Fonts"
set "SRC_TEXTURES=%PROJECT_ROOT%\_Textures"
set "SRC_SHADERS=%PROJECT_ROOT%\_Shaders"
set "SRC_IMGUI=%PROJECT_ROOT%\imgui.ini"

REM 결과 폴더 생성
mkdir "%CONTENTS%" >nul 2>&1

REM 구성 판별: 경로에 "Release" 포함되면 *.cso, 아니면 *.hlsl
set "SHADER_MASK=*.hlsl"
echo %OUT_ROOT% | find /I "Release" >nul && set "SHADER_MASK=*.cso"

REM 폴더별 복사
if exist "%SRC_FONTS%" (
  mkdir "%CONTENTS%\_Fonts" >nul 2>&1
  xcopy "%SRC_FONTS%\*" "%CONTENTS%\_Fonts\" /E /I /Y >nul
)

if exist "%SRC_TEXTURES%" (
  mkdir "%CONTENTS%\_Textures" >nul 2>&1
  xcopy "%SRC_TEXTURES%\*" "%CONTENTS%\_Textures\" /E /I /Y >nul
)

if exist "%SRC_SHADERS%" (
  mkdir "%CONTENTS%\_Shaders" >nul 2>&1
  xcopy "%SRC_SHADERS%\%SHADER_MASK%" "%CONTENTS%\_Shaders\" /E /I /Y >nul
)

REM imgui.ini를 실행 파일 폴더로 복사
if exist "%SRC_IMGUI%" copy /Y "%SRC_IMGUI%" "%OUT_ROOT%" >nul

exit /b 0