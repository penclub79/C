#include <stdio.h>
#include <string.h>
#include <malloc.h>


// Single Linked List

typedef struct NODE
{
	//int iData;
	char aszData[16];

	// 데이터 관리를 위한 포인터
	struct NODE* pNext;
}NODE;

// 출력
void PrintList()
{

}

// 삽입
int InsertNewNode(char* pszData)
{
	
}

// 삭제
void DeleteNode(char* pszData)
{

}

/*
연결리스트 코딩 순서
1. 전체 리스트 출력 함수 작성
2. 새 노드 추가 함수 작성(개발에 앞서 절차를 글로 써본다.)
3. 전체 리스트 삭제 함수
4. 각 함수를 작성할 때마다 main()에서 테스트 코드를 실행한다.
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