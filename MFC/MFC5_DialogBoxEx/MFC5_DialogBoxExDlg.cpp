
// MFC5_DialogBoxExDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFC5_DialogBoxEx.h"
#include "MFC5_DialogBoxExDlg.h"
#include "afxdialogex.h"

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


// CMFC5_DialogBoxExDlg ��ȭ ����



CMFC5_DialogBoxExDlg::CMFC5_DialogBoxExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC5_DialogBoxExDlg::IDD, pParent)
	, m_strEdit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// ���� �ʱ�ȭ
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


// CMFC5_DialogBoxExDlg �޽��� ó����

BOOL CMFC5_DialogBoxExDlg::OnInitDialog()
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMFC5_DialogBoxExDlg::OnPaint()
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
HCURSOR CMFC5_DialogBoxExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFC5_DialogBoxExDlg::OnRadio1()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_listbox.AddString(_T("1�� ���� ��ư ����"));
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnRadio2()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_listbox.AddString(_T("2�� ���� ��ư ����"));
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnClickedCheck1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_bChecked[0] == FALSE)
	{
		m_bChecked[0] = TRUE;
		m_listbox.AddString(_T("1�� üũ �ڽ� ���� TRUE"));
	}
	else
	{
		m_bChecked[0] = FALSE;
		m_listbox.AddString(_T("1�� üũ �ڽ� ���� FALSE"));
	}
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnClickedCheck2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_bChecked[1] == FALSE)
	{
		m_bChecked[1] = TRUE;
		m_listbox.AddString(_T("2�� üũ �ڽ� ���� TRUE"));
	}
	else
	{
		m_bChecked[1] = FALSE;
		m_listbox.AddString(_T("2�� üũ �ڽ� ���� FALSE"));
	}
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnClickedButtonAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_strEdit.IsEmpty() == FALSE)
	{
		m_listbox.AddString(m_strEdit);
		m_strEdit.Empty();
	}
	else
	{
		AfxMessageBox(_T("����Ʈ ���ڿ� ���ڿ��� �����ϴ�."));
	}
	UpdateData(FALSE);
	UpdateComboBox();
}


void CMFC5_DialogBoxExDlg::OnClickedButtonInsert()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		AfxMessageBox(_T("�޺� �ڽ����� ������ �������� �����ϼ���."));
	}
}


void CMFC5_DialogBoxExDlg::OnClickedButtonDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int index = m_cbListItem.GetCurSel();
	if (index != CB_ERR)
	{
		m_listbox.DeleteString(index);
		UpdateComboBox();
	}
	else
	{
		AfxMessageBox(_T("�޺� �ڽ����� ������ �������� �����ϼ���."));
	}
}

void CMFC5_DialogBoxExDlg::UpdateComboBox()
{
	int nCnt = m_listbox.GetCount();
	m_cbListItem.ResetContent();

	for (int i = 0; i < nCnt; i++)
	{
		CString strItem;
		strItem.Format(_T("����Ʈ �׸� : %d"), i + 1);
		m_cbListItem.AddString(strItem);
	}
}