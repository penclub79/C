/*
 layer data

 */

//====================================================================
// uses
#include <GrLayerData.h>

#include <GrDebug.h>



//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
BOOL8	GrLayerDataParsInit(Ptr_GrLayerDataParsRtl A_PtrRtl, void* A_PtrBuf)
{
	// local -------------------
		BOOL8	Tv_Reuslt;
	// code --------------------
		Tv_Reuslt	=	FALSE;

		// init
		A_PtrRtl->PtrBuf		=	(Ptr_GrLayerDataHead)A_PtrBuf;
		A_PtrRtl->Prgs			=	0;
		A_PtrRtl->PtrUserInfo	=	NULL;
			// check header
		if(E_GrLayerDataFcc == A_PtrRtl->PtrBuf->Fcc)
		{
			A_PtrRtl->PtrUserInfo	=	(void*)((Def_GrCalPtr)A_PtrBuf + sizeof(St_GrLayerDataHead));
			A_PtrRtl->PtrCont			=	(void*)((Def_GrCalPtr)A_PtrBuf + sizeof(St_GrLayerDataHead) + (Def_GrCalPtr)A_PtrRtl->PtrBuf->UserInfoSize);
			A_PtrRtl->IsFinish			=	FALSE;
			Tv_Reuslt	=	TRUE;
		}

		return	Tv_Reuslt;
}
//--------------------------------------------------------------------
void*	GrLayerDataParsGetUserInfo(Ptr_GrLayerDataParsRtl A_PtrRtl, __u32* A_PtrRtSize)
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if((NULL != A_PtrRtl->PtrBuf) && (NULL != A_PtrRtl->PtrUserInfo))
		{
			// check userinfo size
			if(0 != A_PtrRtl->PtrBuf->UserInfoSize)
			{
				*A_PtrRtSize	=	A_PtrRtl->PtrBuf->UserInfoSize;
				Tv_Result	=	A_PtrRtl->PtrUserInfo;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrLayerDataParsDo(Ptr_GrLayerDataParsRtl A_PtrRtl, __u16* A_PtrRtDataId, __u32* A_PtrRtSize, __u32* A_PtrRtPara0, __u32* A_PtrRtPara1)
{
	// local -------------------
		void*		Tv_Result;
		Ptr_GrLayerDataLayerHead	Tv_PtrHd;
	// code --------------------
		Tv_Result	=	NULL;

		if((NULL != A_PtrRtl->PtrBuf) && (!A_PtrRtl->IsFinish) && (NULL != A_PtrRtl->PtrCont))
		{
			// check header
			Tv_PtrHd	=	(Ptr_GrLayerDataLayerHead)((Def_GrCalPtr)A_PtrRtl->PtrCont + (Def_GrCalPtr)A_PtrRtl->Prgs);
			if((E_GrLayerDataSeparatorId == Tv_PtrHd->SeparatorId) && (E_GrLayerDataIdEnd != Tv_PtrHd->DataId))
			{
				// update
				*A_PtrRtDataId	=	Tv_PtrHd->DataId;
				*A_PtrRtSize		=	Tv_PtrHd->Size;
				*A_PtrRtPara0		=	Tv_PtrHd->Para0;
				*A_PtrRtPara1		=	Tv_PtrHd->Para1;
				Tv_Result				=	(void*)((Def_GrCalPtr)Tv_PtrHd + sizeof(St_GrLayerDataLayerHead));
				// update next
				A_PtrRtl->Prgs		=	A_PtrRtl->Prgs	 + Tv_PtrHd->Size + sizeof(St_GrLayerDataLayerHead);
			}
			else
			{
				A_PtrRtl->IsFinish		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
