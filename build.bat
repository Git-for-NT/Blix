@echo off
echo Building Tetris Game...

if not exist build mkdir build
cd build

cmake .. -G "Visual Studio 17 2022" -A x64
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    cd ..
    exit /b %errorlevel%
)

cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build failed!
    cd ..
    exit /b %errorlevel%
)

echo Build successful!
cd ..
