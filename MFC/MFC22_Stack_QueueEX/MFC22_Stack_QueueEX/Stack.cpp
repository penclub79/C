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

void CStack::Push(int _iValue)
{
	// Int �϶�

	if (m_iMaxSize > GetCount())
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

void CStack::Push(char* _pszValue, int _iSize)
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

BOOL CStack::Pop(Link_Data* pLinkData)
{
	Link_Item* pNode = m_pRoot;
	BOOL bResult = FALSE;
	//Link_Data* pNodeData = &pNode->stData;

	if (NULL != pNode)
	{
		do
		{
			// ����� ���� POP�Ѵ�.
			if (NULL == pNode->pNext)
			{
				memcpy(pLinkData, &pNode->stData, sizeof(Link_Data));

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

					if (NULL != m_pLast->pPrev)
					{
						m_pLast = pNode->pPrev;
						delete pNode;
						pNode = NULL;
						m_pLast->pNext = NULL;

						bResult = TRUE;
						break;
					}
					else
					{
						delete pNode;
						pNode = NULL;
						m_pRoot = NULL;

						bResult = TRUE;
						break;
					}
				}
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
				if (NULL != pNode->stData.uBuf.iValue || NULL != pNode->stData.uBuf.pszBuffer)
					bResult = TRUE;
				else
					bResult = FALSE;

				// �Ϲ� ������ ���� ���������� �������� memcpy�� ����Ѵ�.
				memcpy(pLinkData, &pNode->stData, sizeof(Link_Data));
	//			pLinkData->iItemType = pNode->stData.iItemType;
	//			pLinkData->iItemLength = pNode->stData.iItemLength;
	//			pLinkData->
 //= pNode->stData.iItemType;

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