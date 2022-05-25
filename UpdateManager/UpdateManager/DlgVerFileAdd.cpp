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
	m_bModalResult	= FALSE;
	m_iVerFileType	= 0;

}

DlgVerFileAdd::~DlgVerFileAdd()
{
}

void DlgVerFileAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VERSION_READ, m_CEdit);
	DDX_Control(pDX, IDC_COMBO_CHOISE_FILE, m_CVerFileType);
	DDX_Control(pDX, IDC_EDIT_FILEPATH2, m_CEditFile);
}

BEGIN_MESSAGE_MAP(DlgVerFileAdd, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_VERSION_OK, &DlgVerFileAdd::OnClickedButtonVersionOk)
	ON_BN_CLICKED(IDC_BUTTON_VERSION_CANCEL, &DlgVerFileAdd::OnClickedButtonVersionCancel)
	ON_BN_CLICKED(IDC_BUTTON_FILE_OPEN, &DlgVerFileAdd::OnClickedButtonFileOpen)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOISE_FILE, &DlgVerFileAdd::OnSelchangeComboChoiseFile)
END_MESSAGE_MAP()


// DlgVerFileAdd �޽��� ó�����Դϴ�.


BOOL DlgVerFileAdd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString strFileType;

	m_strFileName.Empty();
	
	// -----------------------------------Init Model Type
	if (m_strModelName.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT_VERSION_READ, _T("None"));
	}
	else
	{
		SetDlgItemText(IDC_EDIT_VERSION_READ, m_strModelName);
	}
	// --------------------------------------------------


	// -----------------------------------Init Version File Type
	strFileType = "None";
	m_CVerFileType.InsertString(E_FirmUpEntityNone, strFileType);

	strFileType = "Loader";
	m_CVerFileType.InsertString(E_FirmUpEntityLoader, strFileType);

	strFileType = "Fdt";
	m_CVerFileType.InsertString(E_FirmUpEntityFdt, strFileType);

	strFileType = "U-Boot";
	m_CVerFileType.InsertString(E_FirmUpEntityUboot, strFileType);

	strFileType = "Kernel";
	m_CVerFileType.InsertString(E_FirmUpEntityKernel, strFileType);

	strFileType = "Logo";
	m_CVerFileType.InsertString(E_FirmUpEntityLogo, strFileType);

	strFileType = "RootFs";
	m_CVerFileType.InsertString(E_FirmUpEntityRootfs, strFileType);

	m_CVerFileType.SendMessage(CB_SETCURSEL, 0, 0);
	m_iVerFileType = E_FirmUpEntityNone;
	// --------------------------------------------------
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void DlgVerFileAdd::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CDialogEx::OnOK();
}

void DlgVerFileAdd::SetModelType(int _iModelType)
{
	m_iModelType = _iModelType;
}

void DlgVerFileAdd::SetModelName(CString strModelName)
{	
	m_strModelName = strModelName;
}


// Open ��ư
void DlgVerFileAdd::OnClickedButtonFileOpen()
{
	// Local ---------------------------------------
	CFileDialog*	pFileDlg = NULL;
	CString			strPath;
	CString			strFileName;
	// ---------------------------------------------

	pFileDlg = (CFileDialog*)new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("All Files(*.*)|*.*"), NULL);

	pFileDlg->m_ofn.lpstrInitialDir = m_pszNowPath;

	if (IDOK == pFileDlg->DoModal())
	{
		strPath = pFileDlg->GetPathName();
		strFileName = pFileDlg->GetFileName();
		m_CEditFile.Clear();
		m_CEditFile.SetWindowTextW(strPath);
		m_strFileName = strPath;
	}

	if (NULL != pFileDlg)
	{
		delete pFileDlg;
		pFileDlg = NULL;
	}
}

// ���� ����Ÿ�� ���� GET
void DlgVerFileAdd::GetVerFileType(int* _piModelType, int* _piVerFileType, CHAR* _pszFileName, int* _piStrLen)
{
	*_piModelType = m_iModelType; // ModelType�� �����Ͱ� �ȵ�������
	*_piVerFileType = m_iVerFileType;
	*_piStrLen = m_strFileName.GetLength();
	strcpy(_pszFileName, CT2A(m_strFileName)); // CT2A�� ���� ���ƾ� �Ѵ�.
}

// OK ��ư
void DlgVerFileAdd::OnClickedButtonVersionOk()
{
	m_bModalResult = TRUE;
	CDialogEx::OnOK();
}

// Cancel ��ư
void DlgVerFileAdd::OnClickedButtonVersionCancel()
{
	m_bModalResult = FALSE;
	CDialogEx::OnOK();
}

// ComboBox �������� 
void DlgVerFileAdd::OnSelchangeComboChoiseFile()
{
	int iSelIdx = 0;

	iSelIdx = m_CVerFileType.GetCurSel();
	m_iVerFileType = E_FirmUpInfoTypeNone;

	switch (iSelIdx)
	{
	case 1:
		m_iVerFileType = E_FirmUpEntityLoader;
		break;
	case 2:
		m_iVerFileType = E_FirmUpEntityFdt;
		break;
	case 3:
		m_iVerFileType = E_FirmUpEntityUboot;
		break;
	case 4:
		m_iVerFileType = E_FirmUpEntityKernel;
		break;
	case 5:
		m_iVerFileType = E_FirmUpEntityLogo;
		break;
	case 6:
		m_iVerFileType = E_FirmUpEntityRootfs;
		break;
	}
}
