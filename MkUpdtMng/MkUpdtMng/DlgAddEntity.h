#pragma once
#include <GrTypeBase.h>
#include "afxwin.h"

// DlgAddEntity dialog

class DlgAddEntity : public CDialogEx
{
	DECLARE_DYNAMIC(DlgAddEntity)

private:
	__u32		m_ModelType;
	__u32		m_EntityType;
	CString		m_StrFileName;
	TCHAR*		m_PtrStrPath;
public:
	DlgAddEntity(CWnd* pParent, TCHAR* A_PtrPath);   // standard constructor
	virtual ~DlgAddEntity();

	void SetModelType(__u32 A_ModelType);
	void GetAddEntity(__u32* A_PtrModelType, __u32* A_PtrEntityType, PCHAR A_PtrStr, __s32* A_PtrStrLen);

// Dialog Data
	enum { IDD = IDD_DLG_ENTITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_CbModelType;
	CComboBox m_CbEntityType;
	virtual BOOL OnInitDialog();
	CEdit m_EdtFile;
	afx_msg void OnBnClickedBtnSelfile();
	afx_msg void OnCbnSelchangeCbEntity();
	afx_msg void OnCbnSelchangeCbModel();
};
