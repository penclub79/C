/*
	DVR FirmWare Upgrade
*/

#include "GrDumyTool.h"
#include <GrLnxApp.h>
#include <GrDebug.h>
#include "FirmUpdate.h"

CFirmUpdate::CFirmUpdate()
{
	m_stFirmEd		= { 0 };
	m_stFirmHeader	= { 0 };
	m_pMakeUpdate	= NULL;
	
	for (int iModelIdx = 0; iModelIdx < E_FirmUpInfoCnt; iModelIdx++)
	{
		for (int iEntiIdx = 0; iEntiIdx < E_FirmUpEntityCnt; iEntiIdx++)
		{
			m_pData[iModelIdx][iEntiIdx] = NULL;
		}
	}

}

CFirmUpdate::~CFirmUpdate()
{
	// m_pData NULL üũ
	for (int iModelIdx = 0; iModelIdx < E_FirmUpInfoCnt; iModelIdx++)
	{
		for (int iEntiIdx = 0; iEntiIdx < E_FirmUpEntityCnt; iEntiIdx++)
		{
			if (NULL != m_pData[iModelIdx][iEntiIdx])
			{
				free(m_pData[iModelIdx][iEntiIdx]);
				m_pData[iModelIdx][iEntiIdx] = NULL;
			}
		}
	}

	if (NULL != m_pMakeUpdate)
	{
		free(m_pMakeUpdate);
		m_pMakeUpdate = NULL;
	}

}

void CFirmUpdate::FirmInit()
{
	// local ---------------------------------------
	pFirmUpHeader	stFirmHd		= { 0 };
	pFirmUpInfo		stFirmUpInfo	= { 0 };
	pFirmUpEnd		stFirmEnd		= { 0 };
	BOOL			bIsExist		= FALSE;
	// ---------------------------------------------

	GrDumyZeroMem(&m_stFirmHeader,	sizeof(_stFirmUpHeader));
	GrDumyZeroMem(&m_stFirmEd,		sizeof(_stFirmUpEnd));

	// �߿��� ����ü �� ����
	m_stFirmHeader.Fcc		= E_FirmUpHdFcc;
	m_stFirmHeader.UpgdVer	= E_FirmUpVer;
	m_stFirmEd.Fcc			= E_FirmUpEdFcc;
	//m_stFirmHeader.Size		= sizeof(pFirmUpHeader) + sizeof(pFirmUpEnd); <--- ����ü�� ũ�Ⱑ �ȸ¾Ƽ� ��Ű�� ���� ������ ������ �߻�
	m_stFirmHeader.Size = sizeof(_stFirmUpHeader) + sizeof(_stFirmUpEnd);

	// m_pData NULL üũ
	for (int iModelIdx = 0; iModelIdx < E_FirmUpInfoCnt; iModelIdx++)
	{
		for (int iEntiIdx = 0; iEntiIdx < E_FirmUpEntityCnt; iEntiIdx++)
		{
			if (NULL != m_pData[iModelIdx][iEntiIdx])
			{
				free(m_pData[iModelIdx][iEntiIdx]);
				m_pData[iModelIdx][iEntiIdx] = NULL;
			}
		}
	}

	// make Update NULL üũ
	if (NULL != m_pMakeUpdate)
	{
		free(m_pMakeUpdate);
		m_pMakeUpdate = NULL;
	}

}

