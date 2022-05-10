
// UpdateManagerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "UpdateManager.h"
#include "UpdateManagerDlg.h"
#include "FirmUpdate.h"
#include "DlgModelAdd.h"

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
}

BEGIN_MESSAGE_MAP(CUpdateManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON_SELETEMODEL, &CUpdateManagerDlg::OnBnClickedButtonSeletemodel)
	//ON_BN_CLICKED(IDC_BUTTON_SELETEMODEL2, &CUpdateManagerDlg::OnBnClickedButtonSeletemodel2)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_CREATE, &CUpdateManagerDlg::OnClickedButtonModelCreate)
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
		WCHAR szaInitFile[2048] = { 0 };
		Cls_GrFileCtrl* pObjFile = NULL;
		//Cls_GrFileCtrl* pObjFile2 = NULL;
		int iFileSize = 0;
	// ------------------------------------

	// ���� ���� ��� ���
	::GetModuleFileName(NULL, m_szaNowPath, 2048);
	// ��ο��� ���ϸ�� Ȯ���ڸ� ����
	PathRemoveFileSpec(m_szaNowPath);

	m_pObjFwUp = (CFirmUpdate*)new CFirmUpdate();
	m_pObjFwUp->FirmInit();

	memset(m_staTreeNode, 0, sizeof(_stUpdateTreeNode) * 3); // 72
	memset(&m_stUpdateInfo, 0, sizeof(_stUpdateInfo));

	memcpy(&szaInitFile, &m_szaNowPath, 2048);
	GrStrWcat(szaInitFile, _T("\\MkUpdate.init"));

	// MkUpdate.init ���� �ִ��� üũ
	if (GrFileIsExist(szaInitFile))
	{
		pObjFile = (Cls_GrFileCtrl*)new Cls_GrFileCtrl(szaInitFile, FALSE, FALSE);
		//pObjFile2 = new Cls_GrFileCtrl(szaInitFile, FALSE, FALSE); test
	}
}

//void CUpdateManagerDlg::AddModel(unsigned int _uiModelType)
//{
//	// Local ------------------------------
//		int iResult = 0;
//
//	// ------------------------------------
//
//		if (NULL != m_pObjFwUp)
//		{
//			
//		}
//}


// �� ���� ���̾�α� ��ư 
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

void CUpdateManagerDlg::TreeAddModel(int _iModelType)
{
	// Local ------------------------------
	int iModelType	= _iModelType;
	int iResult		= 0;
	// ------------------------------------

	if (NULL != m_pObjFwUp)
	{
		iResult = m_pObjFwUp->AddModelType(iModelType);
	}

}
