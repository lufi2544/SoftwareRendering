@echo off
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="C:/ProgramData/mingw64/mingw64/bin/gcc.exe" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
pause