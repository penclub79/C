#include "stdafx.h"
#include "Stack.h"
#include "MFC22_Stack_QueueEXDlg.h"


// ������
CStack::CStack(int iItemType, int _iSize)
: m_piBuff(NULL)
, m_iMaxSize(0)
, m_pRoot(NULL)
, m_pLast(NULL)
{
//	m_piBuff = (int*) new char[sizeof(int)*_iSize]; �ؿ� �Ҵ��ϴ� �ڵ�� ���� �ǹ��̴�.
	m_piBuff = new int[_iSize];
	m_iMaxSize = _iSize;
	m_iItemType = iItemType;
	int iTempSize = sizeof(Link_Item);
}

// �Ҹ���
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
			pNode->pszBuffer = (unsigned char*)new int;
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

//void CStack::Push(char* pszValue)
//{
//	
//}

BOOL CStack::Pop(char* pszValue, int* _iValue)
{
	if (GetCount())
	{

		/*if (NULL != m_pLast->pszBuffer)
		{
			delete m_pLast->pszBuffer;
			m_pLast->pszBuffer = NULL;
		}*/

		/*if (NULL != m_pLast)
		{
			delete m_pLast;
			m_pLast = NULL;
		}*/

		return TRUE;
	}
	else
		return FALSE;
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

			// ** 
			pNode = pNode->pNext;


		} while (NULL != pNode);
	}

	//if (NULL == m_pRoot->pNext)
	//{
	//	iCount++;
	//}
	
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