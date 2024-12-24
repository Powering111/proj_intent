:: Run this script at first and whenever source files are added or deleted.

cd build

:: Generate makefiles
premake5.exe gmake2
cd ..

:: Copy custom configuration
copy build\raylib-config.h build\external\raylib-master\src\config.h
make SHELL=CMD clean
