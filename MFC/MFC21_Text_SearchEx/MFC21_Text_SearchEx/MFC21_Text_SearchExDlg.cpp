
// MFC21_Text_SearchExDlg.cpp : ���� ����
//
#define _CRT_SECURE_NO_WARNINGS
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
, m_strBuffer(NULL)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


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
	static TCHAR BASED_CODE szFilter[] = _T("�ؽ�Ʈ ����(*.txt) | *.txt;|�������(*.*)|*.*||");
	
	/* 
	CFileDialog 000 (
	1 param : TRUE -> ����� , FALSE -> �����
	2 param : NULL -> �⺻ Ȯ����
	3 param : NULL -> �⺻ ���ϸ�
	4 param : �÷���
	5 param : ���͸� Ȯ����
	)
	*/
	CFileDialog dlg(TRUE, _T("*.txt"), _T(""), OFN_HIDEREADONLY, szFilter);
	CString strTmp;
	if (IDOK == dlg.DoModal())
	{
		CString strPathName = dlg.GetPathName();
		///////////////////////////////////////////////////////////////////
		char Tv_StrTest[1000];

		memset(Tv_StrTest, 0, 1000);

		char* str = Tv_StrTest;
		int temp = 0;

		for (int i = 0; i < 100; i++)
		{
			*str = 100 - i;
			str++;
		}
		////////////////////////////////////////////////////////////////////
		// ���� ���� �о����
		CString strFileName = dlg.GetFileName();	// ���ϸ�
		char* stText = LPSTR(LPCTSTR(strPathName));	// ����ȯ CString -> char
		int fileLoc = 0;
		FILE* p_File = fopen("D:\\C\\MFC\\MFC21_Text_SearchEx\\MFC21_Text_SearchEx\\upload\\test.txt", "r");	// ���� ���뿭��
		//m_strBuffer = fgets(m_strBuffer, 1000, p_File);
		//char *test;

		CSearch strSearch;
		char stFindtext[] = { "ls" };
		
		//BOOL CSearch::IsStrFindGet(char* A_PtrFindStr, int A_FindStrSize, char* A_PtrTextStr, int A_TextSize)
		
		//fseek(p_File, 0, SEEK_END);  // ���� �����͸� ������ ������ �̵�
		//fileLoc = ftell(p_File);	// ���� �������� ���� ��ġ
		//m_strBuffer = (char*)malloc(fileLoc + 1);		// ���� ũ�� + 1����Ʈ(���ڿ� ������)��ŭ ���� �޸� �Ҵ�
		//memset(m_strBuffer, 0, fileLoc + 1);			// ���� ũ�� + 1����Ʈ��ŭ �޸𸮸� 0���� �ʱ�ȭ

		//strSearch.IsStrFindGet(stFindtext, sizeof(stFindtext), m_strBuffer, fileLoc);
		//fseek(p_File, 0, SEEK_SET);			// ���� �����͸� ������ ó������ �̵�
		//
		////m_str = malloc(size);

		////m_obj1 = newn fld; Ŭ���� ���
		//
		////fread(m_strBuffer, 1, fileLoc, p_File); // �޸� �����Ҵ� �� ����� �Լ�
		//
		//CString test;
		//test = (LPSTR)(LPCSTR)m_strBuffer;
		
		SetDlgItemText(IDC_EDIT_IMPORT, strPathName);
		//strFile.Open(_T("test.txt"), CFile::Read);
		//SetDlgItemText(IDC_STATIC_TEXT, test);


		
	}

	if (m_strBuffer)
	{
		free(m_strBuffer);
		m_strBuffer = NULL;
	}
	
}
