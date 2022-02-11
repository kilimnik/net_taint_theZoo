//---------------------------------------------------------------------------


#include "Modules.h"

// Код добавляем только в случае если в модулях
// объявден BotMonitor.h

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef BOTMONITOR
//---------------------------------------------------------------------


#include "BotMonitor.h"
#include "Pipes.h"

#pragma hdrstop
//---------------------------------------------------------------------------

bool BotMonitor::StartServer()
{
	// Запускает серверную часть монитора, которая будет обмениватся
	// с клиентом сообщениями.
	PProcessPipe Monitor = PIPE::CreateProcessPipe((PCHAR)ServerName, true);
   	return Monitor != NULL;
}
//---------------------------------------------------------------------------

bool BotMonitor::SendMessage(PCHAR Message, PCHAR Data, DWORD DataSize)
{
	//  Функция отправляет сообщению клиенту монитора
	return PIPE::SendMessage((PCHAR)ClientName, Message, Data, DataSize, NULL);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
