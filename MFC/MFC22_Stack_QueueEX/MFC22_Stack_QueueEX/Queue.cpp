#include "stdafx.h"
#include "Queue.h"
#include "MFC22_Stack_QueueEXDlg.h"

CQueue::CQueue(int _iSize)
: m_piBuff(NULL)
, m_iIndex(0)
, m_iMaxSize(0)
{
	m_piBuff = new int[_iSize];
	m_iMaxSize = _iSize;
	m_iIndex = 0;
}

CQueue::~CQueue()
{
	if (NULL != m_piBuff)
	{
		delete[] m_piBuff;
		m_piBuff = NULL;
	}
}

void CQueue::Push(int _iValue)
{
	m_piBuff[m_iIndex++] = _iValue;
}


int CQueue::Pop()
{
	for (int i = 0; i < GetCount(); i++)
	{
		return m_piBuff[i];
	}
}

// value count 체크
int CQueue::GetCount()
{
	return m_iIndex;
}

// value를 가져온다
int CQueue::GetAt(int _iIndex)
{
	return m_piBuff[_iIndex];
}