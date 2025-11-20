@echo off
pushd ..\
call vendor\bin\windows\premake\premake5.exe vs2022
popd
PAUSE