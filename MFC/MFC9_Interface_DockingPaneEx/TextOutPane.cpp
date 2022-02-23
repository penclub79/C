// TextOutPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC9_Interface_DockingPaneEx.h"
#include "TextOutPane.h"


// CTextOutPane

IMPLEMENT_DYNAMIC(CTextOutPane, CDockablePane)

CTextOutPane::CTextOutPane()
{

}

CTextOutPane::~CTextOutPane()
{
}


BEGIN_MESSAGE_MAP(CTextOutPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CTextOutPane 메시지 처리기입니다.




int CTextOutPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	if (!m_ctrlTextOut.Create(IDD_DIALOG_TEXTOUT, this))
	{
		TRACE0("문자열 출력 윈도우를 만들지 못했습니다.\n");
		return -1;
	}
	m_ctrlTextOut.ShowWindow(SW_SHOW);

	return 0;
}


void CTextOutPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	if (m_ctrlTextOut.GetSafeHwnd())
	{
		m_ctrlTextOut.MoveWindow(0, 0, cx, cy);
		m_ctrlTextOut.SetFocus();
	}
}
