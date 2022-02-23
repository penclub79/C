
// MFC9_Interface_DockingPaneExView.cpp : CMFC9_Interface_DockingPaneExView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC9_Interface_DockingPaneEx.h"
#endif

#include "MFC9_Interface_DockingPaneExDoc.h"
#include "MFC9_Interface_DockingPaneExView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC9_Interface_DockingPaneExView

IMPLEMENT_DYNCREATE(CMFC9_Interface_DockingPaneExView, CView)

BEGIN_MESSAGE_MAP(CMFC9_Interface_DockingPaneExView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC9_Interface_DockingPaneExView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFC9_Interface_DockingPaneExView ����/�Ҹ�

CMFC9_Interface_DockingPaneExView::CMFC9_Interface_DockingPaneExView()
: m_strOutput(_T(""))
, m_nTextWidth(0)
, m_nTextHeight(0)
, m_ptPosition(0)
, m_bFont(false)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC9_Interface_DockingPaneExView::~CMFC9_Interface_DockingPaneExView()
{
}

BOOL CMFC9_Interface_DockingPaneExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC9_Interface_DockingPaneExView �׸���

void CMFC9_Interface_DockingPaneExView::OnDraw(CDC* pDC)
{
	CMFC9_Interface_DockingPaneExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	// ���� �۲� ������ �����Ѵ�.

	CFont *oldFont, *currentFont, newFont;

	if (m_bFont == TRUE)		// ��Ʈ ��ȭ���ڿ��� ������ �۲��� ���� �۲÷� ����
	{
		newFont.CreateFontIndirect(&m_logFont);
		oldFont = pDC->SelectObject(&newFont);
	}
	else						// ��Ʈ ��ȭ���ڿ��� �۲��� �������� ������ ���� �۲��� ���
	{
		currentFont = pDC->GetCurrentFont();
		oldFont = pDC->SelectObject(currentFont);
	}

	// ���� ��ȭ���ڿ��� ������ ������ �ؽ�Ʈ �������� ����
	pDC->SetTextColor(m_colorText);

	// ������ ��ġ�� ���ڿ� ���
	pDC->TextOut(m_ptPosition.x, m_ptPosition.y, m_strOutput);

	// ��µǴ� ���ڿ��� �ʺ�� ���̸� ����
	CSize sizeText;
	sizeText = pDC->GetTextExtent(m_strOutput);
	m_nTextWidth = sizeText.cx;
	m_nTextHeight = sizeText.cy;

	// �۲��� ����
	pDC->SelectObject(oldFont);
}


// CMFC9_Interface_DockingPaneExView �μ�


void CMFC9_Interface_DockingPaneExView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC9_Interface_DockingPaneExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC9_Interface_DockingPaneExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC9_Interface_DockingPaneExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CMFC9_Interface_DockingPaneExView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC9_Interface_DockingPaneExView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC9_Interface_DockingPaneExView ����

#ifdef _DEBUG
void CMFC9_Interface_DockingPaneExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC9_Interface_DockingPaneExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC9_Interface_DockingPaneExDoc* CMFC9_Interface_DockingPaneExView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC9_Interface_DockingPaneExDoc)));
	return (CMFC9_Interface_DockingPaneExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC9_Interface_DockingPaneExView �޽��� ó����


void CMFC9_Interface_DockingPaneExView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nChar)
	{
		case VK_LEFT:
			m_ptPosition.x -= 10;
			break;
		case VK_RIGHT:
			m_ptPosition.x += 10;
			break;
		case VK_UP:
			m_ptPosition.y -= 10;
			break;
		case VK_DOWN:
			m_ptPosition.y += 10;
			break;
	}
	if (m_ptPosition.x < 0)
	{
		m_ptPosition.x = 0;
		AfxMessageBox(_T("�������� �� �̻� �̵��� �� �����ϴ�."));
	}
	if (m_ptPosition.y < 0)
	{
		m_ptPosition.y = 0;
		AfxMessageBox(_T("�������� �� �̻� �̵��� �� �����ϴ�."));
	}
	RECT rect;
	GetClientRect(&rect);
	if (m_ptPosition.x > rect.right -m_nTextWidth)
	{
		m_ptPosition.x = rect.right - m_nTextWidth;
		AfxMessageBox(_T("���������� �� �̻� �̵��� �� �����ϴ�."));
	}
	if (m_ptPosition.y > rect.bottom -m_nTextHeight)
	{
		m_ptPosition.y = rect.bottom - m_nTextHeight;
		AfxMessageBox(_T("�Ʒ������� �� �̻� �̵��� �� �����ϴ�."));
	}

	// ����� ���ڿ��� ��ǥ ���� ��ŷ ���� X, Y Edit Control�� �ݿ��Ѵ�.
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->m_paneTextOut.m_ctrlTextOut.SetDlgItemInt(IDC_EDIT_X, m_ptPosition.x);
	pFrame->m_paneTextOut.m_ctrlTextOut.SetDlgItemInt(IDC_EDIT_Y, m_ptPosition.y);
	Invalidate();		// ȭ�� ����
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
