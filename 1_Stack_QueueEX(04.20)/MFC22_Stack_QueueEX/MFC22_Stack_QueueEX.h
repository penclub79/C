
// MFC22_Stack_QueueEX.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC22_Stack_QueueEXApp:
// �� Ŭ������ ������ ���ؼ��� MFC22_Stack_QueueEX.cpp�� �����Ͻʽÿ�.
//

class CMFC22_Stack_QueueEXApp : public CWinApp
{
public:
	CMFC22_Stack_QueueEXApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC22_Stack_QueueEXApp theApp;