#include "stdafx.h"
#include "Queue.h"
#include "MFC22_Stack_QueueEXDlg.h"

CQueue::CQueue(int _iSize)
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
	Link_Item* pNode = NULL;
	
	if (m_iMaxSize > GetCount()) // ��ȭ �������� üũ
	{
		Link_Item* pNode = NULL;

		pNode = new Link_Item;
		memset(pNode, 0, sizeof(Link_Item));

		pNode->stData.iItemLength = sizeof(int);
		pNode->stData.uBuf.iValue = _iValue;
		// itemtype ����
		pNode->stData.iItemType = LINK_ITEM_TYPE_INT;

		pNode->pNext = NULL;

		// ������ ���� ���� ��� �ּҸ� ����Ų��.
		pNode->pPrev = m_pLast;

		if (NULL == m_pRoot)
			m_pRoot = pNode;

		// ���� ���� ���� ��带 ����Ų��.
		if (NULL != m_pLast)
			m_pLast->pNext = pNode;

		// ���Ӱ� ������ ���� ������ ��尡 �ȴ�.
		m_pLast = pNode;
	}

}

void CQueue::EnQueue(char* _pszValue, int _iSize)
{
	// String �϶�
	WCHAR* pszBuff = NULL;
	pszBuff = (WCHAR*)_pszValue;

	if (m_iMaxSize > GetCount())
	{
		Link_Item* pNode = NULL;

		pNode = new Link_Item;
		memset(pNode, 0, sizeof(Link_Item));

		// String�� �ޱ� ���� ���� ���� �Ҵ�
		pNode->stData.uBuf.pszBuffer = new unsigned char[_iSize + 1];
		// �ش� ���� �ʱ�ȭ
		memset(pNode->stData.uBuf.pszBuffer, 0, _iSize + 1);

		pNode->stData.iItemLength = _iSize;

		// WCHAR(�����ڵ�) -> char(��Ƽ����Ʈ) �� ����
		WideCharToMultiByte(CP_ACP, 0, pszBuff, -1, (char*)pNode->stData.uBuf.pszBuffer, _iSize, 0, 0);

		// itemtype ����
		pNode->stData.iItemType = LINK_ITEM_TYPE_STRING;

		pNode->pNext = NULL;

		// ������ ���� ���� ��� �ּҸ� ����Ų��.
		pNode->pPrev = m_pLast;

		if (NULL == m_pRoot)
			m_pRoot = pNode;

		// ���� ���� ���� ��带 ����Ų��.
		if (NULL != m_pLast)
			m_pLast->pNext = pNode;

		// ���Ӱ� ������ ���� ������ ��尡 �ȴ�.
		m_pLast = pNode;
	}

}

BOOL CQueue::DeQueue(Link_Data* pLinkData)
{
	Link_Item* pNode = m_pRoot;
	BOOL bResult = FALSE;
	//Link_Data* pNodeData = &pNode->stData;

	if (NULL != pNode)
	{
		// ����� ó���� POP�Ѵ�.
		memcpy(pLinkData, &pNode->stData, sizeof(Link_Data));

		if (LINK_ITEM_TYPE_STRING == pNode->stData.iItemType)
		{
			if (NULL != pNode->stData.uBuf.pszBuffer)
			{
				delete[] pNode->stData.uBuf.pszBuffer;
				pNode->stData.uBuf.pszBuffer = NULL;
			}
		}
		if (NULL != m_pRoot->pNext)
		{
			m_pRoot = pNode->pNext;
			delete pNode;
			pNode = NULL;
			m_pRoot->pPrev = NULL;

			bResult = TRUE;
		}
		else
		{
			delete pNode;
			pNode = NULL;
			m_pRoot = NULL;
			m_pLast = NULL;

			bResult = TRUE;
		}
				
	}
	return bResult;
}

//// value count üũ
int CQueue::GetCount()
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

// ���� Index�� ���� �������� �Լ�
int CQueue::GetAt(int _iIndex, Link_Data* pLinkData)
{
	int iIndex = 0;

	BOOL bResult = FALSE;

	Link_Item* pNode = m_pRoot;

	if (NULL != pNode)
	{
		do
		{
			// ���� �ε����� �����͸� ������ �;��Ѵ�.
			if (_iIndex == iIndex)
			{
				if (NULL != pNode->stData.uBuf.iValue || NULL != pNode->stData.uBuf.pszBuffer)
					bResult = TRUE;
				else
					bResult = FALSE;
				// �Ϲ� ������ ���� ���������� �������� memcpy�� ����Ѵ�.
				memcpy(pLinkData, &pNode->stData, sizeof(Link_Data));

				break;
			}

			pNode = pNode->pNext;
			iIndex++;
		} while (NULL != pNode);

	}

	return bResult;
}

void CQueue::DeleteAll()
{
	Link_Item* pNode = m_pRoot;
	Link_Item* pNext = NULL;

	if (NULL != pNode)
	{
		while (NULL != pNode)
		{
			pNext = pNode->pNext;

			// ���� ��� �޸𸮸� �����Ѵ�.
			if (NULL != pNode)
			{
				if (LINK_ITEM_TYPE_STRING == pNode->stData.iItemType)
				{
					if (NULL != pNode->stData.uBuf.pszBuffer)
					{
						delete[] pNode->stData.uBuf.pszBuffer;
						pNode->stData.uBuf.pszBuffer = NULL;
					}
				}
				delete pNode;
				pNode = NULL;
			}

			// ���� ���� �̵��Ѵ�.
			pNode = pNext;
		}
	}
}
