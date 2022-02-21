// ClockHelpDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC6_DialogBox_DigitalTimeEx.h"
#include "ClockHelpDlg.h"
#include "afxdialogex.h"


// CClockHelpDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CClockHelpDlg, CDialogEx)

CClockHelpDlg::CClockHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClockHelpDlg::IDD, pParent)
{

}

CClockHelpDlg::~CClockHelpDlg()
{
}

void CClockHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClockHelpDlg, CDialogEx)
END_MESSAGE_MAP()


// CClockHelpDlg 메시지 처리기입니다.
