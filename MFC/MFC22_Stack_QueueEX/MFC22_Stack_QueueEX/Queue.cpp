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
	// String 일때
	WCHAR* pszBuff = NULL;
	pszBuff = (WCHAR*)_pszValue;

	if (m_iMaxSize > GetCount())
	{
		Link_Item* pNode = NULL;

		pNode = new Link_Item;
		memset(pNode, 0, sizeof(Link_Item));

		// String을 받기 위한 버퍼 동적 할당
		pNode->stData.pszBuffer = new unsigned char[_iSize + 1];
		// 해당 버퍼 초기화
		memset(pNode->stData.pszBuffer, 0, _iSize + 1);

		// WCHAR(유니코드) -> char(멀티바이트) 값 복사
		WideCharToMultiByte(CP_ACP, 0, pszBuff, -1, (char*)pNode->stData.pszBuffer, _iSize, 0, 0);

		pNode->stData.iItemLength = _iSize;


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
	else
	{
		AfxMessageBox(_T("MAX SIZE 초과"));
	}

}
// value
BOOL CQueue::DeQueue(Link_Data* pLinkData)
{
	Link_Item* pNode = m_pRoot;
	BOOL bResult = FALSE;

	if (NULL != pNode)
	{
		// 노드의 처음을 POP한다.
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

// value count 체크
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

// 현재 Index의 값을 가져오는 함수
int CQueue::GetAt(int _iIndex, Link_Data* pLinkData)
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

void CQueue::DeleteAll()
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
