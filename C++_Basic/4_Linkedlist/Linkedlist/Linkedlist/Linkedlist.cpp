#include <stdio.h>

typedef struct NODE
{
	int iData;

	// ������ ������ ���� ������
	struct NODE* pNext;
}NODE;


int main(void)
{
	int val = 0;
	NODE List[4] = { 0 };
	NODE* pNode = NULL;

	//List[0].pNext = &List[1];
	//List[1].pNext = &List[2];
	//List[2].pNext = &List[3];
	//List[3].pNext = NULL;
	//// ������ List���� NULL = 0

	for (int i = 0; i < 4; i++)
	{
		if (3 == i)
		{
			List[i].pNext = NULL;
			break;
		}
		List[i].pNext = &List[i + 1];
	}

	pNode = &List[0];

	while (pNode != NULL)
	{
		pNode->iData = val + 1;
		pNode = pNode->pNext;
		val++;
	}
	
	return 0;
}