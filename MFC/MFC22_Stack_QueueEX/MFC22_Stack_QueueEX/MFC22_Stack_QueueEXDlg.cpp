
// MFC22_Stack_QueueEXDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFC22_Stack_QueueEX.h"
#include "MFC22_Stack_QueueEXDlg.h"
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


// CMFC22_Stack_QueueEXDlg ��ȭ ����



CMFC22_Stack_QueueEXDlg::CMFC22_Stack_QueueEXDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC22_Stack_QueueEXDlg::IDD, pParent)
	, m_pstrCheckRadio(NULL)
	, m_bIsStack(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pStack = NULL;
}

CMFC22_Stack_QueueEXDlg::~CMFC22_Stack_QueueEXDlg()
{
	if (NULL != m_pStack)
	{
		delete m_pStack;
		m_pStack = NULL;
	}
}

void CMFC22_Stack_QueueEXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_ctlListBox);
}

BEGIN_MESSAGE_MAP(CMFC22_Stack_QueueEXDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_RADIO_STACK, &CMFC22_Stack_QueueEXDlg::OnRadioStack)
	ON_COMMAND(IDC_RADIO_QUEUE, &CMFC22_Stack_QueueEXDlg::OnRadioQueue)
	ON_BN_CLICKED(IDC_BUTTON_PUSH, &CMFC22_Stack_QueueEXDlg::OnClickedButtonPush)
	ON_BN_CLICKED(IDC_BUTTON_POP, &CMFC22_Stack_QueueEXDlg::OnClickedButtonPop)
END_MESSAGE_MAP()


// CMFC22_Stack_QueueEXDlg �޽��� ó����

BOOL CMFC22_Stack_QueueEXDlg::OnInitDialog()
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
	m_pstrCheckRadio = (CButton*)GetDlgItem(IDC_RADIO_STACK);
	m_pstrCheckRadio->SetCheck(TRUE);
	m_bIsStack	= TRUE;

	m_pStack	= new CStack(10);


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMFC22_Stack_QueueEXDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC22_Stack_QueueEXDlg::OnPaint()
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
HCURSOR CMFC22_Stack_QueueEXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// Stack ���� ��ư Ȱ��ȭ
void CMFC22_Stack_QueueEXDlg::OnRadioStack()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_pstrCheckRadio = (CButton*)GetDlgItem(IDC_RADIO_STACK);
	m_pstrCheckRadio->SetCheck(TRUE);
	m_bIsStack = TRUE;

	// Stack ���� Queue ����

}

// Queue ���� ��ư Ȱ��ȭ
void CMFC22_Stack_QueueEXDlg::OnRadioQueue()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_pstrCheckRadio = (CButton*)GetDlgItem(IDC_RADIO_QUEUE);
	m_pstrCheckRadio->SetCheck(TRUE);
	m_bIsStack = FALSE;

	// Queue ���� Stack ����
}

// �� �Է�
void CMFC22_Stack_QueueEXDlg::OnClickedButtonPush()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strValue = _T("");
	int iValue = 0;
	int iIndex = 0;
	int* piStackValue = NULL;
	GetDlgItemText(IDC_EDIT_INPUT, strValue);
	iValue = _ttoi(strValue);


	if (TRUE == m_bIsStack)
	{
		if (NULL != m_pStack)
		{
			if (iValue > 0)
			{
				m_pStack->Push(iValue);
				ReDrawList();
			}
			
			// list�ڽ��� clear ��Ű�� list�ڽ��� ��


			/*int iIndex = m_pStack->GetCount();
			strValue.Format(_T("��:%d, Index:%d"), iValue, iIndex);
			m_strListBox.InsertString(-1, strValue);*/
			
		}
	}
	else
	{
		//strQueue->Push();
	}
	
	/*
	// �޸� ����
	if (NULL != pstrStack)
	{
		delete pstrStack;
		pstrStack = NULL;
	}
	if (NULL != pstrQueue)
	{
		delete pstrQueue;
		pstrQueue = NULL;
	}
	
	*/
}

void CMFC22_Stack_QueueEXDlg::ReDrawList()
{
	m_ctlListBox.ResetContent();

	int iCount = m_pStack->GetCount();
	CString strValue;
	for (int i = 0; i < iCount; i++)
	{
		strValue.Format(_T("Index:%d, ��:%d"), i, m_pStack->GetAt(i));
		m_ctlListBox.InsertString(i, strValue);
	}
}

void CMFC22_Stack_QueueEXDlg::OnClickedButtonPop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	if (TRUE == m_bIsStack)
	{
		if (NULL != m_pStack)
		{
			int iResult = m_pStack->Pop();
			if (0 < iResult)
			{
				// TRUE
			}
			ReDrawList();
		}
	}

}
