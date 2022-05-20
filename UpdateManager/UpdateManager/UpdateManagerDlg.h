
// UpdateManagerDlg.h : ��� ����
//

#pragma once

#include <GrTypeBase.h>
#include "FirmUpdate.h"
#include "afxcmn.h"
#include "afxwin.h"

#define	E_MkUpdt_IniFcc			0x55696E69

// ---------------------------------------------------- struct 
typedef struct _stUpdateTreeNode
{
	__u32					uiType;
	HTREEITEM				stNode;
	CHAR					aszNode[16];
} *pUpdateTreeNode;

typedef struct _stUpdateInfoEntity
{
	__u32					uiType;
	CHAR					aszFile[256];
} *pUpdateInfoEntity;

typedef struct _stUpdateInfoModel
{
	__u32					uiType;
	_stUpdateInfoEntity		astEntity[16];
} *pUpdateInfoModel;

typedef struct _stUpdateInfo
{
	__u32					uiFcc;			//update ini 'Uini' 0x55696E69
	__u32					uiUpgdVersion;
	_stUpdateInfoModel		astModelInfo[16];
	WCHAR					aszUpgdFileName[1024];
} *pUpdateInfo;

// ---------------------------------------------------- struct 


// CUpdateManagerDlg ��ȭ ����
class CUpdateManagerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CUpdateManagerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CUpdateManagerDlg();
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_UPDATEMANAGER_DIALOG };

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

	void Init();
	//BOOL CheckFcc(pUpdateInfo _p);	// MkUpdate.init ������ ���� �� FCCüũ
	void InitCtrl(pUpdateInfo _pstUpdateInfo);				// MkUpdate.init ������ ���� ��
	//void AddModel(unsigned int _uiModelType);

	void TreeAddModel(int _iModelType); // Ʈ���� �� �߰�
	void TreeAddVerFile(int _iModelIdx, int _iVerFileType, CHAR* _pszFilePath, int _iFileLen); // Ʈ���� �𵨿��� �������� �߰�
	void TreeAddVerFileNode(int _iModelType, int _iVerFileType, char* _pszFileName);
	void ProcErrCode(int _iResult); // ���� ó��
	void InitMakeFile(); // init���� ����� �Լ�
	BOOL CheckInit(pUpdateInfo _pstUpdateInfo);
	int FindModelIndex(int _iModelType);
	int GetModelType(int _iIndex);

private:
	WCHAR				m_szaNowPath[2048];

	CFirmUpdate*		m_pObjFwUp;
	_stUpdateTreeNode	m_astTreeNode[E_FirmUpInfoTypeMaxIdx];  // 24
	WCHAR				m_aszMkFileName[1024];
	WCHAR				m_aszMkModelName[1024];
	_stUpdateInfo		m_stUpdateInfo;

	CTreeCtrl			m_CTreeCtrl;
	CEdit				m_CEditPath;
	CEdit				m_CEditModelPath;
	CEdit				m_CEditVer1;
	CEdit				m_CEditVer2;
	CEdit				m_CEditVer3;
	CEdit				m_CEditVer4;
	CButton				m_CBtnPath;
	CButton				m_CBtnMake;
	CString		m_strPathItem;
	__u32				m_iWorteSize;
	__u32				m_iWriteSize;
	__u32				m_iResult;


	int FindTreeNode(int _iModelType);
public:
	afx_msg void OnClickedButtonModelCreate(); // �� �߰�
	afx_msg void OnClickedButtonEntitySelete(); // ���� ���� ����
	afx_msg void OnClickedButtonSavePath();
	afx_msg void OnClickedButtonPackageMake();
	virtual BOOL DestroyWindow();
	afx_msg void OnClickedButtonMainCancel();
	virtual void OnOK();
	afx_msg void OnClickedButtonModelLoad();
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickedButtonEntityDelete();
	afx_msg void OnClickedButtonModelMakes();
	afx_msg void OnClickedButtonModelSavePath();
	
};
