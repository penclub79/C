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


struct Link_Item
{
	Link_Data		stData;
	Link_Item*		pPrev;
	Link_Item*		pNext;
};