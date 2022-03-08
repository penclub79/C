// ColorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC15_ControlEx.h"
#include "ColorDlg.h"
#include "afxdialogex.h"
#include "MFC15_ControlExDlg.h"


// CColorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CColorDlg, CDialogEx)

CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CColorDlg::IDD, pParent)
	, m_nRed(0)
	, m_nGreen(0)
	, m_nBlue(0)
{

}

CColorDlg::~CColorDlg()
{
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_RED, m_sliderRed);
	DDX_Control(pDX, IDC_SLIDER_GREEN, m_sliderGreen);
	DDX_Control(pDX, IDC_SLIDER_BLUE, m_sliderBlue);
	DDX_Text(pDX, IDC_EDIT_RED, m_nRed);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_nGreen);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_nBlue);
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialogEx)
	ON_WM_HSCROLL()
//	ON_COMMAND(IDC_SPIN_SIZE, &CColorDlg::OnSpinSize)
END_MESSAGE_MAP()


// CColorDlg 메시지 처리기입니다.


BOOL CColorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_colorObject = RGB(0, 0, 0);
	m_sliderRed.SetRange(0, 255);
	m_sliderGreen.SetRange(0, 255);
	m_sliderBlue.SetRange(0, 255);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CColorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 탭 컨트롤을 포함하고 있는 메인 대화사장의 인스턴스를 얻는다.
	CMFC15_ControlExDlg* pMainDlg = (CMFC15_ControlExDlg*)AfxGetMainWnd();

	int nRed = m_sliderRed.GetPos();
	int nGreen = m_sliderGreen.GetPos();
	int nBlue = m_sliderBlue.GetPos();

	if (pScrollBar->GetSafeHwnd() == m_sliderRed.m_hWnd)
		m_nRed = nRed;
	else if (pScrollBar->GetSafeHwnd() == m_sliderGreen.m_hWnd)
		m_nGreen = nGreen;
	else if (pScrollBar->GetSafeHwnd() == m_sliderBlue.m_hWnd)
		m_nBlue = nBlue;
	else
		return;

	m_colorObject = RGB(nRed, nGreen, nBlue);
	UpdateData(FALSE);
	pMainDlg->UpdateDrawing();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


//void CColorDlg::OnSpinSize()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//}
