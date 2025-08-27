@echo off

set SCRIPT_DIR=%~dp0

set COMPILE_ALL=0

:parse_args
if "%~1"=="" goto after_parse

if "%~1"=="-a" (
    set COMPILE_ALL=1
) else (
    echo Unknown option: %~1
)

:after_parse

pushd %SCRIPT_DIR%\..\..\

if not exist build_windows mkdir build_windows

pushd build_windows

if %COMPILE_ALL%==1 (
    cl ^
        /c /nologo /EHsc /Zi /Od /I ..\imgui\ ^
        ..\imgui_library.cpp ^
        /Fo:IMGUI.obj

    lib /nologo IMGUI.obj 
)


cl ^
    /nologo /EHsc /Zi /Od /std:c++17 /I ..\imgui\ ^
     ..\run_windows.cpp ^
     /Fe:run_windows.exe /link .\IMGUI.lib opengl32.lib user32.lib kernel32.lib

popd

popd