// �� Ÿ�� �߰�
int	CFirmUpdate::AddModelType(int _iModelType)
{
	// local ---------------------------------------
	int			iResult			= 0;
	pFirmUpInfo stFirmUpInfo	= { 0 };
	BOOL		bIsExist		= FALSE;
	int			iModelIdx		= 0;
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
			iModelIdx = ChkModelType(E_FirmUpInfoTypeNone);

			// ������ �ƴҶ�
			if (E_FirmUpErrCode != iModelIdx)
			{
				stFirmUpInfo = &m_stFirmHeader.FirmInfo[iModelIdx];
				stFirmUpInfo->ModelType = _iModelType;
				iResult = iModelIdx;
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
	int			iResult			= 0;
	pFirmUpInfo stFirmUpInfo	= { 0 };
	// ---------------------------------------------

	iResult = E_FirmUpErrCode;

	stFirmUpInfo = m_stFirmHeader.FirmInfo;
	
	// E_FirmUpInfoCnt = 16
	for (int iIndex = 0; iIndex < E_FirmUpInfoCnt; iIndex++)
	{
		if (stFirmUpInfo->ModelType == _uiModelType)
		{
			iResult = iIndex;
			break;
		}

		stFirmUpInfo++;
	}

	return iResult;
}

int	CFirmUpdate::AddVerFile(int _iModelType, int _iVerFileType, char* _pSrc, int _iFileSize)
{
	// Local --------------------------------------
	int		iResult		= 0;
	int		iModelIdx	= 0;
	int		iEntiIdx	= 0;
	//---------------------------------------------

	iResult = E_FirmUpErrCode;

	// Ʈ���� ���� ������ -1
	iModelIdx = ChkModelType(_iModelType);

	// Ʈ���� ��Ÿ���� ������
	if (0 <= iModelIdx)
	{
		// ���� ����Ÿ�� check�� ������ -1 ��ȯ
		if (0 > ChkEntityType(_iVerFileType, m_stFirmHeader.FirmInfo[iModelIdx].Entity))
		{
			iEntiIdx = ChkEntityType(E_FirmUpEntityNone, m_stFirmHeader.FirmInfo[iModelIdx].Entity);

			if (NULL == m_pData[iModelIdx][iEntiIdx])
			{
				
				m_pData[iModelIdx][iEntiIdx] = malloc(_iFileSize);
				//m_pData[iModelIdx][iVerFileIdx] = new char[_iFileSize];// <----- �� �ڵ�� ���� �˼����� ������ ���װ� ��� �߻�

				/*
					new char() ���� ����� �߸��Ǿ ������ ���� ��
					new char[]�� �����ϸ� ������ ���� ����.
				*/

				if (NULL != m_pData[iModelIdx][iEntiIdx])
				{
					m_stFirmHeader.FirmInfo[iModelIdx].Entity[iEntiIdx].Size = _iFileSize;
					m_stFirmHeader.FirmInfo[iModelIdx].Entity[iEntiIdx].Type = _iVerFileType;

					GrDumyCopyMem(m_pData[iModelIdx][iEntiIdx], _pSrc, _iFileSize);
					m_stFirmHeader.Size = m_stFirmHeader.Size + _iFileSize; 
					iResult = iEntiIdx;
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

// ��Ű�� ����� ������ ���
void* CFirmUpdate::GetMakeUpdate(unsigned int* _puiSize)
{
	//m_stFirmHeader m_pMakeUpdate
	// Local --------------------------------------
	void*			pvResult;
	unsigned char*	puszUpdtMem;
	unsigned int	uiUpdtPos;
	//---------------------------------------------

	pvResult = NULL;

	if (0 < m_stFirmHeader.Size)
	{
		// �޸� ����??
		if (NULL != m_pMakeUpdate)
		{
			free(m_pMakeUpdate);
			m_pMakeUpdate = NULL;
		}

		m_pMakeUpdate = malloc(m_stFirmHeader.Size);
		puszUpdtMem = (unsigned char*)m_pMakeUpdate;
		if (NULL != m_pMakeUpdate)
		{
			puszUpdtMem = puszUpdtMem + sizeof(m_stFirmHeader);
			uiUpdtPos = sizeof(m_stFirmHeader);
			for (int iModelIdx = 0; iModelIdx < E_FirmUpInfoCnt; iModelIdx++)
			{
				for (int iEntiIdx = 0; iEntiIdx < E_FirmUpEntityCnt; iEntiIdx++)
				{
					if (NULL != m_pData[iModelIdx][iEntiIdx])
					{
						GrDumyCopyMem(puszUpdtMem, m_pData[iModelIdx][iEntiIdx], m_stFirmHeader.FirmInfo[iModelIdx].Entity[iEntiIdx].Size);
						m_stFirmHeader.FirmInfo[iModelIdx].Entity[iEntiIdx].Offset = uiUpdtPos;

						uiUpdtPos = uiUpdtPos + m_stFirmHeader.FirmInfo[iModelIdx].Entity[iEntiIdx].Size;
						puszUpdtMem = puszUpdtMem + m_stFirmHeader.FirmInfo[iModelIdx].Entity[iEntiIdx].Size;
					}
				}
			}

			//make end
			GrDumyCopyMem(puszUpdtMem, &m_stFirmEd, sizeof(_stFirmUpEnd));

			//make header
			puszUpdtMem = (unsigned char*)m_pMakeUpdate;
			GrDumyCopyMem(puszUpdtMem, &m_stFirmHeader, sizeof(_stFirmUpHeader));

			*_puiSize = m_stFirmHeader.Size;
			pvResult = m_pMakeUpdate;
		}
	}

	return pvResult;
}

// ���� ����
void CFirmUpdate::DelVerFile(int _iModelType, int _iVerFileType)
{
	// Local --------------------------------------
	int iModelIdx					= 0;
	_stFirmUpInfo*	pFirmInfo		= NULL;
	_stFirmUpEntity* pFirmVerFile	= NULL;
	//---------------------------------------------

	iModelIdx = ChkModelType(_iModelType);

	if (0 <= iModelIdx)
	{
		pFirmInfo = &m_stFirmHeader.FirmInfo[iModelIdx];

		pFirmVerFile = pFirmInfo->Entity;

		for (int iEntiIdx = 0; iEntiIdx < E_FirmUpEntityCnt; iEntiIdx++)
		{
			if (_iVerFileType == pFirmVerFile->Type)
			{
				if (NULL != m_pData[iModelIdx][iEntiIdx])
				{
					free(m_pData[iModelIdx][iEntiIdx]);
					m_pData[iModelIdx][iEntiIdx] = NULL;
				}
				// ������� ���� ���� ����� ����.
				m_stFirmHeader.Size = m_stFirmHeader.Size - pFirmVerFile->Size;
				pFirmVerFile->Offset = 0;
				pFirmVerFile->Size = 0;
				pFirmVerFile->Type = E_FirmUpEntityNone;
			}
			pFirmVerFile++;
		}
	}
}

// �� ����
void CFirmUpdate::DelModelType(int _iModelType)
{
	// Local --------------------------------------
	int iModelIdx = 0;
	int iEntiIdx = 0;
	_stFirmUpInfo* pFirmInfo = NULL;
	_stFirmUpEntity* pFirmVerFile = NULL;
	//---------------------------------------------

	iModelIdx = ChkModelType(_iModelType);
	pFirmVerFile = pFirmInfo->Entity;

	if (0 <= iModelIdx)
	{
		pFirmInfo = &m_stFirmHeader.FirmInfo[iModelIdx];

		while (pFirmInfo->Entity[iEntiIdx].Type)
		{
			m_stFirmHeader.Size = m_stFirmHeader.Size - pFirmInfo->Entity[iEntiIdx].Size;
			if (NULL != m_pData[iModelIdx][iEntiIdx])
			{
				free(m_pData[iModelIdx][iEntiIdx]);
				m_pData[iModelIdx][iEntiIdx] = NULL;
			}
			iEntiIdx++;
		}
		memset(pFirmInfo, 0, sizeof(_stFirmUpInfo));
	}

}
