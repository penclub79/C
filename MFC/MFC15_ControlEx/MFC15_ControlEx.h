
// MFC15_ControlEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC15_ControlExApp:
// �� Ŭ������ ������ ���ؼ��� MFC15_ControlEx.cpp�� �����Ͻʽÿ�.
//

class CMFC15_ControlExApp : public CWinApp
{
public:
	CMFC15_ControlExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC15_ControlExApp theApp;