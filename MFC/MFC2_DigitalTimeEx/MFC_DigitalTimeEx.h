
// MFC_DigitalTimeEx.h : MFC_DigitalTimeEx ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC_DigitalTimeExApp:
// �� Ŭ������ ������ ���ؼ��� MFC_DigitalTimeEx.cpp�� �����Ͻʽÿ�.
//

class CMFC_DigitalTimeExApp : public CWinAppEx
{
public:
	CMFC_DigitalTimeExApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC_DigitalTimeExApp theApp;
