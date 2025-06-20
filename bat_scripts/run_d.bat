@echo off
setlocal enabledelayedexpansion

cd /D %~dp0
cd ..

g++ -O3 cpp_scripts\D.cpp -o d_prog.exe
if not exist "output\d" mkdir output\d

echo Processing all instances with D variant...
for %%f in (instances\*.txt) do (
    set "filename=%%~nf"
    d_prog.exe < "%%f" > "output\d\!filename!.out" 2> "output\d\!filename!.score"
    set /p score=<"output\d\!filename!.score"
    echo   Score for !filename!: !score!
)

del d_prog.exe
echo D outputs generated in output\d!
endlocal