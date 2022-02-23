#pragma once
#include "TextOutCtrl.h"


// CTextOutPane

class CTextOutPane : public CDockablePane
{
	DECLARE_DYNAMIC(CTextOutPane)

public:
	CTextOutPane();
	virtual ~CTextOutPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CTextOutCtrl m_ctrlTextOut;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


