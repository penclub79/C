
// MFC13_ListControlEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC13_ListControlExApp:
// �� Ŭ������ ������ ���ؼ��� MFC13_ListControlEx.cpp�� �����Ͻʽÿ�.
//

class CMFC13_ListControlExApp : public CWinApp
{
public:
	CMFC13_ListControlExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC13_ListControlExApp theApp;