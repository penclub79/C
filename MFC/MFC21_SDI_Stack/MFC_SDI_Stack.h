
// MFC_SDI_Stack.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC_SDI_StackApp:
// �� Ŭ������ ������ ���ؼ��� MFC_SDI_Stack.cpp�� �����Ͻʽÿ�.
//

class CMFC_SDI_StackApp : public CWinApp
{
public:
	CMFC_SDI_StackApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC_SDI_StackApp theApp;