@echo off
setlocal EnableDelayedExpansion

if not defined VCPKG_ROOT (
    echo VCPKG_ROOT is not set. Setting default value to "C:\vcpkg".
    set VCPKG_ROOT=C:\vcpkg
)

if not exist "%VCPKG_ROOT%" (
    echo VCPKG_ROOT does not exist at "%VCPKG_ROOT%". Please check the path.
    exit /b 1
)


set QT_FOUND=false
set QT_VERSION=
set QT_MSVC_PATH=

for /d %%d in (C:\Qt\*) do (
    set "QT_VERSION=%%~nd"
    for /f "tokens=1,2 delims=." %%a in ("!QT_VERSION!") do (
        set "QT_MAJOR=%%a"
        set "QT_MINOR=%%b"
        if !QT_MAJOR! geq 6 (
            if !QT_MINOR! geq 8 (
                for /d %%m in ("%%d\msvc*_64") do (
                    if exist "%%~fm\lib\cmake" (
                        set QT_FOUND=true
                        set "QT_MSVC_PATH=%%~fm\lib\cmake"
                        goto :found_qt
                    )
                )

                for /d %%m in ("%%d\!QT_VERSION!\msvc*_64") do (
                    if exist "%%~fm\lib\cmake" (
                        set QT_FOUND=true
                        set "QT_MSVC_PATH=%%~fm\lib\cmake"
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

echo Using Qt from: !QT_MSVC_PATH!
cmake -S ..\..\ -B ..\..\build -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake -DCMAKE_PREFIX_PATH=!QT_MSVC_PATH!