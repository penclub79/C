#pragma once

#include "Common.h"

class CQueue
{
	
public:
	CQueue(int _iSize);
	~CQueue();

	void EnQueue(int _iValue);
	void EnQueue(char* _pszValue, int _iSize);
	BOOL DeQueue(Link_Data* pLinkData);
	BOOL GetAt(int _iIndex, Link_Data* pLinkData);

	int GetCount();
	void DeleteAll();

protected:


private:
	Link_Item*	m_pLast;
	Link_Item*	m_pRoot;

	int m_iMaxSize;
	int m_iItemType;
};