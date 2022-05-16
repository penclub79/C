
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
	ON_BN_CLICKED(IDC_BUTTON_MODEL_LOAD, &CUpdateManagerDlg::OnClickedButtonModelLoad)
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
		Cls_GrFileCtrl* pObjFile			= NULL;
		int				iFileSize			= 0;
	// ------------------------------------

	// ���� ���� ��� ���
	::GetModuleFileName(NULL, m_szaNowPath, 2048);
	// ��ο��� ���ϸ�� Ȯ���ڸ� ����
	PathRemoveFileSpec(m_szaNowPath);

	m_pObjFwUp = (CFirmUpdate*)new CFirmUpdate();
	m_pObjFwUp->FirmInit();

	memset(m_astTreeNode, 0, sizeof(_stUpdateTreeNode) * 3); // 72
	memset(&m_stUpdateInfo, 0, sizeof(_stUpdateInfo));

	memcpy(&szaInitFile, &m_szaNowPath, 2048);
	GrStrWcat(szaInitFile, _T("\\MkUpdate.init"));
	
	// MkUpdate.init ���� �ִ��� üũ
	//if (GrFileIsExist(szaInitFile))
	//{
	//	pObjFile = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(szaInitFile, FALSE, FALSE);
	//	
	//	if (pObjFile->IsOpened())
	//	{
	//		// ���� ������ ��������
	//		iFileSize = (int)pObjFile->GetSize();
	//		if (iFileSize = sizeof(_stUpdateInfo))
	//		{
	//			pObjFile->Read(&m_stUpdateInfo, iFileSize);
	//		}
	//	}

	//	// UpdateInfo üũ
	//	if (CheckInit(&m_stUpdateInfo))
	//	{
	//		InitCtrl(&m_stUpdateInfo);
	//	}
	//}

	if (NULL != pObjFile)
	{
		delete pObjFile;
		pObjFile = NULL;
	}
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
	CString				strFileName;

	pUpdateInfoModel	pstUpdateInfoModel;
	TCHAR				aszVersion[32]		= { 0 };
	TCHAR*				pszFileName			= NULL;
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

	for (int i = 0; i < E_FirmUpInfoCnt; i++)
	{
		if (E_FirmUpInfoTypeNone != _pstUpdateInfo->staModelInfo[i].uiType)
		{
			TreeAddModel(_pstUpdateInfo->staModelInfo[i].uiType);

			pstUpdateInfoModel = &_pstUpdateInfo->staModelInfo[i];
			for (int si = 0; si < E_FirmUpEntityCnt; si++)
			{
				if (E_FirmUpEntityNone != pstUpdateInfoModel->uiType)
				{
					pszFileName = pstUpdateInfoModel->staEntity[i].szaFile;
					strFileName = (LPCTSTR)pszFileName;
					iStrLen = strFileName.GetLength();

					if (GrFileIsExist(pszFileName))
					{
						TreeAddVerFile(pstUpdateInfoModel->uiType, pstUpdateInfoModel->staEntity[i].uiType, pszFileName, iStrLen);
					}
				}
			}
		}
	}

	m_CEditPath.SetWindowTextW(m_stUpdateInfo.szaUpgdFileName);
	memset(m_aszMkFileName, 0, sizeof(TCHAR) * 1024);
	memcpy(m_aszMkFileName, m_stUpdateInfo.szaUpgdFileName, sizeof(TCHAR) * 1024);
}


