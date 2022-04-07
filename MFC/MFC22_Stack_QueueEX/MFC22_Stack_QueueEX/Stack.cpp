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
	
/*		if (NULL == m_pRoot)
		{
			// POP�Ҷ� �޸� ����
			m_pRoot = new Link_Item1; // 12����Ʈ
			memset(m_pRoot, 0, sizeof(Link_Item1));

			m_pRoot->iItemLength = sizeof(int);
			m_pRoot->uBuf.pszBuffer = (unsigned char*)new int;
			// �ʱ�ȭ
			memset(m_pRoot->uBuf.pszBuffer, 0, sizeof(int));
			*m_pRoot->uBuf.pszBuffer = _iValue;

			m_pRoot->pNext = NULL;
			m_pLast = m_pRoot;
		}
		else
		{
			Link_Item1* pNode = NULL;
			pNode = new Link_Item1;
			memset(pNode, 0, sizeof(Link_Item1));

			pNode->iItemLength = sizeof(int);
			pNode->uBuf.pszBuffer = (unsigned char*)new int;
			// ����
			memcpy(pNode->uBuf.pszBuffer, &_iValue, sizeof(int));
			pNode->pNext = NULL;

			m_pLast->pNext = pNode;
			m_pLast = pNode;

		}
*/
	if (m_iMaxSize > GetCount())
	{
		Link_Item1* pNode = NULL;
			
		pNode = new Link_Item1;
		memset(pNode, 0, sizeof(Link_Item1));

		pNode->stData.iItemLength = sizeof(int);
		pNode->stData.uBuf.iValue = _iValue;
			
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

void CStack::Push(char* _pszValue, int iSize)
{
	// String �϶�
	WCHAR* pszBuff = NULL;
	pszBuff = (WCHAR*)_pszValue;

	if (m_iMaxSize > GetCount())
	{
		Link_Item1* pNode = NULL;

		pNode = new Link_Item1;
		memset(pNode, 0, sizeof(Link_Item1));

		pNode->stData.iItemLength = sizeof(int);

		for (int i = 0; i < iSize; i++)
		{
			pNode->stData.uBuf.pszBuffer = (unsigned char*)pszBuff;
			pszBuff++;
		}

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

//BOOL CStack::Pop(Link_Data* pLinkData)
//{
//	Link_Item1* pCurr = m_pRoot;
//	Link_Item1* pPrev = NULL;
//
//	// Int �϶�
//	if (LINK_ITEM_TYPE_INT == m_iItemType)
//	{
//		if (NULL != _piValue)
//		{
//			/*if (NULL == pNode->pNext)
//			{
//				delete [] pNode->uBuf.pszBuffer;
//				pNode->uBuf.pszBuffer = NULL;
//
//				delete pNode;
//				pNode = NULL;
//
//				m_pRoot = pNode;
//				m_pLast = pNode;
//			}
//			else
//			{
//				if (NULL != pNode)
//				{
//					while (NULL != pNode->pNext)
//					{
//						pPrev = pNode;
//						pNode = pNode->pNext;
//						if (NULL == pNode->pNext)
//						{
//							if (NULL != pNode->uBuf.pszBuffer)
//							{
//								delete[] pNode->uBuf.pszBuffer;
//								pNode->uBuf.pszBuffer = NULL;
//							}
//							
//							if (NULL != pNode)
//							{
//								delete pNode;
//								pNode = NULL;
//							}
//
//							pPrev->pNext = NULL;
//							m_pLast = pPrev;
//
//							break;
//						}
//					}
//				}
//				return TRUE;
//			}*/
//			if (0 < GetCount())
//			{
//				do
//				{
//					if (NULL == pCurr->pNext)
//					{
//						if (NULL != pCurr->uBuf.pszBuffer)
//						{
//							delete[] pCurr->uBuf.pszBuffer;
//							pCurr->uBuf.pszBuffer = NULL;
//						}
//						if (NULL != pCurr)
//						{
//							delete pCurr;
//							pCurr = NULL;
//						}
//						if (NULL != pPrev)
//						{
//							pPrev->pNext = NULL;
//							m_pLast = pPrev;
//						}
//						else
//						{
//							m_pRoot = NULL;
//							m_pLast = NULL;
//						}
//						break;
//					}
//					pPrev = pCurr;
//					pCurr = pCurr->pNext;
//				} while (NULL != pCurr);
//			}
//			return TRUE;
//		}
//		else
//			return FALSE;
//	}
//	return FALSE;
//}

// value count üũ
int CStack::GetCount()
{
	int iCount = 0;
	Link_Item1* pNode = m_pRoot;

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
//BOOL CStack::GetAt(int _iIndex, char* _pszValue, int* _piValue, long long* _plValue)
//{
//	int iIndex = 0;
//	BOOL bResult = TRUE;
//
//	Link_Item1* pNode = m_pRoot;
//
//	if (NULL != pNode)
//	{
//		do
//		{
//			// ���� �ε����� �����͸� ������ �;��Ѵ�.
//			if (_iIndex == iIndex)
//			{
//				if (LINK_ITEM_TYPE_INT == m_iItemType)
//				{
//					//if (NULL != _piValue)
//						//*_piValue = pNode->uBuf.iValue;
//					//else
//						//bResult = FALSE;
//
//				}
//				else if (LINK_ITEM_TYPE_STRING == m_iItemType)
//				{
//					//if (NULL != _pszValue)
//					//	memcpy(_pszValue, pNode->uBuf.pszBuffer, sizeof(char));
//					//else
//					//	bResult = FALSE;
//				}
//
//				break;
//			}
//
//			// ** 
//			pNode = pNode->pNext;
//			iIndex++;
//
//		} while (NULL != pNode);
//	}
//
//
//	return bResult;
//}

// ���α׷� ����� �޸� ���� �Լ�
void CStack::DeleteAll()
{
	Link_Item1* pNode = m_pRoot;
	Link_Item1* pNext = NULL;

	if (NULL != pNode)
	{
		while (NULL != pNode)
		{
			pNext = pNode->pNext;
			
			// ���� ��� �޸𸮸� �����Ѵ�.
			if (NULL != pNode)
			{
				/*if (NULL != pNode->uBuf.pszBuffer)
				{
					delete[] pNode->uBuf.pszBuffer;
					pNode->uBuf.pszBuffer = NULL;
				}*/
				delete pNode;
				pNode = NULL;
			}

			// ���� ���� �̵��Ѵ�.
			pNode = pNext;
		}
	}
}