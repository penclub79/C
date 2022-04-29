/*
	mouse manager
*/

//====================================================================
// uses

#ifndef LINUX_DRV

#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Dvr/GrDvrBase.h>
#include <GrDumyTool.h>

#endif

//====================================================================
//local const



//====================================================================
//local type


//====================================================================
//local function


//====================================================================
//var

#ifndef LINUX_DRV


#endif

//====================================================================
//--------------------------------------------------------------------
__u8	GrDvrRecAtbToThumb( __u8 A_RecAtb )
{
	// local -------------------
		__u8		Tv_Result;
	// code --------------------
		Tv_Result	=	(A_RecAtb & E_GrDvrRecAtbNoramlMask);
		if ( 0 != (E_GrDvrRecAtbSpcMask & A_RecAtb) )
		{
			Tv_Result	=	Tv_Result | E_GrDvrRecAtbSpcMask;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrDvrVdoDecAbleChk(__u8 A_PrvType, __u8 A_PrvSeq, __u8 A_NowType, __u8 A_NowSeq)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ExpSeq;
		__u8	Tv_Exp2x;
		__u8	Tv_Exp4x;
	// code --------------------
		Tv_Result	=	FALSE;

		if((E_GrDvrFrmTypeVdoI <= A_NowType) && (E_GrDvrFrmTypeVdoP4x >= A_NowType))
		{
			Tv_Result	=	TRUE;

			if(E_GrDvrFrmTypeVdoI != A_NowType)
			{
				Tv_ExpSeq	=	A_PrvSeq + 1;
				if(Tv_ExpSeq != A_NowSeq)
				{
					Tv_Result	=	FALSE;
					Tv_Exp2x	=	A_PrvSeq + 2;
					Tv_Exp4x	=	A_PrvSeq + 4;
					if(E_GrDvrFrmTypeVdoI == A_PrvType)
					{
						if((E_GrDvrFrmTypeVdoP2x == A_NowType) && (Tv_Exp2x == A_NowSeq))
						{
							Tv_Result	=	TRUE;
						}
						else if((E_GrDvrFrmTypeVdoP4x == A_NowType) && (Tv_Exp4x == A_NowSeq))
						{
							Tv_Result	=	TRUE;
						}
					}
					else if(E_GrDvrFrmTypeVdoP2x == A_PrvType)
					{
						if((E_GrDvrFrmTypeVdoP4x == A_NowType) && (Tv_Exp2x == A_NowSeq))
						{
							Tv_Result	=	TRUE;
						}
					}
					else if(E_GrDvrFrmTypeVdoP4x == A_PrvType)
					{
						if((E_GrDvrFrmTypeVdoP2x == A_NowType) && (Tv_Exp2x == A_NowSeq))
						{
							Tv_Result	=	TRUE;
						}
						else if((E_GrDvrFrmTypeVdoP4x == A_NowType) && (Tv_Exp4x == A_NowSeq))
						{
							Tv_Result	=	TRUE;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifndef LINUX_DRV
void	GrDvrVdoFmtInfoGet(__u32 A_VdoFmt, __u32* A_PtrRtResX, __u32* A_PtrRtResY, BOOL8* A_PtrRtIsInterlace)
{
	// local -------------------
	// code --------------------
		*A_PtrRtResX	=	(A_VdoFmt >> E_GrDvrVdoFmtResxPos) & E_GrDvrVdoFmtResxMask;
		*A_PtrRtResY	=	(A_VdoFmt >> E_GrDvrVdoFmtResyPos) & E_GrDvrVdoFmtResyMask;
		*A_PtrRtIsInterlace	=	FALSE;
		if (0 != (E_GrDvrVdoFmtResInterlace & A_VdoFmt))
		{
			*A_PtrRtIsInterlace	=	TRUE;
		}
}
#endif
//--------------------------------------------------------------------
#ifndef LINUX_DRV
__u32	GrDvrVdoFmtMake(__u32 A_ResX, __u32 A_ResY, BOOL8 A_IsInterlace)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	A_ResX | (A_ResY << E_GrDvrVdoFmtResyPos);
		if (A_IsInterlace)
		{
			Tv_Result	=	Tv_Result | E_GrDvrVdoFmtResInterlace;
		}
		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------

