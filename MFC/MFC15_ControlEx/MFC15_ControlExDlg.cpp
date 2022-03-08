
// MFC15_ControlExDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFC15_ControlEx.h"
#include "MFC15_ControlExDlg.h"
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


// CMFC15_ControlExDlg 대화 상자



CMFC15_ControlExDlg::CMFC15_ControlExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC15_ControlExDlg::IDD, pParent)
	, m_nSize(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC15_ControlExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SELECTION, m_tabSelection);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_nSize);
	DDX_Control(pDX, IDC_SPIN_SIZE, m_spinSize);
}

BEGIN_MESSAGE_MAP(CMFC15_ControlExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SELECTION, &CMFC15_ControlExDlg::OnSelchangeTabSelection)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SIZE, &CMFC15_ControlExDlg::OnDeltaposSpinSize)
END_MESSAGE_MAP()


// CMFC15_ControlExDlg 메시지 처리기

BOOL CMFC15_ControlExDlg::OnInitDialog()
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
	m_nSize = 50;
	m_spinSize.SetRange(0, 100);

	m_tabSelection.DeleteAllItems();
	m_tabSelection.InsertItem(0, _T("도형 선택"));
	m_tabSelection.InsertItem(1, _T("색상 선택"));

	CRect rect;
	m_dlgObject.Create(IDD_DIALOG_OBJECT, &m_tabSelection);
	m_dlgObject.GetWindowRect(&rect);
	m_dlgObject.MoveWindow(5, 25, rect.Width(), rect.Height());
	m_dlgObject.ShowWindow(SW_SHOW);

	m_dlgColor.Create(IDD_DIALOG_COLOR, &m_tabSelection);
	m_dlgColor.GetWindowRect(&rect);
	m_dlgColor.MoveWindow(5, 25, rect.Width(), rect.Height());
	m_dlgColor.ShowWindow(SW_HIDE);
	UpdateData(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFC15_ControlExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC15_ControlExDlg::OnPaint()
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

	CRect rectView, rectFigure;
	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(&rectView);
	CPoint ptCenter = rectView.CenterPoint();
	ScreenToClient(&ptCenter);

	rectFigure.left = ptCenter.x - (int)(rectView.Width() / 2.0f * (float)m_nSize / 100.0f);
	rectFigure.right = ptCenter.x + (int)(rectView.Width() / 2.0f * (float)m_nSize / 100.0f);
	rectFigure.top = ptCenter.y - (int)(rectView.Width() / 2.0f * (float)m_nSize / 100.0f);
	rectFigure.bottom = ptCenter.y + (int)(rectView.Width() / 2.0f * (float)m_nSize / 100.0f);

	CClientDC dc(this);

	CBrush NewBrush, *oldBrush;

	NewBrush.CreateSolidBrush(m_dlgColor.m_colorObject);
	oldBrush = dc.SelectObject(&NewBrush);

	switch (m_dlgObject.m_nSelObject)
	{
	case 1:
		dc.Rectangle(&rectFigure);
		break;
	case 2:
		dc.Ellipse(&rectFigure);
		break;
	}

	dc.SelectObject(oldBrush);
	NewBrush.DeleteObject();
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFC15_ControlExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC15_ControlExDlg::UpdateDrawing()
{
	CRect rectView;
	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(&rectView);
	ScreenToClient(&rectView);
	rectView.DeflateRect(1, 1, 1, 1);		// 테두리 영역을 제외함

	InvalidateRect(&rectView);
}


void CMFC15_ControlExDlg::OnSelchangeTabSelection(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nSelection = m_tabSelection.GetCurSel();

	switch (nSelection)	
	{
	case 0:
		m_dlgObject.ShowWindow(SW_SHOW);
		m_dlgColor.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_dlgObject.ShowWindow(SW_HIDE);
		m_dlgColor.ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}


void CMFC15_ControlExDlg::OnDeltaposSpinSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateDrawing();
	*pResult = 0;
}
