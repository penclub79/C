#pragma once

// String ���� int���� ItemŸ�� �����ϰ� ���û��


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
����ü�� �������� 4����Ʈ �����̴�.
������� ����ü �ȿ��� char�ڷ����� 4����Ʈ�̴�.
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