
// UpdateManagerDlg.cpp : 구현 파일
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


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CUpdateManagerDlg 대화 상자


// 생성자
CUpdateManagerDlg::CUpdateManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// 소멸자
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
	ON_BN_CLICKED(IDC_BUTTON_MODEL_LOAD, &CUpdateManagerDlg::OnClickedButtonModelLoad)
END_MESSAGE_MAP()


// CUpdateManagerDlg 메시지 처리기

BOOL CUpdateManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	Init();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CUpdateManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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

	// 현재 실행 경로 얻기
	::GetModuleFileName(NULL, m_szaNowPath, 2048);
	// 경로에서 파일명과 확장자만 제거
	PathRemoveFileSpec(m_szaNowPath);

	m_pObjFwUp = (CFirmUpdate*) new CFirmUpdate();
	m_pObjFwUp->FirmInit();

	// Tree 초기화 & UpdateInfo 구조체 초기화
	memset(m_astTreeNode, 0, sizeof(_stUpdateTreeNode) * 3); // 72
	memset(&m_stUpdateInfo, 0, sizeof(_stUpdateInfo));
}

// Init 체크
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

	for (int i = 0; i < E_FirmUpInfoCnt; i++)
	{
		if (E_FirmUpInfoTypeNone != _pstUpdateInfo->astModelInfo[i].uiType)
		{
			TreeAddModel(_pstUpdateInfo->astModelInfo[i].uiType);

			pstUpdateInfoModel = &_pstUpdateInfo->astModelInfo[i];
			for (int si = 0; si < E_FirmUpEntityCnt; si++)
			{
				if (E_FirmUpEntityNone != pstUpdateInfoModel->uiType)
				{
					/*WideCharToMultiByte(CP_ACP, 0, pstUpdateInfoModel->astEntity[i].aszFile, 256, pszFileName, 256, NULL, NULL);*/
					memcpy(pstUpdateInfoModel->astEntity[i].aszFile, pszFileName, 256);

					strFileName = pszFileName;
					iStrLen = strFileName.GetLength();

					if (GrFileIsExist(pszFileName))
					{
						TreeAddVerFile(pstUpdateInfoModel->uiType, pstUpdateInfoModel->astEntity[i].uiType, pszFileName, iStrLen);
					}
				}
			}
		}
	}

	m_CEditPath.SetWindowTextW(m_stUpdateInfo.aszUpgdFileName);
	memset(m_aszMkFileName, 0, sizeof(TCHAR) * 1024);
	memcpy(m_aszMkFileName, m_stUpdateInfo.aszUpgdFileName, sizeof(TCHAR) * 1024);
}


