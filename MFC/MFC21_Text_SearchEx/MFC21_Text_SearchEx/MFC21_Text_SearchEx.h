
// MFC21_Text_SearchEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.



// CMFC21_Text_SearchExApp:
// �� Ŭ������ ������ ���ؼ��� MFC21_Text_SearchEx.cpp�� �����Ͻʽÿ�.
//

class CMFC21_Text_SearchExApp : public CWinApp
{
public:
	CMFC21_Text_SearchExApp();
	

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	int m_iCount;





	DECLARE_MESSAGE_MAP()
};

extern CMFC21_Text_SearchExApp theApp;