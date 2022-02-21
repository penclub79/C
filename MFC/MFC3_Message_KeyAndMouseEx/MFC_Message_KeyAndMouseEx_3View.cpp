
// MFC_Message_KeyAndMouseEx_3View.cpp : CMFC_Message_KeyAndMouseEx_3View Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC_Message_KeyAndMouseEx_3.h"
#endif

#include "MFC_Message_KeyAndMouseEx_3Doc.h"
#include "MFC_Message_KeyAndMouseEx_3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_Message_KeyAndMouseEx_3View

IMPLEMENT_DYNCREATE(CMFC_Message_KeyAndMouseEx_3View, CView)

BEGIN_MESSAGE_MAP(CMFC_Message_KeyAndMouseEx_3View, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CMFC_Message_KeyAndMouseEx_3View ����/�Ҹ�

CMFC_Message_KeyAndMouseEx_3View::CMFC_Message_KeyAndMouseEx_3View()
: m_strOutput(_T(""))
, m_ptLocation(0)
, m_ptClientSize(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC_Message_KeyAndMouseEx_3View::~CMFC_Message_KeyAndMouseEx_3View()
{
}

BOOL CMFC_Message_KeyAndMouseEx_3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC_Message_KeyAndMouseEx_3View �׸���

void CMFC_Message_KeyAndMouseEx_3View::OnDraw(CDC* pDC)
{
	CMFC_Message_KeyAndMouseEx_3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	// �����쿡 ���ڿ� ���
	pDC->TextOutW(m_ptLocation.x, m_ptLocation.y, m_strOutput);
}


// CMFC_Message_KeyAndMouseEx_3View �μ�

BOOL CMFC_Message_KeyAndMouseEx_3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC_Message_KeyAndMouseEx_3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC_Message_KeyAndMouseEx_3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFC_Message_KeyAndMouseEx_3View ����

#ifdef _DEBUG
void CMFC_Message_KeyAndMouseEx_3View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_Message_KeyAndMouseEx_3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_Message_KeyAndMouseEx_3Doc* CMFC_Message_KeyAndMouseEx_3View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_Message_KeyAndMouseEx_3Doc)));
	return (CMFC_Message_KeyAndMouseEx_3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_Message_KeyAndMouseEx_3View �޽��� ó����


void CMFC_Message_KeyAndMouseEx_3View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	int nCharIndex;								
	nCharIndex = m_strOutput.GetLength();		// �Էµ� �������� ���̸� ��´�.

	if (nChar == VK_BACK)						// �齺���̽��� ���� ���.
		m_strOutput.Delete(nCharIndex - 1, 1);	// �� ���� �ϳ��� �����.
	else										// �齺���̽� �̿��� Ű�� ���� ���
		m_strOutput += (WCHAR)nChar;			// Ű����� �Էµ� ���ڸ� ���ڿ��� �߰�

	Invalidate();								// ȭ�� ����

	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CMFC_Message_KeyAndMouseEx_3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nChar)								// ����Ű �ڵ� ���� ����
	{
	case VK_LEFT:								// ���� ȭ��ǥ Ű�� ���� ��
		m_ptLocation.x--;						// �������� 1 �̵�
		break;
	case VK_RIGHT:								// ������ ȭ��ǥ Ű�� ���� ��
		m_ptLocation.x++;						// ���������� 1 �̵�
		break;
	case VK_UP:									// ���� ȭ��ǥ Ű�� ���� ��
		m_ptLocation.y--;						// �������� 1 �̵�
		break;
	case VK_DOWN:								// �Ʒ��� ȭ��ǥ Ű�� ���� ��
		m_ptLocation.y++;						// �Ʒ������� 1 �̵�
		break;
	case VK_PRIOR:								// PageUpŰ�� ���� ��
		m_ptLocation.y -= 50;					// �������� 50 �̵�
		break;
	case VK_NEXT:								// PageDownŰ�� ���� ��
		m_ptLocation.y += 50;					// �Ʒ������� 50 �̵�
		break;
	case VK_HOME:								// HomeŰ�� ���� ��
		m_ptLocation = CPoint(0, 0);			// ó����ġ�� �̵�
		break;
	}

	if (m_ptLocation.x < 0)						// X��ǥ�� 0���� ������
	{
		m_ptLocation.x = 0;						// m_ptLocation.x = 0���� �ʱ�ȭ
		AfxMessageBox(_T("�������� �� �̻� �̵��� �� �����ϴ�."));
	}
	if (m_ptLocation.y < 0)						// Y��ǥ�� 0���� ������
	{
		m_ptLocation.y = 0;						// m_ptLocation.y = 0���� �ʱ�ȭ
		AfxMessageBox(_T("�������� �� �̻� �̵��� �� �����ϴ�."));
	}
	if (m_ptLocation.x > m_ptClientSize.x)		// x��ǥ�� ������ ������ X ������ ũ��
	{
		m_ptLocation.x = m_ptClientSize.x;		// m_ptLocation.x = ������ Xũ��� �ʱ�ȭ
		AfxMessageBox(_T("���������� �� �̻� �̵��� �� �����ϴ�."));
	}
	if (m_ptLocation.y > m_ptClientSize.y)		// y��ǥ�� ������ ������ Y ������ ũ��
	{
		m_ptLocation.y = m_ptClientSize.y;		// m_ptLocation.y = ������ Yũ��� �ʱ�ȭ
		AfxMessageBox(_T("�Ʒ������� �� �̻� �̵��� �� �����ϴ�."));
	}
	Invalidate();								// ȭ�� ����
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMFC_Message_KeyAndMouseEx_3View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_ptClientSize.x = cx;		// ������ ������ ����(x) ���̸� ����
	m_ptClientSize.y = cy;		// ������ ������ ����(y) ���̸� ����

	Invalidate();				// ȭ�� ����
}


void CMFC_Message_KeyAndMouseEx_3View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_ptLocation.x = point.x;
	m_ptLocation.y = point.y;
	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}


void CMFC_Message_KeyAndMouseEx_3View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_strOutput.IsEmpty() == false)
	{
		if (AfxMessageBox(_T("���ڸ� ��� ����ðڽ��ϱ�?"),
			MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			m_strOutput.Empty();
		}
	}
	else
	{
		AfxMessageBox(_T("���ڿ��� �Է��Ͻÿ�"));
	}
	Invalidate();
	CView::OnRButtonDown(nFlags, point);
}
