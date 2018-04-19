#pragma once

#include <Windows.h>
#include <stdio.h>
#include <string.h>

#ifdef DLL_IMPLEMENT
#define DLL_API extern "C" __declspec(dllexport)
#else
#define DLL_API extern "C" __declspec(dllimport)
#endif

#define PATH_LEN 512

bool __stdcall DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
DLL_API void Launcher(char *strList, char *targetFile, DWORD numParameter);

