if not exist release mkdir release
cd release
cmake -S ../ -B . -G "MinGW Makefiles"
mingw32-make.exe
xcopy ..\data .\data /s /e /y

asteroids.exe
cd ..