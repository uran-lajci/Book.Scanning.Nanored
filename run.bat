@echo off

g++ -O3 BF.cpp -o prog.exe
echo Processing B...
prog.exe < input\b.txt > output\b.out
echo Processing F...
prog.exe < input\f.txt > output\f.out

g++ -O3 C.cpp -o prog.exe
echo Processing C...
prog.exe < input\c.txt > output\c.out

g++ -O3 D.cpp -o prog.exe
echo Processing D...
prog.exe < input\d.txt > output\d.out

g++ -O3 E.cpp -o prog.exe
echo Processing E...
prog.exe < input\e.txt > output\e.out

del prog.exe
echo All outputs generated!