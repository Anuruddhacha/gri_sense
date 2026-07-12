@echo off
setlocal

cd /d "%~dp0"

set "QT_PREFIX=C:\Qt\6.11.1\mingw_64"
set "MINGW_BIN=C:\Qt\Tools\mingw1310_64\bin"
set "NINJA_BIN=C:\Qt\Tools\Ninja"
set "PATH=%MINGW_BIN%;%NINJA_BIN%;%QT_PREFIX%\bin;%PATH%"
set "QTFRAMEWORK_BYPASS_LICENSE_CHECK=1"

if not exist build mkdir build
cd build

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
