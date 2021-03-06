
///////////////////////////////////////////////////////////////
// structures for memory access ///////////////////////////////

 typedef struct wMEMORY_STRUCT{
  WORD word0;        
 } wMEMORY;

 typedef struct dMEMORY_STRUCT{
  DWORD dword0;        
 } dMEMORY;

///////////////////////////////////////////////////////////////
// Check is system NT or not //////////////////////////////////

BOOL SYSTEM_NT=FALSE;
DWORD MAJORW=0;

BOOL isSystemNT()
{
	OSVERSIONINFO osver;

	if (/*@S!=*/MAJORW != 0/*@E*/) return SYSTEM_NT;
	
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
	_GetVersionEx(&osver);

	if (/*@S==*/osver.dwPlatformId == VER_PLATFORM_WIN32_NT/*@E*/) SYSTEM_NT=TRUE; //@S
	MAJORW = osver.dwMajorVersion;                                                 //@E

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef dbgdbg
	adddeb("GLOB SYSTEM_NT:%u (1==TRUE)",SYSTEM_NT);
	adddeb("GLOB MAJORW:%u (5==W2K,XP,WS2003 ; 6==Vista,WS2008,W7)",MAJORW);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	return SYSTEM_NT;
}

///////////////////////////////////////////////////////////////
// CloseCreateThread Economy //////////////////////////////////

void CloseCreateThreadEco(LPTHREAD_START_ROUTINE sr)
{
	DWORD tid;
	_CloseHandle(_CreateThread(NULL, 0, sr, NULL, 0, &tid));
}

