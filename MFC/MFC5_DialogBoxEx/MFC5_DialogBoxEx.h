
// MFC5_DialogBoxEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC5_DialogBoxExApp:
// �� Ŭ������ ������ ���ؼ��� MFC5_DialogBoxEx.cpp�� �����Ͻʽÿ�.
//

class CMFC5_DialogBoxExApp : public CWinApp
{
public:
	CMFC5_DialogBoxExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC5_DialogBoxExApp theApp;