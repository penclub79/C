/*
	Font version 3 draw base

*/

//--------------------------------------------------------------------
// uses

#include <FontV3/Dib1/GrFontV3Dib1Base.h>

#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrDebug.h>


//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var


//====================================================================
void	GrFontV3Dib1Draw(Cls_GrGdib* A_Gdib, __u8 A_Pitch, void* A_PtrData, __u32 A_DataSize,
	__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsDrawTxt, BOOL8 A_IsDrawShadow, __u32 A_ClrTxt, __u32 A_ClrShadow)
{
	// local -------------------
		St_GrRect	Tv_RcClip;
		__u8		Tv_Cmd;
		__u32		Tv_PxCnt;
		__s32		Tv_WkX;
		__s32		Tv_WkY;
		__u32		Tv_DecPos;
		__u8*		Tv_PtrData;
		BOOL8		Tv_IsAble[3];
		__u32		Tv_ClrTbl[3];
	// code --------------------
		// check drawable
		if((0 != A_Pitch) && (E_GrFontV3Dib1WidthMax >= A_Pitch))
		{
			// init
			Tv_PtrData	=	(__u8*)A_PtrData;
			Tv_DecPos		=	0;

			Tv_IsAble[0]	=	FALSE;
			Tv_IsAble[1]	=	A_IsDrawTxt;
			Tv_IsAble[2]	=	A_IsDrawShadow;

			Tv_ClrTbl[1]	=	A_ClrTxt;
			Tv_ClrTbl[2]	=	A_ClrShadow;

			// get crop area
			A_Gdib->ClipperGet(&Tv_RcClip);

			Tv_WkX			=	A_PosX;
			Tv_WkY			=	A_PosY;
			for( Tv_DecPos = 0; Tv_DecPos < A_DataSize; Tv_DecPos++)
			{
				Tv_Cmd		=	(Tv_PtrData[Tv_DecPos] >> E_GrFontV3Dib1CmdBitPos) & E_GrFontV3Dib1CmdBitMask;
				Tv_PxCnt	=	(__u32)(Tv_PtrData[Tv_DecPos] & E_GrFontV3Dib1CmdPxCntMask) + 1;
				if(E_GrFontV3Dib1CmdSkipLine == Tv_Cmd)
				{
					// skip line unit
					Tv_WkX	=	A_PosX;
					Tv_WkY	=	Tv_WkY + (__s32)Tv_PxCnt;
				}
				else
				{
					if(Tv_IsAble[Tv_Cmd] && (Tv_WkY >= Tv_RcClip.top) && (Tv_WkY < Tv_RcClip.bottom))
					{
						// draw able
						if(1 == Tv_PxCnt)
						{
							// do pixel
							A_Gdib->SetPixelVal(Tv_WkX, Tv_WkY, Tv_ClrTbl[Tv_Cmd]);
						}
						else
						{
							// do line
							A_Gdib->DrawLine(Tv_WkX, Tv_WkY, Tv_WkX + (__s32)Tv_PxCnt - 1, Tv_WkY, Tv_ClrTbl[Tv_Cmd]);
						}
					}
					// move position
					Tv_WkX	=	Tv_WkX + (__s32)Tv_PxCnt;
					// check finish position
					if(Tv_WkX >= (A_PosX + (__s32)((__u32)A_Pitch)))
					{
						// next line
						Tv_WkX	=	A_PosX;
						Tv_WkY ++;
					}
				}
			}
		}
#ifdef E_GrFontV3Dib1DbgMsg
		else
		{
			if(NULL != A_PtrData)
			{
				DbgMsgPrint("GrFontV3Dib1Draw - Bad script pitch (%d)!\n", (int)A_Pitch);
			}
		}
#endif
}
//--------------------------------------------------------------------

