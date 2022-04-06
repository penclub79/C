#pragma once
/*

��ũ�帮��Ʈ�� Ȱ���� ���� ����
[Data][���� �ּҰ�] -> [Data][���� �ּҰ�] -> [Data][NULL]

PUSH
4����Ʈ�� ��ũ �������� ���̰� ROOT�������� �����Ǹ� ó�� ������ �������� �ּҰ��� NULL�̴�.

POP
LastItem �ּҰ��� ���� ������ ���� ROOT�����۱��� ��� POP�� �ؾ��Ѵ�.

*/

class CStack 
{

public:
	CStack(int iItemType, int _iSize);
	~CStack();
	// String ���� int���� ItemŸ�� �����ϰ� ���û��

	enum { LINK_ITEM_TYPE_INT, LINK_ITEM_TYPE_STRING, LINK_ITEM_TYPE_INT64 };
	
	/*
	����ü�� �������� 4����Ʈ �����̴�.
	������� ����ü �ȿ��� char�ڷ����� 4����Ʈ�̴�.
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

	BOOL Pop(char* pszValue, int* _iValue);		// �ּҰ��� �����Ѵ�
	BOOL GetAt(int _iIndex, char* pszValue, int* _iValue );						// value�� �����´�

	int GetCount();								// value count üũ
	
								
private:
	Link_Item1*	m_pLast;
	Link_Item1*	m_pRoot;

	int	m_iItemType;				// 0 : int, 1: String
	int m_iMaxSize;
};