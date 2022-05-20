
// UpdateManagerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "UpdateManager.h"
#include "UpdateManagerDlg.h"
#include "FirmUpdate.h"
#include "DlgModelAdd.h"
#include "DlgVerFileAdd.h"

#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <GrFileCtrl.h>
#include <GrFileTool.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CUpdateManagerDlg ��ȭ ����


// ������
CUpdateManagerDlg::CUpdateManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// �Ҹ���
CUpdateManagerDlg::~CUpdateManagerDlg()
{
	if (NULL != m_pObjFwUp)
	{
		delete m_pObjFwUp;
		m_pObjFwUp = NULL;
	}
}


void CUpdateManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ITEM, m_CTreeCtrl);
	DDX_Control(pDX, IDC_EDIT_PATH, m_CEditPath);
	DDX_Control(pDX, IDC_EDIT_VERSION1, m_CEditVer1);
	DDX_Control(pDX, IDC_EDIT_VERSION2, m_CEditVer2);
	DDX_Control(pDX, IDC_EDIT_VERSION3, m_CEditVer3);
	DDX_Control(pDX, IDC_EDIT_VERSION4, m_CEditVer4);
}

BEGIN_MESSAGE_MAP(CUpdateManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MODEL_CREATE, &CUpdateManagerDlg::OnClickedButtonModelCreate)
	ON_BN_CLICKED(IDC_BUTTON_ENTITY_SELETE, &CUpdateManagerDlg::OnClickedButtonEntitySelete)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PATH, &CUpdateManagerDlg::OnClickedButtonSavePath)
	ON_BN_CLICKED(IDC_BUTTON_PACKAGE_MAKE, &CUpdateManagerDlg::OnClickedButtonPackageMake)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CANCEL, &CUpdateManagerDlg::OnClickedButtonMainCancel)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_LOAD2, &CUpdateManagerDlg::OnClickedButtonModelLoad)
	ON_BN_CLICKED(IDC_BUTTON_ENTITY_DELETE, &CUpdateManagerDlg::OnClickedButtonEntityDelete)
END_MESSAGE_MAP()


// CUpdateManagerDlg �޽��� ó����

BOOL CUpdateManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	Init();


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CUpdateManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CUpdateManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CUpdateManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUpdateManagerDlg::Init()
{
	// Local ------------------------------
		TCHAR			szaInitFile[2048]	= { 0 };
		//Cls_GrFileCtrl* pObjFile			= NULL;
		int				iFileSize			= 0;
	// ------------------------------------

	// ���� ���� ��� ���
	::GetModuleFileName(NULL, m_szaNowPath, 2048);
	// ��ο��� ���ϸ�� Ȯ���ڸ� ����
	PathRemoveFileSpec(m_szaNowPath);

	m_pObjFwUp = new CFirmUpdate();
	m_pObjFwUp->FirmInit();

	// Tree �ʱ�ȭ & UpdateInfo ����ü �ʱ�ȭ
	memset(m_astTreeNode, 0, sizeof(_stUpdateTreeNode) * 3); // 72
	memset(&m_stUpdateInfo, 0, sizeof(_stUpdateInfo));

}

// Init üũ
BOOL CUpdateManagerDlg::CheckInit(pUpdateInfo _pstUpdateInfo)
{
	BOOL bResult = FALSE;

	if (E_MkUpdt_IniFcc == _pstUpdateInfo->uiFcc)
	{
		bResult = TRUE;
	}

	return bResult;
}

