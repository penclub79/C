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
		pNode->stData.pszBuffer = new unsigned char[_iSize + 1];
		// �ش� ���� �ʱ�ȭ
		memset(pNode->stData.pszBuffer, 0, _iSize + 1);

		// WCHAR(�����ڵ�) -> char(��Ƽ����Ʈ) �� ����
		WideCharToMultiByte(CP_ACP, 0, pszBuff, -1, (char*)pNode->stData.pszBuffer, _iSize, 0, 0);

		pNode->stData.iItemLength = _iSize;


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
	else
	{
		AfxMessageBox(_T("MAX SIZE �ʰ�"));
	}

}
// value
BOOL CQueue::DeQueue(Link_Data* pLinkData)
{
	Link_Item* pNode = m_pRoot;
	BOOL bResult = FALSE;

	if (NULL != pNode)
	{
		// ����� ó���� POP�Ѵ�.
		if (NULL != pLinkData->pszBuffer)
		{
			memcpy(pLinkData->pszBuffer, &pNode->stData.pszBuffer[0], pNode->stData.iItemLength);
			pLinkData->iItemLength = pNode->stData.iItemLength;
		}

		if (NULL != pNode->stData.pszBuffer)
		{
			delete[] pNode->stData.pszBuffer;
			pNode->stData.pszBuffer = NULL;
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

// value count üũ
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
				if (NULL != pNode->stData.pszBuffer)
					bResult = TRUE;
				else
					bResult = FALSE;

				// �Ϲ� ������ ���� ���������� �������� memcpy�� ����Ѵ�.
				if (NULL != pLinkData->pszBuffer)
				{
					memcpy(pLinkData->pszBuffer, &pNode->stData.pszBuffer[0], pNode->stData.iItemLength);
					pLinkData->iItemLength = pNode->stData.iItemLength;
				}

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
				if (NULL != pNode->stData.pszBuffer)
				{
					delete[] pNode->stData.pszBuffer;
					pNode->stData.pszBuffer = NULL;
				}
				
				delete pNode;
				pNode = NULL;
			}

			// ���� ���� �̵��Ѵ�.
			pNode = pNext;
		}
	}
}
