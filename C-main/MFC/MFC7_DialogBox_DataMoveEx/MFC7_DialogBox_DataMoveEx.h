
// MFC7_DialogBox_DataMoveEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC7_DialogBox_DataMoveExApp:
// �� Ŭ������ ������ ���ؼ��� MFC7_DialogBox_DataMoveEx.cpp�� �����Ͻʽÿ�.
//

class CMFC7_DialogBox_DataMoveExApp : public CWinApp
{
public:
	CMFC7_DialogBox_DataMoveExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC7_DialogBox_DataMoveExApp theApp;