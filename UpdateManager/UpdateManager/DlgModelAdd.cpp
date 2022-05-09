#include "stdafx.h"
#include "UpdateManager.h"
#include "DlgModelAdd.h"
#include "afxdialogex.h"
#include "FirmUpdate.h"

IMPLEMENT_DYNAMIC(DlgModelAdd, CDialogEx)

DlgModelAdd::DlgModelAdd(CWnd* pParent /*=NULL*/)
: CDialogEx(DlgModelAdd::IDD, pParent)
{
}

DlgModelAdd::~DlgModelAdd()
{
	
}

void DlgModelAdd::DoDataExchange(CDataExchange* pDX)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MODELTYPE, m_CModelType);

}

BEGIN_MESSAGE_MAP(DlgModelAdd, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_MODELTYPE, &DlgModelAdd::OnSelchangeComboModeltype)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_MAKE, &DlgModelAdd::OnClickedButtonModelMake)
END_MESSAGE_MAP()

BOOL DlgModelAdd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//Local -------------------------------------
	CString strType;
	//-------------------------------------------

	strType = "None";
	m_CModelType.InsertString(E_FirmUpInfoTypeNone, strType);

	strType = "Ja1704";
	m_CModelType.InsertString(1, strType);

	strType = "Ja1708";
	m_CModelType.InsertString(2, strType);

	strType = "Ja1716";
	m_CModelType.InsertString(3, strType);

	m_CModelType.SendMessage(CB_SETCURSEL, E_FirmUpInfoTypeNone, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void DlgModelAdd::OnSelchangeComboModeltype()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	//Local -------------------------------------
	int iSelIdx = 0;
	//-------------------------------------------
	iSelIdx = m_CModelType.GetCurSel();
	m_iModelType = E_FirmUpInfoTypeNone;

	switch (iSelIdx)
	{
	case 1:
		m_iModelType = E_FirmUpInfoTypeJa1704;
		break;
	case 2:
		m_iModelType = E_FirmUpInfoTypeJa1708;
		break;
	case 3:
		m_iModelType = E_FirmUpInfoTypeJa1716;
		break;
	default:
		break;
	}
}


void DlgModelAdd::OnClickedButtonModelMake()
{
	//Local -------------------------------------
	CString strModelName;
	TCHAR* pszBuffer = NULL;
	//-------------------------------------------

	GetDlgItemText(IDC_EDIT_MODEL_NAME, strModelName);

	pszBuffer = new TCHAR[50];
	memset(pszBuffer, 0, sizeof(TCHAR) * 50);

	if (strModelName.IsEmpty())
	{
		AfxMessageBox(_T("�𵨸��� �Է��ؾ� �մϴ�."));
		return;
	}

	if (strModelName.GetLength() > sizeof(TCHAR) * 50)
	{
		AfxMessageBox(_T("50�� �̸��Դϴ�."));
		SetDlgItemText(IDC_EDIT_MODEL_NAME, _T(""));
		return;
	}

	if (NULL != strModelName.GetBuffer(0))
	{
		_tcscpy(&pszBuffer[0], strModelName.GetBuffer(0));
		SetModelName(&pszBuffer[0]);
	}

	if (NULL != pszBuffer)
	{
		delete[] pszBuffer;
		pszBuffer = NULL;
	}

	CDialogEx::OnOK();
}

void DlgModelAdd::SetModelName(TCHAR* _pszName)
{
	TCHAR* pszModelName = _pszName;
	m_strModelName = (LPCTSTR)pszModelName;
}

int	DlgModelAdd::GetModelType()
{
	return m_iModelType;
}

CString DlgModelAdd::GetModelName()
{
	return m_strModelName;
}