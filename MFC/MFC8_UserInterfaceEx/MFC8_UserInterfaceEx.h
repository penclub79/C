
// MFC8_UserInterfaceEx.h : MFC8_UserInterfaceEx ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC8_UserInterfaceExApp:
// �� Ŭ������ ������ ���ؼ��� MFC8_UserInterfaceEx.cpp�� �����Ͻʽÿ�.
//

class CMFC8_UserInterfaceExApp : public CWinAppEx
{
public:
	CMFC8_UserInterfaceExApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC8_UserInterfaceExApp theApp;
