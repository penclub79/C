// DlgVerFileAdd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "UpdateManager.h"
#include "DlgVerFileAdd.h"
#include "FirmUpdate.h"
#include "afxdialogex.h"


// DlgVerFileAdd ��ȭ �����Դϴ�.

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


// DlgVerFileAdd �޽��� ó�����Դϴ�.


BOOL DlgVerFileAdd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_strFileName.Empty();

	// Init Model Type


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void DlgVerFileAdd::SetModelType(int _iModelType)
{
	m_iModelType = _iModelType;
}


void DlgVerFileAdd::SetModelName(TCHAR* _pszModelName)
{
	TCHAR* pszModelName = _pszModelName;
	CString strModelName = (LPCTSTR)pszModelName;
	//SetDlgItemText(IDC_EDIT_VERSION_READ, strModelName);
	m_pEdit = (CEdit*)GetDlgItem(IDC_EDIT_VERSION_READ);

	if (m_pEdit)
	{
		m_pEdit->SetWindowText(_T("sdg"));
	}

}