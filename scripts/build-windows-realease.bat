@echo off
REM Stop the script if an error occurs by checking error levels after each command.
REM Create build directory if it doesn't exist
set "BUILD_DIR=.\build"
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
    echo Created release build directory
)

REM Fetch SDL libraries in Senpai.
pushd Senpai
call fetch-lib-windows.bat
if ERRORLEVEL 1 (
    echo "Fetching libraries failed"
    exit /b 1
)
popd

REM Navigate to build directory
cd "%BUILD_DIR%"

REM Configure CMake project with Release configuration
echo Configuring CMake project for Release...
cmake .. -DCMAKE_BUILD_TYPE=Release
if ERRORLEVEL 1 (
    echo "CMake configuration failed"
    exit /b 1
)

REM Build the project in Release mode
echo Building project in Release mode...
cmake --build . --config Release
if ERRORLEVEL 1 (
    echo "Build failed"
    exit /b 1
)

REM Run tests in Release mode
ctest -C Release
if ERRORLEVEL 1 (
    echo "Tests failed but continuing..."
)

REM Return to original directory
cd ..

REM Create assets directory if it doesn't exist
if not exist "%BUILD_DIR%\Game\Release\assets" (
    mkdir "%BUILD_DIR%\Game\Release\assets"
)

REM Copy the assets folder to the build directory
echo Copying assets...
xcopy /E /Y /I Game\assets "%BUILD_DIR%\Game\Release\assets\"

REM Copy necessary DLLs to the executable directory
echo Copying DLLs to executable directory...
copy "%BUILD_DIR%\Senpai\lib\SDL\Release\SDL3.dll" "%BUILD_DIR%\Game\Release\"
copy "%BUILD_DIR%\Senpai\lib\SDL_ttf\Release\SDL3_ttf.dll" "%BUILD_DIR%\Game\Release\"
copy "%BUILD_DIR%\Senpai\lib\SDL_image\Release\SDL3_image.dll" "%BUILD_DIR%\Game\Release\"
copy "%BUILD_DIR%\Senpai\lib\SDL_mixer\Release\SDL3_mixer.dll" "%BUILD_DIR%\Game\Release\"

echo Release build completed successfully!
echo Executable is located at: %BUILD_DIR%\Game\Release\FiveKnights.exe