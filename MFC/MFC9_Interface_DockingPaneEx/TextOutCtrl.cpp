// TextOutCtrl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC9_Interface_DockingPaneEx.h"
#include "TextOutCtrl.h"
#include "afxdialogex.h"
#include "MFC9_Interface_DockingPaneExDoc.h"
#include "MFC9_Interface_DockingPaneExView.h"
#include "MainFrm.h"


// CTextOutCtrl ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTextOutCtrl, CDialogEx)

CTextOutCtrl::CTextOutCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTextOutCtrl::IDD, pParent)
{

}

CTextOutCtrl::~CTextOutCtrl()
{
}

void CTextOutCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTextOutCtrl, CDialogEx)
//	ON_BN_CLICKED(IDC_BUTTON_TEXTOUT, &CTextOutCtrl::OnBnClickedButtonTextout)
	ON_BN_CLICKED(IDC_BUTTON_TEXTOUT, &CTextOutCtrl::OnClickedButtonTextout)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CTextOutCtrl::OnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_FONT, &CTextOutCtrl::OnClickedButtonFont)
	ON_EN_CHANGE(IDC_EDIT_Y, &CTextOutCtrl::OnEnChangeEditY)
END_MESSAGE_MAP()


// CTextOutCtrl �޽��� ó�����Դϴ�.


BOOL CTextOutCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	SetDlgItemText(IDC_EDIT_COLOR, _T("000000"));
	SetDlgItemText(IDC_EDIT_FONT, _T("system"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


//void CTextOutCtrl::OnBnClickedButtonTextout()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
//	CMFC9_Interface_DockingPaneExApp* pView = (CMFC9_Interface_DockingPaneExApp*)pFrame->GetActiveView();
//
//	GetDlgItemText(IDC_EDIT_TEXT, pView->m_strOutput);
//
//	if (pView->m_strOutput.IsEmpty())
//	{
//		AfxMessageBox(_T("���� �Է��Ͻÿ�"));
//	}
//	else
//	{
//		pView->m_ptPosition.x = GetDlgItemInt(IDC_EDIT_X);
//		pView->m_ptPosition.y = GetDlgItemInt(IDC_EDIT_Y);
//	}
//	pView->Invalidate();
//}


void CTextOutCtrl::OnClickedButtonTextout()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFC9_Interface_DockingPaneExView* pView = (CMFC9_Interface_DockingPaneExView*)pFrame->GetActiveView();

	GetDlgItemText(IDC_EDIT_TEXT, pView->m_strOutput);

	if (pView->m_strOutput.IsEmpty())
	{
		AfxMessageBox(_T("���� �Է��Ͻÿ�"));
	}
	else
	{
		pView->m_ptPosition.x = GetDlgItemInt(IDC_EDIT_X);		
		pView->m_ptPosition.y = GetDlgItemInt(IDC_EDIT_Y);
	}
	pView->Invalidate();
}


void CTextOutCtrl::OnClickedButtonColor()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CColorDialog colorDlg;
	CString strColor;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFC9_Interface_DockingPaneExView* pView = (CMFC9_Interface_DockingPaneExView*)pFrame->GetActiveView();

	if (colorDlg.DoModal() == IDOK)
	{
		pView->m_colorText = colorDlg.GetColor();

		strColor.Format(_T("%06X"), pView->m_colorText);
		SetDlgItemText(IDC_EDIT_COLOR, strColor);
		pView->Invalidate();
	}
}


void CTextOutCtrl::OnClickedButtonFont()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFontDialog fontDlg;
	CString strSize;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFC9_Interface_DockingPaneExView* pView = (CMFC9_Interface_DockingPaneExView*)pFrame->GetActiveView();

	if (fontDlg.DoModal() == IDOK)
	{
		fontDlg.GetCurrentFont(&pView->m_logFont);
		SetDlgItemText(IDC_EDIT_FONT, pView->m_logFont.lfFaceName);

		pView->m_bFont = TRUE;
		pView->Invalidate();
	}
}


void CTextOutCtrl::OnEnChangeEditY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
