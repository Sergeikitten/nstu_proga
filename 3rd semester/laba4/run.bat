
@echo off
gcc -o client.exe source\client.c
gcc -o server.exe source\server.c

@echo on
.\server.exe # t1.txt t2.txt