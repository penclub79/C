
// UniScan.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CUniScanApp:
// �� Ŭ������ ������ ���ؼ��� UniScan.cpp�� �����Ͻʽÿ�.
//

class CUniScanApp : public CWinApp
{
public:
	CUniScanApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CUniScanApp theApp;