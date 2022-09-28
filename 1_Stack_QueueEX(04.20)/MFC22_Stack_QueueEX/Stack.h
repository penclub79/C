#pragma once
/*

링크드리스트를 활용한 스택 구현
[Data][다음 주소값] -> [Data][다음 주소값] -> [Data][NULL]

PUSH
4바이트씩 링크 아이템이 쌓이고 ROOT아이템이 생성되며 처음 생성된 아이템의 주소값은 NULL이다.

POP
LastItem 주소값을 가진 변수를 통해 ROOT아이템까지 모두 POP을 해야한다.

*/

#include "Common.h"

class CStack 
{

public:
	CStack(int _iSize);
	~CStack();

	void Push(char* _pszValue, int _iSize);
	void DeleteAll();
	BOOL Pop(Link_Data* pLinkData);						// 주소값을 참조한다
	BOOL GetAt(int _iIndex, Link_Data* pLinkData);		// value를 가져온다
	int	GetCount();										// value count 체크
	
private:
	Link_Item*	m_pLast;
	Link_Item*	m_pRoot;
	int m_iMaxSize;
};