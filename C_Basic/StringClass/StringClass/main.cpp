#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main()
{
	// String Class 사용전 C에서의 문자열 할당하는 방법
	char* pstr = new char[15];
	char* pnew = new char[15];

	strcpy(pstr, "Old String");

	pstr[15] = 'A';

	strcpy(pnew, pstr);

	strcat(pnew, "& New String");
	

	// 메모리 할당 해제
	if (NULL != pstr)
	{
		delete[] pstr;
		pstr = NULL;
	}

	if (NULL != pnew)
	{
		delete[] pnew;
		pnew = NULL;
	}

	return 0;
}