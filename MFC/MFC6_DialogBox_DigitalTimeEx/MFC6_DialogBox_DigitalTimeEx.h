
// MFC6_DialogBox_DigitalTimeEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC6_DialogBox_DigitalTimeExApp:
// �� Ŭ������ ������ ���ؼ��� MFC6_DialogBox_DigitalTimeEx.cpp�� �����Ͻʽÿ�.
//

class CMFC6_DialogBox_DigitalTimeExApp : public CWinApp
{
public:
	CMFC6_DialogBox_DigitalTimeExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC6_DialogBox_DigitalTimeExApp theApp;