#include "stdafx.h"
#include "BasicSock.h"

#define NUM_THREAD 1

CBasicSock::CBasicSock()
{
	m_dwThreadID = 0;
	m_hThread = 0;
}


CBasicSock::~CBasicSock()
{
	if (NULL != m_hThread)
	{
		/*
		WaitForSingleObject �Լ��� ���� ����Ǿ����� ���� ���� WAIT_OBJECT_0�� �����ϰ� 
		Ÿ�Ӿƿ� �Ǿ �����ϴ� ��쿡�� WAIT_TIMEOUT�� �����Ѵ�.
		*/

		DWORD dwResult = WaitForSingleObject(m_hThread, INFINITE);
		// ������ ���� ����
		if (WAIT_OBJECT_0 == dwResult)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		if (WAIT_TIMEOUT == dwResult)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
		
	}
	
}


DWORD WINAPI ThreadProc(LPVOID _lpParam)
{
	UINT iCount = 0;

	for (UINT i = 0; i < 1000; ++i)
	{
		++iCount;
	}
	return 0;

	// Connect
}

int CBasicSock::MainThread()
{
	//DWORD adwThreadID[NUM_THREAD] = { 0 };
	//HANDLE ahThread[NUM_THREAD] = { 0 };
	

	m_hThread = CreateThread(nullptr, 0, ThreadProc, nullptr, 0, &m_dwThreadID);

	// ������ ������ �Ϸ�� ������ ����Ѵ�.
	//WaitForMultipleObjects(NUM_THREAD, m_hThread, TRUE, INFINITE);
	

	return 0;
}


void CBasicSock::Connect(CString _strIP, CString _strUserID, int _iPort)
{



}

void CBasicSock::Write(char* _pData, int _iLength)
{

}

void CBasicSock::Close()
{

}



