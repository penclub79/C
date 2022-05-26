#include "stdafx.h"
#include "Stack.h"
#include "MFC22_Stack_QueueEXDlg.h"


// ������
CStack::CStack(int _iSize)
: m_iMaxSize(0)
, m_pRoot(NULL)
, m_pLast(NULL)
{
	//m_piBuff = (int*) new char[sizeof(int)*_iSize]; �ؿ� �Ҵ��ϴ� �ڵ�� ���� �ǹ��̴�.
	//m_piBuff = new int[_iSize];
	m_iMaxSize = _iSize;
}

// �Ҹ���
CStack::~CStack()
{
	DeleteAll();
}

void CStack::Push(char* _pszValue, int _iSize)
{
	// String �϶�
	WCHAR*		pszBuff = NULL;
	Link_Item*	pstNode	= NULL;

	pszBuff = (WCHAR*)_pszValue;
	
	if (m_iMaxSize > GetCount())
	{
		pstNode = new Link_Item;
		memset(pstNode, 0, sizeof(Link_Item));

		// String�� �ޱ� ���� ���� ���� �Ҵ�
		pstNode->stData.pszBuffer = new unsigned char[_iSize + 1];

		// �ش� ���� �ʱ�ȭ
		memset(pstNode->stData.pszBuffer, 0, _iSize + 1);
		
		pstNode->stData.iItemLength = _iSize;

		WideCharToMultiByte(CP_ACP, 0, pszBuff, -1, (char*)pstNode->stData.pszBuffer, _iSize, 0, 0);

		pstNode->pNext = NULL;

		// ������ ���� ���� ��� �ּҸ� ����Ų��.
		pstNode->pPrev = m_pLast;

		if (NULL == m_pRoot)
			m_pRoot = pstNode;

		// ���� ���� ���� ��带 ����Ų��.
		if (NULL != m_pLast)
			m_pLast->pNext = pstNode;

		// ���Ӱ� ������ ���� ������ ��尡 �ȴ�.
		m_pLast = pstNode;
	}
	else
	{
		AfxMessageBox(_T("MAX SIZE �ʰ�"));
	}
}

BOOL CStack::Pop(Link_Data* pLinkData)
{
	Link_Item* pNode = m_pRoot;
	BOOL bResult = FALSE;

	if (NULL != pNode)
	{
		do
		{
			// ����� ���� POP�Ѵ�.
			if (NULL == pNode->pNext)
			{
				if (NULL != pLinkData->pszBuffer)
				{
					memcpy(pLinkData->pszBuffer, &pNode->stData.pszBuffer[0], pNode->stData.iItemLength);
					pLinkData->iItemLength = pNode->stData.iItemLength;
				}

				if (NULL != pNode)
				{
					if (NULL != pNode->stData.pszBuffer)
					{
						delete[] pNode->stData.pszBuffer;
						pNode->stData.pszBuffer = NULL;
					}

					// ��尡 2�� �̻��� ��
					if (NULL != m_pLast->pPrev)
					{
						m_pLast = pNode->pPrev;
						delete pNode;
						pNode = NULL;
						m_pLast->pNext = NULL;

					}
					else // ��尡 �ϳ� �϶�
					{
						delete pNode;
						pNode = NULL;
						m_pRoot = NULL;
						m_pLast = NULL;
					}
				}
				bResult = TRUE;
				break;
			}

			pNode = pNode->pNext;

		} while (NULL != pNode);
	}
	return bResult;
}

// value count üũ
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

// value�� �����´�
BOOL CStack::GetAt(int _iIndex, Link_Data* pLinkData)
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

// ���α׷� ����� �޸� ���� �Լ�
void CStack::DeleteAll()
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