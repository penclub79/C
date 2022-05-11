// DlgAddEntity.cpp : implementation file
//

#include "stdafx.h"
#include "MkUpdtMng.h"
#include "DlgAddEntity.h"
#include "afxdialogex.h"
#include "FirmUpdt.h"

// DlgAddEntity dialog

IMPLEMENT_DYNAMIC(DlgAddEntity, CDialogEx)

DlgAddEntity::DlgAddEntity(CWnd* pParent /*=NULL*/, TCHAR* A_PtrPath)
	: CDialogEx(DlgAddEntity::IDD, pParent)
{
		m_ModelType		= E_FirmUpInfoTypeNone;
		m_PtrStrPath	= A_PtrPath;
}

DlgAddEntity::~DlgAddEntity()
{
}

void DlgAddEntity::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CbModelType);
	DDX_Control(pDX, IDC_COMBO2, m_CbEntityType);
	DDX_Control(pDX, IDC_EDTFILE, m_EdtFile);
}


BEGIN_MESSAGE_MAP(DlgAddEntity, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SELFILE, &DlgAddEntity::OnBnClickedBtnSelfile)
	ON_CBN_SELCHANGE(IDC_COMBO2, &DlgAddEntity::OnCbnSelchangeCbEntity)
	ON_CBN_SELCHANGE(IDC_COMBO1, &DlgAddEntity::OnCbnSelchangeCbModel)
END_MESSAGE_MAP()


// DlgAddEntity message handlers
BOOL DlgAddEntity::OnInitDialog()
{
		CDialogEx::OnInitDialog();

		m_StrFileName = "eeee";
		m_StrFileName.Empty();
		//init model type
		CString	 Tv_StrType;
		Tv_StrType		= "None";
		m_CbModelType.InsertString(E_FirmUpInfoTypeNone, Tv_StrType);
		Tv_StrType		= "Ja1704";
		m_CbModelType.InsertString(1, Tv_StrType);
		Tv_StrType		= "Ja1708";
		m_CbModelType.InsertString(2, Tv_StrType);
		Tv_StrType		= "Ja1716";
		m_CbModelType.InsertString(3, Tv_StrType);

		m_CbModelType.SendMessage(CB_SETCURSEL, m_ModelType, 0);

		//init entity type
		Tv_StrType		= "None";
		m_CbEntityType.InsertString(E_FirmUpEntityNone, Tv_StrType);
		Tv_StrType		= "Loader";
		m_CbEntityType.InsertString(E_FirmUpEntityLoader, Tv_StrType);
		Tv_StrType		= "Fdt";
		m_CbEntityType.InsertString(E_FirmUpEntityFdt, Tv_StrType);
		Tv_StrType		= "U-Boot";
		m_CbEntityType.InsertString(E_FirmUpEntityUboot, Tv_StrType);
		Tv_StrType		= "Kernel";
		m_CbEntityType.InsertString(E_FirmUpEntityKernel, Tv_StrType);
		Tv_StrType		= "Logo";
		m_CbEntityType.InsertString(E_FirmUpEntityLogo, Tv_StrType);
		Tv_StrType		= "RootFs";
		m_CbEntityType.InsertString(E_FirmUpEntityRootfs, Tv_StrType);

		m_CbEntityType.SendMessage(CB_SETCURSEL, 0, 0);
		m_EntityType	= E_FirmUpEntityNone;

		return TRUE;  // return TRUE unless you set the focus to a control
}

void DlgAddEntity::SetModelType(__u32 A_ModelType)
{
		m_ModelType = A_ModelType;
}

void DlgAddEntity::OnBnClickedBtnSelfile()
{
	// local -------------------
		CFileDialog*			Tv_FileDlg;
		CString					Tv_StrPath;
		CString					Tv_StrFile;
	// code --------------------
		Tv_FileDlg	= (CFileDialog*) new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("All Files(*.*)|*.*"), NULL);
		Tv_FileDlg->m_ofn.lpstrInitialDir = m_PtrStrPath;

		if(Tv_FileDlg->DoModal() == IDOK)
		{
			Tv_StrPath	= Tv_FileDlg->GetPathName();
			Tv_StrFile	= Tv_FileDlg->GetFileName();
			m_EdtFile.Clear();
			m_EdtFile.SetWindowTextW(Tv_StrPath);
			m_StrFileName		= Tv_StrPath;
		}

		delete Tv_FileDlg;
		Tv_FileDlg = NULL;
}

void DlgAddEntity::GetAddEntity(__u32* A_PtrModelType, __u32* A_PtrEntityType, PCHAR A_PtrStr, __s32* A_PtrStrLen)
{
	// local -------------------
		__s32		Tv_StrLen;
	// code --------------------
		*A_PtrModelType		= m_ModelType;
		*A_PtrEntityType	= m_EntityType;//m_CbEntityType.GetCurSel();
		Tv_StrLen			= m_StrFileName.GetLength();
		WideCharToMultiByte(CP_ACP, 0, m_StrFileName, -1, A_PtrStr, Tv_StrLen, NULL, NULL);
		*A_PtrStrLen		= Tv_StrLen;
}

void DlgAddEntity::OnCbnSelchangeCbEntity()
{
		m_EntityType = m_CbEntityType.GetCurSel();
}

void DlgAddEntity::OnCbnSelchangeCbModel()
{
		int Tv_SelIdx;
		Tv_SelIdx	= m_CbModelType.GetCurSel();
		m_ModelType	= E_FirmUpInfoTypeNone;

		switch(Tv_SelIdx)
		{
		case 1:
			m_ModelType	= E_FirmUpInfoTypeJa1704;
			break;
		case 2:
			m_ModelType	= E_FirmUpInfoTypeJa1708;
			break;
		case 3:
			m_ModelType	= E_FirmUpInfoTypeJa1716;
			break;
		}
}
