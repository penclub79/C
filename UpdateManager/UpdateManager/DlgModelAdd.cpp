#include "stdafx.h"
#include "UpdateManager.h"
#include "DlgModelAdd.h"
#include "FirmUpdate.h"


IMPLEMENT_DYNAMIC(DlgModelAdd, CDialogEx)

DlgModelAdd::DlgModelAdd(CWnd* pParent /*=NULL*/, TCHAR* _pszPath)
: CDialogEx(DlgModelAdd::IDD, pParent)
{
	m_iModelType = E_FirmUpInfoTypeNone;
	m_pszPath = _pszPath;
}

DlgModelAdd::~DlgModelAdd()
{
}

