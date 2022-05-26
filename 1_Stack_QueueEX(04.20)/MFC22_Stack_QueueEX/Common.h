#pragma once

// String 인지 int인지 Item타입 설정하고 스택사용


struct Link_Data
{
	unsigned char*	pszBuffer;
	int				iItemLength;
};

/*
구조체의 변수들은 4바이트 단위이다.
예를들어 구조체 안에서 char자료형도 4바이트이다.
*/


struct Link_Item
{
	Link_Data		stData;
	Link_Item*		pPrev;
	Link_Item*		pNext;
};