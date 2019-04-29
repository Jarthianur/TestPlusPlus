@echo off

where cl >nul 2>nul
IF %ERRORLEVEL% NEQ 0 (echo WARNING: cl is not in the path - please set up Visual Studio to do cl builds) && exit 1

pushd test
cl /GS /W3 /Zi /Od /FC /EHsc /nologo                    main.cpp reflexive_tests.cpp basic_tests.cpp /link /out:cpp11_seq.exe
cl /GS /W3 /Zi /Od /FC /EHsc /nologo            /openmp main.cpp reflexive_tests.cpp basic_tests.cpp /link /out:cpp11_par.exe
cl /GS /W3 /Zi /Od /FC /EHsc /nologo /std:c++14         main.cpp reflexive_tests.cpp basic_tests.cpp /link /out:cpp14_seq.exe
cl /GS /W3 /Zi /Od /FC /EHsc /nologo /std:c++14 /openmp main.cpp reflexive_tests.cpp basic_tests.cpp /link /out:cpp14_par.exe
cl /GS /W3 /Zi /Od /FC /EHsc /nologo /std:c++17         main.cpp reflexive_tests.cpp basic_tests.cpp /link /out:cpp17_seq.exe
cl /GS /W3 /Zi /Od /FC /EHsc /nologo /std:c++17 /openmp main.cpp reflexive_tests.cpp basic_tests.cpp /link /out:cpp17_par.exe
popd
