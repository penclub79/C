#pragma once

class CStack 
{

public:
	CStack(int iItemType, int _iSize);
	~CStack();
	// String ���� int���� ItemŸ�� �����ϰ� ���û��

	enum { LINK_ITEM_TYPE_INT, LINK_ITEM_TYPE_STRING };

	struct Link_Item
	{
		int				iItemLength	;
		unsigned char*	pszBuffer	;
		Link_Item*		pNext		;
	};

	void Push(int _iValue);
	void Push(char* pszValue );

	bool Pop(char* pszValue, int* _iValue); // �ּҰ��� �����Ѵ�

	int GetCount();  // value count üũ
	int GetAt(int _iIndex);  // value�� �����´�

private:
	int* m_piBuff;
	int	m_iItemType;		// 0 : int, 1: String

	int m_iIndex;
	int m_iMaxSize;

	Link_Item*	m_pRoot;
};