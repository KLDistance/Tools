// BmpEncrypter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <string.h>

#define DECRYPTED ((WORD)0x0000)
#define ENCRYPTED_WITHOUT_PASSWORD ((WORD)0xFFFE)
#define ENCRYPTED_WITH_PASSWORD ((WORD)0xFFFF)



char* ReadFileContent(char* filename, DWORD* fileSize)
{
	HANDLE hFile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Fail to open file %s\n", filename);
		return NULL;
	}
	DWORD dwRead;
	DWORD dwSize = GetFileSize(hFile, &dwRead);
	*fileSize = dwSize;
	char* buf = new char[dwSize];
	RtlZeroMemory(buf, dwSize);
	ReadFile(hFile, buf, dwSize, &dwRead, 0);
	if (dwRead != dwSize)
	{
		printf("Improper contents on fileRead!\n");
		return FALSE;
	}
	CloseHandle(hFile);
	return buf;
}

BOOL SaveFile(char* filename, char* buf, int len)
{
	printf("File saving...\n");
	HANDLE hFile = CreateFileA(filename, GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Fail to open file %s.\n", filename);
		return FALSE;
	}
	DWORD dwWrite;
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	WriteFile(hFile, buf, len, &dwWrite, 0);
	CloseHandle(hFile);
	printf("File saved.\n");
	return TRUE;
}

BOOL Hide(char* bmpFileName, char* secretFileName)
{
	printf("Start Encryption without a password...\n");
	DWORD startTime = GetCurrentTime();
	DWORD dwBMPSize, dwSecretSize;
	printf("File loading...\n");
	char* pBMP = ReadFileContent(bmpFileName, &dwBMPSize);
	char* pSecretFile = ReadFileContent(secretFileName, &dwSecretSize);
	printf("File loading done.\n");
	DWORD interval = dwBMPSize / dwSecretSize * 3;
	if (*((WORD*)((char*)pBMP + 6)) == ENCRYPTED_WITHOUT_PASSWORD || *((WORD*)((char*)pBMP + 6)) == ENCRYPTED_WITH_PASSWORD)
	{
		printf("This file has been encrypted once! Please do not encrypt it second time.\n");
		delete[] pBMP;
		delete[] pSecretFile;
		return FALSE;
	}
	DWORD* lpFirstPointOffset = (DWORD*)(pBMP + 10);
	printf("The displacement of first pixel is %u.\n", *lpFirstPointOffset);
	char* pCurrentBMP = pBMP + (char)*lpFirstPointOffset;
	char* pCurrentSecret = pSecretFile;
	*((DWORD*)pCurrentBMP) = dwSecretSize;
	(char*)pCurrentBMP += 6;
	*((DWORD*)pCurrentBMP) = interval;
	(char*)pCurrentBMP += 6;
	for (; pCurrentBMP < (pBMP + dwBMPSize) && pCurrentSecret <= (pSecretFile + dwSecretSize);pCurrentBMP += interval)
	{
		*pCurrentBMP = *pCurrentSecret;
		*(pCurrentBMP + 1) = *(pCurrentSecret + 1);
		*(pCurrentBMP + 2) = *(pCurrentSecret + 2);
		pCurrentSecret += 3;
	}
	*((WORD*)((char*)pBMP + 6)) = ENCRYPTED_WITHOUT_PASSWORD;
	SaveFile(bmpFileName, pBMP, dwBMPSize);
	delete[] pBMP;
	delete[] pSecretFile;
	printf("Encryption completed, %u ms elapsed.\n\n", GetCurrentTime() - startTime + 1);
	return TRUE;
}

