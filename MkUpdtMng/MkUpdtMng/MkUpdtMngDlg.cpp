
// MkUpdtMngDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "MkUpdtMng.h"
#include "MkUpdtMngDlg.h"
#include "FirmUpdt.h"
#include "DlgUpdtAdd.h"
#include <GrDumyTool.h>
#include "DlgAddEntity.h"
#include <GrStrTool.h>
#include <GrFileCtrl.h>
#include <GrFileTool.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMkUpdtMngDlg dialog



CMkUpdtMngDlg::CMkUpdtMngDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMkUpdtMngDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMkUpdtMngDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_TREE_MODEL, m_TreeUpdt);
	DDX_Control(pDX, IDC_TREE_MODEL, m_TreeCtrl);
	//  DDX_Control(pDX, IDC_EDIT1, m_EdtVersion);
	DDX_Control(pDX, IDC_EDT_FILEPATH, m_EdtPath);
	DDX_Control(pDX, IDC_BTN_PATH, m_BtnPath);
	DDX_Control(pDX, IDC_BTN_MAKE, m_BtnMake);
	//  DDX_Control(pDX, IDC_EDIT2, m_Ver2);
	DDX_Control(pDX, IDC_EDIT1, m_EdtVer1);
	DDX_Control(pDX, IDC_EDIT2, m_EdtVer2);
	DDX_Control(pDX, IDC_EDIT3, m_EdtVer3);
	DDX_Control(pDX, IDC_EDIT4, m_EdtVer4);
}

BEGIN_MESSAGE_MAP(CMkUpdtMngDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ADDMODEL, &CMkUpdtMngDlg::OnBnClickedBtnAddmodel)
	ON_BN_CLICKED(IDC_BTN_ADDENTITY, &CMkUpdtMngDlg::OnBnClickedBtnAddentity)
	ON_BN_CLICKED(IDC_BTN_PATH, &CMkUpdtMngDlg::OnBnClickedBtnPath)
	ON_BN_CLICKED(IDC_BTN_MAKE, &CMkUpdtMngDlg::OnBnClickedBtnMake)
END_MESSAGE_MAP()


// CMkUpdtMngDlg message handlers

BOOL CMkUpdtMngDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	LcInit();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMkUpdtMngDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMkUpdtMngDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMkUpdtMngDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMkUpdtMngDlg::OnBnClickedBtnAddmodel()
{
	// local -------------------
		DlgUpdtAdd*		Tv_DlgAdd;
		__u32			Tv_ModelType;
		__s32			Tv_Result;
	// code --------------------
		//MessageBox(_T("test1"), _T("test2"), MB_OK | MB_ICONWARNING);
		Tv_DlgAdd = (DlgUpdtAdd*)new DlgUpdtAdd();
		Tv_DlgAdd->DoModal();

		Tv_ModelType = Tv_DlgAdd->GetAddType();
		if(E_FirmUpInfoTypeNone != Tv_ModelType)
		{
			LcAddModel(Tv_ModelType);
		}

		delete Tv_DlgAdd;
		Tv_DlgAdd = NULL;
}

void CMkUpdtMngDlg::LcProcErrCode(__s32 A_ErrCode)
{
	// local -------------------
		CString		Tv_StrContent;
	// code --------------------
		switch(A_ErrCode)
		{
		case E_FirmUpErrCode:
			Tv_StrContent		= "Error";
			break;
		case E_FirmUpErrExistType:
			Tv_StrContent		= "Exist Same Type";
			break;
		case E_FirmUpErrExistEntity:
			Tv_StrContent		= "Exist Same Entity Type";
			break;
		case E_FirmUpErrExistData:
			Tv_StrContent		= "Exist Same Entity Data Type";
			break;
		default:
			Tv_StrContent		= "Error";
			break;
		}

		MessageBox(_T("Error"), Tv_StrContent, MB_OK | MB_ICONWARNING);
}


