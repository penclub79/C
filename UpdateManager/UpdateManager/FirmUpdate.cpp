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
	pFirmUpHeader	stFirmHd		= { 0 };
	pFirmUpInfo		stFirmUpInfo	= { 0 };
	pFirmUpEnd		stFirmEnd		= { 0 };
	BOOL			bIsExist		= FALSE;
	// ---------------------------------------------

	// �߿��� ����ü �� ����
	m_stFirmHeader.Fcc		= E_FirmUpHdFcc;
	m_stFirmHeader.UpgdVer	= E_FirmUpVer;
	m_stFirmEd.Fcc			= E_FirmUpEdFcc;
	m_stFirmHeader.Size		= sizeof(pFirmUpHeader) + sizeof(pFirmUpEnd);

	// m_pData NULL üũ
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

	// make Update NULL üũ
	if (NULL != m_pMkUpdt)
	{
		free(m_pMkUpdt);
		m_pMkUpdt = NULL;
	}

}

// �� Ÿ�� �߰�
int	CFirmUpdate::AddModelType(int _iModelType)
{
	// local ---------------------------------------
	int			iResult			= 0;
	pFirmUpInfo stFirmUpInfo	= { 0 };
	BOOL		bIsExist		= FALSE;
	int			iIndex			= 0;
	// ---------------------------------------------

	iResult = E_FirmUpErrCode;
	
	// 24���� �۰ų� ���� 0�� �ƴ� model type
	if ((E_FirmUpInfoTypeMaxIdx >= _iModelType) && (_iModelType != E_FirmUpInfoTypeNone))
	{
		// �� �ߺ� üũ
		if (E_FirmUpErrCode != ChkModelType(_iModelType))
		{
			// �� ����
			bIsExist = TRUE;
		}

		// ���� �������� ������
		if (!bIsExist)
		{
			// �� ������ ã�´�. (�� ������ �־�� ���� �߰��� �� ����)
			iIndex = ChkModelType(E_FirmUpInfoTypeNone);

			// ������ �ƴҶ�
			if (E_FirmUpErrCode != iIndex)
			{
				stFirmUpInfo = &m_stFirmHeader.FirmInfo[iIndex];
				stFirmUpInfo->ModelType = _iModelType;
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

// �� Ÿ�� üũ
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

int	CFirmUpdate::AddVerFile(int _iModelType, int _iVerFileType, void* _pSrc, int _iFileSize)
{
	// Local --------------------------------------
	int iResult = 0;
	int iModelIdx = 0;
	int iVerFileIdx = 0;
	//---------------------------------------------


	iResult = E_FirmUpErrCode;
	iModelIdx = ChkModelType(_iModelType);

	if (0 <= iModelIdx)
	{

		// ���� ����Ÿ�� check�� �ߺ��̶��
		if (0 > ChkEntityType(_iVerFileType, m_stFirmHeader.FirmInfo[iModelIdx].Entity))
		{
			iVerFileIdx = ChkEntityType(E_FirmUpEntityNone, m_stFirmHeader.FirmInfo[iModelIdx].Entity);

			if (NULL == m_pData[iModelIdx][iVerFileIdx])
			{
				m_pData[iModelIdx][iVerFileIdx] = new TCHAR(_iFileSize);

				if (NULL != m_pData[iModelIdx][iVerFileIdx])
				{
					m_stFirmHeader.FirmInfo[iModelIdx].Entity[iVerFileIdx].Size = _iFileSize;
					m_stFirmHeader.FirmInfo[iModelIdx].Entity[iVerFileIdx].Type = _iVerFileType;
					memcpy(m_pData[iModelIdx][iVerFileIdx], _pSrc, _iFileSize);
					m_stFirmHeader.Size = m_stFirmHeader.Size + _iFileSize;
					iResult = iVerFileIdx;
				}
				else
				{
					DbgMsgPrint("Cls_FirmUpdt::AddEntity() : malloc failed.\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_FirmUpdt::AddEntity() : error exist data\n");
				iResult = E_FirmUpErrExistData;
			}
		}
		else
		{
			DbgMsgPrint("Cls_FirmUpdt::AddEntity() : error exist entity\n");
			iResult = E_FirmUpErrExistEntity;
		}
	}
	else
	{
		DbgMsgPrint("Cls_FirmUpdt::AddEntity() : Model Type Index error.\n");
	}


	if (NULL != m_pData[iModelIdx][iVerFileIdx])
	{
		delete m_pData[iModelIdx][iVerFileIdx];
		m_pData[iModelIdx][iVerFileIdx] = NULL;
	}

	return iResult;
}

// �������� Ÿ�� üũ
int	CFirmUpdate::ChkEntityType(int _iVerFileType, pFirmUpEntity _stEntity)
{
	// Local --------------------------------------
	int				iResult			= 0;
	pFirmUpEntity	stFirmUpEntity	= { 0 };
	//---------------------------------------------

	iResult = E_FirmUpErrCode;
	stFirmUpEntity = _stEntity;

	for (int i = 0; i < E_FirmUpEntityCnt; i++)
	{
		// �ش� ���� ����Ÿ���� �ִٸ�
		if (stFirmUpEntity->Type == _iVerFileType)
		{
			// �ش� �ε����� ����
			iResult = i;
			break;
		}

		// ���� FirmUpEntity ����ü�� �ѱ�
		stFirmUpEntity++;
	}

	return iResult;
}

int	CFirmUpdate::GetModelTypeIdx(int _iModelType)
{
	return ChkModelType(_iModelType);
}

// ����ü�� ���� �����Ű��
void CFirmUpdate::SetUpdateVersion(unsigned int _uiVersion)
{
	m_stFirmHeader.UpgdVer = _uiVersion;
}