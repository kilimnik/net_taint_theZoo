

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_FILE
#include "bt1.h"

//Delete File
int delete_file(char *szPath)
{

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Deleting file, deletefile()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	if (!file_exists(szPath)) return 2;
	if (!DeleteFile(szPath)) return 1;
	else return 0;
}

//Execute File
bool execute_file(char *szPath, int iVisibility)
{
	BOOL b;
	PROCESS_INFORMATION pInfo;
	STARTUPINFO sInfo;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Executing file, executeh()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		memset(&sInfo, 0, sizeof(sInfo));
		memset(&pInfo, 0, sizeof(pInfo));
		sInfo.cb = sizeof(STARTUPINFO);
		sInfo.dwFlags = STARTF_USESHOWWINDOW;
		sInfo.wShowWindow = SW_HIDE;
	if (iVisibility == 1) sInfo.wShowWindow = SW_SHOW;
		b = CreateProcess(NULL, szPath, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &sInfo, &pInfo);
	if (b == 0)
		return FALSE;
	else
		return TRUE;
}

//Extract Resource
bool extract_resource(char *szResName, char *szResType, char *szExtractPath)
{
	DWORD dwSize, dwWritten = 0;
	HANDLE hFile;
	HGLOBAL hResData;
	HMODULE hInstance = GetModuleHandle(NULL);
	HRSRC hResLocation;
	LPVOID pExe;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Extracting resource, extract_resource()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		hResLocation = FindResource(hInstance, szResName, szResType);
		dwSize = SizeofResource(hInstance, hResLocation);
		hResData = LoadResource(hInstance, hResLocation);
		pExe = LockResource(hResData);
	if (pExe == NULL) return FALSE;
		hFile = CreateFile(szExtractPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;
		WriteFile(hFile, pExe, dwSize, &dwWritten, NULL);
	if (dwWritten != dwSize) return FALSE;
		CloseHandle(hFile);
		return TRUE;
}

//File Exists
bool file_exists(char *szPath)
{
	HANDLE hFile;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Checking if file exists, file_exists()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
			CloseHandle(hFile);
			return FALSE;
	}
		CloseHandle(hFile);
		return TRUE;
}

//Open File
bool open_file(char *szPath)
{
	int i;
		i = (int)ShellExecute(0, "open", szPath, NULL, NULL, SW_SHOW);
	if (i >= 32)
		return TRUE;
	else
		return FALSE;
}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if