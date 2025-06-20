@echo off
setlocal enabledelayedexpansion

cd /D %~dp0
cd ..

g++ -O3 cpp_scripts\E.cpp -o e_prog.exe
if not exist "output\e" mkdir output\e

echo Processing all instances with E variant...
for %%f in (instances\*.txt) do (
    set "filename=%%~nf"
    e_prog.exe < "%%f" > "output\e\!filename!.out" 2> "output\e\!filename!.score"
    set /p score=<"output\e\!filename!.score"
    echo   Score for !filename!: !score!
)

del e_prog.exe
echo E outputs generated in output\e!
endlocal