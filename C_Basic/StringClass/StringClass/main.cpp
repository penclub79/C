#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main()
{
	// String Class ����� C������ ���ڿ� �Ҵ��ϴ� ���
	char* pstr = new char[15];
	char* pnew = new char[15];

	strcpy(pstr, "Old String");

	pstr[15] = 'A';

	strcpy(pnew, pstr);

	strcat(pnew, "& New String");
	

	// �޸� �Ҵ� ����
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