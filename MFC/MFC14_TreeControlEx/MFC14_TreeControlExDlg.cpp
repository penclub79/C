
// MFC14_TreeControlExDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFC14_TreeControlEx.h"
#include "MFC14_TreeControlExDlg.h"
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


// CMFC14_TreeControlExDlg ��ȭ ����



CMFC14_TreeControlExDlg::CMFC14_TreeControlExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC14_TreeControlExDlg::IDD, pParent)
	, m_strNodeText(_T(""))
	, m_strSelectedNode(_T(""))
	, m_bChecked(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC14_TreeControlExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CONTROL, m_treeControl);
	DDX_Text(pDX, IDC_EDIT_NODE_TEXT, m_strNodeText);
	DDX_Text(pDX, IDC_EDIT_SELECTED_NODE, m_strSelectedNode);
}

BEGIN_MESSAGE_MAP(CMFC14_TreeControlExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CONTROL, &CMFC14_TreeControlExDlg::OnSelchangedTreeControl)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CMFC14_TreeControlExDlg::OnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CMFC14_TreeControlExDlg::OnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMFC14_TreeControlExDlg::OnClickedButtonDelete)
END_MESSAGE_MAP()


// CMFC14_TreeControlExDlg �޽��� ó����

BOOL CMFC14_TreeControlExDlg::OnInitDialog()
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
	m_hRoot = m_treeControl.InsertItem(_T("��Ʈ ���"), 0, TVI_LAST);
	((CButton*)GetDlgItem(IDC_CHECK_EXPAND))->SetCheck(TRUE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMFC14_TreeControlExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC14_TreeControlExDlg::OnPaint()
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
HCURSOR CMFC14_TreeControlExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC14_TreeControlExDlg::OnSelchangedTreeControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ���� ������ �������� �ڵ� ���� ��� ������ �����Ѵ�.
	m_hSelectedNode = pNMTreeView->itemNew.hItem;

	// ������ �������� �̸��� ��ȭ������ Edit Control�� �����Ѵ�.
	m_strSelectedNode = m_treeControl.GetItemText(m_hSelectedNode);
	UpdateData(FALSE);		// ��ȭ������ ��Ʈ�ѿ� �����͸� ����Ѵ�.
	*pResult = 0;
}


void CMFC14_TreeControlExDlg::OnClickedButtonInsert()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	// ����ó�� - �Է��� �ؽ�Ʈ�� ����ֳ� �˻��Ѵ�.
	if (!m_strNodeText.IsEmpty())
	{
		m_treeControl.InsertItem(m_strNodeText, m_hSelectedNode, TVI_LAST);
		m_treeControl.Expand(m_hSelectedNode, TVE_EXPAND);
	}
	else
	{
		AfxMessageBox(_T("�Է� ����� �ؽ�Ʈ�� �Է��ϼ���."));
	}
	// Edit Box�� �ؽ�Ʈ�� ����.
	m_strNodeText.Empty();
	UpdateData(FALSE);
}


void CMFC14_TreeControlExDlg::OnClickedButtonModify()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	// �Է��� �ؽ�Ʈ�� ��� �ִ��� �˻�
	if (!m_strNodeText.IsEmpty())
	{
		if (m_hSelectedNode != m_hRoot)
		{
			// ���õ� �������� �ؽ�Ʈ�� �����Ѵ�.
			m_treeControl.SetItemText(m_hSelectedNode, m_strNodeText);
			
			// ���� ���õ� �������� �̸��� ǥ���ϴ� Edit Control�� ���뵵 ������ �ش�.
			m_strSelectedNode = m_strNodeText;
		}
		else
		{
			AfxMessageBox(_T("��Ʈ ���� �����ؼ��� �� �˴ϴ�."));
		}
	}
	else
	{
		AfxMessageBox(_T("���� ����� �ؽ�Ʈ�� �Է��ϼ���."));
	}
	m_strNodeText.Empty();
	UpdateData(FALSE);
}


void CMFC14_TreeControlExDlg::OnClickedButtonDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_hSelectedNode != m_hRoot)
	{
		if (MessageBox(_T("���� �����Ͻðڽ��ϱ�?"), _T("���� ���"), MB_YESNO) == IDYES)
			m_treeControl.DeleteItem(m_hSelectedNode);
	}
	else
	{
		AfxMessageBox(_T("��Ʈ ���� �����ؼ��� �� �˴ϴ�."));
	}
}