void CUpdateManagerDlg::InitCtrl(pUpdateInfo _pstUpdateInfo)
{
	// Local -------------------------------
	CString				strVersion;

	pUpdateInfoModel	pstUpdateInfoModel;
	TCHAR				aszVersion[32]		= { 0 };
	CHAR*				pszFileName			= NULL;
	int					aiVersion[4]		= { 0 };
	int					iStrLen				= 0;
	// -------------------------------------

	GrStrIntToWstr(aszVersion, _pstUpdateInfo->uiUpgdVersion);

	aiVersion[0] = (_pstUpdateInfo->uiUpgdVersion >> 24) & 0xFF;
	aiVersion[1] = (_pstUpdateInfo->uiUpgdVersion >> 16) & 0xFF;
	aiVersion[2] = (_pstUpdateInfo->uiUpgdVersion >> 8) & 0xFF;
	aiVersion[3] = _pstUpdateInfo->uiUpgdVersion & 0xFF;

	strVersion.Format(_T("%d"), aiVersion[0]);
	m_CEditVer1.SetWindowTextW(strVersion.GetString());

	strVersion.Format(_T("%d"), aiVersion[1]);
	m_CEditVer2.SetWindowTextW(strVersion.GetString());

	strVersion.Format(_T("%d"), aiVersion[2]);
	m_CEditVer3.SetWindowTextW(strVersion.GetString());

	strVersion.Format(_T("%d"), aiVersion[3]);
	m_CEditVer4.SetWindowTextW(strVersion.GetString());

	// 16
	for (int i = 0; i < E_FirmUpInfoCnt; i++)
	{
		if (E_FirmUpInfoTypeNone != _pstUpdateInfo->astModelInfo[i].uiType)
		{
			// Ʈ���� �� �߰� �� ���
			TreeAddModel(_pstUpdateInfo->astModelInfo[i].uiType);

			pstUpdateInfoModel = &_pstUpdateInfo->astModelInfo[i];
			for (int si = 0; si < E_FirmUpEntityCnt; si++)
			{
				if (E_FirmUpEntityNone != pstUpdateInfoModel->uiType)
				{
					pszFileName = pstUpdateInfoModel->astEntity[si].aszFile; //Entity ����ü �����̸��� ��´�.
					iStrLen = GrStrLen(pszFileName); // ���̺귯�� ���� ���ϴ� �Լ�

					if (GrFileIsExist(pszFileName))
					{
						TreeAddVerFile(pstUpdateInfoModel->uiType, pstUpdateInfoModel->astEntity[si].uiType, pszFileName, iStrLen);
					}
				}
			}
		}
	}

	m_CEditPath.SetWindowTextW(m_stUpdateInfo.aszUpgdFileName);
	GrDumyZeroMem(m_aszMkFileName, sizeof(WCHAR) * 1024);
	GrStrWcopy(m_aszMkFileName, m_stUpdateInfo.aszUpgdFileName);
}


// �� ���� Button
void CUpdateManagerDlg::OnClickedButtonModelCreate()
{
	// Local ------------------------------
	DlgModelAdd* pDlgModelAdd = NULL;
	int iModelType = 0;
	// ------------------------------------

	pDlgModelAdd = new DlgModelAdd();
	
	pDlgModelAdd->DoModal();

	if (pDlgModelAdd->m_bModalResult)
	{
		iModelType = pDlgModelAdd->GetModelType();

		if (E_FirmUpInfoTypeNone != iModelType)
		{
			TreeAddModel(iModelType);
		}
	}

	if (NULL != pDlgModelAdd)
	{
		delete pDlgModelAdd;
		pDlgModelAdd = NULL;
	}
}

//Ʈ���� �� �߰�
void CUpdateManagerDlg::TreeAddModel(int _iModelType)
{
	// Local ------------------------------
	int iModelType	= _iModelType;
	int iResult		= 0;
	// ------------------------------------

	if (NULL != m_pObjFwUp)
	{
		// �� �߰� �� �ߺ�üũ �Լ��� ����
		iResult = m_pObjFwUp->AddModelType(iModelType);

		if (E_FirmUpInfoTypeNone <= m_pObjFwUp)
		{
			m_astTreeNode[iResult].uiType = _iModelType;

			switch (_iModelType)
			{
			case E_FirmUpInfoTypeJa1704:
				GrStrCopy(m_astTreeNode[iResult].aszNode, "Ja1704");
				m_astTreeNode[iResult].stNode = m_CTreeCtrl.InsertItem(_T("Ja1704"), NULL, NULL);
				break;
			case E_FirmUpInfoTypeJa1708:
				GrStrCopy(m_astTreeNode[iResult].aszNode, "Ja1708");
				m_astTreeNode[iResult].stNode = m_CTreeCtrl.InsertItem(_T("Ja1708"), NULL, NULL);
				break;
			case E_FirmUpInfoTypeJa1716:
				GrStrCopy(m_astTreeNode[iResult].aszNode, "Ja1716");
				m_astTreeNode[iResult].stNode = m_CTreeCtrl.InsertItem(_T("Ja1716"), NULL, NULL);
				break;
			}

			m_stUpdateInfo.astModelInfo[iResult].uiType = _iModelType;
		}
		else
		{
			ProcErrCode(iResult);
		}
	}
}

