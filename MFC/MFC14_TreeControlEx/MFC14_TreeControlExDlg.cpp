
// MFC14_TreeControlExDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFC14_TreeControlEx.h"
#include "MFC14_TreeControlExDlg.h"
#include "afxdialogex.h"

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


// CMFC14_TreeControlExDlg 대화 상자



CMFC14_TreeControlExDlg::CMFC14_TreeControlExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC14_TreeControlExDlg::IDD, pParent)
	, m_strNodeText(_T(""))
	, m_strSelectedNode(_T(""))
	, m_bChecked(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC14_TreeControlExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CONTROL, m_treeControl);
	DDX_Text(pDX, IDC_EDIT_NODE_TEXT, m_strNodeText);
	DDX_Text(pDX, IDC_EDIT_SELECTED_NODE, m_strSelectedNode);
}

BEGIN_MESSAGE_MAP(CMFC14_TreeControlExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CONTROL, &CMFC14_TreeControlExDlg::OnSelchangedTreeControl)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CMFC14_TreeControlExDlg::OnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CMFC14_TreeControlExDlg::OnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMFC14_TreeControlExDlg::OnClickedButtonDelete)
END_MESSAGE_MAP()


// CMFC14_TreeControlExDlg 메시지 처리기

BOOL CMFC14_TreeControlExDlg::OnInitDialog()
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
	m_hRoot = m_treeControl.InsertItem(_T("루트 노드"), 0, TVI_LAST);
	((CButton*)GetDlgItem(IDC_CHECK_EXPAND))->SetCheck(TRUE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFC14_TreeControlExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC14_TreeControlExDlg::OnPaint()
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
HCURSOR CMFC14_TreeControlExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC14_TreeControlExDlg::OnSelchangedTreeControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 현재 선택한 아이템의 핸들 값을 멤버 변수에 저장한다.
	m_hSelectedNode = pNMTreeView->itemNew.hItem;

	// 선택한 아이템의 이름을 대화상자의 Edit Control에 설정한다.
	m_strSelectedNode = m_treeControl.GetItemText(m_hSelectedNode);
	UpdateData(FALSE);		// 대화상자의 컨트롤에 데이터를 출력한다.
	*pResult = 0;
}


void CMFC14_TreeControlExDlg::OnClickedButtonInsert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	// 에러처리 - 입력할 텍스트가 비어있나 검사한다.
	if (!m_strNodeText.IsEmpty())
	{
		m_treeControl.InsertItem(m_strNodeText, m_hSelectedNode, TVI_LAST);
		m_treeControl.Expand(m_hSelectedNode, TVE_EXPAND);
	}
	else
	{
		AfxMessageBox(_T("입력 노드의 텍스트를 입력하세요."));
	}
	// Edit Box의 텍스트를 비운다.
	m_strNodeText.Empty();
	UpdateData(FALSE);
}


void CMFC14_TreeControlExDlg::OnClickedButtonModify()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	// 입력할 텍스트가 비어 있는지 검사
	if (!m_strNodeText.IsEmpty())
	{
		if (m_hSelectedNode != m_hRoot)
		{
			// 선택된 아이템의 텍스트를 수정한다.
			m_treeControl.SetItemText(m_hSelectedNode, m_strNodeText);
			
			// 현재 선택된 아이템의 이름을 표현하는 Edit Control의 내용도 수정해 준다.
			m_strSelectedNode = m_strNodeText;
		}
		else
		{
			AfxMessageBox(_T("루트 노드는 수정해서는 안 됩니다."));
		}
	}
	else
	{
		AfxMessageBox(_T("수정 노드의 텍스트를 입력하세요."));
	}
	m_strNodeText.Empty();
	UpdateData(FALSE);
}


void CMFC14_TreeControlExDlg::OnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_hSelectedNode != m_hRoot)
	{
		if (MessageBox(_T("정말 삭제하시겠습니까?"), _T("삭제 경고"), MB_YESNO) == IDYES)
			m_treeControl.DeleteItem(m_hSelectedNode);
	}
	else
	{
		AfxMessageBox(_T("루트 노드는 삭제해서는 안 됩니다."));
	}
}
