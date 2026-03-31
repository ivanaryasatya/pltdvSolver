@echo off
echo Mengkompilasi PLTDV Solver...
g++ main.cpp -o pltdvSolver.exe -O2 -static -static-libgcc -static-libstdc++
if %errorlevel% neq 0 (
    echo Kompilasi gagal!
    pause
    exit /b %errorlevel%
)
echo Kompilasi berhasil! Menjalankan program...
pltdvSolver.exe