// ���� ó��
void CUpdateManagerDlg::ProcErrCode(int _iResult)
{
	switch (_iResult)
	{
	case E_FirmUpErrCode:
		MessageBox(_T("Error"));
		break;
	case E_FirmUpErrExistType:
		MessageBox(_T("Exist Same Type"));
		break;
	case E_FirmUpErrExistEntity:
		MessageBox(_T("Exist Same Entity Type"));
		break;
	case E_FirmUpErrExistData:
		MessageBox(_T("Exist Same Entity Data Type"));
		break;
	default:
		MessageBox(_T("Error"));
		break;
	}
}

// ���� ���� ���� Button
void CUpdateManagerDlg::OnClickedButtonEntitySelete()
{
	// Local ------------------------------
	DlgVerFileAdd*	pDlgVerFileAdd;
	CString			strModelName;
	HTREEITEM		stTreeItem			= { 0 };
	int				iModelType			= 0;
	int				iModelIdx			= 0;
	int				iVerFileType		= 0;
	CHAR			pszFilePath[128]	= { 0 };
	TCHAR			aszUniNode[16]		= { 0 };
	int				iFileLen			= 0;
	// ------------------------------------

	pDlgVerFileAdd = new DlgVerFileAdd(NULL, m_szaNowPath);

	// ���� ���� Dlg ȣ��

	// Tree���� Ŭ���� ���� ������ �����´�.
	stTreeItem = m_CTreeCtrl.GetSelectedItem();
	if (NULL != stTreeItem)
	{

		strModelName = m_CTreeCtrl.GetItemText(stTreeItem);
		iFileLen = strModelName.GetLength();

		// FirmUpdate Ŭ������ Model�̸� ����
		pDlgVerFileAdd->SetModelName(strModelName);

		for (int i = 0; i < E_FirmUpInfoTypeMaxIdx; i++)
		{
			// Char�� TCHAR�� �ٲ۴�. ������ CString ������ ������ ���ϱ� ���ؼ�
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_astTreeNode[i].aszNode, strlen(m_astTreeNode[i].aszNode), aszUniNode, 16);

			// �� �Լ�
			if (0 == strModelName.Compare(aszUniNode))
			{
				pDlgVerFileAdd->SetModelType(m_astTreeNode[i].uiType);
				break;
			}
		}

		pDlgVerFileAdd->DoModal();
		if (pDlgVerFileAdd->m_bModalResult)
		{
			// ���� ������ ������ FirmUpdateŬ�������� �����´�.
			pDlgVerFileAdd->GetVerFileType(&iModelType, &iVerFileType, pszFilePath, &iFileLen);
		}

		TreeAddVerFile(iModelType, iVerFileType, pszFilePath, iFileLen);
	}
	else
	{
		MessageBox(_T("������ ���� �����ϴ�"));
	}
	
	if (NULL != pDlgVerFileAdd)
	{
		delete pDlgVerFileAdd;
		pDlgVerFileAdd = NULL;
	}

}

