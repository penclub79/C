// LoginDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

// CLoginDlg ��ȭ �����Դϴ�.

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


// CLoginDlg �޽��� ó�����Դϴ�.


void CLoginDlg::OnClickedIdok()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_EditUserName.GetWindowTextW(m_strUsername);
	m_EditPassword.GetWindowTextW(m_strPassword);

	if (0 == m_strUsername.GetLength())
	{
		MessageBox(_T("�Է��� ID�� �����ϴ�."));
	}

	CDialogEx::OnOK();
}
