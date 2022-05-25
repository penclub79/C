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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void DlgModelAdd::OnSelchangeComboModeltype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
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
