
// MFC12_4_BitMapEx.h : MFC12_4_BitMapEx ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC12_4_BitMapExApp:
// �� Ŭ������ ������ ���ؼ��� MFC12_4_BitMapEx.cpp�� �����Ͻʽÿ�.
//

class CMFC12_4_BitMapExApp : public CWinAppEx
{
public:
	CMFC12_4_BitMapExApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC12_4_BitMapExApp theApp;
