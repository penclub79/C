
// MFC12_3_BitMapEx.h : MFC12_3_BitMapEx ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC12_3_BitMapExApp:
// �� Ŭ������ ������ ���ؼ��� MFC12_3_BitMapEx.cpp�� �����Ͻʽÿ�.
//

class CMFC12_3_BitMapExApp : public CWinAppEx
{
public:
	CMFC12_3_BitMapExApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC12_3_BitMapExApp theApp;
