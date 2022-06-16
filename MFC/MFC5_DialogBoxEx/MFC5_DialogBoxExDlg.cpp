
// MFC5_DialogBoxExDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFC5_DialogBoxEx.h"
#include "MFC5_DialogBoxExDlg.h"
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


// CMFC5_DialogBoxExDlg 대화 상자



CMFC5_DialogBoxExDlg::CMFC5_DialogBoxExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC5_DialogBoxExDlg::IDD, pParent)
	, m_strEdit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 변수 초기화
	m_bChecked[0] = m_bChecked[1] = FALSE;
}

void CMFC5_DialogBoxExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STRING, m_strEdit);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_listbox);
	DDX_Control(pDX, IDC_COMBO_AUTO, m_cbListItem);
}

BEGIN_MESSAGE_MAP(CMFC5_DialogBoxExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_RADIO1, &CMFC5_DialogBoxExDlg::OnRadio1)
	ON_COMMAND(IDC_RADIO2, &CMFC5_DialogBoxExDlg::OnRadio2)
	ON_BN_CLICKED(IDC_CHECK1, &CMFC5_DialogBoxExDlg::OnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMFC5_DialogBoxExDlg::OnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMFC5_DialogBoxExDlg::OnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CMFC5_DialogBoxExDlg::OnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMFC5_DialogBoxExDlg::OnClickedButtonDelete)
END_MESSAGE_MAP()


// CMFC5_DialogBoxExDlg 메시지 처리기

BOOL CMFC5_DialogBoxExDlg::OnInitDialog()
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFC5_DialogBoxExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC5_DialogBoxExDlg::OnPaint()
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
HCURSOR CMFC5_DialogBoxExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFC5_DialogBoxExDlg::OnRadio1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_listbox.AddString(_T("1번 라디오 버튼 선택"));
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnRadio2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_listbox.AddString(_T("2번 라디오 버튼 선택"));
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bChecked[0] == FALSE)
	{
		m_bChecked[0] = TRUE;
		m_listbox.AddString(_T("1번 체크 박스 상태 TRUE"));
	}
	else
	{
		m_bChecked[0] = FALSE;
		m_listbox.AddString(_T("1번 체크 박스 상태 FALSE"));
	}
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnClickedCheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bChecked[1] == FALSE)
	{
		m_bChecked[1] = TRUE;
		m_listbox.AddString(_T("2번 체크 박스 상태 TRUE"));
	}
	else
	{
		m_bChecked[1] = FALSE;
		m_listbox.AddString(_T("2번 체크 박스 상태 FALSE"));
	}
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_strEdit.IsEmpty() == FALSE)
	{
		m_listbox.AddString(m_strEdit);
		m_strEdit.Empty();
	}
	else
	{
		AfxMessageBox(_T("에디트 상자에 문자열이 없습니다."));
	}
	UpdateData(FALSE);
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnClickedButtonInsert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strSelText;
	int index = m_cbListItem.GetCurSel();
	if (index != CB_ERR)
	{
		m_listbox.GetText(index, strSelText);
		m_listbox.AddString(strSelText);
		UpdateComboBox();
	}
	else
	{
		AfxMessageBox(_T("콤보 박스에서 삽입할 아이템을 선택하세요."));
	}
}


void CMFC5_DialogBoxExDlg::OnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_cbListItem.GetCurSel();
	if (index != CB_ERR)
	{
		m_listbox.DeleteString(index);
		UpdateComboBox();
	}
	else
	{
		AfxMessageBox(_T("콤보 박스에서 삭제할 아이템을 선택하세요."));
	}
}

void CMFC5_DialogBoxExDlg::UpdateComboBox()
{
	int nCnt = m_listbox.GetCount();
	m_cbListItem.ResetContent();

	for (int i = 0; i < nCnt; i++)
	{
		CString strItem;
		strItem.Format(_T("리스트 항목 : %d"), i + 1);
		m_cbListItem.AddString(strItem);
	}
}