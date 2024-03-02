@ECHO OFF
md build
cd build
cmake -DCMAKE_CONFIGURATION_TYPES=Debug .. -G "Visual Studio 15 2017 Win64"
cmake --build . --config Debug

pause