// Ʈ���� �ش� �𵨿��� �������� Ȯ�� �� ����ü�� ������ ����
void CUpdateManagerDlg::TreeAddVerFile(int _iModelIdx, int _iVerFileType, CHAR* _pszFilePath, int _iFileLen)
{
	// Local ------------------------------
	CString			strPath;
	CString			strFileName;

	Cls_GrFileCtrl* pFileCtrl			= NULL; // ���̺귯��
	PCHAR			pszBuff				= NULL;
	int				iModelType			= 0;
	int				iVerFileType		= 0;
	unsigned int	uiFileSize			= 0;
	int				iResult				= 0;
	int				iModelIdx			= 0;
	
	CHAR			aszMulFileName[64]	= { 0 };
	// ------------------------------------

	if (NULL != m_pObjFwUp)
	{
		iModelType = _iModelIdx;
		iVerFileType = _iVerFileType;
		pFileCtrl = new Cls_GrFileCtrl(_pszFilePath, FALSE, FALSE);
		
		// open�� ���� �����ߴٸ� IsOpened = TRUE
		if (pFileCtrl->IsOpened())
		{
			uiFileSize = pFileCtrl->GetSize();

			//pszBuff = (PCHAR)malloc(uiFileSize);
			pszBuff = (PCHAR)malloc(uiFileSize + 1);
			memset(pszBuff, 0, uiFileSize + 1);

			pFileCtrl->Seek(0);
			pFileCtrl->Read(pszBuff, uiFileSize);
			iResult = m_pObjFwUp->AddVerFile(iModelType, iVerFileType, pszBuff, uiFileSize);

			if (0 <= iResult)
			{
				// ��1704�� ������ 0��° �ε����� ��ȯ��.
				iModelIdx = m_pObjFwUp->GetModelTypeIdx(iModelType);

				if (0 <= iModelIdx)
				{
					m_stUpdateInfo.astModelInfo[iModelIdx].astEntity[iResult].uiType = _iVerFileType;
					
					// ���� ��� ����
					GrStrCopy(m_stUpdateInfo.astModelInfo[iModelIdx].astEntity[iResult].aszFile, _pszFilePath);

					// ���� �̸��� ����
					//strPath = _pszFilePath;
					GrStrFnGetFileName(m_stUpdateInfo.astModelInfo[iModelIdx].astEntity[iResult].aszFile, aszMulFileName);

					// �޸� ī��
					TreeAddVerFileNode(iModelType, iVerFileType, aszMulFileName);
				}
			}
			else
			{
				ProcErrCode(iResult);
			}

			if (NULL != pszBuff)
			{
				free(pszBuff);
				pszBuff = NULL;
			}
		}
		
		if (NULL != pFileCtrl)
		{
			delete pFileCtrl;
			pFileCtrl = NULL;
		}
	}
}


void CUpdateManagerDlg::TreeAddVerFileNode(int _iModelType, int _iVerFileType, char* _pszFileName)
{
	int		iNodeIdx			= 0;
	CHAR	szFileName[64]		= { 0 };
	WCHAR	szNodeName[64]		= { 0 };

	CString strFormatName;

	iNodeIdx = FindTreeNode(_iModelType);

	switch (_iVerFileType)
	{
	case E_FirmUpEntityLoader:
		GrStrCopy(szFileName, "Loader(");
		break;
	case E_FirmUpEntityFdt:
		GrStrCopy(szFileName, "FDT(");
		break;
	case E_FirmUpEntityUboot:
		GrStrCopy(szFileName, "U-Boot(");
		break;
	case E_FirmUpEntityKernel:
		GrStrCopy(szFileName, "Kernel(");
		break;
	case E_FirmUpEntityLogo:
		GrStrCopy(szFileName, "Logo(");
		break;
	case E_FirmUpEntityRootfs:
		GrStrCopy(szFileName, "RootFs(");
		break;
	}

	// Node�� �� ���� ����
	GrStrCat(szFileName, _pszFileName);
	GrStrCat(szFileName, ")");
	GrStrStrToWstr(szNodeName, szFileName);

	// Ʈ�� Node�� �߰�
	m_CTreeCtrl.InsertItem(szNodeName, m_astTreeNode[iNodeIdx].stNode, NULL);
	m_CTreeCtrl.Invalidate(TRUE);

	m_stUpdateInfo.astModelInfo[iNodeIdx].uiType = _iModelType;
}


// ��Ÿ�� Ʈ������ ã��
int CUpdateManagerDlg::FindTreeNode(int _iModelType)
{
	int iResult = 0;

	iResult = -1;

	for (int i = 0; i < E_FirmUpInfoTypeMaxIdx; i++)
	{
		if (_iModelType == m_astTreeNode[i].uiType)
		{
			iResult = i;
			break;
		}
	}

	return iResult;
}

