
// MFC11_GDIPlusEx.h : MFC11_GDIPlusEx ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC11_GDIPlusExApp:
// �� Ŭ������ ������ ���ؼ��� MFC11_GDIPlusEx.cpp�� �����Ͻʽÿ�.
//

class CMFC11_GDIPlusExApp : public CWinAppEx
{
public:
	CMFC11_GDIPlusExApp();


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

extern CMFC11_GDIPlusExApp theApp;
