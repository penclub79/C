/*
	DVR FirmWare Upgrade
*/

#include "GrDumyTool.h"
#include <GrLnxApp.h>
#include <GrDebug.h>
#include "FirmUpdate.h"

CFirmUpdate::CFirmUpdate()
{
	m_stFirmEd = { 0 };
	m_stFirmHeader = { 0 };
	m_pMkUpdt = NULL;
	
	for (int i = 0; i < E_FirmUpInfoCnt; i++)
	{
		for (int si = 0; si < E_FirmUpInfoCnt; si++)
		{
			m_pData[i][si] = NULL;
		}
	}

}

CFirmUpdate::~CFirmUpdate()
{

}

void CFirmUpdate::FirmInit()
{
	// local ---------------------------------------
	unsigned int	uiResult		= 0;
	unsigned int	uiIndex			= 0;
	unsigned int	uiCount			= 0;

	pFirmUpHeader	stFirmHd		= { 0 };
	pFirmUpInfo		stFirmUpInfo	= { 0 };
	pFirmUpEnd		stFirmEnd		= { 0 };
	BOOL			bIsExist		= FALSE;
	// ---------------------------------------------

	// 펌웨어 구조체 값 삽입
	m_stFirmHeader.Fcc		= E_FirmUpHdFcc;
	m_stFirmHeader.UpgdVer	= E_FirmUpVer;
	m_stFirmEd.Fcc			= E_FirmUpEdFcc;
	m_stFirmHeader.Size		= sizeof(pFirmUpHeader) + sizeof(pFirmUpEnd);

	// m_pData NULL 체크
	for (int i = 0; i < E_FirmUpInfoCnt; i++)
	{
		for (int si = 0; si < E_FirmUpInfoCnt; si++)
		{
			if (NULL != m_pData[i][si])
			{
				free(m_pData[i][si]);
				m_pData[i][si] = NULL;
			}
		}
	}

	// make Update NULL 체크
	if (NULL != m_pMkUpdt)
	{
		free(m_pMkUpdt);
		m_pMkUpdt = NULL;
	}

}

// 모델 타입 추가
int	CFirmUpdate::AddModelType(unsigned int _uiModelType)
{
	// local ---------------------------------------
	int iResult = 0;
	pFirmUpInfo stFirmUpInfo = { 0 };
	BOOL bIsExist = FALSE;
	int iIndex = 0;
	// ---------------------------------------------

	iResult = E_FirmUpErrCode;
	
	// 24보다 작거나 같고 0이 아닌 model type
	if ((E_FirmUpInfoTypeMaxIdx >= _uiModelType) && (_uiModelType != E_FirmUpInfoTypeNone))
	{
		// 모델 중복 체크
		if (E_FirmUpErrCode != ChkModelType(_uiModelType))
		{
			// 모델 존재
			bIsExist = TRUE;
		}

		// 모델이 존재하지 않을때
		if (!bIsExist)
		{
			// 빈 슬롯을 찾는다. (빈 슬롯이 있어야 모델을 추가할 수 있음)
			iIndex = ChkModelType(E_FirmUpInfoTypeNone);

			// 에러가 아닐때
			if (E_FirmUpErrCode != iIndex)
			{
				stFirmUpInfo = &m_stFirmHeader.FirmInfo[iIndex];
				stFirmUpInfo->ModelType = _uiModelType;
				iResult = iIndex;
			}
		}
	}
	else
	{
		DbgMsgPrint("Cls_FirmUpdt::AddType() : Model Type Index error.\n");
	}

	return iResult;
}

// 모델 타입 체크
int	CFirmUpdate::ChkModelType(unsigned int _uiModelType)
{
	// local ---------------------------------------
	int iResult = 0;
	pFirmUpInfo stFirmUpInfo = { 0 };
	// ---------------------------------------------
	iResult = E_FirmUpErrCode;

	stFirmUpInfo = m_stFirmHeader.FirmInfo;
	
	// E_FirmUpInfoCnt = 16
	for (int i = 0; i < E_FirmUpInfoCnt; i++)
	{
		if (stFirmUpInfo->ModelType == _uiModelType)
		{
			iResult = i;
			break;
		}

		stFirmUpInfo++;
	}

	return iResult;
}