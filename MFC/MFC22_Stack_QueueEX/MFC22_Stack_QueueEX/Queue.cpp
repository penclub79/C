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
	//DeleteAll();
}

// ������ üũ �Լ� �����

void CQueue::EnQueue(int _iValue)
{
	Link_Item2* pNode = NULL;
	//if ((m_iMaxSize > GetCount()) // ��ȭ �������� üũ
	//{
	pNode = new Link_Item2;
	memset(pNode, 0, sizeof(Link_Item2));

	pNode->iItemLenght = sizeof(int);
	pNode->uBuf.iValue = _iValue;
	pNode->pLeft = NULL;
	pNode->pRight = NULL;
	//}
	
	if (NULL == m_pRoot)
	{
		Link_Item2* pHeader = NULL;



		m_pRoot = pNode;
		m_pLast = m_pRoot;
	}
	else
	{
		
		/*pNode->pRight = 

		m_pLast = pNode;*/
	}

	GetCount();
}

int CQueue::DeQueue()
{
	if (GetCount() <= 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

//// value count üũ
int CQueue::GetCount()
{
	Link_Item2* pNode = m_pRoot;
	int iCount = 0;	

	if (NULL != pNode)
	{
		do
		{
			iCount++;
			if (NULL != pNode->pRight)
			{
				pNode = pNode->pRight;
			}
		} while (NULL != pNode->pRight);
	}
	

	return iCount;
}
//
//// value�� �����´�
int CQueue::GetAt(int _iIndex)
{
	return _iIndex = 0;
//	return m_piBuff[_iIndex];
}
//
//// value�� �����´�
//int CQueue::GetFront()
//{
//	return m_iFront;
//}

//void CQueue::DeleteAll()
//{
//
//}
