
// MFC19_SocketEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

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

public:

	
};

extern CMFC19_SocketExApp theApp;