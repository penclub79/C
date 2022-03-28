#include "stdafx.h"
//#include "Control.h"
#include "Search.h"

// 持失切
CSearch::CSearch()
{
}

// 社瑚切
CSearch::~CSearch()
{
}

BOOL CSearch::IsStrFindGet(char* A_PtrFindStr, int A_FindStrSize, char* A_PtrTextStr, int A_TextSize)
{
#if 0
	char* Tv_PtrTxt;

	Tv_PtrTxt = A_PtrTextStr;
	for (int i = 0; i < A_TextSize; i++)
	{
		*A_PtrTextStr = 10;

		//next
		A_PtrTextStr++;
	}

	Tv_PtrTxt = A_PtrTextStr;

	Tv_PtrTxt = (char*)((unsigned int)A_PtrTextStr + 200);
	for (int i = 0; i < 5; i++)
	{
		*Tv_PtrTxt = 10;
		Tv_PtrTxt++;
	}
#endif
		/*

		Search_Word		= A_PtrFindStr;
		Search_WordCnt	= A_FindStrSize;
		Text			= A_PtrTextStr;
		TextCnt			= A_TextSize;
		
		for (TextCnt = 0; Search_WordCnt < TextCnt; Search_WordCnt++)
		{
			IsStrMatch(Search_Word);


			next
			Search_WordCnt++;
		}
		
		
		*/
	return true;
}

BOOL CSearch::IsStrMatch()
{
	/*
	
	if (Tv_PtrFindStr == *Tv_PtrTextStr)
	{

	}
	
	*/
	return FALSE;
}