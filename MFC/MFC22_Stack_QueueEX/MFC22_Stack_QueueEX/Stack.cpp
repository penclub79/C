#include "stdafx.h"
#include "Stack.h"
#include "MFC22_Stack_QueueEXDlg.h"


// 생성자
CStack::CStack(int _iSize)
: m_iMaxSize(0)
, m_pRoot(NULL)
, m_pLast(NULL)
{
	//m_piBuff = (int*) new char[sizeof(int)*_iSize]; 밑에 할당하는 코드와 같은 의미이다.
	//m_piBuff = new int[_iSize];
	m_iMaxSize = _iSize;
}

// 소멸자
CStack::~CStack()
{
	DeleteAll();
}

void CStack::Push(int _iValue)
{
	// Int 일때
	
/*		if (NULL == m_pRoot)
		{
			// POP할때 메모리 해제
			m_pRoot = new Link_Item1; // 12바이트
			memset(m_pRoot, 0, sizeof(Link_Item1));

			m_pRoot->iItemLength = sizeof(int);
			m_pRoot->uBuf.pszBuffer = (unsigned char*)new int;
			// 초기화
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
			// 복사
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

void CStack::Push(char* _pszValue, int iSize)
{
	// String 일때
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

//BOOL CStack::Pop(Link_Data* pLinkData)
//{
//	Link_Item1* pCurr = m_pRoot;
//	Link_Item1* pPrev = NULL;
//
//	// Int 일때
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

// value count 체크
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

// value를 가져온다
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
//			// 현재 인덱스의 데이터를 가지고 와야한다.
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

// 프로그램 종료시 메모리 해제 함수
void CStack::DeleteAll()
{
	Link_Item1* pNode = m_pRoot;
	Link_Item1* pNext = NULL;

	if (NULL != pNode)
	{
		while (NULL != pNode)
		{
			pNext = pNode->pNext;
			
			// 현재 노드 메모리를 해제한다.
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

			// 다음 노드로 이동한다.
			pNode = pNext;
		}
	}
}