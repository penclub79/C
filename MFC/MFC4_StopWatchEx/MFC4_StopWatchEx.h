
// MFC4_StopWatchEx.h : MFC4_StopWatchEx ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC4_StopWatchExApp:
// �� Ŭ������ ������ ���ؼ��� MFC4_StopWatchEx.cpp�� �����Ͻʽÿ�.
//

class CMFC4_StopWatchExApp : public CWinApp
{
public:
	CMFC4_StopWatchExApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC4_StopWatchExApp theApp;
