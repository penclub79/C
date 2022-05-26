/*
	linux win32 emulation
	s6ke driver emumlation
*/

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Mpeg4/GrMpeg4StrmTool.h>

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
__u32		GrMpeg4GetVoId( __u32 A_VoStartCode )
{
		return	(A_VoStartCode & E_GrMpg4VoIdMsk) >> 24;
}
//--------------------------------------------------------------------
BOOL8		GrMpeg4IsIvop( void* A_PtrVop )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32*	Tv_PtrVopStCd;
		__u8*		Tv_PtrVopType;
		__u8		Tv_VopType;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		Tv_PtrVopStCd		=	(__u32*)A_PtrVop;

		//check vop type
		if ( E_GrMpg4VopStart == *Tv_PtrVopStCd )
		{
			Tv_PtrVopType	=	(__u8*)( (__u32)A_PtrVop + 4 );
			Tv_VopType	=	( *Tv_PtrVopType >> E_GrMpg4VopShftType ) & E_GrMpg4VopMskType;
			if ( E_GrMpg4VopTypeI == Tv_VopType )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

