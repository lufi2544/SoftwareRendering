@echo off
setlocal enabledelayedexpansion

:: Set variables
set SRC_DIR=source
set BIN_DIR=bin
set LIB_DIR=lib
set CFLAGS=-std=c17 -g -gcodeview
set LDFLAGS=-L%LIB_DIR% -lSDL2Main -lSDL2
set SRC_FILES=
:: Create the bin directory if it doesn't exist
if not exist %BIN_DIR% mkdir %BIN_DIR%


:: Compile and link
clang %CFLAGS% -I%SRC_DIR% %SRC_DIR%/main.c %LDFLAGS% -o %BIN_DIR%\SoftwareRendering.exe

:: Print debug information
echo Compiler: gcc
echo Compiler Flags: %CFLAGS%
echo Source Files: main.c

exit /b