BOOL HideWithPassword(char* bmpFileName, char* secretFileName, char* password)
{
	printf("Start Encryption with a password...\n");
	DWORD startTime = GetCurrentTime();
	char* OriginalPassword = password;
	DWORD dwBMPSize, dwSecretSize;
	printf("File loading...\n");
	char* pBMP = ReadFileContent(bmpFileName, &dwBMPSize);
	char* pSecretFile = ReadFileContent(secretFileName, &dwSecretSize);
	printf("File loading done.\n");
	DWORD interval = dwBMPSize / dwSecretSize * 3;
	if (*((WORD*)((char*)pBMP + 6)) == ENCRYPTED_WITHOUT_PASSWORD || *((WORD*)((char*)pBMP + 6)) == ENCRYPTED_WITH_PASSWORD)
	{
		printf("This file has been encrypted once! Please do not encrypt it second time.\n");
		delete[] pBMP;
		delete[] pSecretFile;
		return FALSE;
	}
	DWORD* lpFirstPointOffset = (DWORD*)(pBMP + 10);
	printf("The displacement of first pixel is %u.\n", *lpFirstPointOffset);
	char* pCurrentBMP = pBMP + *lpFirstPointOffset;
	char* pCurrentSecret = pSecretFile;
	*((DWORD*)pCurrentBMP) = dwSecretSize;
	(char*)pCurrentBMP += 6;
	*((DWORD*)pCurrentBMP) = interval;
	(char*)pCurrentBMP += 6;
	for (; pCurrentBMP < (pBMP + dwBMPSize) && pCurrentSecret <= (pSecretFile + dwSecretSize); pCurrentBMP += interval)
	{
		if (*password == 0)
		{
			password = OriginalPassword;
		}
		*(pCurrentBMP) = (*pCurrentSecret)^(*password++);
		if (*password == 0)
		{
			password = OriginalPassword;
		}
		*(pCurrentBMP + 1) = (*(pCurrentSecret + 1))^(*password++);
		if (*password == 0)
		{
			password = OriginalPassword;
		}
		*(pCurrentBMP + 2) = (*(pCurrentSecret + 2))^(*password++);
		pCurrentSecret += 3;
	}
	*((WORD*)((char*)pBMP + 6)) = ENCRYPTED_WITH_PASSWORD;
	SaveFile(bmpFileName, pBMP, dwBMPSize);
	delete[] pBMP;
	delete[] pSecretFile;
	printf("Encryption completed, %u ms elapsed.\n\n", GetCurrentTime() - startTime + 1);
	return TRUE;
}

BOOL Recovery(char* bmpFileName, char* secretFileName)
{
	printf("Start decryption without a password...\n");
	DWORD startTime = GetCurrentTime();
	DWORD dwBMPSize;
	printf("File loading...\n");
	char* pBMP = ReadFileContent(bmpFileName, &dwBMPSize);
	printf("File loading done.\n");
	if (*((WORD*)((char*)pBMP + 6)) == ENCRYPTED_WITH_PASSWORD)
	{
		printf("Private encrypted! Password is required to decrypt the file.\n");
		delete[] pBMP;
		return FALSE;
	}
	DWORD* pFirstPoint = (DWORD*)(pBMP + 10);
	printf("The displacement of first pixel is %u.\n", *pFirstPoint);
	DWORD dwSecretSize = *(DWORD*)(pBMP + *pFirstPoint);
	char* buf = new char[dwSecretSize];
	char* pCurrentBMP = pBMP + *pFirstPoint + 12;
	char* FileName = secretFileName;
	DWORD interval = *((DWORD*)((char*)pCurrentBMP - 6));
	DWORD j = 0;
	for (int i = 0; (pCurrentBMP < pBMP + dwBMPSize) && (i < dwSecretSize); )
	{
		if (j == 3)
		{
			pCurrentBMP += interval;
			j = 0;
			continue;
		}
		buf[i] = *(pCurrentBMP + j);
		j++;
		i++;
	}
	SaveFile(secretFileName, buf, dwSecretSize);
	delete[] pBMP;
	delete[] buf;
	printf("Decryption completed, %u ms elapsed.\n\n", GetCurrentTime() - startTime + 1);
	return TRUE;
}

