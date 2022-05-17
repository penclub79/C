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
	virtual void OnOK();
	afx_msg void OnClickedButtonVersionOk();
	afx_msg void OnClickedButtonVersionCancel();
	afx_msg void OnClickedButtonFileOpen();


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_VERSION_FILE };

	void SetModelType(int _iModelType);
	void SetModelName(TCHAR* _pszModelName);
	void GetVerFileType(int* _piModelType, int* _piVerFileType, CHAR* _pszFileName, int* _piStrLen);

	BOOL m_bModalResult;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	int			m_iModelType;
	int			m_iVerFileType;
	TCHAR*		m_pszNowPath;
	CString		m_strFileName;
	CString		m_strModelName;
	CComboBox	m_CVerFileType;
	CEdit		m_CEdit;
	
public:
	CEdit m_CEditFile;
	afx_msg void OnSelchangeComboChoiseFile();
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
