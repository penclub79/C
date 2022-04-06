

#pragma once

class CQueue
{
	
public:
	CQueue(int _iSize);
	~CQueue();

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
		Link_Item2*		pNext;
	};

	void EnQueue(int _iValue);
	int DeQueue();
	int GetCount();
	int GetAt(int _iIndex);
	int GetFront();

protected:


private:
	Link_Item2*	m_pLast;
	Link_Item2*	m_pRoot;

	int* m_piBuff;
	int m_iRear;
	int m_iFront;
	int m_iMaxSize;
};