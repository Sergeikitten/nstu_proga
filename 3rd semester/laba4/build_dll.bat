gcc -c -o dll\dllmain.o dll\dllmain.c 
gcc -c -o dll\MainLibrary.o dll\MainLibrary.c
gcc -shared -o  dll\fileProcess.dll dll\dllmain.o dll\MainLibrary.o


@echo off

set objects=dll\*.o
if exist %objects% del %objects%

