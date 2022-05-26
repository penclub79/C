/*
	DAMGI Math library


*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrMath
#define	_EPC_GrMath

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#ifdef _WINDOWS

#include <math.h>

#endif

#include <stdlib.h>

//====================================================================
//const


//====================================================================
//type

typedef	struct St_GrMathSha256Ctx 
{
	__u32	Len;
	__u32	hash[8];
	__u8	buf[64];
}	*Ptr_GrMathSha256Ctx;


//====================================================================
//class


//====================================================================
//function

#define	GrAbs32	abs
//__s32	GrAbs32( __s32 A_Val );

BOOL8		GrMathIsHaveCommon( __s32 A_St1, __s32 A_Ed1, __s32 A_St2, __s32 A_Ed2 );
BOOL8		GrMathIsHaveCommonQw( __u64 A_St1, __u64 A_Ed1, __u64 A_St2, __u64 A_Ed2 );

void	GrMathSha256Init( Ptr_GrMathSha256Ctx A_PtrCtx );
void	GrMathSha256DataIn( Ptr_GrMathSha256Ctx A_PtrCtx, void* A_PtrData, __u32 A_Len );
void	GrMathSha256Finish(Ptr_GrMathSha256Ctx A_PtrCtx, __u8* A_PtrRtCode);	/* code = 32byte */

__u16	GrMathCrc16( void* A_PtrBuf, __u32 A_Size );
__u32	GrMathCrc32NoCmp( __u32 A_PrevCrc, void* A_PtrBuf, __u32 A_Size );
__u32	GrMathCrc32( __u32 A_PrevCrc, void* A_PtrBuf, __u32 A_Size );

//====================================================================
//out Environment conditional
#endif
