

#include "bt1.h"

//Write DWORD To Registry
bool regwritedword(HKEY hKey, char *szPath, char *szKey, DWORD dwValue)
{
	HKEY rk = NULL;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Writing registry dword, regwritedword()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	if (RegCreateKeyEx(hKey, szPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &rk,0) != ERROR_SUCCESS) return FALSE;
	if (RegSetValueEx(rk, szKey, 0, REG_DWORD, (BYTE*)&dwValue, sizeof(DWORD)) != ERROR_SUCCESS) return FALSE;
		RegCloseKey(rk);
		return TRUE;
}

//Write Key To Registry
bool regwritekey(HKEY hKey, char *szPath, char *szKey, char *szValue)
{
	HKEY rk = NULL;
	char szVal[MEDBUF];

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Writing registry key, regwritekey()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		sprintf(szVal, szValue);
	if (RegCreateKeyEx(hKey, szPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rk, NULL) != ERROR_SUCCESS) return FALSE;
	if (RegSetValueEx(rk, szKey, 0, REG_SZ, (const unsigned char *)&szVal, sizeof(szVal)) != ERROR_SUCCESS) return FALSE;
		RegCloseKey(rk);
		return TRUE;
}

//Delete Key From Registry
bool regdeletekey(HKEY hKey, char *szPath, char *szKey)
{
	HKEY rk = NULL;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Deleting registry key, regdeletekey()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	if (RegCreateKeyEx(hKey, szPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rk, NULL) != ERROR_SUCCESS) return FALSE;
	if (RegDeleteValue(rk, szKey) != ERROR_SUCCESS) return FALSE;
		RegCloseKey(rk);
		return TRUE;
}