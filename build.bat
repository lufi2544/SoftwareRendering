@echo off
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="C:/ProgramData/mingw64/mingw64/bin/gcc.exe" ..
cmake --build .
pause