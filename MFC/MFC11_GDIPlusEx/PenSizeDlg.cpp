// PenSizeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC11_GDIPlusEx.h"
#include "PenSizeDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MFC11_GDIPlusExDoc.h"
#include "MFC11_GDIPlusExView.h"


// CPenSizeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPenSizeDlg, CDialogEx)

CPenSizeDlg::CPenSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPenSizeDlg::IDD, pParent)
	, m_nPenSize(0)
{

}

CPenSizeDlg::~CPenSizeDlg()
{
}

void CPenSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PEN, m_nPenSize);
	DDX_Control(pDX, IDC_SPIN_PEN, m_spinPen);
}


BEGIN_MESSAGE_MAP(CPenSizeDlg, CDialogEx)
END_MESSAGE_MAP()


// CPenSizeDlg 메시지 처리기입니다.


BOOL CPenSizeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFC11_GDIPlusExView *pView = (CMFC11_GDIPlusExView *)pFrame->GetActiveView();

	m_spinPen.SetRange(1, 10);
	m_nPenSize = pView->m_nPenSize;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
