//---------------------------------------------------------------------------

#include "BotEvents.h"
#include "GetApi.h"
#include "BotDebug.h"
#include "Strings.h"
#include "Utils.h"
//#include "Loader.h"
//#include "RuBnk/java_patcher.h"

#include "Modules.h"

//---------------------------------------------------------------------------


void ExplorerFirstStart(PEventData Data)
{
	// ������ ������ ���� � ����������



}
//---------------------------------------------------------------------------


void ExplorerStart(PEventData Data)
{


}
//---------------------------------------------------------------------------

void SVChostStart(PEventData Data, bool &Cancel)
{
	// �������� ������� ���������� � �������� svchost

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
}
//---------------------------------------------------------------------------

void ApplicationStarted(PEventData Data)
{
	// �������� ����������� ����������

}
//---------------------------------------------------------------------------

