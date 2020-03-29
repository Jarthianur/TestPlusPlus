@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -host_arch=amd64 -arch=amd64
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -test
where cl >nul 2>nul
IF %ERRORLEVEL% NEQ 0 (echo ERROR: cl is not in PATH) && exit 1
set CL_ARGS=/GS /Zi /Od /W3 /WX /wd4996 /FC /EHsc /nologo /I..\include
set CL_FILES=main.cpp reflexive_tests.cpp basic_tests.cpp

pushd test
cl %CL_ARGS%                    %CL_FILES% /link /out:cpp11_seq.exe
cl %CL_ARGS%            /openmp %CL_FILES% /link /out:cpp11_par.exe
cl %CL_ARGS% /std:c++14         %CL_FILES% /link /out:cpp14_seq.exe
cl %CL_ARGS% /std:c++14 /openmp %CL_FILES% /link /out:cpp14_par.exe
cl %CL_ARGS% /std:c++17         %CL_FILES% /link /out:cpp17_seq.exe
cl %CL_ARGS% /std:c++17 /openmp %CL_FILES% /link /out:cpp17_par.exe
popd
