/*
product regist server

*/

//====================================================================
//include

#include <Win/Db/GrDbGrp.h>

#include <GrDebug.h>
#include <GrError.h>

#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrFileCtrl.h>
#include <GrFileTool.h>

#include <Win/Db/GrDbSync.h>

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//global var


//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrDbGrp::Cls_GrDbGrp(WCHAR* A_StrPathBase, __u32 A_GrpIdx, __u32 A_KeyValCnt, __u16 A_KeyValType)
{
	// local -------------------
		__u32		Tv_WkIdx;
		WCHAR*	Tv_StrWk;
		BOOL8		Tv_IsCreate;
	// code --------------------
		// init
#ifdef E_GrDbGrpKeySortUse
		m_KeySrtCnt		=	0;
#endif
		m_KeyTotal		=	0;
		m_KeyDeleted	=	0;
		m_KeyIdLast		=	0;

		m_Tick				=	0;
		m_KeyWriteTimer	=	0;

		m_GrpIdx			=	A_GrpIdx;
		m_KeyValCnt		=	A_KeyValCnt;
		m_KeyValType	=	A_KeyValType;

		switch(m_KeyValType)
		{
			case E_GrVarTypeStr:
				if(E_GrDbKeySize <= m_KeyValCnt)
				{
					DbgMsgPrint("Cls_GrDbGrp::Cls_GrDbGrp - grp %d key count (%d) is bad!\n", A_GrpIdx, A_KeyValCnt);
					m_KeyValCnt	=	E_GrDbKeySize - 1;
				}
				break;
			case  E_GrVarTypeWstr:
				if((E_GrDbKeySize >> 1) <= m_KeyValCnt)
				{
					DbgMsgPrint("Cls_GrDbGrp::Cls_GrDbGrp - grp %d key count (%d) is bad!\n", A_GrpIdx, A_KeyValCnt);
					m_KeyValCnt	=	(E_GrDbKeySize >> 1) - 1;
				}
				break;
			default:
				// fix key value count
				m_KeyValCnt	=	1;
				break;
		}

		GrDumyZeroMem(&m_KeyWt, sizeof(m_KeyWt));

		// cache initialize
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrDbGrpDataCacheCnt; Tv_WkIdx++)
		{
			m_DatCch[Tv_WkIdx].KeyId			=	0;
			m_DatCch[Tv_WkIdx].BlkId			=	0;
			m_DatCch[Tv_WkIdx].Tick				=	0;
			m_DatCch[Tv_WkIdx].TimerFlush	=	0;
		}

		// make path key file name
		Tv_StrWk		=	GrStrWcopy(m_StrFnKey, A_StrPathBase);
		GrStrIntToWstr(Tv_StrWk, (__s32)m_GrpIdx);

		GrStrWcat(Tv_StrWk, L".key");

		// make path data file name
		Tv_StrWk		=	GrStrWcopy(m_StrFnDat, A_StrPathBase);
		GrStrIntToWstr(Tv_StrWk, (__s32)m_GrpIdx);

		GrStrWcat(Tv_StrWk, L".dat");

		// load key
		GrDumyZeroMem(m_KeyTbl, sizeof(m_KeyTbl));
		Tv_IsCreate	=	!GrFileIsExist(m_StrFnKey);
		m_FcKey	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl(m_StrFnKey, TRUE, Tv_IsCreate, FALSE, TRUE);
		if(Tv_IsCreate)
		{
			m_FcKey->Write(m_KeyTbl, sizeof(m_KeyTbl));
		}
		else
		{
			m_FcKey->Read(m_KeyTbl, sizeof(m_KeyTbl));
		}

#ifdef E_GrDbGrpKeyOvlpDel
		LcKeyOvlpRemove();
#endif

		LcKeyStaticBuild();

		// open data
		Tv_IsCreate	=	!GrFileIsExist(m_StrFnDat);
		if(Tv_IsCreate)
		{
			// create data file
			LcDataFileCreate();
		}

		m_FcDat	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl(m_StrFnDat, TRUE, FALSE, FALSE, TRUE);

		// key sort table build
#ifdef E_GrDbGrpKeySortUse
		LcKeySortBuild();