// 모델 생성 Button
void CUpdateManagerDlg::OnClickedButtonModelCreate()
{
	// Local ------------------------------
	DlgModelAdd* pDlgModelAdd;
	int iModelType = 0;
	// ------------------------------------

	pDlgModelAdd = (DlgModelAdd*)new DlgModelAdd();
	
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

//트리에 모델 추가
void CUpdateManagerDlg::TreeAddModel(int _iModelType)
{
	// Local ------------------------------
	int iModelType	= _iModelType;
	int iResult		= 0;
	// ------------------------------------

	if (NULL != m_pObjFwUp)
	{
		// 모델 추가시 중복체크 함수도 포함
		iResult = m_pObjFwUp->AddModelType(iModelType);

		if (E_FirmUpInfoTypeNone <= m_pObjFwUp)
		{
			m_astTreeNode[iResult].uiType = _iModelType;

			switch (_iModelType)
			{
			case E_FirmUpInfoTypeJa1704:
				_tcscpy(m_astTreeNode[iResult].aszNode, _T("Ja1704"));
				m_astTreeNode[iResult].stNode = m_CTreeCtrl.InsertItem(_T("Ja1704"), NULL, NULL);
				break;
			case E_FirmUpInfoTypeJa1708:
				_tcscpy(m_astTreeNode[iResult].aszNode, _T("Ja1708"));
				m_astTreeNode[iResult].stNode = m_CTreeCtrl.InsertItem(_T("Ja1708"), NULL, NULL);
				break;
			case E_FirmUpInfoTypeJa1716:
				_tcscpy(m_astTreeNode[iResult].aszNode, _T("Ja1716"));
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

// 에러 처리
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

// 버전 파일 선택 Button
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
	int				iFileLen			= 0;
	// ------------------------------------

	pDlgVerFileAdd = new DlgVerFileAdd(NULL, m_szaNowPath);

	// 버전 파일 Dlg 호출

	// Tree에서 클릭한 값의 제목을 가져온다.
	stTreeItem = m_CTreeCtrl.GetSelectedItem();
	strModelName = m_CTreeCtrl.GetItemText(stTreeItem);
	iFileLen = strModelName.GetLength();

	pDlgVerFileAdd->SetModelName(strModelName);

	for (int i = 0; i < E_FirmUpInfoTypeMaxIdx; i++)
	{
		if (0 == strModelName.Compare(m_astTreeNode[i].aszNode))
		{
			pDlgVerFileAdd->SetModelType(m_astTreeNode[i].uiType);
			break;
		}
	}

	pDlgVerFileAdd->DoModal();
	// -------------------------------여기까지 문제 없음


	if (pDlgVerFileAdd->m_bModalResult)
	{
		pDlgVerFileAdd->GetVerFileType(&iModelType, &iVerFileType, pszFilePath, &iFileLen);
	}

	if (NULL != pDlgVerFileAdd)
	{
		delete pDlgVerFileAdd;
		pDlgVerFileAdd = NULL;
	}

	TreeAddVerFile(iModelType, iVerFileType, pszFilePath, iFileLen);

}

// 트리에 해당 모델에서 버전파일 확인 후 구조체에 데이터 적용
void CUpdateManagerDlg::TreeAddVerFile(int _iModelIdx, int _iVerFileType, CHAR* _pszFilePath, int _iFileLen)
{
	// Local ------------------------------
	CString			strPath;
	CString			strFileName;

	Cls_GrFileCtrl* pFileCtrl			= NULL; // 라이브러리
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
		
		// open시 파일 선택했다면 IsOpened = TRUE
		if (pFileCtrl->IsOpened())
		{
			uiFileSize = pFileCtrl->GetSize();

			//pszBuff = (PCHAR)malloc(uiFileSize);
			pszBuff = (PCHAR)malloc(uiFileSize+1);
			memset(pszBuff, 0, uiFileSize + 1);

			pFileCtrl->Seek(0);
			pFileCtrl->Read(pszBuff, uiFileSize);
			iResult = m_pObjFwUp->AddVerFile(iModelType, iVerFileType, pszBuff, uiFileSize);

			if (0 <= iResult)
			{
				// 모델 타입 1704 = 1, 1708 = 2, 1716 = 3 대입
				iModelIdx = m_pObjFwUp->GetModelTypeIdx(iModelType);

				if (0 <= iModelIdx)
				{
					m_stUpdateInfo.astModelInfo[iModelIdx].astEntity[iResult].uiType = _iVerFileType;
					
					// 파일 경로 복사
					GrStrCopy(m_stUpdateInfo.astModelInfo[iModelIdx].astEntity[iResult].aszFile, _pszFilePath);

					// 파일 이름만 복사
					//strPath = _pszFilePath;
					GrStrFnGetFileName(m_stUpdateInfo.astModelInfo[iModelIdx].astEntity[iResult].aszFile, aszMulFileName);

					// 메모리 카피
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
	int		iResult				= 0;
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
	// Node에 들어갈 제목 수정
	//strFormatName.Format(_T("%s(%s)"), szTmp, (LPCTSTR)_pszFileName);
	GrStrCat(szFileName, _pszFileName);
	GrStrCat(szFileName, ")");
	GrStrStrToWstr(szNodeName, szFileName);
	//_tcscpy(szNodeName, strFormatName.GetBuffer(0));

	// 트리 Node에 추가
	m_CTreeCtrl.InsertItem(szNodeName, m_astTreeNode[iNodeIdx].stNode, NULL);
	//m_CTreeCtrl.Invalidate(TRUE);
	m_CTreeCtrl.Invalidate(TRUE);

	m_stUpdateInfo.astModelInfo[iNodeIdx].uiType = _iModelType;
}


// 모델타입 트리에서 찾기
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

// 저장 경로 설정
void CUpdateManagerDlg::OnClickedButtonSavePath()
{
	CFileDialog* pFileDlg;
	CString strPath;

	pFileDlg = (CFileDialog*)new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("All Files(*.*)|*.*"), NULL);

	//FileDialog 오픈 했을 때 lpstrInitialDir를 통해 초기 경로 폴더로 열기
	pFileDlg->m_ofn.lpstrInitialDir = m_szaNowPath;
	
	if (IDOK == pFileDlg->DoModal())
	{
		strPath = pFileDlg->GetPathName();
		memset(m_aszMkFileName, 0, sizeof(TCHAR) * 1024);
		_tcscpy(m_aszMkFileName, strPath.GetBuffer(0));
		m_CEditPath.SetWindowTextW(strPath);

		memset(m_stUpdateInfo.aszUpgdFileName, 0, 1024);
		_tcscpy(m_stUpdateInfo.aszUpgdFileName, strPath.GetBuffer(0));
	}
}

// 업데이트 패키지 생성
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
		// String을 int로 받기
		aiVersion[0] = this->GetDlgItemInt(IDC_EDIT_VERSION1);
		aiVersion[1] = this->GetDlgItemInt(IDC_EDIT_VERSION2);
		aiVersion[2] = this->GetDlgItemInt(IDC_EDIT_VERSION3);
		aiVersion[3] = this->GetDlgItemInt(IDC_EDIT_VERSION4);	
		
		uiVersion = (aiVersion[0] << 24) | (aiVersion[1] << 16) | (aiVersion[2] << 8) | aiVersion[3];

		// 버전 저장
		m_pObjFwUp->SetUpdateVersion(uiVersion);

		// 저장할 패키지 사이즈 얻어오기
		//pFile = m_pObjFwUp->GetMakeUpdate(&uiFileSize);
		
		// FileCtrl클래스 할당
		if (NULL == pFileCtrl)
		{
			// 파일 만드는 클래스 할당
			pFileCtrl = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(m_aszMkFileName, TRUE, TRUE);
		}

		if (TRUE == pFileCtrl->IsOpened())
		{
			iWorteSize = 0;

			// GEtMakeUpdate에서 받아온 파일 사이즈 담기
			iWriteSize = uiFileSize;
			
			while (TRUE)
			{
				// Write함수에 파일 포인터, 파일 사이즈로 결과값 담기
				iResult = pFileCtrl->Write(pFile, iWriteSize);
				iWorteSize = iWorteSize + iResult;
				iWriteSize = iWriteSize + iResult;

				// 파일 사이즈랑 같다면
				if (iWorteSize == uiFileSize)
				{
					// 결과 TRUE
					bIsSuccess = TRUE;

					// init파일 만드는 함수호출
					InitMakeFile();
					break;
				}
			}
		}
		else
		{
			MessageBox(_T("File Open Error"), _T("Error"), MB_OK | MB_ICONWARNING);
		}
		// 메모리 해제
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

// init파일 만드는 함수
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
	
	// init파일로 만들기
	strFile.Format(_T("%s\\MkUpdate.init"), aszPath);

	// 경로 + 파일 복사
	_tcscpy(aszPath, strFile);
	
	// 경로에 내 파일 생성
	pFileCtrl = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(aszPath, TRUE, TRUE);
	
	if (pFileCtrl)
	{
		if (pFileCtrl->IsOpened())
		{	// 파일 생성
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

// Cancel 버튼
void CUpdateManagerDlg::OnClickedButtonMainCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PostMessage(WM_CLOSE);
}


void CUpdateManagerDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::OnOK();
}


// 모델 불러오기 버튼
void CUpdateManagerDlg::OnClickedButtonModelLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog*	pFileDlg = NULL;
	CString			strPath;
	TCHAR			aszLdPathFile[2048] = { 0 };
	Cls_GrFileCtrl* pObjFile			= NULL;
	int				iFileSize			= 0;
	BOOL			bIsSameModel		= FALSE;
	
	if (NULL == pFileDlg)
	{
		pFileDlg = (CFileDialog*)new CFileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, _T("Init Files(*.init)|*.init"), NULL);
	}
	
	pFileDlg->m_ofn.lpstrInitialDir = m_szaNowPath;

	if (IDOK == pFileDlg->DoModal())
	{
		strPath = pFileDlg->GetPathName();
		_tcscpy(aszLdPathFile, strPath);
	}
	
	// 불러온 파일이 또 있는지 체크
	bIsSameModel = CheckInit(&m_stUpdateInfo);

	// MkUpdate.init 파일 있는지 체크
	if (FALSE == bIsSameModel)
	{
		if (GrFileIsExist(aszLdPathFile))
		{
			pObjFile = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(aszLdPathFile, FALSE, FALSE);

			if (pObjFile->IsOpened())
			{
				// 파일 사이즈 가져오기
				iFileSize = (int)pObjFile->GetSize();
				if (iFileSize = sizeof(_stUpdateInfo))
				{
					pObjFile->Read(&m_stUpdateInfo, iFileSize);
				}
			}

			// UpdateInfo 체크
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
}


//LRESULT CUpdateManagerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
