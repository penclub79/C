
// SocketTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSocketTestApp:
// �� Ŭ������ ������ ���ؼ��� SocketTest.cpp�� �����Ͻʽÿ�.
//

class CSocketTestApp : public CWinApp
{
public:
	CSocketTestApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSocketTestApp theApp;