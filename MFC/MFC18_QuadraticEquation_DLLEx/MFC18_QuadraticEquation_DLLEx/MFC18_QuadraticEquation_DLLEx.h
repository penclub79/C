
// MFC18_QuadraticEquation_DLLEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC18_QuadraticEquation_DLLExApp:
// �� Ŭ������ ������ ���ؼ��� MFC18_QuadraticEquation_DLLEx.cpp�� �����Ͻʽÿ�.
//

class CMFC18_QuadraticEquation_DLLExApp : public CWinApp
{
public:
	CMFC18_QuadraticEquation_DLLExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC18_QuadraticEquation_DLLExApp theApp;