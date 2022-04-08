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

	if (m_iMaxSize > GetCount())
	{
		Link_Item* pNode = NULL;
			
		pNode = new Link_Item;
		memset(pNode, 0, sizeof(Link_Item));

		pNode->stData.iItemLength = sizeof(int);
		pNode->stData.uBuf.iValue = _iValue;
		// itemtype 지정
		pNode->stData.iItemType = LINK_ITEM_TYPE_INT;		

		pNode->pNext = NULL;


		// 생성된 노드는 이전 노드 주소를 가르킨다.
		pNode->pPrev = m_pLast;

		if (NULL == m_pRoot)
			m_pRoot = pNode;

		// 이전 노드는 다음 노드를 가르킨다.
		if (NULL != m_pLast)
			m_pLast->pNext = pNode;

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
		Link_Item* pNode = NULL;

		pNode = new Link_Item;
		memset(pNode, 0, sizeof(Link_Item));

		// String을 받기 위한 버퍼 동적 할당
		pNode->stData.uBuf.pszBuffer = new unsigned char[iSize + 1];
		// 해당 버퍼 초기화
		memset(pNode->stData.uBuf.pszBuffer, 0, iSize + 1);
		
		pNode->stData.iItemLength = iSize;

		// 버퍼에 String값 넣기 memcpy()로 사용
		/*memcpy(pNode->stData.uBuf.pszBuffer, pszBuff, iSize);*/
		memcpy(pNode->stData.uBuf.pszBuffer, pszBuff, iSize);

		// itemtype 지정
		pNode->stData.iItemType = LINK_ITEM_TYPE_STRING;


		pNode->pNext = NULL;	

		// 생성된 노드는 이전 노드 주소를 가르킨다.
		pNode->pPrev = m_pLast;

		if (NULL == m_pRoot)
			m_pRoot = pNode;

		// 이전 노드는 다음 노드를 가르킨다.
		if (NULL != m_pLast)
			m_pLast->pNext = pNode;

		// 새롭게 생성된 노드는 마지막 노드가 된다.
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
			// 노드의 끝을 POP한다.
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

// value count 체크
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

// value를 가져온다
BOOL CStack::GetAt(int _iIndex, Link_Data* pLinkData)
{
	int iIndex = 0;
	
	BOOL bResult = FALSE;

	Link_Item* pNode = m_pRoot;

	if (NULL != pNode)
	{
		do
		{
			// 현재 인덱스의 데이터를 가지고 와야한다.
			if (_iIndex == iIndex)
			{
				if (NULL != pNode->stData.uBuf.iValue || NULL != pNode->stData.uBuf.pszBuffer)
					bResult = TRUE;
				else
					bResult = FALSE;
				// 일반 변수는 값을 대입하지만 나머지는 memcpy를 사용한다.
				memcpy(pLinkData, &pNode->stData, sizeof(Link_Data));

				break;
			}

			pNode = pNode->pNext;
			iIndex++;
		} while (NULL != pNode);

	}

	return bResult;
}

// 프로그램 종료시 메모리 해제 함수
void CStack::DeleteAll()
{
	Link_Item* pNode = m_pRoot;
	Link_Item* pNext = NULL;

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