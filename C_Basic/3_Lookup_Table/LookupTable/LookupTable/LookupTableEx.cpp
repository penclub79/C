#include <stdio.h>

void LookupTb(int icount)
{
	static char* pszMessage[] = {
		"case 1",
		"case 2",
		"case 3",
		"case 4",
		"case 5"
	};
	if (icount >= 4) icount = 4;
	puts(pszMessage[icount]);
}

void main()
{
	LookupTb(12);
}