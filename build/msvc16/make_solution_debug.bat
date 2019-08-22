@echo off

set "PROJECT=stdex"
set "CONFIGURATION=Debug"
set "SOLUTION_DIR=..\solutions\%PROJECT%_msvc16_%CONFIGURATION%"

@pushd ..
@mkdir %SOLUTION_DIR%
@pushd %SOLUTION_DIR%
CMake -G "Visual Studio 16 2019" "%CD%\..\.." -DCMAKE_CONFIGURATION_TYPES:STRING=%CONFIGURATION% -DCMAKE_BUILD_TYPE:STRING=%CONFIGURATION%
@popd
@popd

@echo on
@pause
