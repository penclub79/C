
// MFC16_Controllers_DataSimulatorExDlg.h : ��� ����
//

#pragma once


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
};
