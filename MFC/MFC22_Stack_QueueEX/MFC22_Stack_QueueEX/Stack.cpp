#include "stdafx.h"
#include "Stack.h"
#include "MFC22_Stack_QueueEXDlg.h"


// ������
CStack::CStack(int iItemType, int _iSize)
: m_iMaxSize(0)
, m_pRoot(NULL)
, m_pLast(NULL)
{
//	m_piBuff = (int*) new char[sizeof(int)*_iSize]; �ؿ� �Ҵ��ϴ� �ڵ�� ���� �ǹ��̴�.
	//m_piBuff = new int[_iSize];
	m_iMaxSize = _iSize;
	m_iItemType = iItemType;
}

// �Ҹ���
CStack::~CStack()
{
	DeleteAll();
}

void CStack::Push(int _iValue)
{
	// Int �϶�
	if (LINK_ITEM_TYPE_INT == m_iItemType)
	{
/*		if (NULL == m_pRoot)
		{
			// POP�Ҷ� �޸� ����
			m_pRoot = new Link_Item; // 12����Ʈ
			memset(m_pRoot, 0, sizeof(Link_Item));

			m_pRoot->iItemLength = sizeof(int);
			m_pRoot->pszBuffer = (unsigned char*)new int;
			// �ʱ�ȭ
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
			// ����
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
			pNode->pszBuffer = (unsigned char*)new unsigned char[sizeof(int)];
			// ����
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

BOOL CStack::Pop(char* pszValue, int* _piValue)
{
	Link_Item* pNode = m_pRoot;
	Link_Item* pPrev = NULL;

	// Int �϶�
	if (LINK_ITEM_TYPE_INT == m_iItemType)
	{
		if (NULL != _piValue)
		{
			if (NULL == pNode->pNext)
			{
				delete [] pNode->pszBuffer;
				pNode->pszBuffer = NULL;

				delete pNode;
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
						if (NULL != pNode->pszBuffer)
						{
							delete[] pNode->pszBuffer;
							pNode->pszBuffer = NULL;
						}
						
						if (NULL != pNode)
						{
							delete pNode;
							pNode = NULL;
						}

						pPrev->pNext = NULL;
						m_pLast = pPrev;

						break;
					}
				}
			}
			return TRUE;
		}
		else
			return FALSE;
	}
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
BOOL CStack::GetAt(int _iIndex, char* _pszValue, int* _piValue)
{
	int iIndex = 0;
	BOOL bResult = TRUE;

	Link_Item* pNode = m_pRoot;

	if (NULL != pNode)
	{
		do
		{
			// ���� �ε����� �����͸� ������ �;��Ѵ�.
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

// ���α׷� ����� �޸� ���� �Լ�
void CStack::DeleteAll()
{
	Link_Item* pNode = m_pRoot;
	//Link_Item* pPrev = NULL;
	Link_Item* pNext = NULL;

	if (NULL != pNode)
	{
		while (NULL != pNode)
		{
			pNext = pNode->pNext;
			
			// ���� ��� �޸𸮸� �����Ѵ�.
			if (NULL != pNode)
			{
				if (NULL != pNode->pszBuffer)
				{
					delete[] pNode->pszBuffer;
					pNode->pszBuffer = NULL;
				}
				delete pNode;
				pNode = NULL;
			}

			// ���� ���� �̵��Ѵ�.
			pNode = pNext;

			if (NULL == pNode->pNext)
			{
				if (NULL != pNode->pszBuffer)
				{
					delete[] pNode->pszBuffer;
					pNode->pszBuffer = NULL;
				}
				delete pNode;
				pNode = NULL;
			}
		}
	}
}