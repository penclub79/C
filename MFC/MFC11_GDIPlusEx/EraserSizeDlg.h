#pragma once
#include "afxcmn.h"


// CEraserSizeDlg ��ȭ �����Դϴ�.

class CEraserSizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEraserSizeDlg)

public:
	CEraserSizeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEraserSizeDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ERASER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nEraserSize;
	CSpinButtonCtrl m_spinEraser;
	virtual BOOL OnInitDialog();
};