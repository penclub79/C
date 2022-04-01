#include "Stack.h"
#include "stdafx.h"
#include "MFC22_Stack_QueueEXDlg.h"


// 持失切
CStack::CStack(int _iSize)
: m_piBuff(NULL)
, m_iIndex(0)
{
	m_piBuff = new int[_iSize];
	m_iIndex = 0;
}

// 社瑚切
CStack::~CStack()
{
}

void CStack::Push(int _iValue)
{
	m_piBuff[m_iIndex++] = _iValue;
}

int CStack::Pop()
{
	return m_piBuff[--m_iIndex];
}