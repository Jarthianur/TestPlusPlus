REM @echo off

where cl >nul 2>nul
IF %ERRORLEVEL% NEQ 0 (echo WARNING: cl is not in the path - please set up Visual Studio to do cl builds) && exit 1

pushd test
cl /GS /W3 /Zi /Od /WX /MTd /std:c++11 /FC /EHsc /nologo /Focpp11_seq.exe main.cpp reflexive_tests.cpp basic_tests.cpp
cl /GS /W3 /Zi /Od /WX /MTd /std:c++11 /FC /EHsc /nologo /openmp /Focpp11_par.exe main.cpp reflexive_tests.cpp basic_tests.cpp
cl /GS /W3 /Zi /Od /WX /MTd /std:c++14 /FC /EHsc /nologo /Focpp14_seq.exe main.cpp reflexive_tests.cpp basic_tests.cpp
cl /GS /W3 /Zi /Od /WX /MTd /std:c++14 /FC /EHsc /nologo /openmp /Focpp14_par.exe main.cpp reflexive_tests.cpp basic_tests.cpp
cl /GS /W3 /Zi /Od /WX /MTd /std:c++17 /FC /EHsc /nologo /Focpp17_seq.exe main.cpp reflexive_tests.cpp basic_tests.cpp
cl /GS /W3 /Zi /Od /WX /MTd /std:c++17 /FC /EHsc /nologo /openmp /Focpp17_par.exe main.cpp reflexive_tests.cpp basic_tests.cpp
popd
