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

// ������ üũ �Լ� �����

void CQueue::EnQueue(int _iValue)
{
	Link_Item2* pNode = NULL;
	
	if (m_iMaxSize > GetCount()) // ��ȭ �������� üũ
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
		
	
		// ������ ���� ���� ��� �ּҸ� ����Ų��.
		pNode->pPrev = m_pLast;

		// ���� ���� ���� ��带 ����Ų��.
		if (NULL != m_pLast)
		{
			m_pLast->pNext = pNode;
		}

		// ���Ӱ� ������ ���� ������ ��尡 �ȴ�.
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

//// value count üũ
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

// ���� Index�� ���� �������� �Լ�
int CQueue::GetAt(int _iIndex)
{
	Link_Item2* pNode = m_pRoot;
	int iIndex = 0;
	int iVal = 0;

	// ��尡 �ִ��� üũ
	if (0 < GetCount())
	{
		do
		{
			// �Ű����� �ε����� �Լ� �ε����� ��ġ�ϸ� �ش� ����� ���� ������ �´�.
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
		// ����� ���� �ּҸ� ��´�.
		pNext = pNode->pNext;

		// ���� ��尡 ������ ������� üũ�Ѵ�.
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

		// ���� ���� ���� ����� �ּҸ� ��´�.
		pNode = pNext;
		
	} while (NULL != pNode);
}
