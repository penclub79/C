#pragma once
#include <GrTypeBase.h>
#include "afxwin.h"

// DlgVerFileAdd ��ȭ �����Դϴ�.

class DlgVerFileAdd : public CDialogEx
{
	DECLARE_DYNAMIC(DlgVerFileAdd)

public:
	DlgVerFileAdd(CWnd* pParent, TCHAR* _pszNowPath);   // ǥ�� �������Դϴ�.
	virtual ~DlgVerFileAdd();
	virtual BOOL OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_VERSION_FILE };

	void SetModelType(int _iModelType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

private:
	int		m_iModelType;
	int		m_iEntityType;
	TCHAR*	m_pszNowPath;
	CString m_strFileName;

};
