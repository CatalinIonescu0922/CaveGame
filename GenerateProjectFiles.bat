@echo off
pushd "%~dp0"

call "Binaries/ThirdParty/Premake/premake5.exe" --file="PremakeConfig.lua" vs2022

popd
pause