BOOL RecoveryWithPassword(char* bmpFileName, char* secretFileName, char* password)
{
	printf("Start Decryption with a password...\n");
	DWORD startTime = GetCurrentTime();
	char* OriginalPassword = password;
	DWORD dwBMPSize;
	printf("File loading...\n");
	char* pBMP = ReadFileContent(bmpFileName, &dwBMPSize);
	printf("File loading done.\n");
	if (*((WORD*)((char*)pBMP + 6)) == ENCRYPTED_WITHOUT_PASSWORD)
	{
		printf("Public encrypted! No need to input a password.\n");
		delete[] pBMP;
		return FALSE;
	}
	DWORD* pFirstPoint = (DWORD*)(pBMP + 10);
	printf("The displacement of first pixel is %u.\n", *pFirstPoint);
	DWORD dwSecretSize = *(DWORD*)(pBMP + *pFirstPoint);
	char* buf = new char[dwSecretSize];
	char* pCurrentBMP = pBMP + *pFirstPoint + 12;
	char* FileName = secretFileName;
	DWORD interval = *((DWORD*)((char*)pCurrentBMP - 6));
	DWORD j = 0;
	for (int i = 0; (pCurrentBMP < pBMP + dwBMPSize) && (i < dwSecretSize); )
	{
		if (j == 3)
		{
			j = 0;
			pCurrentBMP += interval;
			continue;
		}
		if (*password == 0)
		{
			password = OriginalPassword;
		}
		buf[i] = (*(pCurrentBMP + j)) ^ (*password++);
		j++;
		i++;
	}
	SaveFile(secretFileName, buf, dwSecretSize);
	delete[] pBMP;
	delete[] buf;
	printf("Decryption completed, %u ms elapsed.\n\n", GetCurrentTime() - startTime + 1);
	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 2 && strcmp(argv[1], "help") == 0)
	{
		printf("\nThis is a file encrypter aimed to hide small (< 5MB recommended) file in a Bitmap file (.bmp).\n");
		printf("Tutorial of input format and effect:\n\n");
		printf("Encrypt method used to hide message in a Bitmap.\n");
		printf("  Public mode format:  %s Public Encrypt BMP_filename secret_file_name \n", argv[0]);
		printf("  Private mode format:  %s Private Encrypt BMP_filename secret_file_name Password (within 20 alphabets or numbers) \n\n", argv[0]);
		printf("Decrypt method used to extract message from a Bitmap.\n");
		printf("  Public mode format:  %s Public Encrypt BMP_filename secret_file_name \n", argv[0]);
		printf("  Private mode format:  %s Private Decrypt BMP_filename secret_file_name Password (within 20 alphabets or numbers) \n\n", argv[0]);
		return -1;
	}
	else if (argc < 3)
	{
		printf("Improper parameters input.\n");
		printf("Please refer to the help index. (Format: %s help)", argv[0]);
		return -1;
	}
	if (strcmp(argv[2], "Encrypt") == 0)
	{
		if (strcmp(argv[1], "Public") == 0)
			Hide(argv[3], argv[4]);
		else if (strcmp(argv[1], "Private") == 0)
			HideWithPassword(argv[3], argv[4], argv[5]);
		else
		{
			printf("Error in the Access modifier! Only Public or Private is the legal input.");
			return -1;
		}
	}
	else if (strcmp(argv[2], "Decrypt") == 0)
	{
		if (strcmp(argv[1], "Public") == 0)
			Recovery(argv[3], argv[4]);
		else if (strcmp(argv[1], "Private") == 0)
			RecoveryWithPassword(argv[3], argv[4], argv[5]);
		else
		{
			printf("Error in the Access modifier! Only Public or Private is the legal input.");
			return -1;
		}
	}
	else
	{
		printf("\nError in choosing a method!\n");
		return -1;
	}
    return 0;
}

