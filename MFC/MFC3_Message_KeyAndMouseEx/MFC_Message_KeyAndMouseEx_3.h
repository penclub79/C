
// MFC_Message_KeyAndMouseEx_3.h : MFC_Message_KeyAndMouseEx_3 ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC_Message_KeyAndMouseEx_3App:
// �� Ŭ������ ������ ���ؼ��� MFC_Message_KeyAndMouseEx_3.cpp�� �����Ͻʽÿ�.
//

class CMFC_Message_KeyAndMouseEx_3App : public CWinApp
{
public:
	CMFC_Message_KeyAndMouseEx_3App();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC_Message_KeyAndMouseEx_3App theApp;
