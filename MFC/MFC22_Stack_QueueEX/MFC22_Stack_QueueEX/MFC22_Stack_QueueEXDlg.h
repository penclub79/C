
// MFC22_Stack_QueueEXDlg.h : ��� ����
//

#pragma once
#include "resource.h"
#include "afxwin.h"
#include "Stack.h"
#include "Queue.h"

// CMFC22_Stack_QueueEXDlg ��ȭ ����
class CMFC22_Stack_QueueEXDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC22_Stack_QueueEXDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CMFC22_Stack_QueueEXDlg();

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
	
	void ReDrawList();

	bool m_bIsStack;	// Stack Check
	CButton* m_pstrCheckRadio;  // ���� ��ư üũ
	CStack* m_pStack;
	CQueue* m_pQueue;
//	CListBox m_strListBox;
	// ����Ʈ �ڽ�
public:
	afx_msg void OnRadioStack();
	afx_msg void OnRadioQueue();
	afx_msg void OnClickedButtonPush();

protected:
	CListBox m_ctlListBox;
public:
	afx_msg void OnClickedButtonPop();
};
