#pragma once
/*

링크드리스트를 활용한 스택 구현
[Data][다음 주소값] -> [Data][다음 주소값] -> [Data][NULL]

PUSH
4바이트씩 링크 아이템이 쌓이고 ROOT아이템이 생성되며 처음 생성된 아이템의 주소값은 NULL이다.

POP
LastItem 주소값을 가진 변수를 통해 ROOT아이템까지 모두 POP을 해야한다.

*/

class CStack 
{

public:
	CStack(int iItemType, int _iSize);
	~CStack();
	// String 인지 int인지 Item타입 설정하고 스택사용

	enum { LINK_ITEM_TYPE_INT, LINK_ITEM_TYPE_STRING, LINK_ITEM_TYPE_INT64 };
	
	/*
	구조체의 변수들은 4바이트 단위이다.
	예를들어 구조체 안에서 char자료형도 4바이트이다.
	*/

	union UBuffer
	{
		int					iValue;
		unsigned char*		pszBuffer;
		long long			llValue;
	};

	struct Link_Item1
	{
		int				iItemLength	; 
		UBuffer			uBuf;
		Link_Item1*		pNext;
	};

	void Push(int _iValue);
	void Push(char* _pszValue );
	void DeleteAll();

	BOOL Pop(char* pszValue, int* _iValue);		// 주소값을 참조한다
	BOOL GetAt(int _iIndex, char* pszValue, int* _iValue );						// value를 가져온다

	int GetCount();								// value count 체크
	
								
private:
	Link_Item1*	m_pLast;
	Link_Item1*	m_pRoot;

	int	m_iItemType;				// 0 : int, 1: String
	int m_iMaxSize;
};