void CMkUpdtMngDlg::OnBnClickedBtnAddentity()
{
	// local -------------------
		DlgAddEntity*	Tv_DlgEntity;
		CString			Tv_StrItm;
		__u8			Tv_WkCnt;
		__s32			Tv_StrLen;
		CHAR			Tv_StrModel[16];
		__u32			Tv_ModelType;
		__u32			Tv_EntityType;
		__s32			Tv_ModelIdx;
		CHAR			Tv_StrFile[128];
	// code --------------------
		Tv_DlgEntity			= new DlgAddEntity(NULL, m_NowPath);
		HTREEITEM Tv_TreeItm	= m_TreeCtrl.GetSelectedItem();
		Tv_StrItm				= m_TreeCtrl.GetItemText(Tv_TreeItm);

		for(Tv_WkCnt = 0; Tv_WkCnt < E_FirmUpInfoTypeMaxIdx; Tv_WkCnt++)
		{
			Tv_StrLen = Tv_StrItm.GetLength();
			
			WideCharToMultiByte(CP_ACP, 0, Tv_StrItm, -1, Tv_StrModel, Tv_StrLen, NULL, NULL);

			if(0 == GrStrCmp(Tv_StrModel, m_TreeNode[Tv_WkCnt].StrNode, FALSE, Tv_StrLen))
			{
				Tv_ModelIdx	= 0;
				switch(m_TreeNode[Tv_WkCnt].Type)
				{
				case E_FirmUpInfoTypeJa1704:
					Tv_ModelIdx = E_FirmUpInfoTypeJa1704;
					break;
				case E_FirmUpInfoTypeJa1708:
					Tv_ModelIdx = E_FirmUpInfoTypeJa1708;
					break;
				case E_FirmUpInfoTypeJa1716:
					Tv_ModelIdx = E_FirmUpInfoTypeJa1716;
					break;
				}
				Tv_DlgEntity->SetModelType(Tv_ModelIdx);
				break;
			}
		}

		if(IDOK == Tv_DlgEntity->DoModal())
		{
			GrDumyZeroMem(Tv_StrFile, 128);
			
			// 모델 타입, 엔티티 타입, 파일명, 파일명 길이 얻기
			Tv_DlgEntity->GetAddEntity(&Tv_ModelType, &Tv_EntityType, Tv_StrFile, &Tv_StrLen);

			LcAddEntity(Tv_ModelType, Tv_EntityType, Tv_StrFile, Tv_StrLen);
		}

		delete Tv_DlgEntity;
		Tv_DlgEntity = NULL;
}

// TreeAddVerFile
void CMkUpdtMngDlg::LcAddEntity(__u32 A_ModelIdx, __u32 A_EntityType, PCHAR A_PtrStrFile, __u32 A_StrLen)
{
	// local --------------------------------
		FILE*			Tv_PtrFile;
		PCHAR			Tv_PtrFileBuf;
		Cls_GrFileCtrl* Tv_ObjFileCtrl;
		__u32			Tv_FileSize;
		__s32			Tv_Result;
		__u32			Tv_ModelIdx;
		char			Tv_StrChar[64];
		__u32			Tv_ModelType;

	// code ---------------------------------
		if(m_ObjFwUp != NULL)
		{
			//Tv_ModelType	= LcModelTypeGet(A_ModelIdx);
			Tv_ModelType	= A_ModelIdx;
			Tv_ObjFileCtrl	= new Cls_GrFileCtrl(A_PtrStrFile, FALSE, FALSE);
			if(Tv_ObjFileCtrl->IsOpened())
			{
				Tv_FileSize = (__u32)Tv_ObjFileCtrl->GetSize();

				Tv_PtrFileBuf = (PCHAR)malloc(Tv_FileSize);
				Tv_ObjFileCtrl->Seek(0);
				Tv_ObjFileCtrl->Read(Tv_PtrFileBuf, Tv_FileSize);

				Tv_Result = m_ObjFwUp->AddEntity(Tv_ModelType, A_EntityType, Tv_PtrFileBuf, Tv_FileSize);
				if(0 <= Tv_Result)
				{
					//find model
					Tv_ModelIdx = LcFindModelIdx(Tv_ModelType);
					if(0 <= Tv_ModelIdx)
					{
						m_UpdtInfo.ModelInfo[Tv_ModelIdx].Entity[Tv_Result].Type = A_EntityType;
						GrStrCopy(m_UpdtInfo.ModelInfo[Tv_ModelIdx].Entity[Tv_Result].StrFile, A_PtrStrFile);

						GrStrFnGetFileName(m_UpdtInfo.ModelInfo[Tv_ModelIdx].Entity[Tv_Result].StrFile, Tv_StrChar);

						LcAddEntityNode(Tv_ModelType, A_EntityType, Tv_StrChar);
					}
				}
				else
				{
					LcProcErrCode(Tv_Result);
				}
			}
			delete Tv_ObjFileCtrl;
			Tv_ObjFileCtrl = NULL;
		}
}

