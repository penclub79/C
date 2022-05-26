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

void CStack::Push(char* _pszValue, int _iSize)
{
	// String 일때
	WCHAR*		pszBuff = NULL;
	Link_Item*	pstNode	= NULL;

	pszBuff = (WCHAR*)_pszValue;
	
	if (m_iMaxSize > GetCount())
	{
		pstNode = new Link_Item;
		memset(pstNode, 0, sizeof(Link_Item));

		// String을 받기 위한 버퍼 동적 할당
		pstNode->stData.pszBuffer = new unsigned char[_iSize + 1];

		// 해당 버퍼 초기화
		memset(pstNode->stData.pszBuffer, 0, _iSize + 1);
		
		pstNode->stData.iItemLength = _iSize;

		WideCharToMultiByte(CP_ACP, 0, pszBuff, -1, (char*)pstNode->stData.pszBuffer, _iSize, 0, 0);

		pstNode->pNext = NULL;

		// 생성된 노드는 이전 노드 주소를 가르킨다.
		pstNode->pPrev = m_pLast;

		if (NULL == m_pRoot)
			m_pRoot = pstNode;

		// 이전 노드는 다음 노드를 가르킨다.
		if (NULL != m_pLast)
			m_pLast->pNext = pstNode;

		// 새롭게 생성된 노드는 마지막 노드가 된다.
		m_pLast = pstNode;
	}
	else
	{
		AfxMessageBox(_T("MAX SIZE 초과"));
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
			// 노드의 끝을 POP한다.
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

					// 노드가 2개 이상일 때
					if (NULL != m_pLast->pPrev)
					{
						m_pLast = pNode->pPrev;
						delete pNode;
						pNode = NULL;
						m_pLast->pNext = NULL;

					}
					else // 노드가 하나 일때
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
				if (NULL != pNode->stData.pszBuffer)
					bResult = TRUE;
				else
					bResult = FALSE;

				// 일반 변수는 값을 대입하지만 나머지는 memcpy를 사용한다.
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
				if (NULL != pNode->stData.pszBuffer)
				{
					delete[] pNode->stData.pszBuffer;
					pNode->stData.pszBuffer = NULL;
				}
				delete pNode;
				pNode = NULL;
			}

			// 다음 노드로 이동한다.
			pNode = pNext;
		}
	}
}