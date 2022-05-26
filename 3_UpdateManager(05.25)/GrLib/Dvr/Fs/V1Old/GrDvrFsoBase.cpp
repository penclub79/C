/*
 GAUSE file system version 1 Base

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <Dvr/Fs/V1/GrDvrFs1Base.h>

#include	<stdlib.h>

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
void	GrDvrFs1HndlDec( __u64 A_Hndl, __u64* A_PtrRtRpos, __u8* A_PtrRtIntIdx, BOOL8* A_PtrRtIsOldPart, __u16* A_PtrRtOvwtId )
{
		*A_PtrRtRpos		=	A_Hndl & E_GrDvrFs1HndlPosMask;
		*A_PtrRtIntIdx	=	(__u8)( A_Hndl & E_GrDvrFs1HndlDskIdxMask );
		*A_PtrRtOvwtId	=	(__u16)( (A_Hndl >> E_GrDvrFs1HndlSeqPos) & E_GrDvrFs1HndlSeqLimite );
		*A_PtrRtIsOldPart	=	FALSE;
		if ( 0 != (E_GrDvrFs1HndlOldPart & A_Hndl) )
		{
			*A_PtrRtIsOldPart	=	TRUE;
		}
}
//--------------------------------------------------------------------
__u64	GrDvrFs1HndlEnc( __u64 A_Rpos, __u8 A_IntIdx, BOOL8 A_IsOldPart, __u16 A_OvwtId )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	A_Rpos | (__u64)A_IntIdx | ((__u64)A_OvwtId << E_GrDvrFs1HndlSeqPos);
		if ( A_IsOldPart )
		{
			Tv_Result	=	Tv_Result | E_GrDvrFs1HndlOldPart;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
