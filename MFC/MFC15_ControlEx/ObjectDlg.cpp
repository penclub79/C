// ObjectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC15_ControlEx.h"
#include "ObjectDlg.h"
#include "afxdialogex.h"


// CObjectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectDlg, CDialogEx)

CObjectDlg::CObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CObjectDlg::IDD, pParent)
	, m_nSelObject(0)
{

}

CObjectDlg::~CObjectDlg()
{
}

void CObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjectDlg, CDialogEx)
END_MESSAGE_MAP()


// CObjectDlg 메시지 처리기입니다.


BOOL CObjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_nSelObject = 1;
	((CButton*)GetDlgItem(IDC_RADIO_RECT))->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