// ���� ��� ����
void CUpdateManagerDlg::OnClickedButtonSavePath()
{
	CFileDialog* pFileDlg;
	CString		strPath;
	CString		strInitFile;

	pFileDlg = new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("All Files(*.*)|*.*"), NULL);

	//FileDialog ���� ���� �� lpstrInitialDir�� ���� �ʱ� ��� ������ ����
	pFileDlg->m_ofn.lpstrInitialDir = m_szaNowPath;
	
	if (IDOK == pFileDlg->DoModal())
	{
		strPath = pFileDlg->GetPathName();
		GrDumyZeroMem(m_aszMkFileName, sizeof(WCHAR) * 1024);

		// ��� + ���� ����
		//wsprintf(m_aszMkFileName, strPath.GetBuffer(0));
		GrStrWcopy(m_aszMkFileName, (LPWSTR)(LPCTSTR)strPath);
		m_CEditPath.SetWindowTextW(strPath);

		GrDumyZeroMem(m_stUpdateInfo.aszUpgdFileName, 1024);
		GrStrWcopy(m_stUpdateInfo.aszUpgdFileName, (LPWSTR)(LPCTSTR)strPath);
	}
}

// ������Ʈ ��Ű�� ����
void CUpdateManagerDlg::OnClickedButtonPackageMake()
{
	
	Cls_GrFileCtrl* pFileCtrl		= NULL;
	void*			pFile			= NULL;
	BOOL			bIsSuccess		= FALSE;
	__u8			aszVersion[4] = { 0 };
	__u32			uiFileSize = 0;
	__u32			uiVersion = 0;
	__u32				iWorteSize = 0;
	__u32				iWriteSize = 0;
	__u32				iResult = 0;
	CString strVer1;
	CString strVer2;
	CString strVer3;
	CString strVer4;

	if (NULL != m_pObjFwUp)
	{
		// String�� int�� �ޱ�
		/*
		aiVersion[0] = this->GetDlgItemInt(IDC_EDIT_VERSION1);
		aiVersion[1] = this->GetDlgItemInt(IDC_EDIT_VERSION2);
		aiVersion[2] = this->GetDlgItemInt(IDC_EDIT_VERSION3);
		aiVersion[3] = this->GetDlgItemInt(IDC_EDIT_VERSION4);
		*/	

		m_CEditVer1.GetWindowTextW(strVer1);
		m_CEditVer2.GetWindowTextW(strVer2);
		m_CEditVer3.GetWindowTextW(strVer3);
		m_CEditVer4.GetWindowTextW(strVer4);

		aszVersion[0] = (__u8)(_ttoi(strVer1));
		aszVersion[1] = (__u8)(_ttoi(strVer2));
		aszVersion[2] = (__u8)(_ttoi(strVer3));
		aszVersion[3] = (__u8)(_ttoi(strVer4));
		
		uiVersion = (aszVersion[0] << 24) | (aszVersion[1] << 16) | (aszVersion[2] << 8) | aszVersion[3];

		// ����ü�� ���� ����
		m_pObjFwUp->SetUpdateVersion(uiVersion);

		// ������ ��Ű�� ������ ������
		pFile = m_pObjFwUp->GetMakeUpdate(&uiFileSize); //�������� �ּ�ó��
		// �ذ�! int�ڷ����� unsigned int�� ����� ���� �����͸� ��� ������� �ذ���.
		

		// -------------------------------------- ������� ���ϻ����� ����..

		// FileCtrlŬ���� �Ҵ�
		
		// ���� ����� Ŭ���� �Ҵ�
		pFileCtrl = new Cls_GrFileCtrl(m_aszMkFileName, TRUE, TRUE);
		
		if (TRUE == pFileCtrl->IsOpened())
		{
			iWorteSize = 0;

			// GEtMakeUpdate���� �޾ƿ� ���� ������ ���
			iWriteSize = uiFileSize;
			
			while (TRUE)
			{
				// Write�Լ��� ���� ������, ���� ������� ����� ���
				iResult = pFileCtrl->Write(pFile, iWriteSize);
				iWorteSize = iWorteSize + iResult;
				iWriteSize = iWriteSize - iResult;

				// ���� ������� ���ٸ�
				if (iWorteSize == uiFileSize)
				{
					// ��� TRUE
					bIsSuccess = TRUE;

					// init���� ����� �Լ�ȣ��
					InitMakeFile();
					break;
				}
			}
		}
		else
		{
			MessageBox(_T("File Open Error"), _T("Error"), MB_OK | MB_ICONWARNING);
		}
		// �޸� ����
		if (NULL != pFileCtrl)
		{
			free(pFileCtrl);
			pFileCtrl = NULL;
		}
	}
	if (bIsSuccess)
	{
		MessageBox(_T("Make File Success"), NULL, MB_OK | MB_ICONWARNING);
	}

}

