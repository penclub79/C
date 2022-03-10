
// MFC18_QuadraticEquation_DLLExDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFC18_QuadraticEquation_DLLEx.h"
#include "MFC18_QuadraticEquation_DLLExDlg.h"
#include "afxdialogex.h"
#include "CalculateRoot.h"

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


// CMFC18_QuadraticEquation_DLLExDlg ��ȭ ����



CMFC18_QuadraticEquation_DLLExDlg::CMFC18_QuadraticEquation_DLLExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC18_QuadraticEquation_DLLExDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC18_QuadraticEquation_DLLExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC18_QuadraticEquation_DLLExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CALC_ROOT, &CMFC18_QuadraticEquation_DLLExDlg::OnClickedButtonCalcRoot)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMFC18_QuadraticEquation_DLLExDlg::OnClickedButtonClear)
END_MESSAGE_MAP()


// CMFC18_QuadraticEquation_DLLExDlg �޽��� ó����

BOOL CMFC18_QuadraticEquation_DLLExDlg::OnInitDialog()
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMFC18_QuadraticEquation_DLLExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC18_QuadraticEquation_DLLExDlg::OnPaint()
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
HCURSOR CMFC18_QuadraticEquation_DLLExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC18_QuadraticEquation_DLLExDlg::OnClickedButtonCalcRoot()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strTemp;
	double fA, fB, fC, fDet;
	GetDlgItemText(IDC_EDIT_A, strTemp);
	fA = _wtof(strTemp);
	if (fA == 0)
	{
		AfxMessageBox(_T("ù ��° ����� 0�� �� �� �����ϴ�."));
		SetDlgItemText(IDC_EDIT_A, _T(""));
		SetDlgItemText(IDC_EDIT_B, _T(""));
		SetDlgItemText(IDC_EDIT_C, _T(""));
	}
	else
	{
		GetDlgItemText(IDC_EDIT_B, strTemp);
		fB = _wtof(strTemp);
		GetDlgItemText(IDC_EDIT_C, strTemp);
		fC = _wtof(strTemp);
		int nDet = m_calcRoot.Determinant(fA, fB, fC, &fDet);
		strTemp.Format(_T("%g"), fDet);
		switch (nDet)
		{
		case 0:
			SetDlgItemText(IDC_EDIT_RESULT, _T("���� ����"));
			SetDlgItemText(IDC_EDIT_DETERMINANT, strTemp);
			SetDlgItemText(IDC_EDIT_ROOT1, _T(""));
			SetDlgItemText(IDC_EDIT_ROOT2, _T(""));
			break;
		case 1:
			SetDlgItemText(IDC_EDIT_RESULT, _T("�߱�"));
			SetDlgItemText(IDC_EDIT_DETERMINANT, strTemp);
			strTemp.Format(_T("%g"), m_calcRoot.CalcRoot1(fA, fB, fC));
			SetDlgItemText(IDC_EDIT_ROOT1, strTemp);
			strTemp.Format(_T("%g"), m_calcRoot.CalcRoot2(fA, fB, fC));
			SetDlgItemText(IDC_EDIT_ROOT2, strTemp);
			break;
		case 2:
			SetDlgItemText(IDC_EDIT_RESULT, _T("2���� ���� ����"));
			SetDlgItemText(IDC_EDIT_DETERMINANT, strTemp);
			strTemp.Format(_T("%g"), m_calcRoot.CalcRoot1(fA, fB, fC));
			SetDlgItemText(IDC_EDIT_ROOT1, strTemp);
			strTemp.Format(_T("%g"), m_calcRoot.CalcRoot2(fA, fB, fC));
			SetDlgItemText(IDC_EDIT_ROOT2, strTemp);
			break;
		}
	}
}


void CMFC18_QuadraticEquation_DLLExDlg::OnClickedButtonClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetDlgItemText(IDC_EDIT_A, _T(""));
	SetDlgItemText(IDC_EDIT_B, _T(""));
	SetDlgItemText(IDC_EDIT_C, _T(""));
	SetDlgItemText(IDC_EDIT_RESULT, _T(""));
	SetDlgItemText(IDC_EDIT_DETERMINANT, _T(""));
	SetDlgItemText(IDC_EDIT_ROOT1, _T(""));
	SetDlgItemText(IDC_EDIT_ROOT2, _T(""));
}
