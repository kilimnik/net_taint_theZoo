
///////////////////////////////////////////////////////////////////////////
// Lowercase string ///////////////////////////////////////////////////////

void lowerstr(char *s,char *r)
{

 DWORD q;             //@S
 DWORD l=strlen(s);   //@E

 for (q=0;q<l;q++)
 {
  if (/*@S&&*/(s[q]>'A')&&(s[q]<'Z')/*@E*/) r[q]=/*@S+*/s[q]+32/*@E*/; else r[q]=s[q];
 }

}

