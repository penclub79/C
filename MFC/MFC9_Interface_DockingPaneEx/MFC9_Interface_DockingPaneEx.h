
// MFC9_Interface_DockingPaneEx.h : MFC9_Interface_DockingPaneEx ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC9_Interface_DockingPaneExApp:
// �� Ŭ������ ������ ���ؼ��� MFC9_Interface_DockingPaneEx.cpp�� �����Ͻʽÿ�.
//

class CMFC9_Interface_DockingPaneExApp : public CWinAppEx
{
public:
	CMFC9_Interface_DockingPaneExApp();


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

extern CMFC9_Interface_DockingPaneExApp theApp;
