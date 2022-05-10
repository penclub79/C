#pragma once
#include <GrTypeBase.h>
#include "afxwin.h"

// DlgVerFileAdd 대화 상자입니다.

class DlgVerFileAdd : public CDialogEx
{
	DECLARE_DYNAMIC(DlgVerFileAdd)

public:
	DlgVerFileAdd(CWnd* pParent, TCHAR* _pszNowPath);   // 표준 생성자입니다.
	virtual ~DlgVerFileAdd();
	virtual BOOL OnInitDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_VERSION_FILE };

	void SetModelType(int _iModelType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	int		m_iModelType;
	int		m_iEntityType;
	TCHAR*	m_pszNowPath;
	CString m_strFileName;

};