__s32 CMkUpdtMngDlg::LcFindTreeNode(__u32 A_ModelType)
{
	// local -------------------
		__s32			Tv_Result;
		__s32			Tv_WkCnt;
	// code --------------------
		Tv_Result = -1;

		for(Tv_WkCnt = 0;Tv_WkCnt < E_FirmUpInfoTypeMaxIdx;Tv_WkCnt++)
		{
			if(A_ModelType == m_TreeNode[Tv_WkCnt].Type)
			{
				Tv_Result = Tv_WkCnt;
				break;
			}
		}

		return Tv_Result;
}

void CMkUpdtMngDlg::LcAddEntityNode(__u32 A_ModelType, __u32 A_EntityType, char* A_FileName)
{
	// local -------------------
		__s32			Tv_Result;
		__s32			Tv_NodeIdx;
		char			Tv_StrChar[64];
		WCHAR			Tv_StrNode[64];
		char*			Tv_PtrChar;
	// code --------------------
		Tv_NodeIdx = LcFindTreeNode(A_ModelType);

		switch(A_EntityType)
		{
		case E_FirmUpEntityLoader:
			GrStrCopy(Tv_StrChar, "loader(");
			break;
		case E_FirmUpEntityFdt:
			GrStrCopy(Tv_StrChar, "fdt(");
			break;
		case E_FirmUpEntityUboot:
			GrStrCopy(Tv_StrChar, "u-boot(");
			break;
		case E_FirmUpEntityKernel:
			GrStrCopy(Tv_StrChar, "kernel(");
			break;
		case E_FirmUpEntityLogo:
			GrStrCopy(Tv_StrChar, "Logo(");
			break;
		case E_FirmUpEntityRootfs:
			GrStrCopy(Tv_StrChar, "RootFs(");
			break;
		}
		GrStrCat(Tv_StrChar, A_FileName);
		GrStrCat(Tv_StrChar, ")");
		GrStrStrToWstr(Tv_StrNode, Tv_StrChar);

		m_TreeCtrl.InsertItem(Tv_StrNode, m_TreeNode[Tv_NodeIdx].Node, NULL);
		//m_TreeCtrl.RedrawWindow();
		m_TreeCtrl.Invalidate(TRUE);

		m_UpdtInfo.ModelInfo[Tv_NodeIdx].Type = A_ModelType;
}


void CMkUpdtMngDlg::OnBnClickedBtnPath()
{
	// local -------------------
		CFileDialog*		Tv_FileDlg;
		CString					Tv_StrPath;
		WCHAR						Tv_StrNowPath[2048];
	// code --------------------
		Tv_FileDlg	= (CFileDialog*) new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("All Files(*.*)|*.*"), NULL);

		Tv_FileDlg->m_ofn.lpstrInitialDir = m_NowPath;
		if(Tv_FileDlg->DoModal() == IDOK)
		{
			Tv_StrPath	= Tv_FileDlg->GetPathName();

			GrDumyZeroMem(m_MkFileName, sizeof(WCHAR) * 1024);
			//WideCharToMultiByte(CP_ACP, 0, Tv_StrPath, -1, m_MkFileName, Tv_StrPath.GetLength(), NULL, NULL);
			GrStrWcopy(m_MkFileName, (LPWSTR)(LPCTSTR)Tv_StrPath);
			m_EdtPath.SetWindowTextW(Tv_StrPath);

			GrDumyZeroMem(m_UpdtInfo.StrUpdtFileName, 1024);
			GrStrWcopy(m_UpdtInfo.StrUpdtFileName, (LPWSTR)(LPCTSTR)Tv_StrPath);
		}
}

