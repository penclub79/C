// EraserSizeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC11_GDIPlusEx.h"
#include "EraserSizeDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MFC11_GDIPlusExDoc.h"
#include "MFC11_GDIPlusExView.h"


// CEraserSizeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEraserSizeDlg, CDialogEx)

CEraserSizeDlg::CEraserSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEraserSizeDlg::IDD, pParent)
	, m_nEraserSize(0)
{

}

CEraserSizeDlg::~CEraserSizeDlg()
{
}

void CEraserSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ERASER, m_nEraserSize);
	DDX_Control(pDX, IDC_SPIN_ERASER, m_spinEraser);
}


BEGIN_MESSAGE_MAP(CEraserSizeDlg, CDialogEx)
END_MESSAGE_MAP()


// CEraserSizeDlg 메시지 처리기입니다.


BOOL CEraserSizeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFC11_GDIPlusExView *pView = (CMFC11_GDIPlusExView*)pFrame->GetActiveView();

	m_spinEraser.SetRange(2, 20);
	m_nEraserSize = pView->m_nEraserSize;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
