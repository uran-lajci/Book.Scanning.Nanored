@echo off
setlocal enabledelayedexpansion

cd /D %~dp0
cd ..

g++ -O3 cpp_scripts\C.cpp -o c_prog.exe
if not exist "output\c" mkdir output\c

echo Processing all instances with C variant...
for %%f in (instances\*.txt) do (
    set "filename=%%~nf"
    c_prog.exe < "%%f" > "output\c\!filename!.out" 2> "output\c\!filename!.score"
    set /p score=<"output\c\!filename!.score"
    echo   Score for !filename!: !score!
)

del c_prog.exe
echo C outputs generated in output\c!
endlocal