@echo off
pushd "%~dp0"

call "Binaries/ThirdParty/Premake/premake5.exe" --file="premake5.lua" vs2022

popd
pause
