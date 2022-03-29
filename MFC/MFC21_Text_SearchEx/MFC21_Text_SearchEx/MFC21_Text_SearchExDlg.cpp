
// MFC21_Text_SearchExDlg.cpp : ���� ����
//
//#define _CRT_SECURE_NO_WARNINGS
//#define MAX 1000


#include "stdafx.h"
#include "MFC21_Text_SearchEx.h"
#include "MFC21_Text_SearchExDlg.h"
#include "Search.h"
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
public:
	//afx_msg void OnButtonImport();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//ON_COMMAND(IDC_BUTTON_IMPORT, &CAboutDlg::OnButtonImport)
END_MESSAGE_MAP()


// CMFC21_Text_SearchExDlg ��ȭ ����



CMFC21_Text_SearchExDlg::CMFC21_Text_SearchExDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMFC21_Text_SearchExDlg::IDD, pParent)
//, m_pszBuffer(NULL)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CSearch* pSearch = new CSearch;
	St_Test stResult = { 0 };

	int* piBuffer = NULL;				// �޸��ּ� ����
	int iSum = 0;
	

	// ���� �޸� �Ҵ�
	//pszBuffer = (char*)malloc((sizeof(char))* (sizeof(iArr) / sizeof(int)));
	piBuffer = (int*)malloc(1000 * sizeof(int));
	
	// memset
	pSearch->InputValue(piBuffer, 1000, 0);

	// �������� ����
	pSearch->ValueAsc(piBuffer, 1000);

	// 3�� ��� �� ���ϱ�
	iSum = pSearch->SumResult(piBuffer, 1000);

	// ���� ���°�� Value��������
	pSearch->GetValue(piBuffer, 1000, &stResult);// &�� * ��򸮸� �ȵȴ�.

	// �޸� ����
	if (NULL != piBuffer)
	{
		free(piBuffer);
		piBuffer = NULL;
	}
	if (NULL != pSearch)
	{
		delete pSearch;
		pSearch = NULL;
	}

	/////////////////////////////////////////////////////////////////////
	//char Tv_StrTest[1000];

	//memset(Tv_StrTest, 0, 1000);

	//char* str = Tv_StrTest;
	//int temp = 0;
	//

	//for (int i = 0; i < 100; i++)
	//{
	//	temp = 100 - i;
	//	*str = temp;
	//	str++;
	//}

}

void CMFC21_Text_SearchExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC21_Text_SearchExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CMFC21_Text_SearchExDlg::OnClickedButtonImport)
END_MESSAGE_MAP()


// CMFC21_Text_SearchExDlg �޽��� ó����

BOOL CMFC21_Text_SearchExDlg::OnInitDialog()
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

void CMFC21_Text_SearchExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC21_Text_SearchExDlg::OnPaint()
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
HCURSOR CMFC21_Text_SearchExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// LoadFile ��ư
void CMFC21_Text_SearchExDlg::OnClickedButtonImport()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.



	/*CString strPathName = _T("");
	static TCHAR BASED_CODE szFilter[] = _T("|�������(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.txt"), _T("image"), OFN_HIDEREADONLY, szFilter);*/

	/*
	
	if (IDOK == dlg.DoModal())
	{
		strPathName = dlg.GetPathName();
	}
	
	
	
	*/

	
	//ConvertUnicodeToMultybyte(strPathName);
	

	// ���� ���� - �޸� �Ҵ�
	//FILE* pFile = fopen(strPathName, "r");

	// ���� ���� - �޸� ����

	// ���� ������ ���ϱ�

	// �ؽ�Ʈ�� �о�� ���� ���� �Ҵ�

	// ���� - �޸� ����
	
	// ���� �б�

}

//
//char* CMFC21_Text_SearchExDlg::ConvertUnicodeToMultybyte(CString strUnicode)
//{
//	// �����ڵ� -> ��Ƽ����Ʈ
//	int nLen = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
//	char* pMultibyte = new char[nLen];
//	memset(pMultibyte, 0x00, (nLen)*sizeof(char));
//
//	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, pMultibyte, nLen, NULL, NULL);
//
//	return pMultibyte;
//}
