
// MFC17_Calendar_DLLEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC17_Calendar_DLLExApp:
// �� Ŭ������ ������ ���ؼ��� MFC17_Calendar_DLLEx.cpp�� �����Ͻʽÿ�.
//

class CMFC17_Calendar_DLLExApp : public CWinApp
{
public:
	CMFC17_Calendar_DLLExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC17_Calendar_DLLExApp theApp;