void CMkUpdtMngDlg::OnBnClickedBtnMake()
{
	// local -------------------
		__u32			Tv_FileLen;
		void*			Tv_PtrFile;
		Cls_GrFileCtrl* Tv_ObjFile;
		__u32			Tv_WroteSize;
		__u32			Tv_WriteSize;
		__u32			Tv_Result;
		CString			Tv_StrVer1;
		CString			Tv_StrVer2;
		CString			Tv_StrVer3;
		CString			Tv_StrVer4;
		BOOL8			Tv_IsSuccess;
		__u8			Tv_Ver[4];
		__u32			Tv_Version;
	// code --------------------
		Tv_IsSuccess	= FALSE;

		if(m_ObjFwUp != NULL)
		{
			m_EdtVer1.GetWindowTextW(Tv_StrVer1);
			m_EdtVer2.GetWindowTextW(Tv_StrVer2);
			m_EdtVer3.GetWindowTextW(Tv_StrVer3);
			m_EdtVer4.GetWindowTextW(Tv_StrVer4);

			Tv_Ver[0] = (__u8)(_ttoi(Tv_StrVer1));
			Tv_Ver[1] = (__u8)(_ttoi(Tv_StrVer2));
			Tv_Ver[2] = (__u8)(_ttoi(Tv_StrVer3));
			Tv_Ver[3] = (__u8)(_ttoi(Tv_StrVer4));

			Tv_Version = (Tv_Ver[0] << 24) | (Tv_Ver[1] << 16) | (Tv_Ver[2] << 8) | Tv_Ver[3];
			m_ObjFwUp->SetUpdtVer(Tv_Version);

			Tv_PtrFile = m_ObjFwUp->GetMkUpdt(&Tv_FileLen);

			Tv_ObjFile = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(m_MkFileName, TRUE, TRUE);
			if(TRUE == Tv_ObjFile->IsOpened())
			{
				Tv_WroteSize		= 0;
				Tv_WriteSize		= Tv_FileLen;
				while(1)
				{
					Tv_Result		= Tv_ObjFile->Write(Tv_PtrFile, Tv_WriteSize);
					Tv_WroteSize	= Tv_WroteSize + Tv_Result;
					Tv_WriteSize	= Tv_WriteSize - Tv_Result;

					if(Tv_WroteSize == Tv_FileLen)
					{
						Tv_IsSuccess	= TRUE;

						LcMkiniFile();
						break;
					}
				}
			}
			else
			{
				MessageBox(_T("File Open Error"), _T("Error"), MB_OK | MB_ICONWARNING);
			}

			if(Tv_ObjFile != NULL)
			{
				delete Tv_ObjFile;
				Tv_ObjFile	= NULL;
			}
		}

		if(Tv_IsSuccess)
		{
			MessageBox(_T("Make file Success"), NULL, MB_OK | MB_ICONWARNING);
		}
}

void CMkUpdtMngDlg::LcMkiniFile()
{
	// local -------------------
	CString			Tv_StrVer;
	WCHAR			Tv_StrIniFile[2048];
	Cls_GrFileCtrl*	Tv_ObjFile;
	CString			Tv_StrVer1;
	CString			Tv_StrVer2;
	CString			Tv_StrVer3;
	CString			Tv_StrVer4;
	__u8			Tv_Ver[4];
	__u32			Tv_Version;
	// code --------------------

	m_UpdtInfo.Fcc = E_MkUpdt_IniFcc;

	m_EdtVer1.GetWindowTextW(Tv_StrVer1);
	m_EdtVer2.GetWindowTextW(Tv_StrVer2);
	m_EdtVer3.GetWindowTextW(Tv_StrVer3);
	m_EdtVer4.GetWindowTextW(Tv_StrVer4);

	Tv_Ver[0] = (__u8)(_ttoi(Tv_StrVer1));
	Tv_Ver[1] = (__u8)(_ttoi(Tv_StrVer2));
	Tv_Ver[2] = (__u8)(_ttoi(Tv_StrVer3));
	Tv_Ver[3] = (__u8)(_ttoi(Tv_StrVer4));

	Tv_Version = (Tv_Ver[0] << 24) | (Tv_Ver[1] << 16) | (Tv_Ver[2] << 8) | Tv_Ver[3];
	m_UpdtInfo.UpgdVer = Tv_Version;

	GrStrWcopy(Tv_StrIniFile, m_NowPath);
	GrStrWcat(Tv_StrIniFile, _T("\\MkUpdt.init"));
	Tv_ObjFile = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(Tv_StrIniFile, TRUE, TRUE);

	if (Tv_ObjFile->IsOpened())
	{
		Tv_ObjFile->Write(&m_UpdtInfo, sizeof(St_UpdtInfo));
	}

	delete Tv_ObjFile;
	Tv_ObjFile = NULL;
}

