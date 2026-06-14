@echo off

g++ main.cpp -Isrc -std=c++17 -o cvm.exe

if %ERRORLEVEL% NEQ 0 exit /b