// �� ���� Button
void CUpdateManagerDlg::OnClickedButtonModelCreate()
{
	// Local ------------------------------
	DlgModelAdd* pDlgModelAdd;
	int iModelType = 0;
	// ------------------------------------

	pDlgModelAdd = (DlgModelAdd*)new DlgModelAdd();
	pDlgModelAdd->DoModal();

	iModelType = pDlgModelAdd->GetModelType();

	if (E_FirmUpInfoTypeNone != iModelType)
	{
		TreeAddModel(iModelType);
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
		// �� �߰��� �ߺ�üũ �Լ��� ����
		iResult = m_pObjFwUp->AddModelType(iModelType);

		if (E_FirmUpInfoTypeNone <= m_pObjFwUp)
		{
			m_astTreeNode[iResult].uiType = _iModelType;

			switch (_iModelType)
			{
			case E_FirmUpInfoTypeJa1704:
				_tcscpy(m_astTreeNode[iResult].szaNode, _T("Ja1704"));
				m_astTreeNode[iResult].stNode = m_CTreeCtrl.InsertItem(_T("Ja1704"), NULL, NULL);
				break;
			case E_FirmUpInfoTypeJa1708:
				_tcscpy(m_astTreeNode[iResult].szaNode, _T("Ja1708"));
				m_astTreeNode[iResult].stNode = m_CTreeCtrl.InsertItem(_T("Ja1708"), NULL, NULL);
				break;
			case E_FirmUpInfoTypeJa1716:
				_tcscpy(m_astTreeNode[iResult].szaNode, _T("Ja1716"));
				m_astTreeNode[iResult].stNode = m_CTreeCtrl.InsertItem(_T("Ja1716"), NULL, NULL);
				break;
			default:
				break;
			}

			m_stUpdateInfo.staModelInfo[iResult].uiType = _iModelType;
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
	TCHAR			aszModel[16]		= { 0 };
	int				iModelType			= 0;
	int				iVerFileType		= 0;
	TCHAR			pszFilePath[128]	= { 0 };
	int				iFileLen			= 0;
	// ------------------------------------

	pDlgVerFileAdd = (DlgVerFileAdd*)new DlgVerFileAdd(NULL, m_szaNowPath);

	// ���� ���� Dlg ȣ��

	// Tree���� Ŭ���� ���� ������ �����´�.
	stTreeItem = m_CTreeCtrl.GetSelectedItem();
	strModelName = m_CTreeCtrl.GetItemText(stTreeItem);

	_tcscpy(aszModel, strModelName.GetBuffer(0));
	pDlgVerFileAdd->SetModelName(&aszModel[0]);
	

	if (IDOK == pDlgVerFileAdd->DoModal())
	{

		pDlgVerFileAdd->GetVerFileType(&iModelType, &iVerFileType, pszFilePath, &iFileLen);
		TreeAddVerFile(iModelType, iVerFileType, pszFilePath, iFileLen);
	}


	if (NULL != pDlgVerFileAdd)
	{
		delete pDlgVerFileAdd;
		pDlgVerFileAdd = NULL;
	}

}

// Ʈ���� �ش� �𵨿��� �������� Ȯ�� �� ����ü�� ������ ����
void CUpdateManagerDlg::TreeAddVerFile(int _iModelIdx, int _iVerFileType, TCHAR* _pszFilePath, int _iFileLen)
{
	// Local ------------------------------
	FILE*			pFile;
	CString			strPath;
	CString			strFileName;
	Cls_GrFileCtrl* pFileCtrl; // ���̺귯��

	TCHAR*			pszBuff			= NULL;
	int				iModelType		= 0;
	int				iVerFileType	= 0;
	unsigned int	uiFileSize		= 0;
	int				iResult			= 0;
	int				iModelIdx		= 0;
	TCHAR			aszFileName[64] = { 0 };
	// ------------------------------------

	if (NULL != m_pObjFwUp)
	{
		iModelType = _iModelIdx;
		iVerFileType = _iVerFileType;
		pFileCtrl = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(_pszFilePath, FALSE, FALSE);
		
		// open�� ���� �����ߴٸ� IsOpened = TRUE
		if (pFileCtrl->IsOpened())
		{
			uiFileSize = pFileCtrl->GetSize();

			pszBuff = new TCHAR(sizeof(TCHAR) * uiFileSize);
			pFileCtrl->Seek(0);
			pFileCtrl->Read(pszBuff, uiFileSize);
			iResult = m_pObjFwUp->AddVerFile(iModelType, iVerFileType, pszBuff, uiFileSize);

			if (0 <= iResult)
			{
				// find model
				iModelIdx = m_pObjFwUp->GetModelTypeIdx(iModelType);
				if (0 <= iModelIdx)
				{
					m_stUpdateInfo.staModelInfo[iModelIdx].staEntity[iResult].uiType = _iVerFileType;
					// ���� ��� ����
					memcpy(m_stUpdateInfo.staModelInfo[iModelIdx].staEntity[iResult].szaFile, _pszFilePath, uiFileSize);

					// ���� �̸��� ����
					// ���
					strPath = _pszFilePath;
					// '\\'ã�� index���� 
					strFileName = strPath.Right(strPath.GetLength() - strPath.ReverseFind('\\') - 1);
					// �޸� ī��
					_tcscpy(aszFileName, strFileName.GetBuffer(0));
					TreeAddVerFileNode(iModelType, iVerFileType, aszFileName);
				}
			}
			else
			{
				ProcErrCode(iResult);
			}
		}
	}
	if (NULL != pFileCtrl)
	{
		delete pFileCtrl;
		pFileCtrl = NULL;
	}
	if (NULL != pszBuff)
	{
		delete pszBuff;
		pszBuff = NULL;
	}

}


void CUpdateManagerDlg::TreeAddVerFileNode(int _iModelType, int _iVerFileType, TCHAR* _pszFileName)
{
	int		iResult				= 0;
	int		iNodeIdx			= 0;
	TCHAR	szTmp[64]			= { 0 };
	TCHAR	szNodeName[64]		= { 0 };
	CString strFormatName;

	iNodeIdx = FindTreeNode(_iModelType);

	switch (_iVerFileType)
	{
	case E_FirmUpEntityLoader:
		_tcscpy(szTmp, _T("Loader"));
		break;
	case E_FirmUpEntityFdt:
		_tcscpy(szTmp, _T("FDT"));
		break;
	case E_FirmUpEntityUboot:
		_tcscpy(szTmp, _T("U-Boot"));
		break;
	case E_FirmUpEntityKernel:
		_tcscpy(szTmp, _T("Kernel"));
		break;
	case E_FirmUpEntityLogo:
		_tcscpy(szTmp, _T("Logo"));
		break;
	case E_FirmUpEntityRootfs:
		_tcscpy(szTmp, _T("RootFs"));
		break;
	}
	// Node�� �� ���� ����
	strFormatName.Format(_T("%s(%s)"), szTmp, _pszFileName);
	_tcscpy(szNodeName, strFormatName.GetBuffer(0));

	// Ʈ�� Node�� �߰�
	m_CTreeCtrl.InsertItem(szNodeName, m_astTreeNode[iNodeIdx].stNode, NULL);
	m_CTreeCtrl.Invalidate(TRUE);

	m_stUpdateInfo.staModelInfo[iNodeIdx].uiType = _iModelType;
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
	CString strPath;

	
	pFileDlg = (CFileDialog*)new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("All Files(*.*)|*.*"), NULL);

	//FileDialog ���� ���� �� lpstrInitialDir�� ���� �ʱ� ��� ������ ����
	pFileDlg->m_ofn.lpstrInitialDir = m_szaNowPath;
	
	if (IDOK == pFileDlg->DoModal())
	{
		strPath = pFileDlg->GetPathName();
		memset(m_aszMkFileName, 0, sizeof(TCHAR) * 1024);
		_tcscpy(m_aszMkFileName, strPath.GetBuffer(0));
		m_CEditPath.SetWindowTextW(strPath);

		memset(m_stUpdateInfo.szaUpgdFileName, 0, 1024);
		_tcscpy(m_stUpdateInfo.szaUpgdFileName, strPath.GetBuffer(0));
	}
}

// ������Ʈ ��Ű�� ����
void CUpdateManagerDlg::OnClickedButtonPackageMake()
{
	Cls_GrFileCtrl* pFileCtrl		= NULL;
	void*			pFile			= NULL;
	BOOL			bIsSuccess		= FALSE;
	int				aiVersion[4]	= { 0 };
	unsigned int	uiFileSize		= 0;
	unsigned int	uiVersion		= 0;
	int				iWorteSize		= 0;
	int				iWriteSize		= 0;
	int				iResult			= 0;
	

	if (NULL != m_pObjFwUp)
	{
		// String�� int�� �ޱ�
		aiVersion[0] = this->GetDlgItemInt(IDC_EDIT_VERSION1);
		aiVersion[1] = this->GetDlgItemInt(IDC_EDIT_VERSION2);
		aiVersion[2] = this->GetDlgItemInt(IDC_EDIT_VERSION3);
		aiVersion[3] = this->GetDlgItemInt(IDC_EDIT_VERSION4);	
		
		uiVersion = (aiVersion[0] << 24) | (aiVersion[1] << 16) | (aiVersion[2] << 8) | aiVersion[3];

		// ���� ����
		m_pObjFwUp->SetUpdateVersion(uiVersion);

		// ������ ��Ű�� ������ ������
		//pFile = m_pObjFwUp->GetMakeUpdate(&uiFileSize);
		
		// FileCtrlŬ���� �Ҵ�
		if (NULL == pFileCtrl)
		{
			// ���� ����� Ŭ���� �Ҵ�
			pFileCtrl = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(m_aszMkFileName, TRUE, TRUE);
		}

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
				iWriteSize = iWriteSize + iResult;

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
	int				aiVersion[4]	= { 0 };
	int				iVersion		= 0;
	int				iFileNameLen	= 0;

	m_stUpdateInfo.uiFcc = E_MkUpdt_IniFcc;

	aiVersion[0] = this->GetDlgItemInt(IDC_EDIT_VERSION1);
	aiVersion[1] = this->GetDlgItemInt(IDC_EDIT_VERSION2);
	aiVersion[2] = this->GetDlgItemInt(IDC_EDIT_VERSION3);
	aiVersion[3] = this->GetDlgItemInt(IDC_EDIT_VERSION4);

	iVersion = (aiVersion[0] << 24) | (aiVersion[1] << 16) | (aiVersion[2] << 8) | aiVersion[3];

	m_stUpdateInfo.uiUpgdVersion = iVersion;
	
	memcpy(aszPath, m_szaNowPath, 2048);
	
	// init���Ϸ� �����
	strFile.Format(_T("%s\\MkUpdate.init"), aszPath);

	// ���+���� ����
	_tcscpy(aszPath, strFile);
	
	// ��ο� �� ���� ����
	pFileCtrl = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(aszPath, TRUE, TRUE);
	
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
	OnOK();
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


}
