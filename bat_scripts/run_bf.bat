@echo off
setlocal enabledelayedexpansion

cd /D %~dp0
cd ..

g++ -O3 cpp_scripts\BF.cpp -o bf_prog.exe
if not exist "output\bf" mkdir output\bf

echo Processing all instances with BF variant...
for %%f in (instances\*.txt) do (
    set "filename=%%~nf"
    bf_prog.exe < "%%f" > "output\bf\!filename!.out" 2> "output\bf\!filename!.score"
    set /p score=<"output\bf\!filename!.score"
    echo   Score for !filename!: !score!
)

del bf_prog.exe
echo BF outputs generated in output\bf!
endlocal