// init���� ����� �Լ�
void CUpdateManagerDlg::InitMakeFile()
{
	CString			strFile;

	TCHAR			aszPath[2048]	= { 0 };
	Cls_GrFileCtrl* pFileCtrl		= NULL;
	__u8			aszVersion[4]	= { 0 };
	__u32			uiVersion		= 0;
	int				iFileNameLen	= 0;
	CString			strVer1;
	CString			strVer2;
	CString			strVer3;
	CString			strVer4;
	CString			strMakeName;

	m_stUpdateInfo.uiFcc = E_MkUpdt_IniFcc;

	/*aiVersion[0] = this->GetDlgItemInt(IDC_EDIT_VERSION1);
	aiVersion[1] = this->GetDlgItemInt(IDC_EDIT_VERSION2);
	aiVersion[2] = this->GetDlgItemInt(IDC_EDIT_VERSION3);
	aiVersion[3] = this->GetDlgItemInt(IDC_EDIT_VERSION4);*/

	m_CEditVer1.GetWindowTextW(strVer1);
	m_CEditVer2.GetWindowTextW(strVer2);
	m_CEditVer3.GetWindowTextW(strVer3);
	m_CEditVer4.GetWindowTextW(strVer4);

	aszVersion[0] = (__u8)(_ttoi(strVer1));
	aszVersion[1] = (__u8)(_ttoi(strVer2));
	aszVersion[2] = (__u8)(_ttoi(strVer3));
	aszVersion[3] = (__u8)(_ttoi(strVer4));

	uiVersion = (aszVersion[0] << 24) | (aszVersion[1] << 16) | (aszVersion[2] << 8) | aszVersion[3];

	m_stUpdateInfo.uiUpgdVersion = uiVersion;
	
	// �������� �̸� �缳��
	strMakeName.Format(_T("%s_V(%d%d%d%d).udf"), m_aszMkFileName, aszVersion[0], aszVersion[1], aszVersion[2], aszVersion[3]);
	wsprintf(m_aszMkFileName, strMakeName.GetBuffer(0));

	/*memset(m_stUpdateInfo.aszUpgdFileName, 0, sizeof(WCHAR)* 1024);
	wsprintf(m_stUpdateInfo.aszUpgdFileName, strMakeName.GetBuffer(0));*/

	memcpy(aszPath, m_aszMkFileName, 2048);
	
	// init���Ϸ� �����
	//strFile.Format(_T("%s\\MkUpdate.init"), aszPath);
	
	// ��ο� �� ���� ����
	pFileCtrl = new Cls_GrFileCtrl(aszPath, TRUE, TRUE); // ���� ���� �ʿ䰡 ���� �ڵ�
	
	if (pFileCtrl)
	{
		if (pFileCtrl->IsOpened())
		{	// ���� ����
			pFileCtrl->Write(&m_stUpdateInfo, sizeof(_stUpdateInfo));
		}
	}

	int test = pFileCtrl->GetSize();

	if (NULL != pFileCtrl)
	{
		delete pFileCtrl;
		pFileCtrl = NULL;
	}
}

BOOL CUpdateManagerDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	InitMakeFile();
	return CDialogEx::DestroyWindow();
}

int CUpdateManagerDlg::FindModelIndex(int _iModelType)
{
	return m_pObjFwUp->GetModelTypeIdx(_iModelType);
}

