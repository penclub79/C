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

	// m_pData NULL 체크
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

	// 펌웨어 구조체 값 삽입
	m_stFirmHeader.Fcc		= E_FirmUpHdFcc;
	m_stFirmHeader.UpgdVer	= E_FirmUpVer;
	m_stFirmEd.Fcc			= E_FirmUpEdFcc;
	//m_stFirmHeader.Size		= sizeof(pFirmUpHeader) + sizeof(pFirmUpEnd); <--- 구조체의 크기가 안맞아서 패키지 파일 생성시 에러가 발생
	m_stFirmHeader.Size = sizeof(_stFirmUpHeader) + sizeof(_stFirmUpEnd);

	// m_pData NULL 체크
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
	int			iIndex			= 0;
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
			iIndex = ChkModelType(E_FirmUpInfoTypeNone);

			// 에러가 아닐때
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

	// 트리에 모델이 없으면 -1
	Tv_WkIdx = ChkModelType(_iModelType);

	// 트리에 모델타입이 있을때
	if (0 <= Tv_WkIdx)
	{
		// 버전 파일타입 check시 없으면 -1 반환
		if (0 > ChkEntityType(_iVerFileType, m_stFirmHeader.FirmInfo[Tv_WkIdx].Entity))
		{
			Tv_EttIdx = ChkEntityType(E_FirmUpEntityNone, m_stFirmHeader.FirmInfo[Tv_WkIdx].Entity);

			if (NULL == m_pData[Tv_WkIdx][Tv_EttIdx])
			{
				
				m_pData[Tv_WkIdx][Tv_EttIdx] = malloc(_iFileSize);
				//m_pData[iModelIdx][iVerFileIdx] = new char[_iFileSize];// <----- 이 코드로 인해 알수없는 곳에서 버그가 계속 발생

				/*
					new char() 문법 사용이 잘못되어서 에러가 나는 것
					new char[]로 변경하면 에러가 나지 않음.
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
		// 메모리 해제??
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

// 버전 삭제
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
