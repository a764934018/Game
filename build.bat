rmdir /s /q build
mkdir build
cd build
cmake ..
cmake --build .
start Game.sln
start Game.exe