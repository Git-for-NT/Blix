@echo off
echo Building Tetris Game with Visual Studio...

REM Find and run Visual Studio Developer Command Prompt
set "VSCMD_PATH=C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"

if not exist "%VSCMD_PATH%" (
    echo Visual Studio Developer Command Prompt not found!
    echo Please open TetrisGame.sln in Visual Studio and build from there.
    exit /b 1
)

REM Initialize Visual Studio environment
call "%VSCMD_PATH%"

REM Build the solution
msbuild TetrisGame.sln /p:Configuration=Release /p:Platform=x64 /m

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo Build successful!
echo Executable: bin\Release\TetrisGame.exe
echo.
echo Run the game with: bin\Release\TetrisGame.exe
