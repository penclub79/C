#include <stdio.h>
#include <string.h>
#include <malloc.h>


// Single Linked List

typedef struct NODE
{
	//int iData;
	char aszData[16];

	// ������ ������ ���� ������
	struct NODE* pNext;
}NODE;

// ���
void PrintList()
{

}

// ����
int InsertNewNode(char* pszData)
{
	
}

// ����
void DeleteNode(char* pszData)
{

}

/*
���Ḯ��Ʈ �ڵ� ����
1. ��ü ����Ʈ ��� �Լ� �ۼ�
2. �� ��� �߰� �Լ� �ۼ�(���߿� �ռ� ������ �۷� �ẻ��.)
3. ��ü ����Ʈ ���� �Լ�
4. �� �Լ��� �ۼ��� ������ main()���� �׽�Ʈ �ڵ带 �����Ѵ�.
*/
int main(void)
{
	
	InsertNewNode("01");
	InsertNewNode("02");
	InsertNewNode("03");
	
	PrintList();
	
	DeleteNode("03");

	PrintList();

	return 0;
}