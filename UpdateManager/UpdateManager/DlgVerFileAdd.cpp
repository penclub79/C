// DlgVerFileAdd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UpdateManager.h"
#include "DlgVerFileAdd.h"
#include "FirmUpdate.h"
#include "afxdialogex.h"


// DlgVerFileAdd 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgVerFileAdd, CDialogEx)

DlgVerFileAdd::DlgVerFileAdd(CWnd* pParent /*=NULL*/, TCHAR* _pszNowPath)
	: CDialogEx(DlgVerFileAdd::IDD, pParent)
{
	m_iModelType	= E_FirmUpInfoTypeNone;
	m_pszNowPath	= _pszNowPath;
}

DlgVerFileAdd::~DlgVerFileAdd()
{
}

void DlgVerFileAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgVerFileAdd, CDialogEx)
END_MESSAGE_MAP()


// DlgVerFileAdd 메시지 처리기입니다.


BOOL DlgVerFileAdd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_strFileName.Empty();

	// Init Model Type


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void SetModelType(int _iModelType)
{
	
}
