#pragma once
/*

��ũ�帮��Ʈ�� Ȱ���� ���� ����
[Data][���� �ּҰ�] -> [Data][���� �ּҰ�] -> [Data][NULL]

PUSH
4����Ʈ�� ��ũ �������� ���̰� ROOT�������� �����Ǹ� ó�� ������ �������� �ּҰ��� NULL�̴�.

POP
LastItem �ּҰ��� ���� ������ ���� ROOT�����۱��� ��� POP�� �ؾ��Ѵ�.

*/

#include "Common.h"

class CStack 
{

public:
	CStack(int _iSize);
	~CStack();


	void Push(int _iValue, int _iItemType);
	void Push(char* _pszValue, int iSize, int _iItemType);
	void DeleteAll();

	BOOL Pop(Link_Data* pLinkData);						// �ּҰ��� �����Ѵ�
	BOOL GetAt(int _iIndex, Link_Data* pLinkData);		// value�� �����´�

	int GetCount();								// value count üũ
	
								
private:
	Link_Item*	m_pLast;
	Link_Item*	m_pRoot;

	int	m_iItemType;				// 0 : int, 1: String
	int m_iMaxSize;
};