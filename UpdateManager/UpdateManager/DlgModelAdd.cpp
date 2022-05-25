#include "stdafx.h"
#include "UpdateManager.h"
#include "DlgModelAdd.h"
#include "afxdialogex.h"
#include "FirmUpdate.h"

IMPLEMENT_DYNAMIC(DlgModelAdd, CDialogEx)

DlgModelAdd::DlgModelAdd(CWnd* pParent /*=NULL*/)
: CDialogEx(DlgModelAdd::IDD, pParent)
{
	m_bModalResult = FALSE;
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
	ON_BN_CLICKED(IDC_BUTTON_MODEL_SELECT, &DlgModelAdd::OnClickedButtonModelMake)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_CANCEL, &DlgModelAdd::OnClickedButtonModelCancel)
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
	//CDialogEx::OnOK();
	m_bModalResult = TRUE;
	CDialogEx::OnOK();
}

int	DlgModelAdd::GetModelType()
{
	return m_iModelType;
}

void DlgModelAdd::OnClickedButtonModelCancel()
{
	m_bModalResult = FALSE;
	CDialogEx::OnOK();
}


//
//LRESULT DlgModelAdd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//	case WM_CLOSE:
//		break;
//
//	default:
//		break;
//	}
//
//	return CDialogEx::WindowProc(message, wParam, lParam);
//}
