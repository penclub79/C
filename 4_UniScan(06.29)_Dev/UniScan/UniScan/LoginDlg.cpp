// LoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

// CLoginDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_EditUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_EditPassword);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_IDOK, &CLoginDlg::OnClickedIdok)
END_MESSAGE_MAP()


// CLoginDlg 메시지 처리기입니다.


void CLoginDlg::OnClickedIdok()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_EditUserName.GetWindowTextW(m_strUsername);
	m_EditPassword.GetWindowTextW(m_strPassword);

	if (0 == m_strUsername.GetLength())
	{
		MessageBox(_T("입력한 ID가 없습니다."));
	}

	CDialogEx::OnOK();
}
