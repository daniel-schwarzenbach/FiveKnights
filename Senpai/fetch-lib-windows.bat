@echo off
REM Stop the script if an error occurs by checking ERRORLEVEL after critical commands.

REM Directory where libraries will be stored
set "LIB_DIR=lib"

REM Create libs directory if it doesn't exist
IF NOT EXIST "%LIB_DIR%" (
    echo Creating directory: %LIB_DIR%
    mkdir "%LIB_DIR%"
)

pushd "%LIB_DIR%"

REM Clone SDL repository if not already present
IF NOT EXIST "SDL" (
    echo Cloning SDL...
    git clone --depth 1 https://github.com/libsdl-org/SDL.git
    if ERRORLEVEL 1 exit /b 1
)

REM Clone SDL_ttf repository if not already present
IF NOT EXIST "SDL_ttf" (
    echo Cloning SDL_ttf...
    git clone --depth 1 https://github.com/libsdl-org/SDL_ttf.git
    powershell.exe -ExecutionPolicy Bypass -File .\SDL_ttf\external\Get-GitModules.ps1
    if ERRORLEVEL 1 exit /b 1
)

REM Clone SDL_image repository if not already present
IF NOT EXIST "SDL_image" (
    echo Cloning SDL_image...
    git clone --depth 1 https://github.com/libsdl-org/SDL_image.git
    REM powershell.exe -ExecutionPolicy Bypass -File .\SDL_image\external\Get-GitModules.ps1
    if ERRORLEVEL 1 exit /b 1
)

REM Clone SDL_mixer repository if not already present
IF NOT EXIST "SDL_mixer" (
    echo Cloning SDL_mixer...
    git clone --depth 1 https://github.com/libsdl-org/SDL_mixer.git
    REM powershell.exe -ExecutionPolicy Bypass -File .\SDL_mixer\external\Get-GitModules.ps1
    if ERRORLEVEL 1 exit /b 1
)



echo All necessary SDL libraries have been fetched.

popd