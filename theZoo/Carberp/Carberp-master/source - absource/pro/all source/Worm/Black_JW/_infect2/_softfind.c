
void softfind_CSIDL(int csidl)
{
 // programs

 char szPath[MAX_PATH];
 HRESULT sf=_SHGetFolderPath(NULL,csidl,NULL,0,szPath);
 if (/*@S!=*/sf!=0/*@E*/) return;

 _strcat(szPath,"\\");

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef dbgdbg
  adddeb("\nEXE FINDDIR:%s",szPath);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 rscan(szPath,'C',TRUE);
}

void softfind()
{
 softfind_CSIDL(CSIDL_PROGRAMS); //@S
 softfind_CSIDL(CSIDL_DESKTOP);  //@E
}
