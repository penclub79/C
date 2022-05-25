/*
	DVR FirmWare Upgrade
*/

#include "GrDumyTool.h"
#include <GrLnxApp.h>
#include <GrDebug.h>
#include "FirmUpdate.h"

CFirmUpdate::CFirmUpdate()
{
	__u8		Tv_WkCnt;
	__u8		Tv_WkIdx;

	m_stFirmEd		= { 0 };
	m_stFirmHeader	= { 0 };
	m_pMakeUpdate	= NULL;
	
	for (Tv_WkCnt = 0; Tv_WkCnt < E_FirmUpInfoCnt; Tv_WkCnt++)
	{
		for (Tv_WkIdx = 0; Tv_WkIdx < E_FirmUpEntityCnt; Tv_WkIdx++)
		{
			m_pData[Tv_WkCnt][Tv_WkIdx] = NULL;
		}
	}

}

CFirmUpdate::~CFirmUpdate()
{

	__u8		Tv_WkCnt;
	__u8		Tv_WkIdx;

	// m_pData NULL üũ
	for (Tv_WkCnt = 0; Tv_WkCnt < E_FirmUpInfoCnt; Tv_WkCnt++)
	{
		for (Tv_WkIdx = 0; Tv_WkIdx < E_FirmUpEntityCnt; Tv_WkIdx++)
		{
			if (NULL != m_pData[Tv_WkCnt][Tv_WkIdx])
			{
				free(m_pData[Tv_WkCnt][Tv_WkIdx]);
				m_pData[Tv_WkCnt][Tv_WkIdx] = NULL;
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
	__s32			Tv_WkIdx;
	__s32			Tv_WkCnt;
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
	for (Tv_WkIdx = 0; Tv_WkIdx < E_FirmUpInfoCnt; Tv_WkIdx++)
	{
		for (Tv_WkCnt = 0; Tv_WkCnt < E_FirmUpEntityCnt; Tv_WkCnt++)
		{
			if (NULL != m_pData[Tv_WkIdx][Tv_WkCnt])
			{
				free(m_pData[Tv_WkIdx][Tv_WkCnt]);
				m_pData[Tv_WkIdx][Tv_WkCnt] = NULL;
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

int	CFirmUpdate::AddVerFile(int _iModelType, int _iVerFileType, char* _pSrc, int _iFileSize)
{
	// Local --------------------------------------
	int iResult		= 0;
	int iModelIdx	= 0;
	int iVerFileIdx = 0;
	__s32		Tv_WkIdx;
	__s32		Tv_EttIdx;
	//---------------------------------------------

	iResult = E_FirmUpErrCode;

	// Ʈ���� ���� ������ -1
	Tv_WkIdx = ChkModelType(_iModelType);

	// Ʈ���� ��Ÿ���� ������
	if (0 <= Tv_WkIdx)
	{
		// ���� ����Ÿ�� check�� ������ -1 ��ȯ
		if (0 > ChkEntityType(_iVerFileType, m_stFirmHeader.FirmInfo[Tv_WkIdx].Entity))
		{
			Tv_EttIdx = ChkEntityType(E_FirmUpEntityNone, m_stFirmHeader.FirmInfo[Tv_WkIdx].Entity);

			if (NULL == m_pData[Tv_WkIdx][Tv_EttIdx])
			{
				
				m_pData[Tv_WkIdx][Tv_EttIdx] = malloc(_iFileSize);
				//m_pData[iModelIdx][iVerFileIdx] = new char[_iFileSize];// <----- �� �ڵ�� ���� �˼����� ������ ���װ� ��� �߻�

				/*
					new char() ���� ����� �߸��Ǿ ������ ���� ��
					new char[]�� �����ϸ� ������ ���� ����.
				*/

				if (NULL != m_pData[Tv_WkIdx][Tv_EttIdx])
				{
					m_stFirmHeader.FirmInfo[Tv_WkIdx].Entity[Tv_EttIdx].Size = _iFileSize;
					m_stFirmHeader.FirmInfo[Tv_WkIdx].Entity[Tv_EttIdx].Type = _iVerFileType;

					GrDumyCopyMem(m_pData[Tv_WkIdx][Tv_EttIdx], _pSrc, _iFileSize);
					m_stFirmHeader.Size = m_stFirmHeader.Size + _iFileSize; 
					iResult = Tv_EttIdx;
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
		// local -------------------
	void*		Tv_Result;
	__u8*		Tv_PtrUpdt;
	__u8		Tv_MdCnt;
	__u8		Tv_EttCnt;
	__u32		Tv_UpdtPos;
	// code --------------------
	Tv_Result = NULL;

	if (0 < m_stFirmHeader.Size)
	{
		// �޸� ����??
		if (NULL != m_pMakeUpdate)
		{
			free(m_pMakeUpdate);
			m_pMakeUpdate = NULL;
		}

		m_pMakeUpdate = malloc(m_stFirmHeader.Size);
		Tv_PtrUpdt = (__u8*)m_pMakeUpdate;
		if (NULL != m_pMakeUpdate)
		{
			Tv_PtrUpdt = Tv_PtrUpdt + sizeof(m_stFirmHeader);
			Tv_UpdtPos = sizeof(m_stFirmHeader);
			for (Tv_MdCnt = 0; Tv_MdCnt < E_FirmUpInfoCnt; Tv_MdCnt++)
			{
				for (Tv_EttCnt = 0; Tv_EttCnt < E_FirmUpEntityCnt; Tv_EttCnt++)
				{
					if (NULL != m_pData[Tv_MdCnt][Tv_EttCnt])
					{
						GrDumyCopyMem(Tv_PtrUpdt, m_pData[Tv_MdCnt][Tv_EttCnt], m_stFirmHeader.FirmInfo[Tv_MdCnt].Entity[Tv_EttCnt].Size);
						m_stFirmHeader.FirmInfo[Tv_MdCnt].Entity[Tv_EttCnt].Offset = Tv_UpdtPos;

						Tv_UpdtPos = Tv_UpdtPos + m_stFirmHeader.FirmInfo[Tv_MdCnt].Entity[Tv_EttCnt].Size;
						Tv_PtrUpdt = Tv_PtrUpdt + m_stFirmHeader.FirmInfo[Tv_MdCnt].Entity[Tv_EttCnt].Size;
					}
				}
			}

			//make end
			GrDumyCopyMem(Tv_PtrUpdt, &m_stFirmEd, sizeof(_stFirmUpEnd));

			//make header
			Tv_PtrUpdt = (__u8*)m_pMakeUpdate;
			GrDumyCopyMem(Tv_PtrUpdt, &m_stFirmHeader, sizeof(_stFirmUpHeader));

			*_puiSize = m_stFirmHeader.Size;
			Tv_Result = m_pMakeUpdate;
		}
	}

	return Tv_Result;
}

// ���� ����
void CFirmUpdate::DelVerFile(int _iModelType, int _iVerFileType)
{
	int iModelIdx					= 0;
	_stFirmUpInfo*	pFirmInfo		= NULL;
	_stFirmUpEntity* pFirmVerFile	= NULL;

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
	int iModelIdx = 0;
	int iEntiIdx = 0;
	_stFirmUpInfo* pFirmInfo = NULL;
	_stFirmUpEntity* pFirmVerFile = NULL;

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
