#pragma once

// String 인지 int인지 Item타입 설정하고 스택사용


enum {
	LINK_ITEM_TYPE_INT, 
	LINK_ITEM_TYPE_STRING 
};

union UBuffer
{
	int				iValue;
	unsigned char*	pszBuffer;
};


struct Link_Data
{
	int				iItemType : 8;
	int				iItemLength : 24;
	UBuffer			uBuf;
};

/*
구조체의 변수들은 4바이트 단위이다.
예를들어 구조체 안에서 char자료형도 4바이트이다.
*/

struct Link_Item1
{
	Link_Data		stData;
	Link_Item1*		pPrev;
	Link_Item1*		pNext;
};

struct Link_Item2
{
	Link_Data		stData;
	Link_Item2*		pPrev;
	Link_Item2*		pNext;
};