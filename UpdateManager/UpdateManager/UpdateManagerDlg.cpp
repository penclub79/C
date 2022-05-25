
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
	m_pObjFwUp = NULL;
	memset(m_aszNowPath, 0, 2048);
	memset(m_aszMkFileName, 0, 1024);
	memset(m_aszMkModelName, 0, 2048);
	memset(m_astTreeNode, 0, E_FirmUpInfoTypeMaxIdx);
	m_stUpdateInfo = { 0 };
	
	for (int i = 0; i < E_FirmUpInfoTypeMaxIdx; i++)
	{
		m_astTreeNode[i] = { 0 };
	}
	

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
	DDX_Control(pDX, IDC_EDIT_MODEL_PATH, m_CEditModelPath);
}

BEGIN_MESSAGE_MAP(CUpdateManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MODEL_CREATE, &CUpdateManagerDlg::OnClickedButtonModelCreate)
	ON_BN_CLICKED(IDC_BUTTON_ENTITY_SELETE, &CUpdateManagerDlg::OnClickedButtonEntitySelete)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CANCEL, &CUpdateManagerDlg::OnClickedButtonMainCancel)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_LOAD2, &CUpdateManagerDlg::OnClickedButtonModelLoad)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_SAVE_PATH, &CUpdateManagerDlg::OnClickedButtonModelSavePath)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PATH, &CUpdateManagerDlg::OnClickedButtonSavePath)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_MAKES, &CUpdateManagerDlg::OnClickedButtonModelMakes)
	ON_BN_CLICKED(IDC_BUTTON_PACKAGE_MAKE, &CUpdateManagerDlg::OnClickedButtonPackageMake)
	ON_BN_CLICKED(IDC_BUTTON_ENTITY_DELETE, &CUpdateManagerDlg::OnClickedButtonEntityDelete)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_DELETE, &CUpdateManagerDlg::OnClickedButtonModelDelete)
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

	if (m_strPathItem.IsEmpty())
		GetDlgItem(IDC_BUTTON_MODEL_MAKES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_BUTTON_MODEL_MAKES)->EnableWindow(TRUE);


	
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
	// ���� ���� ��� ���
	::GetModuleFileName(NULL, m_aszNowPath, 2048);
	// ��ο��� ���ϸ�� Ȯ���ڸ� ����
	PathRemoveFileSpec(m_aszNowPath);

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
	pUpdateInfoModel	pstUpdateInfoModel	= { 0 };
	TCHAR				aszVersion[32]		= { 0 };
	CHAR*				pszFileName			= NULL;
	int					aiVersion[4]		= { 0 };
	int					iStrLen				= 0;
	// -------------------------------------

	GrStrIntToWstr(aszVersion, _pstUpdateInfo->uiUpgdVersion);

	aiVersion[3] = (_pstUpdateInfo->uiUpgdVersion >> 24) & 0xFF;
	aiVersion[2] = (_pstUpdateInfo->uiUpgdVersion >> 16) & 0xFF;
	aiVersion[1] = (_pstUpdateInfo->uiUpgdVersion >> 8) & 0xFF;
	aiVersion[0] = _pstUpdateInfo->uiUpgdVersion & 0xFF;

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
			for (int iEntiIdx = 0; iEntiIdx < E_FirmUpEntityCnt; iEntiIdx++)
			{
				if (E_FirmUpEntityNone != pstUpdateInfoModel->uiType)
				{
					pszFileName = pstUpdateInfoModel->astEntity[iEntiIdx].aszFile; //Entity ����ü �����̸��� ��´�.
					iStrLen = GrStrLen(pszFileName);						// ���̺귯�� ���� ���ϴ� �Լ�

					if (GrFileIsExist(pszFileName))
					{
						TreeAddVerFile(pstUpdateInfoModel->uiType, pstUpdateInfoModel->astEntity[iEntiIdx].uiType, pszFileName, iStrLen);
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
	DlgModelAdd*	pDlgModelAdd	= NULL;
	int				iModelType		= 0;
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
			GetDlgItem(IDC_BUTTON_MODEL_DELETE)->EnableWindow(TRUE);
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
	CString			strModelName;
	DlgVerFileAdd*	pDlgVerFileAdd		= NULL;
	HTREEITEM		stTreeItem			= { 0 };
	int				iModelType			= 0;
	int				iModelIdx			= 0;
	int				iVerFileType		= 0;
	CHAR			pszFilePath[128]	= { 0 };
	TCHAR			aszUniNode[16]		= { 0 };
	int				iFileLen			= 0;
	// ------------------------------------

	pDlgVerFileAdd = new DlgVerFileAdd(NULL, m_aszNowPath);

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
	CHAR			aszMulFileName[64]	= { 0 };
	PCHAR			pszBuff				= NULL;
	int				iModelType			= 0;
	int				iVerFileType		= 0;
	unsigned int	uiFileSize			= 0;
	int				iResult				= 0;
	int				iModelIdx			= 0;
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
	// Local ------------------------------
	int		iNodeIdx			= 0;
	CHAR	szFileName[64]		= { 0 };
	WCHAR	szNodeName[64]		= { 0 };
	CString strFormatName;
	// ------------------------------------

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

	for (int iNodeIdx = 0; iNodeIdx < E_FirmUpInfoTypeMaxIdx; iNodeIdx++)
	{
		if (_iModelType == m_astTreeNode[iNodeIdx].uiType)
		{
			iResult = iNodeIdx;
			break;
		}
	}

	return iResult;
}

// ���� ��� ����
void CUpdateManagerDlg::OnClickedButtonSavePath()
{
	// Local ------------------------------
	CFileDialog*	pFileDlg = NULL;
	CString			strPath;
	CString			strInitFile;
	// ------------------------------------

	pFileDlg = new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("All Files(*.*)|*.*"), NULL);

	//FileDialog ���� ���� �� lpstrInitialDir�� ���� �ʱ� ��� ������ ����
	pFileDlg->m_ofn.lpstrInitialDir = m_aszNowPath;
	
	if (IDOK == pFileDlg->DoModal())
	{
		strPath = pFileDlg->GetPathName();
		GrDumyZeroMem(m_aszMkFileName, sizeof(WCHAR) * 1024);

		// ��� + ���� ����
		//wsprintf(m_aszMkFileName, strPath.GetBuffer(0));
		GrStrWcopy(m_aszMkFileName, (LPWSTR)(LPCTSTR)strPath);
		m_CEditPath.SetWindowTextW(strPath);

		// ����ü�� ������Ʈ ���+���ϸ� �ֱ�
		GrDumyZeroMem(m_stUpdateInfo.aszUpgdFileName, 1024);
		GrStrWcopy(m_stUpdateInfo.aszUpgdFileName, (LPWSTR)(LPCTSTR)strPath);
	}
}

// ������Ʈ ��Ű�� ����
void CUpdateManagerDlg::OnClickedButtonPackageMake()
{
	// Local ------------------------------
	CString			strPath;
	Cls_GrFileCtrl* pFileCtrl		= NULL;
	void*			pFile			= NULL;
	BOOL			bIsSuccess		= FALSE;
	unsigned int	uiFileSize		= 0;
	unsigned int	uiVersion		= 0;
	unsigned int	uiWorteSize		= 0;
	unsigned int	uiWriteSize		= 0;
	unsigned int	uiResult		= 0;
	int				aiVersion[4]	= { 0 };
	// ------------------------------------

	if (NULL != m_pObjFwUp)
	{
		if (!m_stUpdateInfo.uiUpgdVersion)
		{
			aiVersion[0] = this->GetDlgItemInt(IDC_EDIT_VERSION4);
			aiVersion[1] = this->GetDlgItemInt(IDC_EDIT_VERSION3);
			aiVersion[2] = this->GetDlgItemInt(IDC_EDIT_VERSION2);
			aiVersion[3] = this->GetDlgItemInt(IDC_EDIT_VERSION1);
		}
		else
		{
			aiVersion[0] = (m_stUpdateInfo.uiUpgdVersion >> 24) & 0xFF;
			aiVersion[1] = (m_stUpdateInfo.uiUpgdVersion >> 16) & 0xFF;
			aiVersion[2] = (m_stUpdateInfo.uiUpgdVersion >> 8)	& 0xFF;
			aiVersion[3] = m_stUpdateInfo.uiUpgdVersion			& 0xFF;
		}

		uiVersion = (aiVersion[0] << 24) | (aiVersion[1] << 16) | (aiVersion[2] << 8) | aiVersion[3];

		// ����ü�� ���� ����
		m_pObjFwUp->SetUpdateVersion(uiVersion);

		// ������ ��Ű�� ������ ������
		pFile = m_pObjFwUp->GetMakeUpdate(&uiFileSize); //�������� �ּ�ó��
		// �ذ�! int�ڷ����� unsigned int�� ����� ���� �����͸� ��� ������� �ذ���.
		
		// ���� ����� Ŭ���� �Ҵ�
		pFileCtrl = new Cls_GrFileCtrl(m_aszMkFileName, TRUE, TRUE);

		// ����ü�� ������Ʈ ���+���ϸ� �ֱ�
		memset(m_stUpdateInfo.aszUpgdFileName, 0, 1024);
		//GrStrWcopy(m_stUpdateInfo.aszUpgdFileName, m_aszMkFileName);
		wsprintf(m_stUpdateInfo.aszUpgdFileName, m_aszMkFileName);
		
		if (TRUE == pFileCtrl->IsOpened())
		{
			uiWorteSize = 0;

			// GEtMakeUpdate���� �޾ƿ� ���� ������ ���
			uiWriteSize = uiFileSize;
			
			while (!bIsSuccess)
			{
				// Write�Լ��� ���� ������, ���� ������� ����� ���
				uiResult = pFileCtrl->Write(pFile, uiWriteSize);
				uiWorteSize = uiWorteSize + uiResult;
				uiWriteSize = uiWriteSize - uiResult;

				// ���� ������� ���ٸ�
				if (uiWorteSize == uiFileSize)
				{
				//	// ��� TRUE
					bIsSuccess = TRUE;
					InitMakeModel();
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
	// Local ------------------------------
	CString			strFile;
	TCHAR			aszPath[2048]	= { 0 };
	Cls_GrFileCtrl* pFileCtrl		= NULL;
	unsigned int	uiVersion		= 0;
	int				iFileNameLen	= 0;
	int				aiVersion[4]	= { 0 };
	// ------------------------------------

	m_stUpdateInfo.uiFcc = E_MkUpdt_IniFcc;

	if (!m_stUpdateInfo.uiUpgdVersion)
	{
		aiVersion[0] = this->GetDlgItemInt(IDC_EDIT_VERSION4);
		aiVersion[1] = this->GetDlgItemInt(IDC_EDIT_VERSION3);
		aiVersion[2] = this->GetDlgItemInt(IDC_EDIT_VERSION2);
		aiVersion[3] = this->GetDlgItemInt(IDC_EDIT_VERSION1);
	}
	else
	{
		aiVersion[0] = (m_stUpdateInfo.uiUpgdVersion >> 24) & 0xFF;
		aiVersion[1] = (m_stUpdateInfo.uiUpgdVersion >> 16) & 0xFF;
		aiVersion[2] = (m_stUpdateInfo.uiUpgdVersion >> 8) & 0xFF;
		aiVersion[3] = m_stUpdateInfo.uiUpgdVersion & 0xFF;
	}

	uiVersion = (aiVersion[0] << 24) | (aiVersion[1] << 16) | (aiVersion[2] << 8) | aiVersion[3];

	m_stUpdateInfo.uiUpgdVersion = uiVersion;
	
	// ��ο� �� ���� ����
	pFileCtrl = new Cls_GrFileCtrl(aszPath, TRUE, TRUE);
	
	if (pFileCtrl)
	{
		if (pFileCtrl->IsOpened())
		{	// ���� ����
			pFileCtrl->Write(&m_stUpdateInfo, sizeof(_stUpdateInfo));
		}
	}

	if (NULL != pFileCtrl)
	{
		delete pFileCtrl;
		pFileCtrl = NULL;
	}
}

BOOL CUpdateManagerDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//InitMakeFile();
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
	// Local ------------------------------
	CString			strPath;
	TCHAR			aszLdPathFile[2048] = { 0 };
	CFileDialog*	pFileDlg			= NULL;
	Cls_GrFileCtrl* pObjFile			= NULL;
	unsigned int	uiFileSize			= 0;
	BOOL			bIsSameModel		= FALSE;
	// ------------------------------------
	
	if (NULL == pFileDlg)
	{
		pFileDlg = new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("Init Files(*.init)|*.init"), NULL);
	}
	
	pFileDlg->m_ofn.lpstrInitialDir = m_aszNowPath;

	if (IDOK == pFileDlg->DoModal())
	{
		// Init
		m_pObjFwUp->FirmInit();
		m_CTreeCtrl.DeleteAllItems();

		GrDumyZeroMem(&m_stUpdateInfo, sizeof(_stUpdateInfo));

		strPath = pFileDlg->GetPathName();
		wsprintf(aszLdPathFile, strPath);
	
		GrStrWcopy(m_aszMkModelName, (LPWSTR)(LPCTSTR)strPath);
		m_CEditModelPath.SetWindowTextW(strPath);
		m_strPathItem = strPath;

		if (m_strPathItem.IsEmpty())
			GetDlgItem(IDC_BUTTON_MODEL_MAKES)->EnableWindow(FALSE);
		else
			GetDlgItem(IDC_BUTTON_MODEL_MAKES)->EnableWindow(TRUE);

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
				uiFileSize = (unsigned int)pObjFile->GetSize();

				if (uiFileSize == sizeof(_stUpdateInfo))
				{
					pObjFile->Read(&m_stUpdateInfo, uiFileSize);
				}
			}

			// UpdateInfo üũ
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

// �� ������ ��� ��ư
void CUpdateManagerDlg::OnClickedButtonModelSavePath()
{
	// Local ------------------------------
	CFileDialog*	pFileDlg = NULL;
	CString			strPath;
	CString			strInitFile;
	// ------------------------------------

	pFileDlg = new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("All Files(*.init)|*.init"), NULL);

	pFileDlg->m_ofn.lpstrInitialDir = m_aszNowPath;

	if (IDOK == pFileDlg->DoModal())
	{
		strPath = pFileDlg->GetPathName();
		GrDumyZeroMem(m_aszMkModelName, sizeof(WCHAR) * 2048);

		GrStrWcopy(m_aszMkModelName, (LPWSTR)(LPCTSTR)strPath);
		m_CEditModelPath.SetWindowTextW(strPath);
		m_strPathItem = strPath;
	}

	if (m_strPathItem.IsEmpty())
		GetDlgItem(IDC_BUTTON_MODEL_MAKES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_BUTTON_MODEL_MAKES)->EnableWindow(TRUE);

}


// �� ���� ���� ��ư
void CUpdateManagerDlg::OnClickedButtonModelMakes()
{
	unsigned int uiVersion = 0;
	
	if (NULL != m_pObjFwUp)
	{
		// ����ü�� ���� ����
		m_pObjFwUp->SetUpdateVersion(uiVersion);

		InitMakeModel();
		MessageBox(_T("Make Model Success"), NULL, MB_OK | MB_ICONWARNING);
	}
}

// Model���� ����� �Լ�
void CUpdateManagerDlg::InitMakeModel() 
{
	// Local ------------------------------
	CString			strFile;
	CString			strMakeName;
	CFileFind		pFileFind;
	BOOL			bIsFile			= FALSE;
	TCHAR			aszPath[2048]	= { 0 };
	Cls_GrFileCtrl* pFileCtrl		= NULL;
	int				aiVersion[4]	= { 0 };
	unsigned int	uiVersion		= 0;
	int				iFileNameLen	= 0;
	// ------------------------------------

	m_stUpdateInfo.uiFcc = E_MkUpdt_IniFcc;

	if (!m_stUpdateInfo.uiUpgdVersion)
	{
		aiVersion[0] = this->GetDlgItemInt(IDC_EDIT_VERSION4);
		aiVersion[1] = this->GetDlgItemInt(IDC_EDIT_VERSION3);
		aiVersion[2] = this->GetDlgItemInt(IDC_EDIT_VERSION2);
		aiVersion[3] = this->GetDlgItemInt(IDC_EDIT_VERSION1);
	}
	else
	{
		aiVersion[0] = (m_stUpdateInfo.uiUpgdVersion >> 24) & 0xFF;
		aiVersion[1] = (m_stUpdateInfo.uiUpgdVersion >> 16) & 0xFF;
		aiVersion[2] = (m_stUpdateInfo.uiUpgdVersion >> 8)	& 0xFF;
		aiVersion[3] = m_stUpdateInfo.uiUpgdVersion			& 0xFF;
	}

	uiVersion = (aiVersion[0] << 24) | (aiVersion[1] << 16) | (aiVersion[2] << 8) | aiVersion[3];

	m_stUpdateInfo.uiUpgdVersion = uiVersion;
	
	// ���� ���� ���� üũ
	bIsFile = pFileFind.FindFile(m_aszMkModelName); 
	
	// ������ �������� ������ ���ο� �������� ������ �����
	if (!bIsFile)
	{
		strMakeName.Format(_T("%s_V(%d%d%d%d).init"), m_aszMkModelName, aiVersion[3], aiVersion[2], aiVersion[1], aiVersion[0]);
		wsprintf(m_aszMkModelName, strMakeName.GetBuffer(0));
	}
	
	memcpy(aszPath, m_aszMkModelName, 2048);

	// init���Ϸ� �����
	// ��ο� �� ���� ����
	pFileCtrl = new Cls_GrFileCtrl(aszPath, TRUE, TRUE);

	if (pFileCtrl)
	{
		if (pFileCtrl->IsOpened())
		{	// ���� ����
			pFileCtrl->Write(&m_stUpdateInfo, sizeof(_stUpdateInfo));
		}
	}

	if (NULL != pFileCtrl)
	{
		delete pFileCtrl;
		pFileCtrl = NULL;
	}

}

void CUpdateManagerDlg::OnClickedButtonEntityDelete()
{
	// Local ------------------------------
	HTREEITEM		stRootTree;
	HTREEITEM		stFirstTreeNode;
	HTREEITEM		stNowTreeNode;
	CString			strSelectName;
	CString			strNodeName;
	int				iModelIdx	= 0;
	int				iNodeIdx	= 0;
	// ------------------------------------

	// ���� ������ Node�� Root Tree�� ��������
	stRootTree = m_CTreeCtrl.GetSelectedItem();
	while (stRootTree)
	{
		stRootTree = m_CTreeCtrl.GetNextItem(stRootTree, TVGN_NEXT);
		if (NULL == stRootTree)
		{
			break;
		}
		iModelIdx++;
	}

	// ���� ������ Tree�� Node�� ��������
	stNowTreeNode = m_CTreeCtrl.GetSelectedItem();
	strSelectName = m_CTreeCtrl.GetItemText(stNowTreeNode);

	stFirstTreeNode = m_CTreeCtrl.GetChildItem(stNowTreeNode);
	stFirstTreeNode = m_CTreeCtrl.GetNextItem(stFirstTreeNode, TVGN_CHILD); // Tree Root��������
	stFirstTreeNode = m_CTreeCtrl.GetNextItem(stFirstTreeNode, TVGN_CHILD); // Tree Root�� Node��������

	while (stFirstTreeNode)
	{
		strNodeName = m_CTreeCtrl.GetItemText(stFirstTreeNode);
		stFirstTreeNode = m_CTreeCtrl.GetNextItem(stFirstTreeNode, TVGN_NEXT); // Tree Root�� ���� Node ��������

		if (strSelectName == strNodeName) //���� ������ ���ڿ� ������ break
		{
			break;
		}
		iNodeIdx++;
	}

	// FirmWare �����͸� ����
	m_pObjFwUp->DelVerFile(m_astTreeNode[iModelIdx].uiType, m_stUpdateInfo.astModelInfo[iModelIdx].astEntity[iNodeIdx].uiType);

	if (stNowTreeNode != NULL)
	{
		// ����
		m_CTreeCtrl.DeleteItem(stNowTreeNode);												// Ʈ�� ����
		m_stUpdateInfo.astModelInfo[iModelIdx].astEntity[iNodeIdx].uiType = 0;				// ����ü ������ ����1
		memset(m_stUpdateInfo.astModelInfo[iModelIdx].astEntity[iNodeIdx].aszFile, 0, 256); // ����ü ������ ����2
	}
	else
	{
		AfxMessageBox(_T("������ ������ �����ؾ� �մϴ�."));
	}
}

void CUpdateManagerDlg::OnClickedButtonModelDelete()
{
	// Local ------------------------------
	HTREEITEM	stRootTree;
	CFileFind	pFileFind;
	int			iModelIdx = 0;
	// ------------------------------------

	stRootTree = m_CTreeCtrl.GetSelectedItem();

	if (stRootTree)
	{
		while (stRootTree)
		{
			stRootTree = m_CTreeCtrl.GetNextItem(stRootTree, TVGN_NEXT);
			if (NULL == stRootTree)
			{
				break;
			}
			iModelIdx++;
		}

		if (IDYES == AfxMessageBox(_T("�ش� ���� ����ðڽ��ϱ�?"), MB_YESNO))
		{

			m_pObjFwUp->DelModelType(m_astTreeNode[iModelIdx].uiType);	// �߿��� ������ ����
			m_CTreeCtrl.DeleteItem(stRootTree);							// Ʈ������ �� ����

			m_stUpdateInfo.astModelInfo[iModelIdx].uiType = 0;											// ����ü���� ������ ���� 1
			memset(m_stUpdateInfo.astModelInfo[iModelIdx].astEntity, 0, sizeof(_stUpdateInfoModel));	// ����ü���� ������ ���� 2

		}
	}
	else
	{
		AfxMessageBox(_T("������ ���� �����ؾ� �մϴ�."));
	}
}
