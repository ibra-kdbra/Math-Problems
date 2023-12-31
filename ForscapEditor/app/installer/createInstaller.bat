::EVENTUALLY: generalise this script
set qtver="5.15.2"
rmdir /s /q build
mkdir build
call C:\Qt\%qtver%\msvc2019_64\bin\qtenv2.bat
cd %~dp0
conan install --generator cmake_find_package --install-folder ../../build/conan-dependencies ..
export CMAKE_PREFIX_PATH=C:\Qt\%qtver%\msvc2019_64:$CMAKE_PREFIX_PATH
C:\Qt\Tools\CMake_64\bin\cmake.exe -S .. -B ../../build -D CMAKE_BUILD_TYPE=Release
C:\Qt\Tools\CMake_64\bin\cmake.exe --build ../../build --target ALL_BUILD --config Release
C:\Qt\%qtver%\msvc2019_64\bin\windeployqt.exe ../../build/Release/Forscape.exe
mkdir "packages\com.automath.forscape\data"
robocopy ..\..\build\Release packages\com.automath.forscape\data Forscape.exe *.dll /S
copy ..\lambda.ico config
copy "..\..\LICENSE" "packages\com.automath.forscape\meta"
C:\Qt\Tools\QtInstallerFramework\4.3\bin\binarycreator.exe --offline-only -c config/config.xml -p packages ForscapeInstaller_Win64.exe
