

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_CRYPTO
#include "bt1.h"

//Hash String
DWORD hash(char *szString)
{
	DWORD dwHash = 0, dwStore;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Hashing, hash()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	while (*szString)
	{
			dwHash = (dwHash << 4) + *szString++;
		if (dwStore = dwHash & 0xF0000000)
			dwHash ^= dwStore >> 24;
			dwHash &= ~dwStore;
	}
		return dwHash;
}

//XOR String
void xor(char *szString, int szKey)
{
	int i, length;
	if (szString == NULL) return;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] xoring, xor()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		length = strlen(szString);
	for (i = 0; i < length; i++)
	{
			*szString ^= szKey;
			szString++;
	}
		return;
}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if