void CMkUpdtMngDlg::LcInit()
{
	// local -------------------
		WCHAR				Tv_StrIniFile[2048];
		Cls_GrFileCtrl*		Tv_ObjFile;
		__u32				Tv_FileSize;
	// code --------------------
		::GetModuleFileName(NULL, m_NowPath, 2048);
		PathRemoveFileSpec(m_NowPath);

		m_ObjFwUp	= (Cls_FirmUpdt*)new Cls_FirmUpdt();
		m_ObjFwUp->Init();
		GrDumyZeroMem(m_TreeNode, sizeof(St_UpdtTreeNode) * 3);
		GrDumyZeroMem(&m_UpdtInfo, sizeof(St_UpdtInfo));

		GrStrWcopy(Tv_StrIniFile, m_NowPath);
		GrStrWcat(Tv_StrIniFile, _T("\\MkUpdt.init")); // init파일 경로

		if(GrFileIsExist(Tv_StrIniFile)) // init 파일 존재하는지 체크
		{
			//exist
			Tv_ObjFile = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(Tv_StrIniFile, FALSE, FALSE);

			if(Tv_ObjFile->IsOpened())
			{
				Tv_FileSize = (__u32)Tv_ObjFile->GetSize(); // init파일 사이즈 가져오기
				int test = sizeof(St_UpdtInfo);
				if(Tv_FileSize == sizeof(St_UpdtInfo))
				{
					Tv_ObjFile->Read(&m_UpdtInfo, Tv_FileSize);
				}
			}

			//check update info
			if(LcCheckIni(&m_UpdtInfo))
			{
				//init control
				LcInitCtrl(&m_UpdtInfo);
			}

			delete Tv_ObjFile;
			Tv_ObjFile	= NULL;
		}
}

BOOL8 CMkUpdtMngDlg::LcCheckIni(Ptr_UpdtInfo A_PtrInfo)
{
	// local -------------------
		BOOL8			Tv_Result;
	// code --------------------
		Tv_Result		= FALSE;

		if(E_MkUpdt_IniFcc == A_PtrInfo->Fcc)
		{
			Tv_Result		= TRUE;
		}
		return Tv_Result;
}
	
void CMkUpdtMngDlg::LcAddModel(__u32 A_ModelType)
{
	// local -------------------
		__s32			Tv_Result;
	// code --------------------
		if(NULL != m_ObjFwUp)
		{
			Tv_Result = m_ObjFwUp->AddType(A_ModelType);
			if(0 <= Tv_Result)
			{
				m_TreeNode[Tv_Result].Type	= A_ModelType;
				switch(A_ModelType)
				{
				case E_FirmUpInfoTypeJa1704:
					GrStrCopy(m_TreeNode[Tv_Result].StrNode, "Ja1704");
					m_TreeNode[Tv_Result].Node	= m_TreeCtrl.InsertItem(_T("Ja1704"), NULL, NULL);
					break;
				case E_FirmUpInfoTypeJa1708:
					GrStrCopy(m_TreeNode[Tv_Result].StrNode, "Ja1708");
					m_TreeNode[Tv_Result].Node	= m_TreeCtrl.InsertItem(_T("Ja1708"), NULL, NULL);
					break;
				case E_FirmUpInfoTypeJa1716:
					GrStrCopy(m_TreeNode[Tv_Result].StrNode, "Ja1716");
					m_TreeNode[Tv_Result].Node	= m_TreeCtrl.InsertItem(_T("Ja1716"), NULL, NULL);
					break;
				}

				m_UpdtInfo.ModelInfo[Tv_Result].Type = A_ModelType;
			}
			else
			{
				LcProcErrCode(Tv_Result);
			}
		}
}