int CUpdateManagerDlg::GetModelType(int _iIndex)
{
	int iResult = 0;
	
	iResult = -1;

	switch (_iIndex)
	{
	case 1:
		iResult = E_FirmUpInfoTypeJa1704;
		break;
	case 2:
		iResult = E_FirmUpInfoTypeJa1708;
		break;
	case 3:
		iResult = E_FirmUpInfoTypeJa1716;
		break;
	}

	return iResult;
}

// Cancel ��ư
void CUpdateManagerDlg::OnClickedButtonMainCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	PostMessage(WM_CLOSE);
}


void CUpdateManagerDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CDialogEx::OnOK();
}


// �� �ҷ����� ��ư
void CUpdateManagerDlg::OnClickedButtonModelLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString			strPath;
	TCHAR			aszLdPathFile[2048] = { 0 };
	CFileDialog*	pFileDlg			= NULL;
	Cls_GrFileCtrl* pObjFile			= NULL;
	__u32			iFileSize			= 0;
	BOOL			bIsSameModel		= FALSE;
	
	
	if (NULL == pFileDlg)
	{
		pFileDlg = new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("Init Files(*.udf)|*.udf"), NULL);
	}
	
	pFileDlg->m_ofn.lpstrInitialDir = m_szaNowPath;

	if (IDOK == pFileDlg->DoModal())
	{
		// Init
		m_pObjFwUp->FirmInit();
		m_CTreeCtrl.DeleteAllItems();

		strPath = pFileDlg->GetPathName();
		wsprintf(aszLdPathFile, strPath);
	
		// ���� Ʈ������ �ִ� �𵨰� �ҷ��� ������ ���� ���� FCC�� TRUE �ٸ��� FALSE
		bIsSameModel = CheckInit(&m_stUpdateInfo);

		// ���� �����ϴ��� üũ
		if (GrFileIsExist(aszLdPathFile))
		{
			// ������ Ŭ���� �����ͷ� ��´�
			pObjFile = new Cls_GrFileCtrl(aszLdPathFile, FALSE, FALSE);

			// ���� ����
			if (pObjFile->IsOpened())
			{
				// ���� ������ ��������
				iFileSize = (__u32)pObjFile->GetSize();

				if (iFileSize == sizeof(_stUpdateInfo))
				{
					pObjFile->Read(&m_stUpdateInfo, iFileSize);
				}
			}

			//// UpdateInfo üũ
			if (CheckInit(&m_stUpdateInfo))
			{
				InitCtrl(&m_stUpdateInfo);
			}
			
		}

		if (NULL != pObjFile)
		{
			delete pObjFile;
			pObjFile = NULL;
		}
	}

	if (NULL != pFileDlg)
	{
		delete pFileDlg;
		pFileDlg = NULL;
	}
}


//LRESULT CUpdateManagerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//
//	switch (message)
//	{
//	case WM_CLOSE:
//		break;
//
//	default:
//		break;
//	}
//	return CDialogEx::WindowProc(message, wParam, lParam);
//}


void CUpdateManagerDlg::OnClickedButtonEntityDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM stMyItem;
	HTREEITEM stChildItem;

	DlgVerFileAdd* pDlgVerFileAdd = NULL;
	int iModelType		= 0;
	int iModelIdx		= 0;
	int iVerFileType	= 0;
	int iFileLen		= 0;
	CHAR	pszFilePath[128] = { 0 };

	pDlgVerFileAdd = new DlgVerFileAdd(NULL, m_szaNowPath);
	
	stChildItem = m_CTreeCtrl.GetSelectedItem();
	if (stChildItem != NULL)
	{
		m_CTreeCtrl.DeleteItem(stChildItem);

		// ���� ������ ������ FirmUpdateŬ�������� �����´�.
		pDlgVerFileAdd->GetVerFileType(&iModelType, &iVerFileType, pszFilePath, &iFileLen);

		iModelIdx = m_pObjFwUp->GetModelTypeIdx(iModelType);
		
		iVerFileType = FindTreeNode(iModelType);
		m_pObjFwUp->DelVerFile(iModelType, iVerFileType);
	}
	else
	{
		MessageBox(_T("������ ������ �����ؾ� �մϴ�."));
	}


	if (NULL != pDlgVerFileAdd)
	{
		delete pDlgVerFileAdd;
		pDlgVerFileAdd = NULL;
	}
}
