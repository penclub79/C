
// MFC22_Stack_QueueEXDlg.h : ��� ����
//

#pragma once


// CMFC22_Stack_QueueEXDlg ��ȭ ����
class CMFC22_Stack_QueueEXDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC22_Stack_QueueEXDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC22_STACK_QUEUEEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CButton* m_pstrCheckRadio;  // ���� ��ư üũ


};
