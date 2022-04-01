#include "stdafx.h"
#include "Queue.h"
#include "MFC22_Stack_QueueEXDlg.h"

CQueue::CQueue(int _iSize)
: m_piBuff(NULL)
, m_iRear(0)
, m_iFront(0)
, m_iMaxSize(0)

{
	m_iMaxSize = _iSize;
	m_piBuff = new int[m_iMaxSize];
	
}

CQueue::~CQueue()
{
	if (NULL != m_piBuff)
	{
		delete[] m_piBuff;
		m_piBuff = NULL;
	}
}

// 사이즈 체크 함수 만들기

void CQueue::EnQueue(int _iValue)
{
	if ((m_iMaxSize - 1) <= GetCount()) // 포화 상태인지 체크
	{
		
	}
	else
	{
		m_iRear = (m_iRear + 1) % m_iMaxSize;
		m_piBuff[m_iRear] = _iValue;
	}
}

int CQueue::DeQueue()
{
	if (GetCount() <= 0)
	{
		return -1;
	}
	else
	{
		m_iFront = (m_iFront + 1) % m_iMaxSize;
		return m_piBuff[m_iFront];
	}
}

// value count 체크
int CQueue::GetCount()
{
	int iCount = m_iRear - m_iFront;
	
	// front가 rear 보다 클 경우
	if (iCount < 0)
	{
		iCount = (m_iMaxSize + m_iRear) - m_iFront;
	}
	return iCount;
}

// value를 가져온다
int CQueue::GetAt(int _iIndex)
{
	return m_piBuff[_iIndex];
}

// value를 가져온다
int CQueue::GetFront()
{
	return m_iFront;
}
