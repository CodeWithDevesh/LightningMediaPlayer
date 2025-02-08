@echo off
rem Exit immediately if a command exits with a non-zero status
setlocal enabledelayedexpansion
set "VCPKG_DIR=vcpkg"

rem Default preset
set "PRESET=with_gui"
set "GENERATOR="
set "COMPILER="

rem Parse command-line arguments
:parse_args
if "%~1"=="" goto done

if "%~1"=="--preset" (
    set "PRESET=%~2"
    shift
    shift
    goto parse_args
)

if "%~1"=="--generator" (
    set "GENERATOR=%~2"
    shift
    shift
    goto parse_args
)

if "%~1"=="--compiler" (
    set "COMPILER=%~2"
    shift
    shift
    goto parse_args
)

echo Unknown argument: %~1
exit /b 1

:done

echo Configuring for %PRESET%

rem Clone vcpkg if not already cloned
if not exist "%VCPKG_DIR%" (
    echo Cloning vcpkg...
    git clone https://github.com/microsoft/vcpkg.git %VCPKG_DIR%
) else (
    echo vcpkg is already cloned.
)

rem Bootstrap vcpkg
echo Bootstrapping vcpkg...
cd %VCPKG_DIR%
call bootstrap-vcpkg.bat
cd ..

rem Configure CMake command
set "CMAKE_CMD=cmake -S . -B build --preset %PRESET%"

rem Add generator if specified
if defined GENERATOR (
    set "CMAKE_CMD=%CMAKE_CMD% -G %GENERATOR%"
)

rem Add compiler if specified
if defined COMPILER (
    set "CMAKE_CMD=%CMAKE_CMD% -DCMAKE_C_COMPILER=%COMPILER% -DCMAKE_CXX_COMPILER=%COMPILER%"
)

rem Run CMake with specified parameters
echo Running CMake with the following command: %CMAKE_CMD%
%CMAKE_CMD%
