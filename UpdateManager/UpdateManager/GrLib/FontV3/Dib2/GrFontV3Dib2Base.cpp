/*
	Font version 3 draw base

*/

//--------------------------------------------------------------------
// uses

#include <FontV3/Dib2/GrFontV3Dib2Base.h>

#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrDebug.h>


//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

typedef struct St_GrFontV3Dib2BitDec
{
	__u8*	Ptr;
	__u32	Deced;
	__u32	Total;
	__u8	Bit;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrFontV3Dib2BitDec;

//--------------------------------------------------------------------
//local function

void	GrFontV3Dib2BitDecInit(Ptr_GrFontV3Dib2BitDec A_PtrCtrl, void* A_PtrBuf, __u32 A_TotalBit);
BOOL8	GrFontV3Dib2BitDecGet(Ptr_GrFontV3Dib2BitDec A_PtrCtrl, __u8* A_PtrRt);

//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------
void	GrFontV3Dib2BitDecInit(Ptr_GrFontV3Dib2BitDec A_PtrCtrl, void* A_PtrBuf, __u32 A_TotalBit)
{
	// local -------------------
	// code --------------------
		A_PtrCtrl->Ptr		=	(__u8*)A_PtrBuf;
		A_PtrCtrl->Total	=	A_TotalBit;
		A_PtrCtrl->Deced	=	0;
		A_PtrCtrl->Bit		=	0;
}
//--------------------------------------------------------------------
BOOL8	GrFontV3Dib2BitDecGet(Ptr_GrFontV3Dib2BitDec A_PtrCtrl, __u8* A_PtrRt)
{
	// local -------------------
		__u8	Tv_RtVal;
	// code --------------------
		*A_PtrRt	=	0;
		if(A_PtrCtrl->Deced >= A_PtrCtrl->Total)
		{
			return	FALSE;
		}

		Tv_RtVal	=	((*A_PtrCtrl->Ptr) >> A_PtrCtrl->Bit) & 0x0F;
		// go next
		A_PtrCtrl->Deced	=	A_PtrCtrl->Deced + 4;
		A_PtrCtrl->Bit		=	A_PtrCtrl->Bit ^ 4;
		if(0 == A_PtrCtrl->Bit)
		{
			A_PtrCtrl->Ptr ++;
		}

		*A_PtrRt	=	Tv_RtVal;

		return	TRUE;
}
//--------------------------------------------------------------------
void	GrFontV3Dib2Draw(Cls_GrGdib* A_Gdib, __u8 A_Pitch, void* A_PtrData, __u32 A_BitSize,
	__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsDrawTxt, BOOL8 A_IsDrawShadow, __u32* A_PtrClrTbl)
{
	// local -------------------
#if 0
		St_GrRect	Tv_RcClip;
		St_GrFontV3Dib2BitDec	Tv_Bc;
		__u8		Tv_Cmd;
		__u8		Tv_Tmp;
		__u8		Tv_TblIdx;
		__u32		Tv_VbPos;		// value bit pos
		__u32		Tv_Val;
		__s32		Tv_WkX;
		__s32		Tv_WkY;

		St_GrRect	Tv_RcLn;

		BOOL8		Tv_IsAble[5];
#endif
	// code --------------------
		// check drawable
		if((0 != A_Pitch) && (E_GrFontV3Dib2WidthMax >= A_Pitch))
		{
			A_Gdib->FontV3Dib2Draw(A_Pitch, A_PtrData, A_BitSize, A_PosX, A_PosY, A_IsDrawTxt, A_IsDrawShadow, A_PtrClrTbl);

#if 0
			// init
			GrFontV3Dib2BitDecInit(&Tv_Bc, A_PtrData, A_BitSize);

			Tv_IsAble[0]	=	A_IsDrawShadow;
			Tv_IsAble[1]	=	A_IsDrawTxt;
			Tv_IsAble[2]	=	A_IsDrawTxt;
			Tv_IsAble[3]	=	A_IsDrawTxt;
			Tv_IsAble[4]	=	A_IsDrawTxt;

			// get crop area
			A_Gdib->ClipperGet(&Tv_RcClip);

			Tv_WkX			=	A_PosX;
			Tv_WkY			=	A_PosY;

			while(Tv_WkY < Tv_RcClip.bottom)
			{
				// get command
				if(!GrFontV3Dib2BitDecGet(&Tv_Bc, &Tv_Tmp))
				{
					break;
				}
				Tv_Cmd	=	Tv_Tmp & 7;

				// get value
				Tv_Val		=	0;
				if(0 != (E_GrFontV3Dib2AtbNext & Tv_Tmp))
				{
					Tv_VbPos	=	0;
					// get next value
					while(GrFontV3Dib2BitDecGet(&Tv_Bc, &Tv_Tmp))
					{
						Tv_Val	=	Tv_Val | (((__u32)(Tv_Tmp & 7)) << Tv_VbPos);
						if(0 == (E_GrFontV3Dib2AtbNext & Tv_Tmp))
						{
							break;
						}
						// next
						Tv_VbPos	=	Tv_VbPos + 3;
					}
				}
				Tv_Val ++;

				// run command
				if(E_GrFontV3Dib2CmdLnSkip == Tv_Cmd)
				{
					Tv_WkX	=	A_PosX;
					Tv_WkY	=	Tv_WkY + (__s32)Tv_Val;
				}
				else if(E_GrFontV3Dib2CmdPxIntp8 >= Tv_Cmd)
				{
					if(E_GrFontV3Dib2CmdPxSkip != Tv_Cmd)
					{
						Tv_TblIdx	=	Tv_Cmd - E_GrFontV3Dib2CmdPxShadow;
						if(Tv_IsAble[Tv_TblIdx])
						{
							// draw able
							if(1 == Tv_Val)
							{
								// do pixel
								A_Gdib->SetPixelVal(Tv_WkX, Tv_WkY, A_PtrClrTbl[Tv_TblIdx]);
							}
							else
							{
								// do line
								Tv_RcLn.left		=	Tv_WkX;
								Tv_RcLn.top			=	Tv_WkY;
								Tv_RcLn.right		=	Tv_WkX + (__s32)Tv_Val;
								Tv_RcLn.bottom	=	Tv_WkY + 1;
								A_Gdib->FillRect(&Tv_RcLn, A_PtrClrTbl[Tv_TblIdx]);
								//A_Gdib->DrawLine(Tv_WkX, Tv_WkY, Tv_WkX + (__s32)Tv_Val - 1, Tv_WkY, A_PtrClrTbl[Tv_TblIdx]);
							}
						}
					}
					// go next
					Tv_WkX	=	Tv_WkX + (__s32)Tv_Val;
				}
				else
				{
					DbgMsgPrint("GrFontV3Dib2Draw - unknown command!\n");
				}
			}	//	while(Tv_WkY < Tv_RcClip.bottom)
#endif
		}
#ifdef E_GrFontV3Dib2DbgMsg
		else
		{
			if(NULL != A_PtrData)
			{
				DbgMsgPrint("GrFontV3Dib2Draw - Bad script pitch (%d)!\n", (int)A_Pitch);
			}
		}
#endif
}
//--------------------------------------------------------------------

