
// MFC12_2_BitMapEx.h : MFC12_2_BitMapEx ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC12_2_BitMapExApp:
// �� Ŭ������ ������ ���ؼ��� MFC12_2_BitMapEx.cpp�� �����Ͻʽÿ�.
//

class CMFC12_2_BitMapExApp : public CWinAppEx
{
public:
	CMFC12_2_BitMapExApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC12_2_BitMapExApp theApp;
