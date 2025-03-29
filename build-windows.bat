@echo off
REM Stop the script if an error occurs by checking error levels after each command.
REM Create build directory if it doesn't exist
set "BUILD_DIR=.\build"
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
    echo Created build directory
)

REM Fetch SDL libraries in Senpai.
REM If you have a Windows variant of the fetch-lib script, for example "fetch-lib-windows.bat", call it.
pushd Senpai
call fetch-lib-windows.bat
if ERRORLEVEL 1 (
    echo "Fetching libraries failed"
    exit /b 1
)
popd

REM Navigate to build directory
cd "%BUILD_DIR%"

REM Configure CMake project
echo Configuring CMake project...
cmake .. 
if ERRORLEVEL 1 (
    echo "CMake configuration failed"
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --verbose
if ERRORLEVEL 1 (
    echo "Build failed"
    exit /b 1
)

ctest
if ERRORLEVEL 1 (
    echo "Tests failed"
    exit /b 1
)

REM Return to original directory
cd ..

REM Copy the assets folder to the build directory.
REM The /E flag copies directories and subdirectories, including empty ones.
REM The /Y flag suppresses prompting to confirm overwriting files.
REM The /I flag assumes the destination is a directory.
xcopy /E /Y /I Game\assets build\Game\Debug\assets\

REM Copy necessary DLLs to the executable directory
echo Copying DLLs to executable directory...
copy build\Senpai\lib\SDL\Debug\SDL3.dll build\Game\Debug\
copy build\Senpai\lib\SDL_ttf\Debug\SDL3_ttf.dll build\Game\Debug\
copy build\Senpai\lib\SDL_image\Debug\SDL3_image.dll build\Game\Debug\
copy build\Senpai\lib\SDL_mixer\Debug\SDL3_mixer.dll build\Game\Debug\