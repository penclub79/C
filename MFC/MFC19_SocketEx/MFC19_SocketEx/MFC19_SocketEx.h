
// MFC19_SocketEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "BasicSock.h"

// CMFC19_SocketExApp:
// �� Ŭ������ ������ ���ؼ��� MFC19_SocketEx.cpp�� �����Ͻʽÿ�.
//

class CMFC19_SocketExApp : public CWinApp
{
public:
	CMFC19_SocketExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	DECLARE_MESSAGE_MAP()

private:
	// Ŭ����
	CBasicSock* m_pClient;

	// �ڷ���
	int m_iConnectCode;


public:
	// �Լ�
	void CleanUp(void);

	// ����
	void Connect(CString strIP, int iPort);

	// ���� ����
	void Close();

	// ���� ����
	int GetConnectStatus();
	void SetConnectStatus(int iErrorCode);

	// �����ID �ޱ�
	void SetUserID(CString strUserID);

	// ������ �ޱ�
	void ReceiveData();

	// ������ ������
	void SendData(CString strData);

	// ������ ������ ��
	void CloseChild();
};

extern CMFC19_SocketExApp theApp;