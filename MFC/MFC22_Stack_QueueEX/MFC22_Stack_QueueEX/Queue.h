

#pragma once

class CQueue
{
	
public:
	CQueue(int _iSize);
	~CQueue();

	enum { LINK_ITEM_TYPE_INT, LINK_ITEM_TYPE_STRING, LINK_ITEM_TYPE_INT64 };

	union UBuffer
	{
		int				iValue;
		unsigned char*	pszBuffer;
		long long		lIValue;
	};

	struct Link_Item2
	{
		int				iItemLenght;
		UBuffer			uBuf;
		Link_Item2*		pLeft;
		Link_Item2*		pRight;
	};

	void EnQueue(int _iValue);
	int DeQueue();
	int GetCount();
	int GetAt(int _iIndex);
	int GetFront();
	void DeleteAll();

protected:


private:
	Link_Item2*	m_pLast;
	Link_Item2*	m_pRoot;

	int m_iMaxSize;
};