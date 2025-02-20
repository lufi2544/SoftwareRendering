@echo off
setlocal enabledelayedexpansion

:: Set variables
set SRC_DIR=source
set BIN_DIR=bin
set LIB_DIR=lib
set CFLAGS=-std=c17 -Wall -Wextra -g 
set LDFLAGS=-L%LIB_DIR% -lSDL2Main -lSDL2
set SRC_FILES=
:: Create the bin directory if it doesn't exist
if not exist %BIN_DIR% mkdir %BIN_DIR%

:: Find all .c and .h files recursively
for /r %SRC_DIR% %%f in (*.c) do (
    set "SRC_FILES=!SRC_FILES! "%%f""
)


:: Compile and link
gcc %CFLAGS% -I%SRC_DIR% !SRC_FILES! %LDFLAGS% -o %BIN_DIR%\SoftwareRendering.exe

:: Print debug information
echo Compiler: gcc
echo Compiler Flags: %CFLAGS%
echo Source Files: !SRC_FILES!

exit /b
