#define DLL_IMPLEMENT

#include "Launcher.h"

bool __stdcall DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return true;
}
DLL_API void Launcher(char *strList, char *targetFile, DWORD numParameter)
{
	FILE *pFile = NULL;
	char *fileStr = new char[PATH_LEN];
	char *fileStrWithPara = new char[PATH_LEN];
	char *tmpStr = new char[PATH_LEN];
	char *tmpStr2 = tmpStr;
	char *tmpPointer = strList;

	memset(fileStr, 0, PATH_LEN);
	memset(fileStrWithPara, 0, PATH_LEN);
	memset(tmpStr, 0, PATH_LEN);

	sprintf(fileStr, "%s%s", strList, targetFile);

	pFile = fopen(fileStr, "r");
	if (!pFile)
	{
		MessageBoxA(NULL, "Invalid target file!", "Error", MB_OK);
		return;
	}
	fclose(pFile);

	//Execute Windows system command
	//Turn to the directory of the desired subfile (to the same hierarchy with Shelxtl.exe)
	sprintf(tmpStr, "cd %s\n", strList);
	system(tmpStr);

	memset(tmpStr, 0, PATH_LEN);

	//Open the subfile
	strncpy(tmpStr2, targetFile, strlen(targetFile));
	tmpStr2 += strlen(targetFile);
	strncpy(tmpStr2, " ", 1);
	tmpStr2++;
	
	DWORD i;
	for (i = 1; i < numParameter + 1; i++)
	{
		tmpPointer = (char*)((i * PATH_LEN) + strList);
		strncpy(tmpStr2, tmpPointer, strlen(tmpPointer));
		tmpStr2 += strlen(tmpPointer);
		strncpy(tmpStr2, " ", 1);
		tmpStr2++;
	}
	strncpy(tmpStr2, "\n", 1);
	system(tmpStr);
	
	delete fileStr;
	delete fileStrWithPara;
	delete tmpStr;
}
