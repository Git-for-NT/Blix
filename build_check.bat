@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"
msbuild TetrisGame.sln /p:Configuration=Release /p:Platform=x64 /v:minimal
