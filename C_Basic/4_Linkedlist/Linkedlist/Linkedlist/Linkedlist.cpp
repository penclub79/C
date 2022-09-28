#include <stdio.h>


// Single Linked List

typedef struct NODE
{
	int iData;

	// 데이터 관리를 위한 포인터
	struct NODE* pNext;
}NODE;


int main(void)
{
	int val = 0;
	NODE List[4] = { 0 };
	NODE* pNode = NULL;
	//// 마지막 List에는 NULL = 0

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