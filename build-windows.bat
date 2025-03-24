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
cmake --build .
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
REM The /I flag assumes the destination is a directory.
xcopy /E /I Game\assets build\Game\