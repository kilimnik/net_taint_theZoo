#include <windows.h>
#include <time.h>
#include <stdio.h>

#include "dprint.h"


//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef ENABLE_DPRINTF

//------------------------------------------------------------------------------
void logoutputv(const char *message, va_list ptr)
{
	char logTextBuffer[1024];
	wvsprintfA( logTextBuffer, message, ptr );
	OutputDebugStringA(logTextBuffer);
}
//------------------------------------------------------------------------------
void LogOutput( const char* message, ...)
{
  va_list ptr;
  va_start(ptr, message);
  logoutputv(message, ptr);  
  va_end(ptr);
}
//------------------------------------------------------------------------------
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

