@echo off
:: only support windows
:: Project: Pig Web Server
:: Make File Version: 1.0.0
:: This file is converted from "make.sh", if you want to see the source code, please see "make.sh"

set a=Pig Web Server
set b=0.0.1 Alpha
set c=pws
title %a% %b% && echo Project: %a% && echo Version: %b%
if "%1" == "debug" echo Current Mode: Debug && goto :a
if "%1" == "release" echo Current Mode: Release && goto :a
echo Error: Unknown Mode: %1 && echo Forced Mode: release && goto :a
:a
echo. && gcc -v >nul 2>&1
if "%ERRORLEVEL%" == "0" goto :b
echo Error: gcc not found && echo Please install it && pause >nul 2>&1 && exit 2
:b
mkdir build >nul 2>&1 && cd build && echo Compiling... Please wait a minute.
if "%1" == "debug" goto :c
goto :d
:c
gcc -o "build/%c%.exe" src/index.c -Wall -Wextra -O0 -ggdb -std=c99 -funsigned-char -m32 -DDEBUG -lws2_32
if "%ERRORLEVEL%" == "0" goto :e
goto :f
:d
gcc -o "build/%c%.exe" src/index.c -O3 -std=c99 -funsigned-char -m32 "-DRELEASE=\"%b%\"" -lws2_32
if "%ERRORLEVEL%" == "0" goto :e
goto :f
:e
cd .. && echo. && echo Your file is in the ./build/ directory && echo Thanks for using %a% %b% %1 && pause > nul && exit 0
:f
cd .. && echo. && echo Compile Error! && pause > nul && exit 0