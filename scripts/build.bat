@echo off
setlocal

cd ..
rmdir /s /q build
mkdir bin
cmake -G "MinGW Makefiles" -B build -S src -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=../bin
cd build
make

endlocal