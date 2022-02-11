#include "stdafx.h"

#include <windows.h>

#include "disasm.h"

extern "C" int WINAPI LDE(void *, int);

void Disasm::init(void)
{

}

void Disasm::uninit(void)
{

}

DWORD Disasm::_getOpcodeLength(void *pAddress)
{
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if defined _WIN64  
  return (DWORD)LDE(pAddress, 64);
#else
  return (DWORD)LDE(pAddress, 0);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
}
