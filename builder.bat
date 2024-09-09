@echo off
set architecture=x64
gcc -o .\lib\prng.o -c .\src\prng.c -Iheader -D%architecture%
gcc -o .\lib\queue.o -c .\src\queue.c -Iheader -D%architecture%
gcc -o .\lib\btree.o -c .\src\btree.c -Iheader -D%architecture%
gcc -o .\out\main.exe .\main.c .\lib\prng.o .\lib\queue.o .\lib\btree.o -Iheader -D%architecture%

.\out\main.exe