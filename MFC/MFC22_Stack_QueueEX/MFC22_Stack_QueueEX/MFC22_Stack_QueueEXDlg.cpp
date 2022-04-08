
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
	m_pQueue = NULL;
}

CMFC22_Stack_QueueEXDlg::~CMFC22_Stack_QueueEXDlg()
{
	if (NULL != m_pStack)
	{
		delete m_pStack;
		m_pStack = NULL;
	}

	if (NULL != m_pQueue)
	{
		delete m_pQueue;
		m_pQueue = NULL;
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

	m_pStack = new CStack(10);
	m_pQueue = new CQueue(10);


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
	m_ctlListBox.ResetContent();

	// Stack ���� Queue ����

}

// Queue ���� ��ư Ȱ��ȭ
void CMFC22_Stack_QueueEXDlg::OnRadioQueue()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_pstrCheckRadio = (CButton*)GetDlgItem(IDC_RADIO_QUEUE);
	m_pstrCheckRadio->SetCheck(TRUE);
	m_bIsStack = FALSE;
	m_ctlListBox.ResetContent();

	// Queue ���� Stack ����
}

// �� �Է�
void CMFC22_Stack_QueueEXDlg::OnClickedButtonPush()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	CString strValue;
	int iValue	= 0;
	int iIndex	= 0;
	int iItemType = LINK_ITEM_TYPE_INT;

	GetDlgItemText(IDC_EDIT_INPUT, strValue);

	// �Է� ���� Item Ÿ�� üũ
	if (TRUE == IsNumeric(strValue))
	{
		// int �϶� 
		iItemType = LINK_ITEM_TYPE_INT;
		
	}
	else
	{
		// String �϶�
		iItemType = LINK_ITEM_TYPE_STRING;
	}

	if (TRUE == m_bIsStack)										
	{
		if (NULL != m_pStack)									
		{
			if (LINK_ITEM_TYPE_INT == iItemType)
			{
				iValue = _ttoi(strValue);	// CString -> int�� ��ȯ

				if (0 < iValue)
					m_pStack->Push(iValue);
			}
			else
			{
				int iStrLen = strValue.GetLength();
				WCHAR* szBuff = NULL;

				szBuff = strValue.GetBuffer(sizeof(WCHAR) * iStrLen + 1);
				//strcpy((char*)szBuff, (char*)strValue.GetBuffer());

				m_pStack->Push((char*)szBuff, sizeof(WCHAR)*iStrLen);

				strValue.ReleaseBuffer();
			}

			ReDrawList();
		}
	}
	else
	{	
		if (NULL != m_pQueue)
		{
			if (LINK_ITEM_TYPE_INT == iItemType)
				m_pQueue->EnQueue(iValue);
			else
				//m_pQueue->EnQueue(strValue.GetBuffer(), strValue.GetLength());

			ReDrawList();
		}
	}
}


void CMFC22_Stack_QueueEXDlg::OnClickedButtonPop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iCount = 0;
	Link_Data	stLinkData = { 0 };

	BOOL bResult = FALSE;

	if (TRUE == m_bIsStack)
	{
		if (NULL != m_pStack)
		{
			iCount = m_pStack->GetCount();

			if (iCount > 0)
			{
				bResult = m_pStack->Pop(&stLinkData);

				if (TRUE == bResult)
					ReDrawList();
			}
			else
			{
				AfxMessageBox(_T("���� �����ϴ�."));
			}
		}
	}
	else
	{
		iCount = m_pQueue->GetCount();
		if (0 < iCount)
		{
			if (NULL != m_pQueue)
			{
				//m_pQueue->DeQueue(&szValue[0], &iPos);
				ReDrawList();
			}
		}
	}
}


void CMFC22_Stack_QueueEXDlg::ReDrawList()
{
	m_ctlListBox.ResetContent();

	int		iVal = 0;
	int		iCount = 0;
	BOOL	bPos = FALSE;
	CString strValue;
	Link_Data stLinkData = { 0 };

	// ����
	if (TRUE == m_bIsStack)
	{
		iCount = m_pStack->GetCount();
		
		if (0 < iCount)
		{
			for (int i = 0; i < iCount; i++)
			{
				if (TRUE == m_pStack->GetAt(i, &stLinkData))
				{
					if (LINK_ITEM_TYPE_INT == stLinkData.iItemType)
					{
						iVal = stLinkData.uBuf.iValue;
						strValue.Format(_T("�ε���:%d, ��:%d"), i, iVal);
					}
					else
					{
						CString strString;
						strString = stLinkData.uBuf.pszBuffer;
						strValue.Format(_T("�ε���:%d, ��:%s"), i, strString);
					}
					
					m_ctlListBox.InsertString(i, strValue);
				}
			}
		}
	}
	// ť
	else
	{
		iCount = m_pQueue->GetCount();
		if (0 < iCount)
		{
			/*for (int i = 0; i < iCount; i++)
			{
				strValue.Format(_T("�ε���:%d, ��:%d"), i, m_pQueue->GetAt(i));
				m_ctlListBox.InsertString(i, strValue);
			}*/
		}
	}
	
}

BOOL CMFC22_Stack_QueueEXDlg::IsNumeric(CString _strValue)
{
	// �����ڵ�� �� ���ڿ� 2����Ʈ�̴�.
	
	BOOL bResult = TRUE;
	char* pszBuffer = NULL;
	//memset(pszBuffer, 0, (sizeof(WCHAR)* iLen) + 1);

	// CT2A�� class�̸� UTF8�� �� ���ڸ� ��ȯ�Ѵ�.
	CT2A ascii(_strValue, CP_UTF8);
	pszBuffer = ascii.m_psz;
	
	int iStrSize = strlen(pszBuffer);
	int iValueASC = 0;

	for (int i = 0; i < iStrSize; i++)
	{
		iValueASC = *pszBuffer;

		// 0 ~ 9���� ASCII�ڵ� üũ
		if (48 > iValueASC || 57 < iValueASC)
		{  
			// ���ڰ� �ϳ��� ������ IsNumeric�� FALSE�̴�.
			bResult	= FALSE;
			break;
		}

		pszBuffer++;
	}

	return bResult;
}
