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
		WaitForSingleObject 함수는 정상 종료되었을시 리턴 값을 WAIT_OBJECT_0을 리턴하고 
		타임아웃 되어서 리턴하는 경우에는 WAIT_TIMEOUT을 리턴한다.
		*/

		DWORD dwResult = WaitForSingleObject(m_hThread, INFINITE);
		// 스레드 정상 종료
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

	// 쓰레드 실행이 완료될 때까지 대기한다.
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



