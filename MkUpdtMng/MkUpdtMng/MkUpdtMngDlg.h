
// MkUpdtMngDlg.h : header file
//

#pragma once
#include <GrTypeBase.h>
#include "FirmUpdt.h"
#include "afxcmn.h"
#include "afxwin.h"

#define	E_MkUpdt_IniFcc			0x55696E69

typedef struct St_UpdtTreeNode
{
	__u32 Type;
	HTREEITEM Node;
	//CString		StrNode;
	CHAR			StrNode[16];
} *Ptr_UpdtTreeNode;

typedef struct St_UpdtInfoEntity
{
	__u32		Type;//entity type
	CHAR		StrFile[256];
} *Ptr_UpdtInfoEntity;

typedef struct St_UpdtInfoModel
{
	__u32		Type;//model type
	St_UpdtInfoEntity Entity[16];
} *Ptr_UpdtInfoModel;

typedef struct St_UpdtInfo
{
	__u32		Fcc;//update ini 'Uini' 0x55696E69
	__u32		UpgdVer;
	St_UpdtInfoModel ModelInfo[16];
	WCHAR		StrUpdtFileName[1024];
} *Ptr_UpdtInfo;

// CMkUpdtMngDlg dialog
class CMkUpdtMngDlg : public CDialogEx
{
private:
	Cls_FirmUpdt*  m_ObjFwUp;
	St_UpdtTreeNode m_TreeNode[E_FirmUpInfoTypeMaxIdx];

	WCHAR	m_MkFileName[1024];
	WCHAR m_NowPath[2048];

	St_UpdtInfo		m_UpdtInfo;
// Construction
public:
	CMkUpdtMngDlg(CWnd* pParent = NULL);	// standard constructor

protected:
	void	LcProcErrCode(__s32 A_ErrCode);
	void	LcAddEntity(__u32 A_ModelIdx, __u32 A_EntityType, PCHAR A_PtrStrFile, __u32 A_StrLen);
	__s32	LcFindTreeNode(__u32 A_ModelType);
	void	LcAddEntityNode(__u32 A_ModelType, __u32 A_EntityType, char* A_FileName);
	void	LcAddModel(__u32 A_ModelType);
	void	LcInit();
	void	LcInitCtrl(Ptr_UpdtInfo A_PtrInfo);
	__s32 LcFindModelIdx(__u32 A_ModelType);
	__s32	LcModelTypeGet(__u32 A_Idx);
	//__s32 LcFindEntityIdx(__u32 A_ModelType);

	BOOL8 LcCheckIni(Ptr_UpdtInfo A_PtrInfo);//ini file check
	void	LcMkiniFile();
// Dialog Data
	enum { IDD = IDD_MKUPDTMNG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAddmodel();
	afx_msg void OnBnClickedBtnAddentity();
	CTreeCtrl m_TreeCtrl;
//	CEdit m_EdtVersion;
	CEdit m_EdtPath;
	CButton m_BtnPath;
	CButton m_BtnMake;
	afx_msg void OnBnClickedBtnPath();
	afx_msg void OnBnClickedBtnMake();
	virtual BOOL DestroyWindow();
//	CEdit m_Ver2;
	CEdit m_EdtVer1;
	CEdit m_EdtVer2;
	CEdit m_EdtVer3;
	CEdit m_EdtVer4;
};
