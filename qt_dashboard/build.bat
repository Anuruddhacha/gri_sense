@echo off
setlocal EnableExtensions

cd /d "%~dp0"

set "QT_PREFIX=C:\Qt\6.11.1\mingw_64"
set "MINGW_BIN=C:\Qt\Tools\mingw1310_64\bin"
set "NINJA_BIN=C:\Qt\Tools\Ninja"
set "PATH=%MINGW_BIN%;%NINJA_BIN%;%QT_PREFIX%\bin;%PATH%"
set "QTFRAMEWORK_BYPASS_LICENSE_CHECK=1"

if not exist "third_party\paho.mqtt.c\CMakeLists.txt" (
    echo Cloning Eclipse Paho MQTT C into third_party...
    if not exist third_party mkdir third_party
    git clone --depth 1 --branch v1.3.13 https://github.com/eclipse/paho.mqtt.c.git third_party\paho.mqtt.c
    if errorlevel 1 (
        echo Failed to clone Paho MQTT.
        exit /b 1
    )
)

REM Use build-mingw to avoid locked/stale "build" folders from earlier renames
set "BUILD_DIR=build-mingw"

if exist "%BUILD_DIR%\CMakeCache.txt" (
    findstr /I /C:"mqtt_sub" "%BUILD_DIR%\CMakeCache.txt" >nul 2>&1
    if not errorlevel 1 (
        echo Clearing stale cache...
        rmdir /s /q "%BUILD_DIR%" 2>nul
    )
)

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

cmake -G Ninja ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_PREFIX_PATH="%QT_PREFIX%" ^
  -DCMAKE_C_COMPILER="%MINGW_BIN%\gcc.exe" ^
  -DCMAKE_CXX_COMPILER="%MINGW_BIN%\g++.exe" ^
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
  ..
if errorlevel 1 (
    echo CMake configure failed.
    exit /b 1
)

cmake --build .
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo.
echo Build OK.
echo Run with: run.bat
exit /b 0
