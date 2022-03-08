
// MFC16_Controllers_DataSimulatorExDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CMFC16_Controllers_DataSimulatorExDlg ��ȭ ����
class CMFC16_Controllers_DataSimulatorExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC16_Controllers_DataSimulatorExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC16_CONTROLLERS_DATASIMULATOREX_DIALOG };

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
	int m_nTransmitRate;
	CIPAddressCtrl m_addrSenderIP;
	CIPAddressCtrl m_addrReceiverIP;
	CSpinButtonCtrl m_spinData;
	CProgressCtrl m_prgsTransmit;
	CEdit m_edSummary;
	CDateTimeCtrl m_timeTransmit;
	afx_msg void OnClickedButtonTransmit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nData;
};
