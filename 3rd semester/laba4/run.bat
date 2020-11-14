
@echo off
gcc -o client.exe core\client.c
gcc -o server.exe core\server.c

@echo on
.\server.exe # t1.txt t2.txt