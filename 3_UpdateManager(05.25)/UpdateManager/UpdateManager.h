
// UpdateManager.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

//#include <GrTypeBase.h>
#include "resource.h"		// �� ��ȣ�Դϴ�.
#include <GrTypeBase.h>

// CUpdateManagerApp:
// �� Ŭ������ ������ ���ؼ��� UpdateManager.cpp�� �����Ͻʽÿ�.
//

class CUpdateManagerApp : public CWinApp
{
public:
	CUpdateManagerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CUpdateManagerApp theApp;