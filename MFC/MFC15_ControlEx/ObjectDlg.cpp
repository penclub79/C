// ObjectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC15_ControlEx.h"
#include "ObjectDlg.h"
#include "afxdialogex.h"
#include "MFC15_ControlExDlg.h"


// CObjectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectDlg, CDialogEx)

CObjectDlg::CObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CObjectDlg::IDD, pParent)
	, m_nSelObject(0)
{

}

CObjectDlg::~CObjectDlg()
{
}

void CObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjectDlg, CDialogEx)
	ON_COMMAND(IDC_RADIO_RECT, &CObjectDlg::OnRadioRect)
	ON_COMMAND(IDC_RADIO_CIRCLE, &CObjectDlg::OnRadioCircle)
END_MESSAGE_MAP()


// CObjectDlg 메시지 처리기입니다.


BOOL CObjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_nSelObject = 1;
	((CButton*)GetDlgItem(IDC_RADIO_RECT))->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjectDlg::OnRadioRect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 탭 컨트롤을 포함하고 있는 메인 대화상자의 인스턴스를 얻는다.
	CMFC15_ControlExDlg* pMainDlg = (CMFC15_ControlExDlg*)AfxGetMainWnd();

	m_nSelObject = 1;
	pMainDlg->UpdateDrawing();
}


void CObjectDlg::OnRadioCircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 탭 컨트롤을 포함하고 있는 메인 대화상자의 인스턴스를 얻는다.
	CMFC15_ControlExDlg* pMainDlg = (CMFC15_ControlExDlg*)AfxGetMainWnd();

	m_nSelObject = 2;
	pMainDlg->UpdateDrawing();
}
