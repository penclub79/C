// DlgUpdtAdd.cpp : implementation file
//

#include "stdafx.h"
#include "MkUpdtMng.h"
#include "DlgUpdtAdd.h"
#include "afxdialogex.h"
#include "FirmUpdt.h"


// DlgUpdtAdd dialog

IMPLEMENT_DYNAMIC(DlgUpdtAdd, CDialogEx)

DlgUpdtAdd::DlgUpdtAdd(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgUpdtAdd::IDD, pParent)
{
		//m_CbType.InsertString(0, (LPCTSTR)"test1");
}

DlgUpdtAdd::~DlgUpdtAdd()
{
}

void DlgUpdtAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBTYPE, m_CbType);
}


BEGIN_MESSAGE_MAP(DlgUpdtAdd, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgUpdtAdd::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgUpdtAdd message handlers


BOOL DlgUpdtAdd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_AddType = E_FirmUpInfoTypeNone;

	CString	 Tv_StrModel;
	Tv_StrModel		= "None";
	m_CbType.InsertString(E_FirmUpInfoTypeNone, Tv_StrModel);
	Tv_StrModel		= "Ja1704";
	m_CbType.InsertString(1, Tv_StrModel);
	Tv_StrModel		= "Ja1708";
	m_CbType.InsertString(2, Tv_StrModel);
	Tv_StrModel		= "Ja1716";
	m_CbType.InsertString(3, Tv_StrModel);

	m_CbType.SendMessage(CB_SETCURSEL, E_FirmUpInfoTypeNone, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DlgUpdtAdd::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	int		Tv_SelType;
	Tv_SelType		= m_CbType.GetCurSel();

	m_AddType	= E_FirmUpInfoTypeNone;
	switch(Tv_SelType)
	{
	case 1:
		m_AddType		= E_FirmUpInfoTypeJa1704;
		break;
	case 2:
		m_AddType		= E_FirmUpInfoTypeJa1708;
		break;
	case 3:
		m_AddType		= E_FirmUpInfoTypeJa1716;
		break;
	}
	

	CDialogEx::OnOK();
}

__u32 DlgUpdtAdd::GetAddType()
{
		return m_AddType;
}
