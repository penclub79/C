#pragma once

class CStack 
{

public:
	CStack(int iItemType, int _iSize);
	~CStack();
	// String 인지 int인지 Item타입 설정하고 스택사용

	enum { LINK_ITEM_TYPE_INT, LINK_ITEM_TYPE_STRING };

	struct Link_Item
	{
		int				iItemLength	;
		unsigned char*	pszBuffer	;
		Link_Item*		pNext		;
	};

	void Push(int _iValue);
	void Push(char* pszValue );

	bool Pop(char* pszValue, int* _iValue); // 주소값을 참조한다

	int GetCount();  // value count 체크
	int GetAt(int _iIndex);  // value를 가져온다

private:
	int* m_piBuff;
	int	m_iItemType;		// 0 : int, 1: String

	int m_iIndex;
	int m_iMaxSize;

	Link_Item*	m_pRoot;
};