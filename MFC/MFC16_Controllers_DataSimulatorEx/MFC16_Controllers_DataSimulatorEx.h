
// MFC16_Controllers_DataSimulatorEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC16_Controllers_DataSimulatorExApp:
// �� Ŭ������ ������ ���ؼ��� MFC16_Controllers_DataSimulatorEx.cpp�� �����Ͻʽÿ�.
//

class CMFC16_Controllers_DataSimulatorExApp : public CWinApp
{
public:
	CMFC16_Controllers_DataSimulatorExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC16_Controllers_DataSimulatorExApp theApp;