#pragma once

// String ���� int���� ItemŸ�� �����ϰ� ���û��


struct Link_Data
{
	unsigned char*	pszBuffer;
	int				iItemLength;
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