#endif

}
//--------------------------------------------------------------------
Cls_GrDbGrp::~Cls_GrDbGrp()
{
	// local -------------------
		__u32	Tv_CchIdx;
	// code --------------------
		// data cache flush
		for(Tv_CchIdx = 0; Tv_CchIdx < E_GrDbGrpDataCacheCnt; Tv_CchIdx++)
		{
			LcDatCchFlushByCchIdx(Tv_CchIdx);
		}
		// key flush
		if((0 != m_KeyWt.KeyIdSt) && (0 != m_KeyWt.KeyIdEd))
		{
			LcKeyWriteFlush();
		}

		if(NULL != m_FcDat)
		{
			delete	m_FcDat;
			m_FcDat		=	NULL;
		}

		if(NULL != m_FcKey)
		{
			delete	m_FcKey;
			m_FcKey		=	NULL;
		}

}
//--------------------------------------------------------------------
void	Cls_GrDbGrp::LcKeyStaticBuild(void)
{
	// local -------------------
		__u32	Tv_TblIdx;
	// code --------------------
		m_KeyTotal		=	0;
		m_KeyDeleted	=	0;
		m_KeyIdLast		=	0;

		for(Tv_TblIdx = 0; Tv_TblIdx < E_GrDbKeyMaxCnt; Tv_TblIdx++)
		{
			// check key exist
			if(0 != m_KeyTbl[Tv_TblIdx].Atb.Key[0])
			{
				// exist item
				m_KeyTotal ++;
				m_KeyIdLast	=	Tv_TblIdx + 1;		// index to id
				// check deleted
				if(m_KeyTbl[Tv_TblIdx].Atb.IsDel)
				{
					m_KeyDeleted ++;
				}
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbGrp::LcIsKeyValExist(void* A_PtrKeyVal)
{
	// local -------------------
	// code --------------------
		if(0 != *((__u64*)A_PtrKeyVal))
		{
			return	TRUE;
		}
		
		return	FALSE;
}
//--------------------------------------------------------------------
__s32	Cls_GrDbGrp::LcCmpVal(void* A_PtrSr, void* A_PtrTg, __u16 A_ValType, __u16 A_ValCnt)
{
	// local -------------------
		__s32		Tv_Result;
		__u32		Tv_SrU32;
		__u32		Tv_TgU32;
		__u64		Tv_SrU64;
		__u64		Tv_TgU64;
		__u8*		Tv_PtrU8Sr;
		__u8*		Tv_PtrU8Tg;

	// code --------------------
		Tv_Result	=	0;

		switch(A_ValType)
		{
			case E_GrVarTypeU8:
			case E_GrVarTypeHexU8:
				if((*((__u8*)A_PtrSr)) < (*((__u8*)A_PtrTg)))
				{
					Tv_Result	=	-1;
				}
				else if((*((__u8*)A_PtrSr)) > (*((__u8*)A_PtrTg)))
				{
					Tv_Result	=	1;
				}
				break;
			case E_GrVarTypeS8:
				if((*((__s8*)A_PtrSr)) < (*((__s8*)A_PtrTg)))
				{
					Tv_Result	=	-1;
				}
				else if((*((__s8*)A_PtrSr)) > (*((__s8*)A_PtrTg)))
				{
					Tv_Result	=	1;
				}
				break;
			case E_GrVarTypeU16:
			case E_GrVarTypeHexU16:
				if((*((__u16*)A_PtrSr)) < (*((__u16*)A_PtrTg)))
				{
					Tv_Result	=	-1;
				}
				else if((*((__u16*)A_PtrSr)) > (*((__u16*)A_PtrTg)))
				{
					Tv_Result	=	1;
				}
				break;
			case E_GrVarTypeS16:
				if((*((__s16*)A_PtrSr)) < (*((__s16*)A_PtrTg)))
				{
					Tv_Result	=	-1;
				}
				else if((*((__s16*)A_PtrSr)) > (*((__s16*)A_PtrTg)))
				{
					Tv_Result	=	1;
				}
				break;
			case E_GrVarTypeU32:
			case E_GrVarTypeHexU32:
				if((*((__u32*)A_PtrSr)) < (*((__u32*)A_PtrTg)))
				{
					Tv_Result	=	-1;
				}
				else if((*((__u32*)A_PtrSr)) > (*((__u32*)A_PtrTg)))
				{
					Tv_Result	=	1;
				}
				break;
			case E_GrVarTypeS32:
				if((*((__s32*)A_PtrSr)) < (*((__s32*)A_PtrTg)))
				{
					Tv_Result	=	-1;
				}
				else if((*((__s32*)A_PtrSr)) > (*((__s32*)A_PtrTg)))
				{
					Tv_Result	=	1;
				}
				break;
			case E_GrVarTypeU64:
			case E_GrVarTypeHexU64:
				if((*((__u64*)A_PtrSr)) < (*((__u64*)A_PtrTg)))
				{
					Tv_Result	=	-1;
				}
				else if((*((__u64*)A_PtrSr)) > (*((__u64*)A_PtrTg)))
				{
					Tv_Result	=	1;
				}
				break;
			case E_GrVarTypeS64:
				if((*((__s64*)A_PtrSr)) < (*((__s64*)A_PtrTg)))
				{
					Tv_Result	=	-1;
				}
				else if((*((__s64*)A_PtrSr)) > (*((__s64*)A_PtrTg)))
				{
					Tv_Result	=	1;
				}
				break;
			case E_GrVarTypeIpV4:
				Tv_SrU32	=	GrDumyEndian4byteGet(A_PtrSr);
				Tv_TgU32	=	GrDumyEndian4byteGet(A_PtrTg);
				if(Tv_SrU32 < Tv_TgU32)
				{
					Tv_Result	=	-1;
				}
				else if(Tv_SrU32 > Tv_TgU32)
				{
					Tv_Result	=	1;
				}
				break;
			case E_GrVarTypeMacAdr:
				// copy source
				Tv_PtrU8Sr	=	(__u8*)A_PtrSr;
				Tv_PtrU8Tg	=	(__u8*)&Tv_SrU64;
				Tv_PtrU8Tg[0]		=	Tv_PtrU8Sr[5];
				Tv_PtrU8Tg[1]		=	Tv_PtrU8Sr[4];
				Tv_PtrU8Tg[2]		=	Tv_PtrU8Sr[3];
				Tv_PtrU8Tg[3]		=	Tv_PtrU8Sr[2];
				Tv_PtrU8Tg[4]		=	Tv_PtrU8Sr[1];
				Tv_PtrU8Tg[5]		=	Tv_PtrU8Sr[0];
				Tv_PtrU8Tg[6]		=	0;
				Tv_PtrU8Tg[7]		=	0;

				// copy target
				Tv_PtrU8Sr	=	(__u8*)A_PtrTg;
				Tv_PtrU8Tg	=	(__u8*)&Tv_TgU64;
				Tv_PtrU8Tg[0]		=	Tv_PtrU8Sr[5];
				Tv_PtrU8Tg[1]		=	Tv_PtrU8Sr[4];
				Tv_PtrU8Tg[2]		=	Tv_PtrU8Sr[3];
				Tv_PtrU8Tg[3]		=	Tv_PtrU8Sr[2];
				Tv_PtrU8Tg[4]		=	Tv_PtrU8Sr[1];
				Tv_PtrU8Tg[5]		=	Tv_PtrU8Sr[0];
				Tv_PtrU8Tg[6]		=	0;
				Tv_PtrU8Tg[7]		=	0;

				// compare
				if(Tv_SrU64 < Tv_TgU64)
				{
					Tv_Result	=	-1;
				}
				else if(Tv_SrU64 > Tv_TgU64)
				{
					Tv_Result	=	1;
				}
				break;

			case E_GrVarTypeStr:
				Tv_Result	=	GrStrCmp((char*)A_PtrSr, (char*)A_PtrTg, FALSE, (__u32)A_ValCnt);
				break;

			case E_GrVarTypeWstr:
				Tv_Result	=	GrStrWcmp((WCHAR*)A_PtrSr, (WCHAR*)A_PtrTg, FALSE, (__u32)A_ValCnt);
				break;

			default:
				// bad type
				Tv_Result	=	1;
				break;

		}	//	switch(A_ValType)

		return	Tv_Result;

}
//--------------------------------------------------------------------
#ifdef E_GrDbGrpKeySortUse
__s32	Cls_GrDbGrp::LcKeySortPosFirstSrGe(void* A_PtrKeyVal, __u32 A_IdxSt, __u32 A_IdxEd)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_SrtIdx;
		__u32	Tv_KeyIdx;
	// code --------------------
		Tv_Result	=	-1;
		Tv_SrtIdx	=	(__s32)A_IdxSt;

		while(Tv_SrtIdx >= (__s32)A_IdxEd)
		{
			// check sort item exist
			if(0 == m_KeySort[Tv_SrtIdx].KeyId)
			{
				break;
			}

			Tv_KeyIdx	=	m_KeySort[Tv_SrtIdx].KeyId - 1;
			if(0 <= LcCmpVal(A_PtrKeyVal, m_KeyTbl[Tv_KeyIdx].Atb.Key, m_KeyValType, m_KeyValCnt))
			{
				// found
				Tv_Result	=	Tv_SrtIdx;
				break;
			}

			// next
			Tv_SrtIdx --;
		}

		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------
#ifdef E_GrDbGrpKeySortUse
__s32	Cls_GrDbGrp::LcKeySortPosFirstSrLe(void* A_PtrKeyVal, __u32 A_IdxSt, __u32 A_IdxEd)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_SrtIdx;
		__u32	Tv_KeyIdx;
	// code --------------------
		Tv_Result	=	-1;
		Tv_SrtIdx	=	(__s32)A_IdxSt;

		while(Tv_SrtIdx <= (__s32)A_IdxEd)
		{
			// check sort item exist
			if(0 == m_KeySort[Tv_SrtIdx].KeyId)
			{
				break;
			}

			Tv_KeyIdx	=	m_KeySort[Tv_SrtIdx].KeyId - 1;
			if(0 >= LcCmpVal(A_PtrKeyVal, m_KeyTbl[Tv_KeyIdx].Atb.Key, m_KeyValType, m_KeyValCnt))
			{
				// found
				Tv_Result	=	Tv_SrtIdx;
				break;
			}

			// next
			Tv_SrtIdx ++;
		}

		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------
#ifdef E_GrDbGrpKeySortUse
void	Cls_GrDbGrp::LcKeySortIns(__u32 A_KeyId, __u32 A_SrtIdx)
{
	// local -------------------
		__u32	Tv_MovSize;
	// code --------------------
		// check need make a gap
		if(A_SrtIdx < m_KeySrtCnt)
		{
			// make a gap
			Tv_MovSize	=	(m_KeySrtCnt - A_SrtIdx) * sizeof(St_GrDbGrpKeySortItem);
			GrDumyMoveMem(&m_KeySort[A_SrtIdx + 1], &m_KeySort[A_SrtIdx], Tv_MovSize);
		}

		// insert
		m_KeySort[A_SrtIdx].KeyId	=	A_KeyId;
		m_KeySrtCnt ++;

}
#endif
//--------------------------------------------------------------------
#ifdef E_GrDbGrpKeySortUse
void	Cls_GrDbGrp::LcKeySortAdd(__u32 A_KeyId)
{
	// local -------------------
		__s32	Tv_SrtIdx;
	// code --------------------
		Tv_SrtIdx	=	0;

		if(0 != m_KeySrtCnt)
		{
			Tv_SrtIdx	=	LcKeySortPosFirstSrLe(m_KeyTbl[A_KeyId - 1].Atb.Key, 0, m_KeySrtCnt - 1);
			if(0 > Tv_SrtIdx)
			{
				Tv_SrtIdx	=	m_KeySrtCnt;
			}
		}

		// insert
		LcKeySortIns(A_KeyId, Tv_SrtIdx);

}
#endif
//--------------------------------------------------------------------
#ifdef E_GrDbGrpKeySortUse
void	Cls_GrDbGrp::LcKeySortBuild(void)
{
	// local -------------------
		__u32	Tv_TblIdx;
		__s32	Tv_SrtIdx;
	// code --------------------
		m_KeySrtCnt	=	0;

		// reset sort table
		GrDumyZeroMem(m_KeySort, sizeof(m_KeySort));

		Tv_SrtIdx		=	0;
		for(Tv_TblIdx = 0; Tv_TblIdx < m_KeyIdLast; Tv_TblIdx++)
		{
			// check exist item
			if(0 != m_KeyTbl[Tv_TblIdx].Atb.Key[0])
			{
				// exist item
				Tv_SrtIdx		=	0;
				if(0 != m_KeySrtCnt)
				{
					Tv_SrtIdx	=	LcKeySortPosFirstSrLe(m_KeyTbl[Tv_TblIdx].Atb.Key, 0, m_KeySrtCnt - 1);
					if(0 > Tv_SrtIdx)
					{
						Tv_SrtIdx	=	m_KeySrtCnt;
					}
				}
				// insert
				LcKeySortIns(Tv_TblIdx + 1, Tv_SrtIdx);
			}
		}
}
#endif
//--------------------------------------------------------------------
__u32	Cls_GrDbGrp::KeyIdGetByKeyVal(void* A_PtrKeyVal)
{
#ifdef E_GrDbGrpKeySortUse
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_SrtIdx;
		__u32	Tv_FndStep;
		__u32	Tv_KeyIdTg;
		__s32	Tv_CmpStat;
		BOOL8	Tv_IsDirInc;
		BOOL8	Tv_IsIncCpl;
		BOOL8	Tv_IsDecCpl;
	// code --------------------
		if(0 == m_KeySrtCnt)
		{
			return	0;
		}

		Tv_Result	=	0;

		// find fron sorted buffer
		Tv_IsDirInc	=	TRUE;
		Tv_IsIncCpl	=	FALSE;
		Tv_IsDecCpl	=	FALSE;
		Tv_SrtIdx		=	0;
		Tv_FndStep	=	m_KeySrtCnt;

		while(TRUE)
		{
			// compare 
			Tv_KeyIdTg	=	m_KeySort[Tv_SrtIdx].KeyId;
			Tv_CmpStat	=	LcCmpVal(A_PtrKeyVal, m_KeyTbl[Tv_KeyIdTg - 1].Atb.Key, m_KeyValType, m_KeyValCnt);
			if(0 == Tv_CmpStat)
			{
				// found
				Tv_Result	=	Tv_KeyIdTg;
				break;
			}

			if(Tv_IsDirInc)
			{
				// direction increase
				if(0 > Tv_CmpStat)
				{
					// smaller source
					if(1 == Tv_FndStep)
					{
						Tv_IsIncCpl	=	TRUE;
						if(Tv_IsDecCpl)
						{
							// stop
							break;
						}
					}
					// do reverse
					Tv_IsDirInc	=	FALSE;
				}
				else
				{
					// bigger source
					if((Tv_SrtIdx + 1) >= m_KeySrtCnt)
					{
						// arrived end
						break;
					}
				}
			}	//	if(Tv_IsDirInc)
			else
			{
				// direction decrease
				if(0 < Tv_CmpStat)
				{
					// bigger source
					if(1 == Tv_FndStep)
					{
						Tv_IsDecCpl	=	TRUE;
						if(Tv_IsIncCpl)
						{
							// stop
							break;
						}
					}
					// do reverse
					Tv_IsDirInc	=	TRUE;
				}
				else
				{
					// smaller source
					if(0 == Tv_SrtIdx)
					{
						// arrived end
						break;
					}
				}
			}	// if(Tv_IsDirInc) .. else

			// go next
			Tv_FndStep	=	Tv_FndStep >> 1;
			if(0 == Tv_FndStep)
			{
				Tv_FndStep ++;
			}

			if(Tv_IsDirInc)
			{
				Tv_SrtIdx	=	Tv_SrtIdx + Tv_FndStep;
				if(m_KeySrtCnt <= Tv_SrtIdx)
				{
					Tv_SrtIdx	=	m_KeySrtCnt - 1;
				}
			}
			else
			{
				if(Tv_SrtIdx < Tv_FndStep)
				{
					Tv_SrtIdx	=	0;
				}
				else
				{
					Tv_SrtIdx	=	Tv_SrtIdx - Tv_FndStep;
				}
			}

		}	//	while(TRUE)

		return	Tv_Result;
#else
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_KeyIdTg;
		__s32	Tv_CmpStat;
	// code --------------------
		Tv_Result	=	0;

		for(Tv_KeyIdTg = 1; Tv_KeyIdTg <= E_GrDbKeyMaxCnt; Tv_KeyIdTg++)
		{
			// check key exist
			Tv_CmpStat	=	LcCmpVal(A_PtrKeyVal, m_KeyTbl[Tv_KeyIdTg - 1].Atb.Key, m_KeyValType, m_KeyValCnt);
			if(0 == Tv_CmpStat)
			{
				// found
				Tv_Result	=	Tv_KeyIdTg;
				break;
			}
		}

		return	Tv_Result;
#endif
}
//--------------------------------------------------------------------
__u32	Cls_GrDbGrp::LcKeyIdFindEmpty(void)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_TblIdx;
	// code --------------------
		Tv_Result	=	E_GrDbKeyIdNone;

		if(0 != m_KeyIdLast)
		{
			for(Tv_TblIdx = 0; Tv_TblIdx < m_KeyIdLast; Tv_TblIdx++)
			{
				if(m_KeyTbl[Tv_TblIdx].Atb.IsDel)
				{
					// found
					Tv_Result	=	Tv_TblIdx + 1;
					break;
				}

				if(0 == m_KeyTbl[Tv_TblIdx].Atb.Key[0])
				{
					// found
					Tv_Result	=	Tv_TblIdx + 1;
					break;
				}
			}
		}

		if(E_GrDbKeyIdNone == Tv_Result)
		{
			if((m_KeyIdLast + 1) < E_GrDbKeyMaxCnt)
			{
				Tv_Result	=	m_KeyIdLast + 1;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDbGrp::LcDatCchFindOldest(void)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_CchIdx;
		__u32	Tv_FndTick;
	// code --------------------
		Tv_Result		=	-1;
		Tv_FndTick	=	0;

		for(Tv_CchIdx = 0; Tv_CchIdx < E_GrDbGrpDataCacheCnt; Tv_CchIdx++)
		{
			if(E_GrDbKeyIdNone != m_DatCch[Tv_CchIdx].KeyId)
			{
				if(0 > Tv_Result)
				{
					Tv_Result		=	Tv_CchIdx;
					Tv_FndTick	=	m_DatCch[Tv_CchIdx].Tick;
				}
				else
				{
					if(Tv_FndTick > m_DatCch[Tv_CchIdx].Tick)
					{
						Tv_Result		=	Tv_CchIdx;
						Tv_FndTick	=	m_DatCch[Tv_CchIdx].Tick;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDbGrp::LcDatCchFindEmpty(void)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_CchIdx;
	// code --------------------
		Tv_Result		=	-1;

		for(Tv_CchIdx = 0; Tv_CchIdx < E_GrDbGrpDataCacheCnt; Tv_CchIdx++)
		{
			if(E_GrDbKeyIdNone == m_DatCch[Tv_CchIdx].KeyId)
			{
				// found
				Tv_Result	=	Tv_CchIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDbGrp::LcKeyWriteFlush(void)
{
	// local -------------------
		__s64	Tv_Fpos;
		__u32	Tv_WtSize;
	// code --------------------
		// check exist changed data
		if((0 != m_KeyWt.KeyIdSt) && (0 != m_KeyWt.KeyIdEd))
		{
			// seek
			Tv_Fpos		=	(__s64)((m_KeyWt.KeyIdSt - 1) * sizeof(St_GrDbKeyInfo));
			if(Tv_Fpos == m_FcKey->Seek(Tv_Fpos))
			{
				// do write
				Tv_WtSize	=	(m_KeyWt.KeyIdEd - m_KeyWt.KeyIdSt + 1) * sizeof(St_GrDbKeyInfo);

				if(Tv_WtSize != m_FcKey->Write(&m_KeyTbl[m_KeyWt.KeyIdSt -1], Tv_WtSize))
				{
					DbgMsgPrint("Cls_GrDbGrp::LcKeyWriteFlush - file write error!\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDbGrp::LcKeyWriteFlush - file seek error!\n");
			}

			// reset
			m_KeyWt.KeyIdSt		=	0;
			m_KeyWt.KeyIdEd		=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDbGrp::LcKeyWriteAdd(__u32 A_KeyId)
{
	// local -------------------
		__u32	Tv_StId;
		__u32	Tv_EdId;
	// code --------------------
		// check gap over
		Tv_StId	=	m_KeyWt.KeyIdSt;
		Tv_EdId	=	m_KeyWt.KeyIdEd;

		if((0 == Tv_StId) || (A_KeyId < Tv_StId))
		{
			Tv_StId	=	A_KeyId;
		}

		if((0 == Tv_EdId) || (A_KeyId > Tv_EdId))
		{
			Tv_EdId	=	A_KeyId;
		}

		// check new range
		if(E_GrDbGrpKeyWriteImmGap <= (Tv_EdId - Tv_StId))
		{
			LcKeyWriteFlush();		// flush now
			Tv_StId	=	A_KeyId;
			Tv_EdId	=	A_KeyId;
		}

		// update new id
		m_KeyWt.KeyIdSt		=	Tv_StId;
		m_KeyWt.KeyIdEd		=	Tv_EdId;

}
//--------------------------------------------------------------------
void	Cls_GrDbGrp::RtlTimer(void)
{
	// local -------------------
		__u32	Tv_CchIdx;
	// code --------------------
		m_Tick ++;

		// key flush flush
		if((0 != m_KeyWt.KeyIdSt) && (0 != m_KeyWt.KeyIdEd))
		{
			m_KeyWriteTimer ++;
			if(E_GrDbGrpKeyWritePeriod <= m_KeyWriteTimer)
			{
				LcKeyWriteFlush();
				m_KeyWriteTimer		=	0;
			}
		}
		else
		{
			m_KeyWriteTimer	=	0;
		}

		// data cache flush
		for( Tv_CchIdx = 0; Tv_CchIdx < E_GrDbGrpDataCacheCnt; Tv_CchIdx++)
		{
			if(1 == m_DatCch[Tv_CchIdx].TimerFlush)
			{
				LcDatCchFlushByCchIdx(Tv_CchIdx);
			}
			else if(1 < m_DatCch[Tv_CchIdx].TimerFlush)
			{
				m_DatCch[Tv_CchIdx].TimerFlush --;
			}
		}
}
//--------------------------------------------------------------------
__s32	Cls_GrDbGrp::LcDatCchFindByKeyId(__u32 A_KeyId, __u32 A_BlkId)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_CchIdx;
	// code --------------------
		Tv_Result	=	-1;

		for(Tv_CchIdx = 0; Tv_CchIdx < E_GrDbGrpDataCacheCnt; Tv_CchIdx++)
		{
			if((A_KeyId == m_DatCch[Tv_CchIdx].KeyId) && (A_BlkId == m_DatCch[Tv_CchIdx].BlkId))
			{
				// found
				Tv_Result	=	Tv_CchIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDbGrp::LcDatCchFlushByCchIdx(__u32 A_CchIdx)
{
	// local -------------------
	// code --------------------
		// check cache index range
		if(E_GrDbGrpDataCacheCnt <= A_CchIdx)
		{
			return;
		}

		// check empty cache
		if(E_GrDbKeyIdNone == m_DatCch[A_CchIdx].KeyId)
		{
			return;
		}

		// check dirt cache
		if(0 == m_DatCch[A_CchIdx].TimerFlush)
		{
			return;
		}

		// write cache data
		if(m_DatCch[A_CchIdx].Fpos == m_FcDat->Seek(m_DatCch[A_CchIdx].Fpos))
		{
			if(E_GrDbDatBlkSize != m_FcDat->Write(m_DatCch[A_CchIdx].Buf, E_GrDbDatBlkSize))
			{
				DbgMsgPrint("Cls_GrDbGrp::LcDatCchFlushByCchIdx - key %d block %d write fail!\n", m_DatCch[A_CchIdx].KeyId, m_DatCch[A_CchIdx].BlkId);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDbGrp::LcDatCchFlushByCchIdx - key %d block %d seek fail!\n", m_DatCch[A_CchIdx].KeyId, m_DatCch[A_CchIdx].BlkId);
		}
		// reset flush timer
		m_DatCch[A_CchIdx].TimerFlush	=	0;

}
//--------------------------------------------------------------------
__s32	Cls_GrDbGrp::LcDatCchObtain(__u32 A_KeyId, __u32 A_BlkId, BOOL8 A_IsRead)
{
	// local -------------------
		__s32		Tv_CchIdx;
	// code --------------------
		// check key exist
		if(0 == m_KeyTbl[A_KeyId-1].Atb.Key[0])
		{
			DbgMsgPrint("Cls_GrDbGrp::LcDatCchObtain - key %d not exist!\n", A_KeyId);
			return	-1;
		}
		// check key deleted
		if(m_KeyTbl[A_KeyId-1].Atb.IsDel)
		{
			DbgMsgPrint("Cls_GrDbGrp::LcDatCchObtain - key %d deleted!\n", A_KeyId);
			//return	-1;
		}
		// check block index
		if((E_GrDbDatBlkIdKeyInfo == A_BlkId) || (A_BlkId > E_GrDbDatBlkMaxCnt))
		{
			DbgMsgPrint("Cls_GrDbGrp::LcDatCchObtain - bad block id %d input!\n", A_BlkId);
			return	-1;
		}

		// find block on cache table
		Tv_CchIdx	=	LcDatCchFindByKeyId(A_KeyId, A_BlkId);
		if(0 <= Tv_CchIdx)
		{
			// found match cache
			return	Tv_CchIdx;
		}

		if(0 > Tv_CchIdx)
		{
			// find empty cache
			Tv_CchIdx	=	LcDatCchFindEmpty();
		}

		if(0 > Tv_CchIdx)
		{
			// find oldest cache
			Tv_CchIdx	=	LcDatCchFindOldest();
			if(0 <= Tv_CchIdx)
			{
				// flush cache slot
				LcDatCchFlushByCchIdx((__u32)Tv_CchIdx);
			}
		}

		// check able
		if(0 > Tv_CchIdx)
		{
			DbgMsgPrint("Cls_GrDbGrp::LcDatCchObtain - available cache not found!\n");
			return -1;
		}
		// build info
		// calculate file position
		m_DatCch[Tv_CchIdx].Fpos				=	(E_GrDbDatSize * (__s64)(A_KeyId - 1)) + (__s64)((A_BlkId - 1) << E_GrDbDatBlkShft);
		m_DatCch[Tv_CchIdx].KeyId				=	A_KeyId;
		m_DatCch[Tv_CchIdx].BlkId				=	A_BlkId;
		m_DatCch[Tv_CchIdx].Tick				=	m_Tick;
		m_DatCch[Tv_CchIdx].TimerFlush	=	0;

		// read data
		if(A_IsRead)
		{
			if(m_DatCch[Tv_CchIdx].Fpos == m_FcDat->Seek(m_DatCch[Tv_CchIdx].Fpos))
			{
				if(E_GrDbDatBlkSize != m_FcDat->Read(m_DatCch[Tv_CchIdx].Buf, E_GrDbDatBlkSize))
				{
					DbgMsgPrint("Cls_GrDbGrp::LcDatCchObtain - key %d blk %d read fail!!\n", A_KeyId, A_BlkId);
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDbGrp::LcDatCchObtain - key %d blk %d seek fail!!\n", A_KeyId, A_BlkId);
			}
		}

		return	Tv_CchIdx;
}
//--------------------------------------------------------------------
__u8	Cls_GrDbGrp::LcDatWrite(__u32 A_KeyId, __u32 A_Ofs, __u32 A_Size, void* A_PtrSr)
{
	// local -------------------
		__u32	Tv_OfsDat;
		__u32	Tv_Remain;
		void*	Tv_PtrSr;
		void*	Tv_PtrTg;
		__s32	Tv_CchIdx;
		__u32	Tv_BlkId;
		__u32	Tv_CpSize;
		__u32	Tv_CpOfs;
		__u32	Tv_Mili;
		BOOL8	Tv_IsRead;
	// code --------------------
		// check key exist
		if( 0 == m_KeyTbl[A_KeyId-1].Atb.Key[0] )
		{
			DbgMsgPrint("Cls_GrDbGrp::LcDatWrite - key %d not exist!\n", A_KeyId);
			return	E_GrErrNotFound;
		}
		// check key deleted
		if(m_KeyTbl[A_KeyId-1].Atb.IsDel)
		{
			DbgMsgPrint("Cls_GrDbGrp::LcDatWrite - key %d deleted!\n", A_KeyId);
			//return	E_GrErrNotFound;
		}
		
		// write progress
		Tv_OfsDat		=	A_Ofs;
		Tv_Remain		=	A_Size;
		Tv_PtrSr		=	A_PtrSr;

		while(0 != Tv_Remain)
		{
			// block index calculate
			Tv_BlkId	=	(Tv_OfsDat >> E_GrDbDatBlkShft) + 1;

			// calculate copy parametter
			Tv_CpOfs	=	Tv_OfsDat & E_GrDbDatBlkMaskRem;
			Tv_CpSize	=	Tv_Remain;
			if((Tv_CpOfs + Tv_CpSize) > E_GrDbDatBlkSize)
			{
				Tv_CpSize	=	E_GrDbDatBlkSize - Tv_CpOfs;
			}

			// request data block
			Tv_IsRead	=	TRUE;
			if(E_GrDbDatBlkSize <= Tv_CpSize)
			{
				Tv_IsRead	=	FALSE;
			}
			Tv_CchIdx	=	LcDatCchObtain(A_KeyId, Tv_BlkId, Tv_IsRead);
			if(0 > Tv_CchIdx)
			{
				DbgMsgPrint("Cls_GrDbGrp::LcDatWrite - key %d blk %d open fail!\n", A_KeyId, Tv_BlkId);
				break;
			}

			// copy data
			Tv_PtrTg	=	(void*)((Def_GrCalPtr)m_DatCch[Tv_CchIdx].Buf + (Def_GrCalPtr)Tv_CpOfs);
			GrDumyCopyMem(Tv_PtrTg, Tv_PtrSr, Tv_CpSize);

			// replace checksum
			m_KeyTbl[A_KeyId - 1].BlkStat[Tv_BlkId - 1].ChkSum		=	LcBlkChkSumCalc(m_DatCch[Tv_CchIdx].Buf);

			// repleace time update
			m_KeyTbl[A_KeyId - 1].BlkStat[Tv_BlkId - 1].TimeUpdt	=	GrTimeSysTimeGet(&Tv_Mili);

			// mark dirty
			m_DatCch[Tv_CchIdx].TimerFlush	=	E_GrDbGrpDataCchFlushPeriod;
			m_DatCch[Tv_CchIdx].Tick				=	m_Tick;

			// next
			Tv_OfsDat	=	Tv_BlkId << E_GrDbDatBlkShft;
			Tv_PtrSr	=	(void*)((Def_GrCalPtr)Tv_PtrSr + Tv_CpSize);
			Tv_Remain	=	Tv_Remain - Tv_CpSize;
		}

		return	E_GrErrNone;
}
//--------------------------------------------------------------------
__u8	Cls_GrDbGrp::LcDatRead(__u32 A_KeyId, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		__u32	Tv_OfsDat;
		__u32	Tv_Remain;
		void*	Tv_PtrSr;
		void*	Tv_PtrTg;
		__s32	Tv_CchIdx;
		__u32	Tv_BlkId;
		__u32	Tv_CpSize;
		__u32	Tv_CpOfs;
	// code --------------------
		// check key exist
		if(0 == m_KeyTbl[A_KeyId-1].Atb.Key[0])
		{
			DbgMsgPrint("Cls_GrDbGrp::LcDatRead - key %d not exist!\n", A_KeyId);
			return	E_GrErrNotFound;
		}
		// check key deleted
		if(m_KeyTbl[A_KeyId-1].Atb.IsDel)
		{
			DbgMsgPrint("Cls_GrDbGrp::LcDatRead - key %d deleted!\n", A_KeyId);
			//return	E_GrErrNotFound;
		}

		// write progress
		Tv_OfsDat		=	A_Ofs;
		Tv_Remain		=	A_Size;
		Tv_PtrTg		=	A_PtrBuf;

		while(0 != Tv_Remain)
		{
			Tv_BlkId	=	(Tv_OfsDat >> E_GrDbDatBlkShft) + 1;
			// request data block
			Tv_CchIdx	=	LcDatCchObtain(A_KeyId, Tv_BlkId, TRUE);
			if(0 > Tv_CchIdx)
			{
				DbgMsgPrint("Cls_GrDbGrp::LcDatRead - key %d blk %d open fail!\n", A_KeyId, Tv_BlkId);
				return	E_GrErrBadIo;
			}

			// copy data
			Tv_CpOfs	=	Tv_OfsDat & E_GrDbDatBlkMaskRem;
			Tv_CpSize	=	Tv_Remain;
			if((Tv_CpOfs + Tv_CpSize) > E_GrDbDatBlkSize)
			{
				Tv_CpSize	=	E_GrDbDatBlkSize - Tv_CpOfs;
			}
			Tv_PtrSr	=	(void*)((Def_GrCalPtr)m_DatCch[Tv_CchIdx].Buf + (Def_GrCalPtr)Tv_CpOfs);
			GrDumyCopyMem(Tv_PtrTg, Tv_PtrSr, Tv_CpSize);

			// next
			Tv_OfsDat	=	Tv_BlkId << E_GrDbDatBlkShft;
			Tv_PtrTg	=	(void*)((Def_GrCalPtr)Tv_PtrTg + Tv_CpSize);
			Tv_Remain	=	Tv_Remain - Tv_CpSize;
		}

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
__u8	Cls_GrDbGrp::KeyCreate(void* A_PtrKeyVal, __u32* A_PtrRtKeyId)
{
	// local -------------------
		__u32	Tv_KeyId;
		__u32	Tv_BlkId;
		__u32	Tv_Mili;
		BOOL8	Tv_IsAddNew;
	// code --------------------
		// init
		*A_PtrRtKeyId	=	E_GrDbKeyIdNone;
		Tv_IsAddNew	=	FALSE;
		// check alreay equal key
		Tv_KeyId	=	KeyIdGetByKeyVal(A_PtrKeyVal);
		if(E_GrDbKeyIdNone != Tv_KeyId)
		{
			// check deleted
			if(!m_KeyTbl[Tv_KeyId -1].Atb.IsDel)
			{
				// already exist
				return	E_GrErrAlreadyExist;
			}
		}

		// find empty key slot
		if(E_GrDbKeyIdNone == Tv_KeyId)
		{
			Tv_KeyId	=	LcKeyIdFindEmpty();
			if(E_GrDbKeyIdNone == Tv_KeyId)
			{
				// empty slot not found
				return	E_GrErrBufFull;
			}
			Tv_IsAddNew	=	TRUE;
		}

		// key create
		GrDumyCopyMem(m_KeyTbl[Tv_KeyId - 1].Atb.Key, A_PtrKeyVal, E_GrDbKeySize);
		m_KeyTbl[Tv_KeyId - 1].Atb.IsDel	=	FALSE;
		// update time
		if(!Tv_IsAddNew)
		{
			m_KeyTbl[Tv_KeyId - 1].Atb.TimeAtbUpdt	=	GrTimeSysTimeGet(&Tv_Mili);
		}

		// key sort table resort
		if(Tv_IsAddNew)
		{
#ifdef E_GrDbGrpKeySortUse
			LcKeySortAdd(Tv_KeyId);
#endif
			if(Tv_KeyId > m_KeyIdLast)
			{
				m_KeyIdLast		=	Tv_KeyId;
			}
			m_KeyTotal ++;
		}

		// add key info write request
		LcKeyWriteAdd(Tv_KeyId);

		// data write
		for(Tv_BlkId = 1; Tv_BlkId <= E_GrDbDatBlkMaxCnt; Tv_BlkId++)
		{
			LcBlkFillZero(Tv_KeyId, Tv_BlkId, 0);
		}

		*A_PtrRtKeyId	=	Tv_KeyId;
		return	E_GrErrNone;
}
//--------------------------------------------------------------------
__u8	Cls_GrDbGrp::KeyDelByKeyVal(void* A_PtrKeyVal)
{
	// local -------------------
		__u32	Tv_KeyId;
		//__u32	Tv_BlkId;
		__u32	Tv_Mili;
	// code --------------------
		// init

		// check alreay equal key
		Tv_KeyId	=	KeyIdGetByKeyVal(A_PtrKeyVal);
		if(E_GrDbKeyIdNone == Tv_KeyId)
		{
			return	E_GrErrNotFound;
		}

		// check deleted
		if(m_KeyTbl[Tv_KeyId -1].Atb.IsDel)
		{
			// already exist
			return	E_GrErrBadParam;
		}

		// set mark delete
		m_KeyTbl[Tv_KeyId - 1].Atb.IsDel				=	TRUE;
		m_KeyTbl[Tv_KeyId - 1].Atb.TimeAtbUpdt	=	GrTimeSysTimeGet(&Tv_Mili);

		// add key info write request
		LcKeyWriteAdd(Tv_KeyId);

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
__u8	Cls_GrDbGrp::WriteDataByKeyId(__u32 A_KeyId, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
	// code --------------------
		if((E_GrDbKeyIdNone == A_KeyId) || (E_GrDbKeyMaxCnt < A_KeyId))
		{
			DbgMsgPrint("Cls_GrDbGrp::WriteDataByKeyId - bad keyid %d input!\n", A_KeyId);
			return	E_GrErrBadParam;
		}

		// check offset and size
		if((A_Ofs + A_Size) > E_GrDbDatSize)
		{
			DbgMsgPrint("Cls_GrDbGrp::WriteDataByKeyId - bad offset %d or size %d input!\n", A_Ofs, A_Size);
			return	E_GrErrBadParam;
		}

		// check deleted
		if(m_KeyTbl[A_KeyId - 1].Atb.IsDel)
		{
			DbgMsgPrint("Cls_GrDbGrp::WriteDataByKeyId - keyid %d is deleted!\n", A_KeyId);
			return	E_GrErrNotFound;
		}

		// write
		return	LcDatWrite(A_KeyId, A_Ofs, A_Size, A_PtrBuf);

}
//--------------------------------------------------------------------
__u8	Cls_GrDbGrp::ReadDataByKeyId(__u32 A_KeyId, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
	// code --------------------
		if((E_GrDbKeyIdNone == A_KeyId) || (E_GrDbKeyMaxCnt < A_KeyId))
		{
			DbgMsgPrint("Cls_GrDbGrp::ReadDataByKeyId - bad keyid %d input!\n", A_KeyId);
			return	E_GrErrBadParam;
		}

		// check offset and size
		if((A_Ofs + A_Size) > E_GrDbDatSize)
		{
			DbgMsgPrint("Cls_GrDbGrp::ReadDataByKeyId - bad offset %d or size %d input!\n", A_Ofs, A_Size);
			return	E_GrErrBadParam;
		}

		// check deleted
		if(m_KeyTbl[A_KeyId - 1].Atb.IsDel)
		{
			DbgMsgPrint("Cls_GrDbGrp::ReadDataByKeyId - keyid %d is deleted!\n", A_KeyId);
			return	E_GrErrNotFound;
		}

		// read
		return	LcDatRead(A_KeyId, A_Ofs, A_Size, A_PtrBuf);
}
//--------------------------------------------------------------------
__u32	Cls_GrDbGrp::KeyInfosGet(__u32 A_KeyIdSt, __u32 A_MaxCnt, Ptr_GrDbKeyInfo A_PtrBuf, __u32* A_PtrRtKeyIdProc)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_KeyIdx;
	// code --------------------
		// check key id start
		if((E_GrDbKeyIdNone == A_KeyIdSt) || (E_GrDbKeyMaxCnt < A_KeyIdSt))
		{
			*A_PtrRtKeyIdProc	=	E_GrDbKeyMaxCnt + 1;
			return	0;
		}
		
		Tv_Result	=	0;
		Tv_KeyIdx	=	A_KeyIdSt - 1;

		while(Tv_KeyIdx < E_GrDbKeyMaxCnt)
		{
			// check exist key
			if(0 != m_KeyTbl[Tv_KeyIdx].Atb.Key[0])
			{
				// exist , do copy
				GrDumyCopyMem(&A_PtrBuf[Tv_Result], &m_KeyTbl[Tv_KeyIdx], sizeof(St_GrDbKeyInfo));
				Tv_Result ++;
			}
			// next
			Tv_KeyIdx ++;
			if(A_MaxCnt <= Tv_Result)
			{
				break;
			}
		}

		*A_PtrRtKeyIdProc	=	Tv_KeyIdx + 1;
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDbGrp::DataChgInfoMake(Ptr_GrDbKeyInfo A_PtrKeyInfos, __u32 A_KeyInfoCnt, Ptr_GrDbDataChgInfo A_PtrRtChgInfo)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_KisIdx;
		__u32	Tv_KeyId;
		__u32	Tv_BlkIdx;
		BOOL8	Tv_IsBody;
	// code --------------------
		Tv_Result	=	0;

		Tv_KisIdx	=	0;
		while(Tv_KisIdx < A_KeyInfoCnt)
		{
			// init change map
			A_PtrRtChgInfo[Tv_Result].ChgMap	=	0;

			// find key
			Tv_KeyId	=	KeyIdGetByKeyVal(A_PtrKeyInfos[Tv_KisIdx].Atb.Key);
			if(E_GrDbKeyIdNone == Tv_KeyId)
			{
				// mark all
				A_PtrRtChgInfo[Tv_Result].ChgMap	=	(1 << (E_GrDbDatBlkMaxCnt + 1)) - 1;
			}
			else
			{
				// head compare
				Tv_IsBody	=	TRUE;
				if(m_KeyTbl[Tv_KeyId - 1].Atb.TimeAtbUpdt < A_PtrKeyInfos[Tv_KisIdx].Atb.TimeAtbUpdt)
				{
					// assign changed
					A_PtrRtChgInfo[Tv_Result].ChgMap	=	A_PtrRtChgInfo[Tv_Result].ChgMap | 1;
				}
				else if ((m_KeyTbl[Tv_KeyId - 1].Atb.TimeAtbUpdt == A_PtrKeyInfos[Tv_KisIdx].Atb.TimeAtbUpdt) &&
					(m_KeyTbl[Tv_KeyId - 1].Atb.IsDel != A_PtrKeyInfos[Tv_KisIdx].Atb.IsDel))
				{
					A_PtrRtChgInfo[Tv_Result].ChgMap	=	A_PtrRtChgInfo[Tv_Result].ChgMap | 1;
					Tv_IsBody	=	m_KeyTbl[Tv_KeyId - 1].Atb.IsDel;
				}
				// compare body
				if(Tv_IsBody)
				{
					for(Tv_BlkIdx = 0; Tv_BlkIdx < E_GrDbDatBlkMaxCnt; Tv_BlkIdx++)
					{
						if(m_KeyTbl[Tv_KeyId - 1].BlkStat[Tv_BlkIdx].TimeUpdt < A_PtrKeyInfos[Tv_KisIdx].BlkStat[Tv_BlkIdx].TimeUpdt)
						{
							A_PtrRtChgInfo[Tv_Result].ChgMap	=	A_PtrRtChgInfo[Tv_Result].ChgMap | (1 << (Tv_BlkIdx + 1));
						}
						else if((m_KeyTbl[Tv_KeyId - 1].BlkStat[Tv_BlkIdx].TimeUpdt == A_PtrKeyInfos[Tv_KisIdx].BlkStat[Tv_BlkIdx].TimeUpdt) &&
							(m_KeyTbl[Tv_KeyId - 1].BlkStat[Tv_BlkIdx].ChkSum != A_PtrKeyInfos[Tv_KisIdx].BlkStat[Tv_BlkIdx].ChkSum))
						{
							A_PtrRtChgInfo[Tv_Result].ChgMap	=	A_PtrRtChgInfo[Tv_Result].ChgMap | (1 << (Tv_BlkIdx + 1));
						}
					}	//	for(Tv_BlkIdx = 0; Tv_BlkIdx < E_GrDbDatBlkMaxCnt; Tv_BlkIdx++)
				}	//	if(Tv_IsBody)

			}

			// check exist change
			if(0 != A_PtrRtChgInfo[Tv_Result].ChgMap)
			{
				GrDumyCopyMem(A_PtrRtChgInfo[Tv_Result].Key, A_PtrKeyInfos[Tv_KisIdx].Atb.Key, E_GrDbKeySize);
				Tv_Result ++;
			}

			// next
			Tv_KisIdx ++;
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
void	Cls_GrDbGrp::DataChgInfoToChgMap(Ptr_GrDbDataChgInfo A_PtrChgInfo, __u32 A_InfoCnt, __u32* A_PtrMap)
{
	// local -------------------
		__u32	Tv_InfIdx;
		__u32	Tv_KeyId;
	// code --------------------
		Tv_InfIdx		=	0;

		while(Tv_InfIdx < A_InfoCnt)
		{
			// get key id
			Tv_KeyId	=	KeyIdGetByKeyVal(A_PtrChgInfo[Tv_InfIdx].Key);
			if(E_GrDbKeyIdNone != Tv_KeyId)
			{
				A_PtrMap[Tv_KeyId - 1]	=	A_PtrMap[Tv_KeyId - 1] | A_PtrChgInfo[Tv_InfIdx].ChgMap;
			}
			// next
			Tv_InfIdx ++;
		}

}
//--------------------------------------------------------------------
__u32	Cls_GrDbGrp::KeyInfoGetByKeyVal(void* A_PtrKeyVal, Ptr_GrDbKeyInfo A_PtrRtKeyInfo)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		// find key id
		Tv_Result = KeyIdGetByKeyVal(A_PtrKeyVal);
		if (E_GrDbKeyIdNone == Tv_Result)
		{
			return	E_GrDbKeyIdNone;
		}

		GrDumyCopyMem(A_PtrRtKeyInfo, &m_KeyTbl[Tv_Result - 1], sizeof(St_GrDbKeyInfo));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbGrp::KeyInfoGetByKeyId(__u32 A_KeyId, Ptr_GrDbKeyInfo A_PtrRtBuf)
{
	// local -------------------
	// code --------------------
		// check key id
		if ((E_GrDbKeyIdNone == A_KeyId) || (E_GrDbKeyMaxCnt < A_KeyId))
		{
			return	FALSE;
		}

		// do copy
		GrDumyCopyMem(A_PtrRtBuf, &m_KeyTbl[A_KeyId - 1], sizeof(St_GrDbKeyInfo));
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbGrp::ReadBlkDataAndInfoByKeyId(__u32 A_KeyId, __u32 A_BlkId, void* A_PtrRtBlkBuf, void* A_PtrRtKeyVal,
	__u32* A_PtrRtChkSum, __u32* A_PtrRtTmUpdt)
{
	// local -------------------
		__s32	Tv_CchIdx;
	// code --------------------
		*A_PtrRtChkSum	=	0;
		// check key id
		if((E_GrDbKeyIdNone == A_KeyId) || (E_GrDbKeyMaxCnt < A_KeyId))
		{
			return	FALSE;
		}
		// check block index
		if((E_GrDbDatBlkIdKeyInfo == A_BlkId) || (E_GrDbDatBlkMaxCnt < A_BlkId))
		{
			return	FALSE;
		}

		// get block data
		Tv_CchIdx	=	LcDatCchObtain(A_KeyId, A_BlkId,TRUE);
		if (0 > Tv_CchIdx)
		{
			return	FALSE;
		}

		GrDumyCopyMem(A_PtrRtBlkBuf, m_DatCch[Tv_CchIdx].Buf, E_GrDbDatBlkSize);
		GrDumyCopyMem(A_PtrRtKeyVal, m_KeyTbl[A_KeyId -1].Atb.Key, E_GrDbKeySize);

		*A_PtrRtChkSum	=	m_KeyTbl[A_KeyId - 1].BlkStat[A_BlkId - 1].ChkSum;
		*A_PtrRtTmUpdt	=	m_KeyTbl[A_KeyId - 1].BlkStat[A_BlkId - 1].TimeUpdt;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbGrp::KeyAtbUpdate(Ptr_GrDbKeyAtb A_PtrKeyAtb, BOOL8 A_IsForce)
{
	// local -------------------
		__u32	Tv_KeyId;
	// code --------------------
		// find key
		Tv_KeyId	=	KeyIdGetByKeyVal(A_PtrKeyAtb->Key);
		if(E_GrDbKeyIdNone == Tv_KeyId)
		{
			// create
			KeyCreate(A_PtrKeyAtb->Key, &Tv_KeyId);
			if(E_GrDbKeyIdNone == Tv_KeyId)
			{
				return	FALSE;
			}
		}

		// update key attribute
		if(A_IsForce || (A_PtrKeyAtb->TimeAtbUpdt > m_KeyTbl[Tv_KeyId - 1].Atb.TimeAtbUpdt))
		{
			m_KeyTbl[Tv_KeyId - 1].Atb.IsDel				=	A_PtrKeyAtb->IsDel;
			m_KeyTbl[Tv_KeyId - 1].Atb.TimeAtbUpdt	=	A_PtrKeyAtb->TimeAtbUpdt;
			// request write
			LcKeyWriteAdd(Tv_KeyId);
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbGrp::WriteBlkAndTimeByKeyVal(void* A_PtrKeyVal, __u32 A_BlkId, __u32 A_TmUpdt, void* A_PtrBlkData, BOOL8 A_IsForce)
{
	// local -------------------
		__u32	Tv_KeyId;
		__s32	Tv_CchIdx;
	// code --------------------
		// check block id
		if((E_GrDbDatBlkIdKeyInfo == A_BlkId) || (E_GrDbDatBlkMaxCnt < A_BlkId))
		{
			DbgMsgPrint("Cls_GrDbGrp::WriteBlkAndTimeByKeyVal - bad block id %d input\n", A_BlkId);
			return	FALSE;
		}
		// find key
		Tv_KeyId	=	KeyIdGetByKeyVal(A_PtrKeyVal);
		if(E_GrDbKeyIdNone == Tv_KeyId)
		{
			DbgMsgPrint("Cls_GrDbGrp::WriteBlkAndTimeByKeyVal - key id not found\n");
			return	FALSE;
		}

		// check block time
		if((!A_IsForce) && (A_TmUpdt <= m_KeyTbl[Tv_KeyId - 1].BlkStat[A_BlkId - 1].TimeUpdt))
		{
			return	TRUE;
		}

		Tv_CchIdx	=	LcDatCchObtain(Tv_KeyId, A_BlkId,FALSE);
		if(0 > Tv_CchIdx)
		{
			DbgMsgPrint("Cls_GrDbGrp::WriteBlkAndTimeByKeyVal - cache optain fail!\n");
			return	FALSE;
		}

		// update data
		GrDumyCopyMem(m_DatCch[Tv_CchIdx].Buf, A_PtrBlkData, E_GrDbDatBlkSize);
		// update access time
		m_DatCch[Tv_CchIdx].TimerFlush	=	E_GrDbGrpDataCchFlushPeriod;
		m_DatCch[Tv_CchIdx].Tick				=	m_Tick;
		// upate update time
		m_KeyTbl[Tv_KeyId - 1].BlkStat[A_BlkId - 1].ChkSum		=	LcBlkChkSumCalc(m_DatCch[Tv_CchIdx].Buf);
		m_KeyTbl[Tv_KeyId - 1].BlkStat[A_BlkId - 1].TimeUpdt	=	A_TmUpdt;
		// set key info update
		LcKeyWriteAdd(Tv_KeyId);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbGrp::KeyAtbGetByKeyId(__u32 A_KeyId, Ptr_GrDbKeyAtb A_PtrRtKeyAtb)
{
	// local -------------------
	// code --------------------
		// check key id
		if((E_GrDbKeyIdNone == A_KeyId) || (E_GrDbKeyMaxCnt < A_KeyId))
		{
			return	FALSE;
		}

		// do copy
		GrDumyCopyMem(A_PtrRtKeyAtb, &m_KeyTbl[A_KeyId - 1], sizeof(St_GrDbKeyAtb));
		return	TRUE;

}
//--------------------------------------------------------------------
void	Cls_GrDbGrp::LcDataFileCreate(void)
{
	// local -------------------
		Def_WstrGrFnPath	Tv_StrCmd;
		WCHAR*	Tv_StrWk;
	// code --------------------
		Tv_StrWk	=	GrStrWcopy(Tv_StrCmd, L"fsutil file createnew ");
		Tv_StrWk	=	GrStrWcat(Tv_StrWk, m_StrFnDat);
		Tv_StrWk	=	GrStrWcat(Tv_StrWk, L" 68719476736");

		_wsystem(Tv_StrCmd);
		
}
//--------------------------------------------------------------------
__u32	Cls_GrDbGrp::LcBlkChkSumCalc(void* A_PtrBuf)
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_WkIdx;
		__u32*	Tv_PtrSr;
	// code --------------------
		Tv_Result	=	0;

		Tv_PtrSr	=	(__u32*)A_PtrBuf;
		for(Tv_WkIdx = 0; Tv_WkIdx < (E_GrDbDatBlkSize >> 2); Tv_WkIdx++)
		{
			Tv_Result	=	Tv_Result + (*Tv_PtrSr);
			// next
			Tv_PtrSr ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbGrp::LcBlkFillZero(__u32 A_KeyId, __u32 A_BlkId, __u32 A_TimeUpdt)
{
	// local -------------------
		__s32	Tv_CchIdx;
	// code --------------------
		// check key id
		if((E_GrDbKeyIdNone == A_KeyId) || (E_GrDbKeyMaxCnt < A_KeyId))
		{
			return	FALSE;
		}
		// check block index
		if((E_GrDbDatBlkIdKeyInfo == A_BlkId) || (E_GrDbDatBlkMaxCnt < A_BlkId))
		{
			return	FALSE;
		}

		Tv_CchIdx	=	LcDatCchObtain(A_KeyId, A_BlkId, FALSE);
		if(0 > Tv_CchIdx)
		{
			DbgMsgPrint("Cls_GrDbGrp::LcBlkFillZero - key %d blk %d cache obtain fail!\n", A_KeyId,A_BlkId);
		}

		GrDumyZeroMem(m_DatCch[Tv_CchIdx].Buf, E_GrDbDatBlkSize);
		// mark dirty
		m_DatCch[Tv_CchIdx].TimerFlush	=	E_GrDbGrpDataCchFlushPeriod;
		m_DatCch[Tv_CchIdx].Tick				=	m_Tick;

		// set block status
		m_KeyTbl[A_KeyId-1].BlkStat[A_BlkId-1].TimeUpdt	=	A_TimeUpdt;
		m_KeyTbl[A_KeyId-1].BlkStat[A_BlkId-1].ChkSum		=	0;
		
		LcKeyWriteAdd(A_KeyId);

		return	TRUE;

}
//--------------------------------------------------------------------
__u32	Cls_GrDbGrp::KeyFindByValRng(void* A_PtrKeySt, void* A_PtrKeyEd, void* A_PtrRtBuf, __u32 A_MaxCnt)
{
#ifdef E_GrDbGrpKeySortUse

	// local -------------------
		__u32	Tv_Result;
		__s32	Tv_SrtSti;
		__s32	Tv_SrtEdi;
		__s32	Tv_SrtIdx;
		void*	Tv_PtrTg;
		__u32	Tv_KeyId;
	// code --------------------
		// check exist
		if(0 == m_KeySrtCnt)
		{
			return	0;
		}
		if(0 == A_MaxCnt)
		{
			return	0;
		}
		Tv_SrtSti	=	0;
		Tv_SrtEdi	=	m_KeySrtCnt - 1;
		// find start
		if(LcIsKeyValExist(A_PtrKeySt))
		{
			Tv_SrtSti	=	LcKeySortPosFirstSrLe(A_PtrKeySt, 0, m_KeySrtCnt - 1);
			if(0 > Tv_SrtSti)
			{
				return	0;
			}
		}

		// find end
		if(LcIsKeyValExist(A_PtrKeyEd))
		{
			Tv_SrtEdi	=	LcKeySortPosFirstSrGe(A_PtrKeyEd, m_KeySrtCnt - 1, 0);
			if(0 > Tv_SrtEdi)
			{
				return	0;
			}
		}

		// copy result
		Tv_PtrTg	=	(void*)A_PtrRtBuf;
		Tv_Result	=	0;
		Tv_SrtIdx	=	Tv_SrtSti;

		while(Tv_SrtIdx <= Tv_SrtEdi)
		{
			Tv_KeyId	=	m_KeySort[Tv_SrtIdx].KeyId;

			// check deleted
			if(!m_KeyTbl[Tv_KeyId - 1].Atb.IsDel)
			{
				GrDumyCopyMem(Tv_PtrTg, m_KeyTbl[Tv_KeyId - 1].Atb.Key, E_GrDbKeySize);
				Tv_Result ++;
				if(Tv_Result >= A_MaxCnt)
				{
					break;
				}
				Tv_PtrTg	=	(void*)((Def_GrCalPtr)Tv_PtrTg + E_GrDbKeySize);
			}

			// next
			Tv_SrtIdx ++;
		}

		return	Tv_Result;

#else
	// local -------------------
		__u32	Tv_Result;
		BOOL8	Tv_IsStExist;
		BOOL8	Tv_IsEdExist;
		__u32	Tv_KeyId;
		void*	Tv_PtrTg;
	// code --------------------
		if(0 == A_MaxCnt)
		{
			return	0;
		}

		Tv_IsStExist	=	LcIsKeyValExist(A_PtrKeySt);
		Tv_IsEdExist	=	LcIsKeyValExist(A_PtrKeyEd);

		Tv_PtrTg	=	(void*)A_PtrRtBuf;
		Tv_Result	=	0;

		for(Tv_KeyId = 1; Tv_KeyId <= E_GrDbKeyMaxCnt; Tv_KeyId++)
		{
			if (LcIsKeyValExist(m_KeyTbl[Tv_KeyId - 1].Atb.Key))
			{
				if(!m_KeyTbl[Tv_KeyId - 1].Atb.IsDel)
				{
					// check compare
					if(Tv_IsStExist && (0 < LcCmpVal(m_KeyTbl[Tv_KeyId - 1].Atb.Key,A_PtrKeySt,m_KeyValType,m_KeyValCnt)))
					{
						continue;
					}
					if(Tv_IsEdExist && (0 > LcCmpVal(m_KeyTbl[Tv_KeyId - 1].Atb.Key, A_PtrKeyEd, m_KeyValType, m_KeyValCnt)))
					{
						continue;
					}
					GrDumyCopyMem(Tv_PtrTg, m_KeyTbl[Tv_KeyId - 1].Atb.Key, E_GrDbKeySize);
					Tv_Result ++;
					if(Tv_Result >= A_MaxCnt)
					{
						break;
					}
					Tv_PtrTg	=	(void*)((Def_GrCalPtr)Tv_PtrTg + E_GrDbKeySize);
				}
			}
		}

		return	Tv_Result;
#endif

}
//--------------------------------------------------------------------
#ifdef E_GrDbGrpKeyOvlpDel
void	Cls_GrDbGrp::LcKeyOvlpRemove(void)
{
	// local -------------------
		__u32	Tv_RmvCnt;
		__u32	Tv_SrIdx;
		__u32	Tv_TgIdx;
		__u32	Tv_Mili;
	// code --------------------
		Tv_RmvCnt	=	0;

		Tv_SrIdx		=	0;
		while(Tv_SrIdx < (E_GrDbKeyMaxCnt - 1))
		{
			if(LcIsKeyValExist(m_KeyTbl[Tv_SrIdx].Atb.Key))
			{
				// remove equal
				Tv_TgIdx	=	Tv_SrIdx + 1;
				while(Tv_TgIdx < E_GrDbKeyMaxCnt)
				{
					if(LcIsKeyValExist(m_KeyTbl[Tv_TgIdx].Atb.Key))
					{
						if(0 == LcCmpVal(m_KeyTbl[Tv_SrIdx].Atb.Key, m_KeyTbl[Tv_TgIdx].Atb.Key,m_KeyValType,m_KeyValCnt))
						{
							// delete
							GrDumyZeroMem(m_KeyTbl[Tv_TgIdx].Atb.Key, E_GrDbKeySize);
							m_KeyTbl[Tv_TgIdx].Atb.TimeAtbUpdt	= GrTimeSysTimeGet(&Tv_Mili);
							Tv_RmvCnt	++;
						}
					}
					// next
					Tv_TgIdx ++;
				}
			}
			// next
			Tv_SrIdx ++;
		}

		if(0 != Tv_RmvCnt)
		{
			DbgMsgPrint("[PATCH] %d overlapped key removed.\n", Tv_RmvCnt);
			m_KeyWt.KeyIdSt		=	1;
			m_KeyWt.KeyIdEd		=	E_GrDbKeyMaxCnt;

			LcKeyWriteFlush();
		}

}
#endif
//--------------------------------------------------------------------
