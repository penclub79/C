
// MFC14_TreeControlEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC14_TreeControlExApp:
// �� Ŭ������ ������ ���ؼ��� MFC14_TreeControlEx.cpp�� �����Ͻʽÿ�.
//

class CMFC14_TreeControlExApp : public CWinApp
{
public:
	CMFC14_TreeControlExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC14_TreeControlExApp theApp;