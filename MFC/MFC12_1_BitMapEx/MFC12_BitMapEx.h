
// MFC12_BitMapEx.h : MFC12_BitMapEx ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC12_BitMapExApp:
// �� Ŭ������ ������ ���ؼ��� MFC12_BitMapEx.cpp�� �����Ͻʽÿ�.
//

class CMFC12_BitMapExApp : public CWinAppEx
{
public:
	CMFC12_BitMapExApp();


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

extern CMFC12_BitMapExApp theApp;
