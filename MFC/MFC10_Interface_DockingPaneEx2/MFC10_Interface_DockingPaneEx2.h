
// MFC10_Interface_DockingPaneEx2.h : MFC10_Interface_DockingPaneEx2 ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC10_Interface_DockingPaneEx2App:
// �� Ŭ������ ������ ���ؼ��� MFC10_Interface_DockingPaneEx2.cpp�� �����Ͻʽÿ�.
//

class CMFC10_Interface_DockingPaneEx2App : public CWinAppEx
{
public:
	CMFC10_Interface_DockingPaneEx2App();


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

extern CMFC10_Interface_DockingPaneEx2App theApp;
