#include "stdafx.h"
#include "Stack.h"
#include "MFC22_Stack_QueueEXDlg.h"


// ������
CStack::CStack(int _iSize)
: m_piBuff(NULL)
, m_iIndex(0)
, m_iMaxSize(0)
{
//	m_piBuff = (int*) new char[sizeof(int)*_iSize]; �ؿ� �Ҵ��ϴ� �ڵ�� ���� �ǹ��̴�.
	m_piBuff = new int[_iSize];
	m_iMaxSize = _iSize;
	m_iIndex = 0;
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
	if (m_iMaxSize > m_iIndex)
		m_piBuff[m_iIndex++] = _iValue;
	else
		AfxMessageBox(_T("������ �ʰ�"));

}

int CStack::Pop()
{
	
	if (GetCount())
		return m_piBuff[--m_iIndex];
	else
		return -1;
}

// value count üũ
int CStack::GetCount()
{
	return m_iIndex;
}

// value�� �����´�
int CStack::GetAt(int _iIndex)
{
	return m_piBuff[_iIndex];
}