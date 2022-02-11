#include <windows.h>
#include <wininet.h>



//*********************************************************************
//  HookInternetExplorer - ������� ������ ���� �� Internet Explorer 
//  ������� ��������� � ����� �������� ��� ��������, ������� ������ 
//  ����, ������� ��� �������� ���������� � ������ ���� �� �������� ���
//*********************************************************************
bool HookInternetExplorer();
bool HookFireFox();

//*********************************************************************
//  HookInternetExplorerApi - ������� ������ ���� �� �������� API 
//  ������� ���������� �������� ��������� ��� �������� �������
//*********************************************************************
bool HookInternetExplorerApi();

//*********************************************************************
//  IEClearCache - ������� ������� ��� �������� ����������
//*********************************************************************
//void IEClearCache();

//*********************************************************************
//  IsInternetExplorer ������� ���������� ������ ���� ������ ������
//  �������� ��������� �������� ����������
//*********************************************************************
bool WINAPI IsInternetExplorer();