void CMkUpdtMngDlg::LcInitCtrl(Ptr_UpdtInfo A_PtrInfo)
{
	// local -------------------
		__s32				Tv_Result;
		__s32				Tv_MdCnt;//model count
		__s32				Tv_EntCnt;//entity count
		__s32				Tv_StrLen;
		CHAR*				Tv_PtrStr;
		Ptr_UpdtInfoModel	Tv_PtrInfoMd;
		WCHAR				Tv_StrVer[32];
		__u32				Tv_Ver[4];
		CString				Tv_CStrVer;
	// code --------------------
		GrDumyZeroMem(Tv_StrVer, 32);
		GrStrIntToWstr(Tv_StrVer, A_PtrInfo->UpgdVer);
		Tv_Ver[0] = (A_PtrInfo->UpgdVer >> 24) & 0xFF;
		Tv_Ver[1] = (A_PtrInfo->UpgdVer >> 16) & 0xFF;
		Tv_Ver[2] = (A_PtrInfo->UpgdVer >> 8) & 0xFF;
		Tv_Ver[3] = A_PtrInfo->UpgdVer & 0xFF;

		Tv_CStrVer.Format(_T("%d"), Tv_Ver[0]);
		m_EdtVer1.SetWindowTextW(Tv_CStrVer.GetString());
		Tv_CStrVer.Format(_T("%d"), Tv_Ver[1]);
		m_EdtVer2.SetWindowTextW(Tv_CStrVer.GetString());
		Tv_CStrVer.Format(_T("%d"), Tv_Ver[2]);
		m_EdtVer3.SetWindowTextW(Tv_CStrVer.GetString());
		Tv_CStrVer.Format(_T("%d"), Tv_Ver[3]);
		m_EdtVer4.SetWindowTextW(Tv_CStrVer.GetString());
		//m_EdtVersion.SetWindowTextW(Tv_StrVer);// Tv_UpdtInfo.UpgdVer

		
		for(Tv_MdCnt = 0;Tv_MdCnt < E_FirmUpInfoCnt;Tv_MdCnt++)
		{
			if(E_FirmUpInfoTypeNone != A_PtrInfo->ModelInfo[Tv_MdCnt].Type) 
			{
				LcAddModel(A_PtrInfo->ModelInfo[Tv_MdCnt].Type);

				Tv_PtrInfoMd = &A_PtrInfo->ModelInfo[Tv_MdCnt];
				for(Tv_EntCnt = 0;Tv_EntCnt < E_FirmUpEntityCnt;Tv_EntCnt++)
				{
					if(E_FirmUpEntityNone != Tv_PtrInfoMd->Type)
					{
						Tv_PtrStr = Tv_PtrInfoMd->Entity[Tv_EntCnt].StrFile;
						Tv_StrLen = GrStrLen(Tv_PtrStr);

						if(GrFileIsExist(Tv_PtrStr))
						{
							LcAddEntity(Tv_PtrInfoMd->Type, Tv_PtrInfoMd->Entity[Tv_EntCnt].Type, Tv_PtrStr, Tv_StrLen);
						}
						else
						{
							Tv_PtrInfoMd->Entity[Tv_EntCnt].Type = E_FirmUpEntityNone;
							GrDumyZeroMem(Tv_PtrInfoMd->Entity[Tv_EntCnt].StrFile, 256);
						}
					}
				}
			}
		}

		m_EdtPath.SetWindowTextW(m_UpdtInfo.StrUpdtFileName);
		GrDumyZeroMem(m_MkFileName, sizeof(WCHAR) * 1024);
		GrStrWcopy(m_MkFileName, m_UpdtInfo.StrUpdtFileName);
}



BOOL CMkUpdtMngDlg::DestroyWindow()
{
		// TODO: Add your specialized code here and/or call the base class
		LcMkiniFile();

		return CDialogEx::DestroyWindow();
}

__s32 CMkUpdtMngDlg::LcFindModelIdx(__u32 A_ModelType)
{
		return m_ObjFwUp->GetModelTblIdx(A_ModelType);
}

__s32 CMkUpdtMngDlg::LcModelTypeGet(__u32 A_Idx)
{
		__s32 Tv_Result;

		Tv_Result	= -1;
		switch(A_Idx)
		{
		case 1:
			Tv_Result	= E_FirmUpInfoTypeJa1704;
			break;
		case 2:
			Tv_Result	= E_FirmUpInfoTypeJa1708;
			break;
		case 3:
			Tv_Result	= E_FirmUpInfoTypeJa1716;
			break;
		}

		return Tv_Result;
}
