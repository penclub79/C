// ObjectDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC15_ControlEx.h"
#include "ObjectDlg.h"
#include "afxdialogex.h"
#include "MFC15_ControlExDlg.h"


// CObjectDlg ��ȭ �����Դϴ�.

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


// CObjectDlg �޽��� ó�����Դϴ�.


BOOL CObjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_nSelObject = 1;
	((CButton*)GetDlgItem(IDC_RADIO_RECT))->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CObjectDlg::OnRadioRect()
{
	// TODO: ���⿡ ���� ó���� �ڵ带 �߰��մϴ�.
	// �� ��Ʈ���� �����ϰ� �ִ� ���� ��ȭ������ �ν��Ͻ��� ��´�.
	CMFC15_ControlExDlg* pMainDlg = (CMFC15_ControlExDlg*)AfxGetMainWnd();

	m_nSelObject = 1;
	pMainDlg->UpdateDrawing();
}


void CObjectDlg::OnRadioCircle()
{
	// TODO: ���⿡ ���� ó���� �ڵ带 �߰��մϴ�.
	// �� ��Ʈ���� �����ϰ� �ִ� ���� ��ȭ������ �ν��Ͻ��� ��´�.
	CMFC15_ControlExDlg* pMainDlg = (CMFC15_ControlExDlg*)AfxGetMainWnd();

	m_nSelObject = 2;
	pMainDlg->UpdateDrawing();
}