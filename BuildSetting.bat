@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM =====================================================
REM [BuildSetting.bat]
REM 목적:
REM   - 출력 디렉토리 안에 Contents 폴더 생성
REM   - _Fonts, _Textures, _Shaders 복사
REM   - Debug  : .hlsl
REM   - Release: .cso  (IShader::SaveCompiledShader 캐시 활용)
REM   - imgui.ini는 실행 파일과 같은 폴더(OUT_ROOT)에 배치
REM 사용법(포스트 빌드):
REM   call "$(SolutionDir)BuildSetting.bat" "$(SolutionDir)" "$(ProjectDir)" "$(PlatformTarget)" "$(Configuration)" "$(ProjectName)"
REM 경로 규칙:
REM   $(SolutionDir)Build\bin\$(ProjectName)\$(PlatformTarget)\$(Configuration)\Contents
REM =====================================================

REM [1] 인자 수집
set "SOLUTION_DIR=%~1"
set "PROJECT_DIR=%~2"
set "PLATFORM=%~3"
set "CONFIG=%~4"
set "PROJECT_NAME=%~5"

REM [2] 기본값 보정
if "%SOLUTION_DIR%"=="" set "SOLUTION_DIR=%CD%\"
if "%PROJECT_DIR%"==""  set "PROJECT_DIR=%CD%\"
if "%PLATFORM%"==""     set "PLATFORM=x64"
if "%CONFIG%"==""       set "CONFIG=Debug"
if "%PROJECT_NAME%"=="" set "PROJECT_NAME=App"

if not "%SOLUTION_DIR:~-1%"=="\" set "SOLUTION_DIR=%SOLUTION_DIR%\"
if not "%PROJECT_DIR:~-1%"=="\"  set "PROJECT_DIR=%PROJECT_DIR%\"

REM [3] 경로 계산
set "OUT_ROOT=%SOLUTION_DIR%Build\bin\%PROJECT_NAME%\%PLATFORM%\%CONFIG%"
set "CONTENTS=%OUT_ROOT%\Contents"

set "SRC_FONTS=%PROJECT_DIR%_Fonts"
set "SRC_TEXTURES=%PROJECT_DIR%_Textures"
set "SRC_SHADERS=%PROJECT_DIR%_Shaders"

set "DST_FONTS=%CONTENTS%\_Fonts"
set "DST_TEXTURES=%CONTENTS%\_Textures"
set "DST_SHADERS=%CONTENTS%\_Shaders"

REM ImGui ini 소스 후보(프로젝트 루트 우선, 없으면 솔루션 루트)
set "IMGUI_FILE=imgui.ini"
set "SRC_IMGUI_1=%PROJECT_DIR%%IMGUI_FILE%"
set "SRC_IMGUI_2=%SOLUTION_DIR%%IMGUI_FILE%"

echo ====================================================
echo  Output Root : "%OUT_ROOT%"
echo  Contents    : "%CONTENTS%"
echo  Config      : %CONFIG%
echo ====================================================

REM [4] 대상 폴더 생성
mkdir "%OUT_ROOT%"   >nul 2>&1
mkdir "%CONTENTS%"   >nul 2>&1
mkdir "%DST_FONTS%"  >nul 2>&1
mkdir "%DST_TEXTURES%">nul 2>&1
mkdir "%DST_SHADERS%" >nul 2>&1

REM [5] 복사 공통 옵션
REM /E: 하위 포함, 빈 폴더 포함
REM /XO: 오래된 파일 미복사
REM /R:1 /W:1 : 재시도 1회, 대기 1초
set "ROBO_OPTS=/E /XO /R:1 /W:1"

set "ERROR_FLAG=0"

REM [6] _Fonts
call :CopyWithFallback "%SRC_FONTS%" "%DST_FONTS%" "*" "Fonts"

REM [7] _Textures
call :CopyWithFallback "%SRC_TEXTURES%" "%DST_TEXTURES%" "*" "Textures"

REM [8] _Shaders (형상별 마스크)
set "SHADER_MASK=*.hlsl"
echo %CONFIG% | find /I "Release" >nul && set "SHADER_MASK=*.cso"
call :CopyWithFallback "%SRC_SHADERS%" "%DST_SHADERS%" "%SHADER_MASK%" "Shaders"

REM [9] ImGui 설정 파일(imgui.ini) 배치
if exist "%SRC_IMGUI_1%" (
  echo [ImGui]   "%SRC_IMGUI_1%" -> "%OUT_ROOT%"
  copy /Y "%SRC_IMGUI_1%" "%OUT_ROOT%" >nul || set "ERROR_FLAG=1"
) else if exist "%SRC_IMGUI_2%" (
  echo [ImGui]   "%SRC_IMGUI_2%" -> "%OUT_ROOT%"
  copy /Y "%SRC_IMGUI_2%" "%OUT_ROOT%" >nul || set "ERROR_FLAG=1"
) else (
  echo [ImGui]   원본 파일 없음: "%SRC_IMGUI_1%" 또는 "%SRC_IMGUI_2%"
)

REM [10] 종료 코드
if "%ERROR_FLAG%"=="1" (
  echo 완료(오류 있음). ROBOCOPY 오류코드 8 이상 발생.
  exit /b 1
)
echo 완료(성공). 배포 완료.
exit /b 0

REM ----------------------------------------------------
REM [Helper] 디렉터리 복사 + 실패 시 XCOPY 재시도
REM   인자: 1=원본, 2=대상, 3=마스크, 4=라벨
:CopyWithFallback
set "_SRC=%~1"
set "_DST=%~2"
set "_MASK=%~3"
set "_LABEL=%~4"

if "%_MASK%"=="" set "_MASK=*"

if not exist "%_SRC%" (
  echo [%_LABEL%] 원본 폴더 없음: "%_SRC%"
  goto :EOF
)

echo [%_LABEL%] "%_SRC%" -> "%_DST%"  (mask: %_MASK%)
robocopy "%_SRC%" "%_DST%" %_MASK% %ROBO_OPTS%
set "_RC=%ERRORLEVEL%"

for /f "delims=0123456789" %%I in ("%_RC%") do set "_RC=16"

if %_RC% LSS 8 (
  if %_RC% GTR 0 echo [%_LABEL%] ROBOCOPY 경고 코드 %_RC% (성공 범위) -> 계속.
  goto :EOF
)

echo [%_LABEL%] ROBOCOPY 실패 (코드 %_RC%). XCOPY 재시도...
set "_XCOPY_SRC=%_SRC%"
set "_XCOPY_MASK=%_MASK%"

if "%_MASK%"=="*" (
  xcopy "%_XCOPY_SRC%\*" "%_DST%\" /E /I /Y >nul
) else (
  xcopy "%_XCOPY_SRC%\%_XCOPY_MASK%" "%_DST%\" /E /I /Y >nul
)

set "_XC=%ERRORLEVEL%"

if %_XC% EQU 0 (
  echo [%_LABEL%] XCOPY 성공.
  goto :EOF
)

if %_XC% EQU 1 (
  echo [%_LABEL%] XCOPY: 복사할 파일 없음(코드 1) -> 계속.
  goto :EOF
)

echo [%_LABEL%] XCOPY 실패 (코드 %_XC%).
set "ERROR_FLAG=1"
goto :EOF
