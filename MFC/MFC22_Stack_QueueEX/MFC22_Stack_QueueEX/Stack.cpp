#include "stdafx.h"
#include "Stack.h"
#include "MFC22_Stack_QueueEXDlg.h"


// 생성자
CStack::CStack(int iItemType, int _iSize)
: m_piBuff(NULL)
, m_iMaxSize(0)
, m_pRoot(NULL)
, m_pLast(NULL)
{
//	m_piBuff = (int*) new char[sizeof(int)*_iSize]; 밑에 할당하는 코드와 같은 의미이다.
	m_piBuff = new int[_iSize];
	m_iMaxSize = _iSize;
	m_iItemType = iItemType;
	int iTempSize = sizeof(Link_Item);
}

// 소멸자
CStack::~CStack()
{
	if (NULL != m_piBuff)
	{
		delete [] m_piBuff;
		m_piBuff = NULL;
	}
}

void CStack::Push(int _iValue)
{
	// Int 일때
	if (LINK_ITEM_TYPE_INT == m_iItemType)
	{
		
/*		if (NULL == m_pRoot)
		{
			// POP할때 메모리 해제
			m_pRoot = new Link_Item; // 12바이트
			memset(m_pRoot, 0, sizeof(Link_Item));

			m_pRoot->iItemLength = sizeof(int);
			m_pRoot->pszBuffer = (unsigned char*)new int;
			// 초기화
			memset(m_pRoot->pszBuffer, 0, sizeof(int));
			*m_pRoot->pszBuffer = _iValue;

			m_pRoot->pNext = NULL;
			m_pLast = m_pRoot;
		}
		else
		{
			Link_Item* pNode = NULL;
			pNode = new Link_Item;
			memset(pNode, 0, sizeof(Link_Item));

			pNode->iItemLength = sizeof(int);
			pNode->pszBuffer = (unsigned char*)new int;
			// 복사
			memcpy(pNode->pszBuffer, &_iValue, sizeof(int));
			pNode->pNext = NULL;

			m_pLast->pNext = pNode;
			m_pLast = pNode;

		}
*/

		if (m_iMaxSize > GetCount())
		{
			Link_Item* pNode = NULL;

			pNode = new Link_Item;
			memset(pNode, 0, sizeof(Link_Item));

			pNode->iItemLength = sizeof(int);
			pNode->pszBuffer = (unsigned char*)new int;
			// 복사
			memcpy(pNode->pszBuffer, &_iValue, sizeof(int));
			pNode->pNext = NULL;


			if (NULL == m_pRoot)
			{
				m_pRoot = pNode;
				m_pLast = m_pRoot;
			}
			else
			{
				m_pLast->pNext = pNode;
				m_pLast = pNode;
			}
		}
		
	}
}

void CStack::Push(char* _pszValue)
{
	// String 일때
	if (LINK_ITEM_TYPE_STRING == m_iItemType)
	{
		Link_Item* pNode = NULL;

		pNode = new Link_Item;
		memset(pNode, 0, sizeof(Link_Item));

		pNode->iItemLength = sizeof(char);
		pNode->pszBuffer = (unsigned char*)new char;
		// 복사
		memcpy(pNode->pszBuffer, &_pszValue, sizeof(char));
		pNode->pNext = NULL;

		if (NULL == m_pRoot)
		{
			m_pRoot = pNode;
			m_pLast = m_pRoot;
		}
		else
		{
			m_pLast->pNext = pNode;
			m_pLast = pNode;
		}
	}
}

BOOL CStack::Pop(char* pszValue, int* _piValue)
{
	Link_Item* pNode = m_pRoot;
	Link_Item* pPrev = NULL;

	// Int 일때
	if (LINK_ITEM_TYPE_INT == m_iItemType)
	{
		if (NULL != _piValue)
		{
			
			if (NULL == pNode->pNext)
			{
				pNode = NULL;
				m_pRoot = pNode;
				m_pLast = pNode;
			}

			if (NULL != pNode)
			{
				while (NULL != pNode->pNext)
				{
					pPrev = pNode;
					pNode = pNode->pNext;
					if (NULL == pNode->pNext)
					{
						pPrev->pNext = NULL;
						m_pLast = pPrev;
					}
				}
			}

			return TRUE;
		}
		else
			return FALSE;
	}
	
}

// value count 체크
int CStack::GetCount()
{
	int iCount = 0;
	Link_Item* pNode = m_pRoot;

	if (NULL != pNode)
	{
		do
		{
			iCount++;
			pNode = pNode->pNext;
		} while (NULL != pNode);

	}
	return iCount;
}

// value를 가져온다
BOOL CStack::GetAt(int _iIndex, char* _pszValue, int* _piValue)
{
	int iIndex = 0;
	BOOL bResult = TRUE;

	Link_Item* pNode = m_pRoot;

	if (NULL != pNode)
	{
		do
		{
			// 현재 인덱스의 데이터를 가지고 와야한다.
			if (_iIndex == iIndex)
			{
				if (LINK_ITEM_TYPE_INT == m_iItemType)
				{
					if (NULL != _piValue)
						memcpy(_piValue, pNode->pszBuffer, sizeof(int));
					else
						bResult = FALSE;

				}
				else if (LINK_ITEM_TYPE_STRING == m_iItemType)
				{
					if (NULL != _pszValue)
						_pszValue = (char*)pNode->pszBuffer;
					else
						bResult = FALSE;
				}
				break;
			}

			// ** 
			pNode = pNode->pNext;
			iIndex++;

		} while (NULL != pNode);
	}


	return bResult;
}