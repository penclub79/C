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
	// m_pData NULL 체크
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

	// 펌웨어 구조체 값 삽입
	m_stFirmHeader.Fcc		= E_FirmUpHdFcc;
	m_stFirmHeader.UpgdVer	= E_FirmUpVer;
	m_stFirmEd.Fcc			= E_FirmUpEdFcc;
	//m_stFirmHeader.Size		= sizeof(pFirmUpHeader) + sizeof(pFirmUpEnd); <--- 구조체의 크기가 안맞아서 패키지 파일 생성시 에러가 발생
	m_stFirmHeader.Size = sizeof(_stFirmUpHeader) + sizeof(_stFirmUpEnd);

	// m_pData NULL 체크
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

	// make Update NULL 체크
	if (NULL != m_pMakeUpdate)
	{
		free(m_pMakeUpdate);
		m_pMakeUpdate = NULL;
	}

}

// 모델 타입 추가
int	CFirmUpdate::AddModelType(int _iModelType)
{
	// local ---------------------------------------
	int			iResult			= 0;
	pFirmUpInfo stFirmUpInfo	= { 0 };
	BOOL		bIsExist		= FALSE;
	int			iModelIdx		= 0;
	// ---------------------------------------------

	iResult = E_FirmUpErrCode;
	
	// 24보다 작거나 같고 0이 아닌 model type
	if ((E_FirmUpInfoTypeMaxIdx >= _iModelType) && (_iModelType != E_FirmUpInfoTypeNone))
	{
		// 모델 중복 체크
		if (E_FirmUpErrCode != ChkModelType(_iModelType))
		{
			// 모델 존재
			bIsExist = TRUE;
		}

		// 모델이 존재하지 않을때
		if (!bIsExist)
		{
			// 빈 슬롯을 찾는다. (빈 슬롯이 있어야 모델을 추가할 수 있음)
			iModelIdx = ChkModelType(E_FirmUpInfoTypeNone);

			// 에러가 아닐때
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

// 모델 타입 체크
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

	// 트리에 모델이 없으면 -1
	iModelIdx = ChkModelType(_iModelType);

	// 트리에 모델타입이 있을때
	if (0 <= iModelIdx)
	{
		// 버전 파일타입 check시 없으면 -1 반환
		if (0 > ChkEntityType(_iVerFileType, m_stFirmHeader.FirmInfo[iModelIdx].Entity))
		{
			iEntiIdx = ChkEntityType(E_FirmUpEntityNone, m_stFirmHeader.FirmInfo[iModelIdx].Entity);

			if (NULL == m_pData[iModelIdx][iEntiIdx])
			{
				
				m_pData[iModelIdx][iEntiIdx] = malloc(_iFileSize);
				//m_pData[iModelIdx][iVerFileIdx] = new char[_iFileSize];// <----- 이 코드로 인해 알수없는 곳에서 버그가 계속 발생

				/*
					new char() 문법 사용이 잘못되어서 에러가 나는 것
					new char[]로 변경하면 에러가 나지 않음.
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

// 버전파일 타입 체크
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
		// 해당 버전 파일타입이 있다면
		if (stFirmUpEntity->Type == _iVerFileType)
		{
			// 해당 인덱스를 추출
			iResult = i;
			break;
		}

		// 다음 FirmUpEntity 구조체로 넘김
		stFirmUpEntity++;
	}

	return iResult;
}

int	CFirmUpdate::GetModelTypeIdx(int _iModelType)
{
	return ChkModelType(_iModelType);
}

// 구조체에 버전 적용시키기
void CFirmUpdate::SetUpdateVersion(unsigned int _uiVersion)
{
	m_stFirmHeader.UpgdVer = _uiVersion;
}

// 패키지 저장시 사이즈 얻기
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
		// 메모리 해제??
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

// 버전 삭제
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
				// 헤더에서 버전 파일 사이즈를 뺀다.
				m_stFirmHeader.Size = m_stFirmHeader.Size - pFirmVerFile->Size;
				pFirmVerFile->Offset = 0;
				pFirmVerFile->Size = 0;
				pFirmVerFile->Type = E_FirmUpEntityNone;
			}
			pFirmVerFile++;
		}
	}
}

// 모델 삭제
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
