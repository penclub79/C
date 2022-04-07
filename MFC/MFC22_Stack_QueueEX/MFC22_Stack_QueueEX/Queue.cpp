#include "stdafx.h"
#include "Queue.h"
#include "MFC22_Stack_QueueEXDlg.h"

CQueue::CQueue(int _iItemType, int _iSize)
: m_pRoot(NULL)
, m_pLast(NULL)
, m_iMaxSize(0)
{

	m_iMaxSize = _iSize;

}

CQueue::~CQueue()
{
	DeleteAll();
}

// 사이즈 체크 함수 만들기

void CQueue::EnQueue(int _iValue)
{
	Link_Item2* pNode = NULL;
	
	if (m_iMaxSize > GetCount()) // 포화 상태인지 체크
	{
		pNode = new Link_Item2;
		memset(pNode, 0, sizeof(Link_Item2));

		if (LINK_ITEM_TYPE_INT == m_iItemType)
		{
			pNode->iItemLenght = sizeof(int);
			pNode->uBuf.iValue = _iValue;
		}
		/*else if (LINK_ITEM_TYPE_STRING == m_iItemType)
		{

		}*/

		pNode->pPrev = NULL;
		pNode->pNext = NULL;
		
	
		// 생성된 노드는 이전 노드 주소를 가르킨다.
		pNode->pPrev = m_pLast;

		// 이전 노드는 다음 노드를 가르킨다.
		if (NULL != m_pLast)
		{
			m_pLast->pNext = pNode;
		}

		// 새롭게 생성된 노드는 마지막 노드가 된다.
		m_pLast = pNode;
	}

}

int CQueue::DeQueue()
{
	if (GetCount() <= 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

//// value count 체크
int CQueue::GetCount()
{
	Link_Item2* pNode = m_pRoot;
	int iCount = 0;	

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

// 현재 Index의 값을 가져오는 함수
int CQueue::GetAt(int _iIndex)
{
	Link_Item2* pNode = m_pRoot;
	int iIndex = 0;
	int iVal = 0;

	// 노드가 있는지 체크
	if (0 < GetCount())
	{
		do
		{
			// 매개변수 인덱스와 함수 인덱스가 일치하면 해당 노드의 값을 가지고 온다.
			if (iIndex == _iIndex)
			{
				return iVal = pNode->uBuf.iValue;
			}
			else
			{
				pNode = pNode->pNext;
				iIndex++;
			}
		} while (NULL != pNode);
	}

	return iVal;
}

void CQueue::DeleteAll()
{
	Link_Item2* pNode = m_pRoot;
	Link_Item2* pNext = NULL;

	do
	{
		// 노드의 다음 주소를 담는다.
		pNext = pNode->pNext;

		// 현재 노드가 마지막 노드인지 체크한다.
		if (NULL != pNode->pNext)
		{
			if (LINK_ITEM_TYPE_STRING == m_iItemType)
			{
				if (NULL != pNode->uBuf.pszBuffer)
				{
					delete[] pNode->uBuf.pszBuffer;
					pNode->uBuf.pszBuffer = NULL;
				}
			}
			delete pNode;
			pNode = NULL;
		}

		// 현재 노드는 다음 노드의 주소를 담는다.
		pNode = pNext;
		
	} while (NULL != pNode);
}
