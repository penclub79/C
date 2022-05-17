/*
	DVR Firmware Upgrade
*/

//====================================================================
// uses
#include <GrLnxApp.h>
#include "FirmUpdt.h"
#include "GrDumyTool.h"
#include <GrDebug.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_FirmUpdt::Cls_FirmUpdt()
{
	// local -------------------
		__u8		Tv_WkCnt;
		__u8		Tv_WkIdx;
	// code --------------------
		for(Tv_WkCnt = 0;Tv_WkCnt < E_FirmUpInfoCnt;Tv_WkCnt++)
		{
			for(Tv_WkIdx = 0;Tv_WkIdx < E_FirmUpEntityCnt;Tv_WkIdx++)
			{
				m_PtrData[Tv_WkCnt][Tv_WkIdx] = NULL;
			}
		}

		m_PtrMkUpdt = NULL;
}
//--------------------------------------------------------------------
Cls_FirmUpdt::~Cls_FirmUpdt()
{
	// local -------------------
		__s32		Tv_WkIdx;
		__s32		Tv_WkCnt;
	// code --------------------
		for(Tv_WkCnt = 0;Tv_WkCnt < E_FirmUpInfoCnt;Tv_WkCnt++)
		{
			for(Tv_WkIdx = 0;Tv_WkIdx < E_FirmUpEntityCnt;Tv_WkIdx++)
			{
				if(NULL != m_PtrData[Tv_WkCnt][Tv_WkIdx])
				{
					free(m_PtrData[Tv_WkCnt][Tv_WkIdx]);
					m_PtrData[Tv_WkCnt][Tv_WkIdx] = NULL;
				}
			}
		}

		if(NULL != m_PtrMkUpdt)
		{
			free(m_PtrMkUpdt);
			m_PtrMkUpdt = NULL;
		}
}
//--------------------------------------------------------------------
void Cls_FirmUpdt::Init()
{
	// local -------------------
		__s32			Tv_Result;
		Ptr_FirmUpInfo	Tv_PtrInfo;
		BOOL8			Tv_IsExist;
		__s32			Tv_WkIdx;
		__s32			Tv_WkCnt;
	// code --------------------
		GrDumyZeroMem(&m_FirmHd, sizeof(St_FirmUpHd));
		GrDumyZeroMem(&m_FirmEd, sizeof(St_FirmUpEd));

		m_FirmHd.Fcc			= E_FirmUpHdFcc;
		m_FirmHd.UpgdVer		= E_FirmUpVer;
		m_FirmEd.Fcc			= E_FirmUpEdFcc;
		m_FirmHd.Size			= sizeof(St_FirmUpHd) + sizeof(St_FirmUpEd);



		// 이미 m_PtrData는 초기화가 되었다. 생성자에서
		for(Tv_WkCnt = 0;Tv_WkCnt < E_FirmUpInfoCnt;Tv_WkCnt++)
		{
			for(Tv_WkIdx = 0;Tv_WkIdx < E_FirmUpEntityCnt;Tv_WkIdx++)
			{
				if(NULL != m_PtrData[Tv_WkCnt][Tv_WkIdx])
				{
					free(m_PtrData[Tv_WkCnt][Tv_WkIdx]);
					m_PtrData[Tv_WkCnt][Tv_WkIdx] = NULL;
				}
			}
		}

		if(NULL != m_PtrMkUpdt)
		{
			free(m_PtrMkUpdt);
			m_PtrMkUpdt = NULL;
		}
}
//--------------------------------------------------------------------
__s32 Cls_FirmUpdt::AddType(__u32 A_ModelType)
{
	// local -------------------
		__s32		Tv_Result;
		Ptr_FirmUpInfo Tv_PtrInfo;
		BOOL8		Tv_IsExist;
		__s32		Tv_WkIdx;
	// code --------------------
		Tv_Result		= E_FirmUpErrCode;
		Tv_IsExist	= FALSE;

		if((E_FirmUpInfoTypeMaxIdx >= A_ModelType) && (A_ModelType != E_FirmUpInfoTypeNone))
		{
			//check same type
			if(E_FirmUpErrCode != LcChkModelType(A_ModelType))
			{
				Tv_IsExist = TRUE;
			}

			if(!Tv_IsExist)
			{
				//find empty slot
				Tv_WkIdx = LcChkModelType(E_FirmUpInfoTypeNone);

				if(E_FirmUpErrCode != Tv_WkIdx)
				{
					Tv_PtrInfo = &m_FirmHd.FirmInfo[Tv_WkIdx];
					Tv_PtrInfo->ModelType = A_ModelType;

					Tv_Result		= Tv_WkIdx;
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_FirmUpdt::AddType() : Model Type Index error.\n");
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
__s32 Cls_FirmUpdt::LcChkModelType(__u32 A_ModelType)
{
	// local -------------------
		__s32			Tv_Result;
		__s32			Tv_WkCnt;
		Ptr_FirmUpInfo	Tv_PtrInfo;
	// code --------------------
		Tv_Result		= E_FirmUpErrCode;

		Tv_PtrInfo = m_FirmHd.FirmInfo;
		for(Tv_WkCnt = 0;Tv_WkCnt < E_FirmUpInfoCnt;Tv_WkCnt++)
		{
			if(Tv_PtrInfo->ModelType == A_ModelType)
			{
				Tv_Result = Tv_WkCnt;
				break;
			}

			//next
			Tv_PtrInfo++;
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
__s32 Cls_FirmUpdt::AddEntity(__u32 A_ModelType, __u32 A_EntityType, void* A_PtrSrc, __u32 A_Size)
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
		Ptr_FirmUpInfo Tv_PtrInfo;
		__s32		Tv_EttIdx;
	// code --------------------
		Tv_Result = E_FirmUpErrCode;

		Tv_WkIdx = LcChkModelType(A_ModelType);

		if(0 <= Tv_WkIdx)
		{
			//check same entity
			if(0 > LcChkEntityType(A_EntityType, m_FirmHd.FirmInfo[Tv_WkIdx].Entity))
			{
				Tv_EttIdx = LcChkEntityType(E_FirmUpEntityNone, m_FirmHd.FirmInfo[Tv_WkIdx].Entity);

				if(NULL == m_PtrData[Tv_WkIdx][Tv_EttIdx])
				{
					m_PtrData[Tv_WkIdx][Tv_EttIdx] = malloc(A_Size);

					if(NULL != m_PtrData[Tv_WkIdx][Tv_EttIdx])
					{
						m_FirmHd.FirmInfo[Tv_WkIdx].Entity[Tv_EttIdx].Size = A_Size;
						m_FirmHd.FirmInfo[Tv_WkIdx].Entity[Tv_EttIdx].Type = A_EntityType;
						GrDumyCopyMem(m_PtrData[Tv_WkIdx][Tv_EttIdx], A_PtrSrc, A_Size);
						m_FirmHd.Size = m_FirmHd.Size + A_Size;
						Tv_Result = Tv_EttIdx;
					}
					else
					{
						DbgMsgPrint("Cls_FirmUpdt::AddEntity() : malloc failed.\n");
					}
				}
				else
				{
					DbgMsgPrint("Cls_FirmUpdt::AddEntity() : error exist data\n");
					Tv_Result = E_FirmUpErrExistData;
				}
			}
			else
			{
				DbgMsgPrint("Cls_FirmUpdt::AddEntity() : error exist entity\n");
				Tv_Result = E_FirmUpErrExistEntity;
			}
		}
		else
		{
			DbgMsgPrint("Cls_FirmUpdt::AddEntity() : Model Type Index error.\n");
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
__s32 Cls_FirmUpdt::LcChkEntityType(__u32 A_EntityType, Ptr_FirmUpEntity A_PtrEntity)
{
	// local -------------------
		__s32		Tv_Result;
		__u32		Tv_WkCnt;
		Ptr_FirmUpEntity Tv_PtrEntity;
	// code --------------------
		Tv_Result			= E_FirmUpErrCode;
		Tv_PtrEntity	= A_PtrEntity;

		for(Tv_WkCnt = 0;Tv_WkCnt < E_FirmUpEntityCnt;Tv_WkCnt++)
		{
			if(Tv_PtrEntity->Type == A_EntityType)
			{
				Tv_Result = Tv_WkCnt;
				break;
			}
			//next
			Tv_PtrEntity++;
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
void* Cls_FirmUpdt::GetMkUpdt(__u32* A_PtrSize)
{
	// local -------------------
		void*		Tv_Result;
		__u8*		Tv_PtrUpdt;
		__u8		Tv_MdCnt;
		__u8		Tv_EttCnt;
		__u32		Tv_UpdtPos;
	// code --------------------
		Tv_Result		= NULL;

		if(0 < m_FirmHd.Size)
		{
			// 메모리 해제??
			if(NULL != m_PtrMkUpdt)
			{
				free(m_PtrMkUpdt);
				m_PtrMkUpdt = NULL;
			}

			m_PtrMkUpdt = malloc(m_FirmHd.Size);
			Tv_PtrUpdt  = (__u8*)m_PtrMkUpdt;
			if(NULL != m_PtrMkUpdt)
			{
				Tv_PtrUpdt	= Tv_PtrUpdt + sizeof(St_FirmUpHd);
				Tv_UpdtPos	= sizeof(St_FirmUpHd);
				for(Tv_MdCnt = 0;Tv_MdCnt < E_FirmUpInfoCnt;Tv_MdCnt++)
				{
					for(Tv_EttCnt = 0;Tv_EttCnt < E_FirmUpEntityCnt;Tv_EttCnt++)
					{
						if(NULL != m_PtrData[Tv_MdCnt][Tv_EttCnt])
						{
							GrDumyCopyMem(Tv_PtrUpdt, m_PtrData[Tv_MdCnt][Tv_EttCnt], m_FirmHd.FirmInfo[Tv_MdCnt].Entity[Tv_EttCnt].Size);
							m_FirmHd.FirmInfo[Tv_MdCnt].Entity[Tv_EttCnt].Offset = Tv_UpdtPos;

							Tv_UpdtPos = Tv_UpdtPos + m_FirmHd.FirmInfo[Tv_MdCnt].Entity[Tv_EttCnt].Size;
							Tv_PtrUpdt = Tv_PtrUpdt + m_FirmHd.FirmInfo[Tv_MdCnt].Entity[Tv_EttCnt].Size;
						}
					}
				}

				//make end
				GrDumyCopyMem(Tv_PtrUpdt, &m_FirmEd, sizeof(St_FirmUpEd));

				//make header
				Tv_PtrUpdt  = (__u8*)m_PtrMkUpdt;
				GrDumyCopyMem(Tv_PtrUpdt, &m_FirmHd, sizeof(St_FirmUpHd));

				*A_PtrSize	= m_FirmHd.Size;
				Tv_Result	= m_PtrMkUpdt;
			}
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
void Cls_FirmUpdt::DelType(__u32 A_ModelType)
{
	// local -------------------
		__u32		Tv_WkCnt;
		__s32		Tv_WkIdx;
		St_FirmUpInfo* Tv_PtrInfo;
		St_FirmUpEntity* Tv_PtrEntity;
	// code --------------------
		Tv_WkIdx = LcChkModelType(A_ModelType);

		if(0 <= Tv_WkIdx)
		{
			Tv_PtrInfo = &m_FirmHd.FirmInfo[Tv_WkIdx];

			Tv_PtrEntity = Tv_PtrInfo->Entity;
			for(Tv_WkCnt = 0;Tv_WkCnt < E_FirmUpEntityCnt;Tv_WkCnt++)
			{
				if(0 < Tv_PtrEntity->Type)
				{
					if(m_PtrData[Tv_WkIdx][Tv_WkCnt] != NULL)
					{
						free(m_PtrData[Tv_WkIdx][Tv_WkCnt]);
						m_PtrData[Tv_WkIdx][Tv_WkCnt]		= NULL;
					}
					m_FirmHd.Size					= m_FirmHd.Size - Tv_PtrEntity->Size;
					Tv_PtrEntity->Offset	= 0;
					Tv_PtrEntity->Size		= 0;
					Tv_PtrEntity->Type		= E_FirmUpEntityNone;
				}
				//next
				Tv_PtrEntity++;
			}

			Tv_PtrInfo->ModelType		= E_FirmUpInfoTypeNone;
		}
}
//--------------------------------------------------------------------
void Cls_FirmUpdt::DelEntity(__u32 A_ModelType, __u32 A_EntityType)
{
	// local -------------------
		__u32		Tv_WkCnt;
		__s32		Tv_WkIdx;
		St_FirmUpInfo* Tv_PtrInfo;
		St_FirmUpEntity* Tv_PtrEntity;
	// code --------------------
		Tv_WkIdx = LcChkModelType(A_ModelType);

		if(0 <= Tv_WkIdx)
		{
			Tv_PtrInfo = &m_FirmHd.FirmInfo[Tv_WkIdx];

			Tv_PtrEntity = Tv_PtrInfo->Entity;
			for(Tv_WkCnt = 0;Tv_WkCnt < E_FirmUpEntityCnt;Tv_WkCnt++)
			{
				if(A_EntityType == Tv_PtrEntity->Type)
				{
					if(m_PtrData[Tv_WkIdx][Tv_WkCnt] != NULL)
					{
						free(m_PtrData[Tv_WkIdx][Tv_WkCnt]);
						m_PtrData[Tv_WkIdx][Tv_WkCnt]		= NULL;
					}
					m_FirmHd.Size					= m_FirmHd.Size - Tv_PtrEntity->Size;
					Tv_PtrEntity->Offset	= 0;
					Tv_PtrEntity->Size		= 0;
					Tv_PtrEntity->Type		= E_FirmUpEntityNone;
				}
				//next
				Tv_PtrEntity++;
			}
		}
}
//--------------------------------------------------------------------
void Cls_FirmUpdt::SetUpdtVer(__u32 A_Ver)
{
		m_FirmHd.UpgdVer	= A_Ver;
}
//--------------------------------------------------------------------
__s32 Cls_FirmUpdt::GetModelTblIdx(__u32 A_ModelType)
{
		return LcChkModelType(A_ModelType);
}
//--------------------------------------------------------------------
