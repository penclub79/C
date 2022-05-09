#pragma once
#include <GrTypeBase.h>
#include "afxwin.h"


class DlgModelAdd : public CDialogEx
{
	DECLARE_DYNAMIC(DlgModelAdd)

public:
	DlgModelAdd(CWnd* pParent, TCHAR* _pszPath);
	~DlgModelAdd();

	enum { IDD = IDD_DIALOG_MODEL_ADD };

	

protected:

private:
	int m_iModelType;


};

