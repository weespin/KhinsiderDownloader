@echo off
setlocal EnableDelayedExpansion

set PROJECT_DIR=%~dp0..\..
set QT_FOUND=false
set QT_VERSION=
set QT_MSVC_PATH=
set MAIN_DIR=%cd%

for /d %%d in (C:\Qt\*) do (
    set "QT_VERSION=%%~nd"
    for /f "tokens=1,2 delims=." %%a in ("!QT_VERSION!") do (
        set "QT_MAJOR=%%a"
        set "QT_MINOR=%%b"
        if !QT_MAJOR! geq 6 (
            if !QT_MINOR! geq 8 (
                for /d %%m in ("%%d\msvc*_64") do (
                    if exist "%%~fm\bin" (
                        set QT_FOUND=true
                        set "QT_MSVC_PATH=%%~fm\bin"
                        goto :found_qt
                    )
                )
                
                for /d %%m in ("%%d\!QT_VERSION!\msvc*_64") do (
                    if exist "%%~fm\bin" (
                        set QT_FOUND=true
                        set "QT_MSVC_PATH=%%~fm\bin"
                        goto :found_qt
                    )
                )
            )
        )
    )
)

:found_qt
if not !QT_FOUND!==true (
    echo No Qt version ^>= 6.8 found in C:\Qt\.
    exit /b 1
)

if not exist "!QT_MSVC_PATH!" (
    echo MSVC version of Qt not found at !QT_MSVC_PATH!.
    exit /b 1
)

set EXE_PATH=%PROJECT_DIR%\build\Release\appKhinsiderQT.exe
echo %PROJECT_DIR%
echo %EXE_PATH%
::TODO! Exclude unused libraries (--no-)
"!QT_MSVC_PATH!\windeployqt.exe" --qmldir "%PROJECT_DIR%"\src\ui --no-translations --release --force-openssl "%EXE_PATH%"