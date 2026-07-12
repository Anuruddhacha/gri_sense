@echo off
setlocal

cd /d "%~dp0"

set "QT_PREFIX=C:\Qt\6.11.1\mingw_64"
set "PATH=%QT_PREFIX%\bin;%PATH%"

set EXE=
if exist "build-mingw\agri_sense_dash.exe" set EXE=build-mingw\agri_sense_dash.exe
if exist "build\agri_sense_dash.exe" set EXE=build\agri_sense_dash.exe
if exist "build\Release\agri_sense_dash.exe" set EXE=build\Release\agri_sense_dash.exe
if exist "build\Debug\agri_sense_dash.exe" set EXE=build\Debug\agri_sense_dash.exe

if "%EXE%"=="" (
    echo agri_sense_dash.exe not found. Run build.bat first.
    exit /b 1
)

echo Running %EXE% ...
"%EXE%"
exit /b %ERRORLEVEL%
