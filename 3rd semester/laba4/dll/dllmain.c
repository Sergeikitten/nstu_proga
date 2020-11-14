// dllmain.cpp : Определяет точку входа для приложения DLL.


//#include "stdafx.h"


//#include "pch.h"
#include <stdio.h>
#include <windows.h>
 

__declspec(dllexport) CHAR __cdecl fileProcessing(CHAR*, char);
BOOL APIENTRY DLLMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //printf("The DLL is loaded, DLL Skeleton");
        break;
    case DLL_THREAD_ATTACH:
        //printf("A thread is created in this process, DLL Skeleton");
        break;
    case DLL_THREAD_DETACH:
        //printf("A thread is destroyed in this process, DLL Skeleton");
        break;
    case DLL_PROCESS_DETACH:
        //printf("The DLL is unloaded, DLL Skeleton");
        break;
    }
    return TRUE;
}

