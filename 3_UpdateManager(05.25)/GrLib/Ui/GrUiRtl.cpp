/*
 gause ui runtime parameter

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiRtl.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiBase.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiDoSkin.h>
#include	<GrDumyTool.h>
#include	<Ui/GrUiDoWin.h>
#include	<Ui/GrUiDoFrm.h>
#include	<Ui/GrUiDoListbox.h>
#include	<Ui/GrUiDoBtn.h>
#include	<Ui/GrUiDoLbl.h>
#include	<Ui/GrUiConsole.h>
#include	<GrGdibTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiDoSbar.h>
#include	<Ui/GrUiDoSpinBase.h>
#include	<Ui/GrUiDoSpinNum.h>
#include	<Ui/GrUiDoSpinTxt.h>
#include	<Ui/GrUiDoSpinDgt.h>
#include	<Ui/GrUiDoSmpBtn.h>
#include	<Ui/GrUiDoEdit.h>
#include	<Ui/GrUiDoSysBtn.h>
#include	<Ui/GrUiDoCalendar.h>
#include	<Ui/GrUiDoQuad.h>
#include	<Ui/GrUiDoGrpSel.h>
#include	<Ui/GrUiDoGrpEdit.h>
#include	<Ui/GrUiDoBitGrid.h>
#include	<Ui/GrUiDoSpinImg.h>
#include	<Ui/GrUiDoPrgsbar.h>
#include	<Ui/GrUiDoInpTime.h>
#include	<Ui/GrUiDoArea.h>
#include	<Ui/GrUiDoCdvArea.h>
#include	<Ui/GrUiDoAcBtn.h>
#include	<Ui/GrUiDoAcSpNum.h>
#include	<Ui/GrUiDoAcSpTxt.h>
#include	<Ui/GrUiDoAcSpImg.h>
#include	<Ui/GrUiDoAcSpDgt.h>
//#include	<Ui/GrUiDoMosaicGrid.h>//new-120905~mosaic

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function
BOOL8	GrUiRtlPrStvBase( Cls_GrUiDoBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvSkin( Cls_GrUiDoSkin* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvWin( Cls_GrUiDoWin* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvFrm( Cls_GrUiDoFrm* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvListbox( Cls_GrUiDoListbox* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvBtn( Cls_GrUiDoBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvLbl( Cls_GrUiDoLbl* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvSbar( Cls_GrUiDoSbar* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvSpinBase( Cls_GrUiDoSpinBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvSpinNum( Cls_GrUiDoSpinNum* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvSpinTxt( Cls_GrUiDoSpinTxt* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvSpinDgt( Cls_GrUiDoSpinDgt* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvSmpBtn( Cls_GrUiDoSmpBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvEdit( Cls_GrUiDoEdit* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvSysBtn( Cls_GrUiDoSysBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvCalendar( Cls_GrUiDoCalendar* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvQuad( Cls_GrUiDoQuad* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvGrpSel( Cls_GrUiDoGrpSel* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvGrpEdit( Cls_GrUiDoGrpEdit* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvBitGrid( Cls_GrUiDoBitGrid* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvSpinImg( Cls_GrUiDoSpinImg* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvPrgsBar( Cls_GrUiDoPrgsbar* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvInpTime( Cls_GrUiDoInpTime* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvArea( Cls_GrUiDoArea* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvCdvArea( Cls_GrUiDoCdvArea* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvAcBtn( Cls_GrUiDoAcBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvAcSpNum( Cls_GrUiDoAcSpNum* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvAcSpTxt( Cls_GrUiDoAcSpTxt* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvAcSpImg( Cls_GrUiDoAcSpImg* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
BOOL8	GrUiRtlPrStvAcSpDgt( Cls_GrUiDoAcSpDgt* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
//BOOL8	GrUiRtlPrStvMosaicGrid( Cls_GrUiDoMosaicGrid* A_Obj, __u16 A_ParaId, void* A_PtrBuf );//new-120905~mosaic

//====================================================================
//var
extern	Cls_GrUiMng*		V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
BOOL8	GrUiRtlParaSetVal( Cls_GrUiDoBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u16		Tv_ParaType;
	// code --------------------
		Tv_Result	=	FALSE;

		//get para value
		Tv_ParaType	=	E_GrUiScptMaskPrIdType & A_ParaId;
		switch( Tv_ParaType )
		{
			case	E_GrUiScptPrIdBaseObjType:
				Tv_Result	=	GrUiRtlPrStvBase( A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSkinObjType:
				Tv_Result	=	GrUiRtlPrStvSkin( (Cls_GrUiDoSkin*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdWinObjType:
				Tv_Result	=	GrUiRtlPrStvWin( (Cls_GrUiDoWin*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdFrmObjType:
				Tv_Result	=	GrUiRtlPrStvFrm( (Cls_GrUiDoFrm*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdLblObjType:
				Tv_Result	=	GrUiRtlPrStvLbl( (Cls_GrUiDoLbl*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdBtnObjType:
				Tv_Result	=	GrUiRtlPrStvBtn( (Cls_GrUiDoBtn*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSbarObjType:
				Tv_Result	=	GrUiRtlPrStvSbar( (Cls_GrUiDoSbar*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdLbxObjType:
				Tv_Result	=	GrUiRtlPrStvListbox( (Cls_GrUiDoListbox*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpbBaseObjType:
				Tv_Result	=	GrUiRtlPrStvSpinBase( (Cls_GrUiDoSpinBase*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpbNumObjType:
				Tv_Result	=	GrUiRtlPrStvSpinNum( (Cls_GrUiDoSpinNum*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpbTxtObjType:
				Tv_Result	=	GrUiRtlPrStvSpinTxt( (Cls_GrUiDoSpinTxt*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpbDgtObjType:
				Tv_Result	=	GrUiRtlPrStvSpinDgt( (Cls_GrUiDoSpinDgt*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSmpbObjType:
				Tv_Result	=	GrUiRtlPrStvSmpBtn( (Cls_GrUiDoSmpBtn*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdEditObjType:
				Tv_Result	=	GrUiRtlPrStvEdit( (Cls_GrUiDoEdit*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSysBtnObjType:
				Tv_Result	=	GrUiRtlPrStvSysBtn( (Cls_GrUiDoSysBtn*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdCalenObjType:
				Tv_Result	=	GrUiRtlPrStvCalendar( (Cls_GrUiDoCalendar*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdQuadObjType:
				Tv_Result	=	GrUiRtlPrStvQuad( (Cls_GrUiDoQuad*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdGrpSelObjType:
				Tv_Result	=	GrUiRtlPrStvGrpSel( (Cls_GrUiDoGrpSel*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdGrpEdtObjType:
				Tv_Result	=	GrUiRtlPrStvGrpEdit( (Cls_GrUiDoGrpEdit*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdBitGridObjType:
				Tv_Result	=	GrUiRtlPrStvBitGrid( (Cls_GrUiDoBitGrid*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpnImgObjType:
				Tv_Result	=	GrUiRtlPrStvSpinImg( (Cls_GrUiDoSpinImg*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdPrgbarObjType:
				Tv_Result	=	GrUiRtlPrStvPrgsBar( (Cls_GrUiDoPrgsbar*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdInpTimeObjType:
				Tv_Result	=	GrUiRtlPrStvInpTime( (Cls_GrUiDoInpTime*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAreaObjType:
				Tv_Result	=	GrUiRtlPrStvArea( (Cls_GrUiDoArea*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdCdvAreaObjType:
				Tv_Result	=	GrUiRtlPrStvCdvArea( (Cls_GrUiDoCdvArea*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcBtnObjType:
				Tv_Result	=	GrUiRtlPrStvAcBtn( (Cls_GrUiDoAcBtn*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcSpNumObjType:
				Tv_Result	=	GrUiRtlPrStvAcSpNum( (Cls_GrUiDoAcSpNum*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcSpTxtObjType:
				Tv_Result	=	GrUiRtlPrStvAcSpTxt( (Cls_GrUiDoAcSpTxt*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcSpImgObjType:
				Tv_Result	=	GrUiRtlPrStvAcSpImg( (Cls_GrUiDoAcSpImg*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcSpDgtObjType:
				Tv_Result	=	GrUiRtlPrStvAcSpDgt( (Cls_GrUiDoAcSpDgt*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			//case	E_GrUiScptPrIdMosaicGridObjType:
			//	Tv_Result	=	GrUiRtlPrStvMosaicGrid( (Cls_GrUiDoMosaicGrid*)A_Obj, A_ParaId, A_PtrBuf );
			//	break;//new-120905~mosaic
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvBase( Cls_GrUiDoBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		//init
		Tv_Idx		=	0;
		Tv_Result	=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdIsEnable:
				A_Obj->EnableSet( *((E_GrUiScptPrTyIsEnable*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdFocusMap:
				A_Obj->FocusMapSet( *((E_GrUiScptPrTyFocusMap*)A_PtrBuf) ); 
				break;
			case	E_GrUiScptPrIdCliMargin:
				A_Obj->ClientMarginSet( ((E_GrUiScptPrTyCliMargin*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdUserData0:
				A_Obj->UserDataSet( *((E_GrUiScptPrTyUserData0*)A_PtrBuf), 0 ); 
				break;
			case	E_GrUiScptPrIdUserData1:
				A_Obj->UserDataSet( *((E_GrUiScptPrTyUserData1*)A_PtrBuf), 1 ); 
				break;
			case	E_GrUiScptPrIdUserData2:
				A_Obj->UserDataSet( *((E_GrUiScptPrTyUserData2*)A_PtrBuf), 2 ); 
				break;
			case	E_GrUiScptPrIdUserData3:
				A_Obj->UserDataSet( *((E_GrUiScptPrTyUserData3*)A_PtrBuf), 3 ); 
				break;
			case	E_GrUiScptPrIdExtPosY:
				A_Obj->PalExtPosySet( *((E_GrUiScptPrTyExtPosY*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdExtSizeY:
				A_Obj->PalExtSizeySet( *((E_GrUiScptPrTyExtSizeY*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdRelFcsLblId:
				A_Obj->RelFocusLabelIdSet( *((E_GrUiScptPrTyRelFcsLblId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdRelFcsTxtId:
				A_Obj->RelFocusTextIdSet( *((E_GrUiScptPrTyRelFcsTxtId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdFocusBlock:
				A_Obj->FocusBlockSet( *((E_GrUiScptPrTyFocusBlock*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdFocusBaseId:
				A_Obj->FocusBaseIdSet( *((E_GrUiScptPrTyFocusBaseId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCndVisMap1:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCndVisMap0:
				A_Obj->CondVisibleMapSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyCndVisMap0*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvSkin( Cls_GrUiDoSkin* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result	=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdIsBgSprite:
				A_Obj->BgSpriteModeSet( *((E_GrUiScptPrTyIsBgSprite*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdClrBg:
				A_Obj->BgColorSet( *((E_GrUiScptPrTyClrBg*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSkin0:
			case	E_GrUiScptPrIdSkin1:
			case	E_GrUiScptPrIdSkin2:
			case	E_GrUiScptPrIdSkin3:
			case	E_GrUiScptPrIdSkin4:
			case	E_GrUiScptPrIdSkin5:
			case	E_GrUiScptPrIdSkin6:
			case	E_GrUiScptPrIdSkin7:
			case	E_GrUiScptPrIdSkin8:
			case	E_GrUiScptPrIdSkin9:
			case	E_GrUiScptPrIdSkin10:
			case	E_GrUiScptPrIdSkin11:
			case	E_GrUiScptPrIdSkin12:
			case	E_GrUiScptPrIdSkin13:
			case	E_GrUiScptPrIdSkin14:
			case	E_GrUiScptPrIdSkin15:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSkin0;
				A_Obj->BgSkinSet( (__u8)Tv_Idx, ((E_GrUiScptPrTySkin0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBorderAtb:
				A_Obj->BorderAtbSet( *((E_GrUiScptPrTyBorderAtb*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBorderDepth:
				A_Obj->BorderDepthSet( *((E_GrUiScptPrTyBorderDepth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBorderClrLt:
			case	E_GrUiScptPrIdBorderClrDk:
			case	E_GrUiScptPrIdBorderClrMl:
			case	E_GrUiScptPrIdBorderClrMd:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdBorderClrLt;
				A_Obj->BorderColorSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyBorderClrLt*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvWin( Cls_GrUiDoWin* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdIsMoveAble:
				A_Obj->MoveableSet( *((E_GrUiScptPrTyIsMoveAble*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCloseKeyType:
				A_Obj->CloseKeyTypeSet( *((E_GrUiScptPrTyCloseKeyType*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCloseTime:
				A_Obj->CloseTimeSet( *((E_GrUiScptPrTyCloseTime*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdIsLayerLock:
				A_Obj->LayerFcsLockSet( *((E_GrUiScptPrTyIsLayerLock*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdIsFocusAble:
				A_Obj->FocusAbleSet( *((E_GrUiScptPrTyIsFocusAble*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdWinRelFrmId:
				A_Obj->RelateFrameIdSet( *((E_GrUiScptPrTyWinRelFrmId*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvFrm( Cls_GrUiDoFrm* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		Tv_Result	=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdFrmPageId0:
			case	E_GrUiScptPrIdFrmPageId1:
			case	E_GrUiScptPrIdFrmPageId2:
			case	E_GrUiScptPrIdFrmPageId3:
			case	E_GrUiScptPrIdFrmPageId4:
			case	E_GrUiScptPrIdFrmPageId5:
			case	E_GrUiScptPrIdFrmPageId6:
			case	E_GrUiScptPrIdFrmPageId7:
			case	E_GrUiScptPrIdFrmPageId8:
			case	E_GrUiScptPrIdFrmPageId9:
			case	E_GrUiScptPrIdFrmPageId10:
			case	E_GrUiScptPrIdFrmPageId11:
			case	E_GrUiScptPrIdFrmPageId12:
			case	E_GrUiScptPrIdFrmPageId13:
			case	E_GrUiScptPrIdFrmPageId14:
			case	E_GrUiScptPrIdFrmPageId15:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdFrmPageId0;
				A_Obj->FrmPageObjIdSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyFrmPageId0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdFrmViewPage:
				A_Obj->FrmPageViewIdxSet( *((E_GrUiScptPrTyFrmViewPage*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdFrmRtlLblId:
				A_Obj->FrmRelLabelIdSet( *((E_GrUiScptPrTyFrmRtlLblId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdFrmNoFcsMov:
				A_Obj->FrmNoMoveFocusSet( *((E_GrUiScptPrTyFrmNoFcsMov*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvListbox( Cls_GrUiDoListbox* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		Tv_Result	=	TRUE;
		Tv_Idx		=	0;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdLbxEditAble:
				A_Obj->EditAbleSet( *((E_GrUiScptPrTyLbxEditAble*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxEditNum:
				A_Obj->EditTypeNum( *((E_GrUiScptPrTyLbxEditNum*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxKeyDel:
				A_Obj->KeyDelAbleSet( *((E_GrUiScptPrTyLbxKeyDel*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxDelQst:
				A_Obj->KeyDelQuestSet( *((E_GrUiScptPrTyLbxDelQst*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxItmHeight:
				A_Obj->ItemHeightSet( *((E_GrUiScptPrTyLbxItmHeight*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxItmMargin:
				A_Obj->ItemMarginSet( ((E_GrUiScptPrTyLbxItmMargin*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxItmBrdDep:
				A_Obj->ItemBrdDepthSet( *((E_GrUiScptPrTyLbxItmBrdDep*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxItmBrdAtb:
				A_Obj->ItemBrdAtbSet( *((E_GrUiScptPrTyLbxItmBrdAtb*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxItmGap:
				A_Obj->ItemGapSet( *((E_GrUiScptPrTyLbxItmGap*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxIconWidth:
				A_Obj->IconWidthSet( *((E_GrUiScptPrTyLbxIconWidth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxIconCount:
				A_Obj->IconCountSet( *((E_GrUiScptPrTyLbxIconCount*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxIconPos:
				A_Obj->IconPosSet( *((E_GrUiScptPrTyLbxIconPos*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxIconGap:
				A_Obj->IconGapSet( *((E_GrUiScptPrTyLbxIconGap*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxImgFcsOut:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxImgOnCus:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxImgNormal:
				A_Obj->ItemSkinSet( (__u8)Tv_Idx, ((E_GrUiScptPrTyLbxImgNormal*)A_PtrBuf)  );
				break;
			case	E_GrUiScptPrIdLbxClrFcsOut:
				Tv_Idx++;
			case	E_GrUiScptPrIdLbxClrOnCus:
				Tv_Idx++;
			case	E_GrUiScptPrIdLbxClrNormal:
				A_Obj->ItemColorSet( Tv_Idx, *((E_GrUiScptPrTyLbxClrNormal*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxTclrFcsOut:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxTclrOnCus:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxTclrNormal:
				A_Obj->TextColorSet( Tv_Idx, *((E_GrUiScptPrTyLbxTclrNormal*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxBgClrDis:
			case	E_GrUiScptPrIdLbxBgClrNrm:
			case	E_GrUiScptPrIdLbxBgClrFcs:
			case	E_GrUiScptPrIdLbxBgClrCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdLbxBgClrDis;
				A_Obj->LbxColorSet( Tv_Idx, *((E_GrUiScptPrTyLbxBgClrDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxIconImg0:
			case	E_GrUiScptPrIdLbxIconImg1:
			case	E_GrUiScptPrIdLbxIconImg2:
			case	E_GrUiScptPrIdLbxIconImg3:
			case	E_GrUiScptPrIdLbxIconImg4:
			case	E_GrUiScptPrIdLbxIconImg5:
			case	E_GrUiScptPrIdLbxIconImg6:
			case	E_GrUiScptPrIdLbxIconImg7:
			case	E_GrUiScptPrIdLbxIconImg8:
			case	E_GrUiScptPrIdLbxIconImg9:
			case	E_GrUiScptPrIdLbxIconImg10:
			case	E_GrUiScptPrIdLbxIconImg11:
			case	E_GrUiScptPrIdLbxIconImg12:
			case	E_GrUiScptPrIdLbxIconImg13:
			case	E_GrUiScptPrIdLbxIconImg14:
			case	E_GrUiScptPrIdLbxIconImg15:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdLbxIconImg0;
				A_Obj->IconSet( (__u8)Tv_Idx, ((E_GrUiScptPrTyLbxIconImg0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxSbarId:
				A_Obj->ScrlSbarIdSet( *((E_GrUiScptPrTyLbxSbarId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxTextPara:
				A_Obj->TextParaSet( ((E_GrUiScptPrTyLbxTextPara*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLbxKeyFcsCtrl:
				A_Obj->KeyFocusCtrlSet( *((E_GrUiScptPrTyLbxKeyFcsCtrl*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
	return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvBtn( Cls_GrUiDoBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdBtnType:
				A_Obj->BtnTypeSet( *((E_GrUiScptPrTyBtnType*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBtnCheck:
				A_Obj->BtnCheckSet( *((E_GrUiScptPrTyBtnCheck*)A_PtrBuf) );
				break;

			case	E_GrUiScptPrIdBtnImgDisable:
			case	E_GrUiScptPrIdBtnImgNormal:
			case	E_GrUiScptPrIdBtnImgNrmFcs:
			case	E_GrUiScptPrIdBtnImgNrmPush:
			case	E_GrUiScptPrIdBtnImgCheck:
			case	E_GrUiScptPrIdBtnImgChkFcs:
			case	E_GrUiScptPrIdBtnImgChkPush:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdBtnImgDisable;
				A_Obj->BtnStatImgSet( (__u8)Tv_Idx, ((E_GrUiScptPrTyBtnImgDisable*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBtnClrDisable:
			case	E_GrUiScptPrIdBtnClrNormal:
			case	E_GrUiScptPrIdBtnClrNrmFcs:
			case	E_GrUiScptPrIdBtnClrNrmPush:
			case	E_GrUiScptPrIdBtnClrCheck:
			case	E_GrUiScptPrIdBtnClrChkFcs:
			case	E_GrUiScptPrIdBtnClrChkPush:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdBtnClrDisable;
				A_Obj->BtnStatClrSet( Tv_Idx, *((E_GrUiScptPrTyBtnClrDisable*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBtnTclrDisable:
			case	E_GrUiScptPrIdBtnTclrNormal:
			case	E_GrUiScptPrIdBtnTclrNrmFcs:
			case	E_GrUiScptPrIdBtnTclrNrmPush:
			case	E_GrUiScptPrIdBtnTclrCheck:
			case	E_GrUiScptPrIdBtnTclrChkFcs:
			case	E_GrUiScptPrIdBtnTclrChkPush:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdBtnTclrDisable;
				A_Obj->BtnTxtClrSet( Tv_Idx, *((E_GrUiScptPrTyBtnTclrDisable*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBtnTextId:
				A_Obj->TextIdSet( *((E_GrUiScptPrTyBtnTextId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBtnTextPara:
				A_Obj->TextParaSet( ((E_GrUiScptPrTyBtnTextPara*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBtnEvtHostId:
				A_Obj->BtnEvtHostIdSet( *((E_GrUiScptPrTyBtnEvtHostId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBtnImgChkDis:
				A_Obj->BtnStatImgSet( E_GrUiDoBtnImgIdxChkDis, ((E_GrUiScptPrTyBtnImgChkDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBtnClrChkDis:
				A_Obj->BtnStatClrSet( E_GrUiDoBtnImgIdxChkDis, *((E_GrUiScptPrTyBtnClrChkDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBtnTclrChkDis:
				A_Obj->BtnTxtClrSet( E_GrUiDoBtnImgIdxChkDis, *((E_GrUiScptPrTyBtnTclrChkDis*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvSbar( Cls_GrUiDoSbar* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSbarDirecton:
				A_Obj->BarDirSet( *((E_GrUiScptPrTySbarDirecton*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarBtnBrdAtb:
				A_Obj->SbtnBrdAtbSet( *((E_GrUiScptPrTySbarBtnBrdAtb*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarBtnBrdDepth:
				A_Obj->SbtnBrdDepthSet( *((E_GrUiScptPrTySbarBtnBrdDepth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarBtnImgDis:
			case	E_GrUiScptPrIdSbarBtnImgNrm:
			case	E_GrUiScptPrIdSbarBtnImgFcs:
			case	E_GrUiScptPrIdSbarBtnImgCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSbarBtnImgDis;
				A_Obj->SbtnStatImgSet( Tv_Idx, ((E_GrUiScptPrTySbarBtnImgNrm*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarBtnClrDis:
			case	E_GrUiScptPrIdSbarBtnClrNrm:
			case	E_GrUiScptPrIdSbarBtnClrFcs:
			case	E_GrUiScptPrIdSbarBtnClrCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSbarBtnClrDis;
				A_Obj->SbtnStatClrSet( Tv_Idx, *((E_GrUiScptPrTySbarBtnClrDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarRngMin:
				A_Obj->RangeMinSet( *((E_GrUiScptPrTySbarRngMin*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarRngMax:
				A_Obj->RangeMaxSet( *((E_GrUiScptPrTySbarRngMax*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarValPos:
				A_Obj->PosSet( *((E_GrUiScptPrTySbarValPos*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarPageUnit:
				A_Obj->PageUnitSet( *((E_GrUiScptPrTySbarPageUnit*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarEvtHostId:
				A_Obj->EvtHostIdSet( *((E_GrUiScptPrTySbarEvtHostId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarBtnSize:
				A_Obj->SbtnSizeSet( *((E_GrUiScptPrTySbarBtnSize*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSbarOnKeyCtrl:
				A_Obj->OnKeyCtrlSet( *((E_GrUiScptPrTySbarOnKeyCtrl*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvSpinBase( Cls_GrUiDoSpinBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpbBaseTxtPara:
				A_Obj->TextParaSet( ((E_GrUiScptPrTySpbBaseTxtPara*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbBaseClrDis:
			case	E_GrUiScptPrIdSpbBaseClrNrm:
			case	E_GrUiScptPrIdSpbBaseClrFcs:
			case	E_GrUiScptPrIdSpbBaseClrCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSpbBaseClrDis;
				A_Obj->StatClrSet( (__u8)Tv_Idx, *((E_GrUiScptPrTySpbBaseClrDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbBaseTclrDis:
			case	E_GrUiScptPrIdSpbBaseTclrNrm:
			case	E_GrUiScptPrIdSpbBaseTclrFcs:
			case	E_GrUiScptPrIdSpbBaseTclrCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSpbBaseTclrDis;
				A_Obj->StatTxtClrSet( (__u8)Tv_Idx, *((E_GrUiScptPrTySpbBaseTclrDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbBaseSkinDis:
			case	E_GrUiScptPrIdSpbBaseSkinNrm:
			case	E_GrUiScptPrIdSpbBaseSkinFcs:
			case	E_GrUiScptPrIdSpbBaseSkinCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSpbBaseSkinDis;
				A_Obj->StatSkinSet( (__u8)Tv_Idx, ((E_GrUiScptPrTySpbBaseSkinDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbBaseEvtHostId:
				A_Obj->EvtHostIdSet( *((E_GrUiScptPrTySpbBaseEvtHostId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbBaseOnKeyCtrl:
				A_Obj->OnKeyCtrlSet( *((E_GrUiScptPrTySpbBaseOnKeyCtrl*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvSpinNum( Cls_GrUiDoSpinNum* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpbNumRngMax:
				A_Obj->RangeMaxSet( *((E_GrUiScptPrTySpbNumRngMax*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbNumRngMin:
				A_Obj->RangeMinSet( *((E_GrUiScptPrTySpbNumRngMin*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbNumValue:
				A_Obj->ValueSet( *((E_GrUiScptPrTySpbNumValue*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbNumStep:
				A_Obj->StepSet( *((E_GrUiScptPrTySpbNumStep*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbNumTxtIdZero:
				A_Obj->ZeroTxtIdSet( *((E_GrUiScptPrTySpbNumTxtIdZero*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbNumVertical:
				A_Obj->VerticalTypeSet( *((E_GrUiScptPrTySpbNumVertical*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbNumCipher:
				A_Obj->CipherSet( *((E_GrUiScptPrTySpbNumCipher*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbNumTxtIdPre:
				A_Obj->PrefixTxtIdSet( *((E_GrUiScptPrTySpbNumTxtIdPre*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbNumTxtIdSuf:
				A_Obj->SuffixTxtIdSet( *((E_GrUiScptPrTySpbNumTxtIdSuf*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbNumCenterSz:
				A_Obj->CenterSizeSet( *((E_GrUiScptPrTySpbNumCenterSz*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvSpinTxt( Cls_GrUiDoSpinTxt* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpbTxtTxtIdSt:
				A_Obj->TextIdStartSet( *((E_GrUiScptPrTySpbTxtTxtIdSt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbTxtTxtIdCnt:
				A_Obj->TextIdCountSet( *((E_GrUiScptPrTySpbTxtTxtIdCnt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbTxtVertical:
				A_Obj->VerticalTypeSet( *((E_GrUiScptPrTySpbTxtVertical*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbTxtCenterSz:
				A_Obj->CenterSizeSet( *((E_GrUiScptPrTySpbTxtCenterSz*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvSpinDgt( Cls_GrUiDoSpinDgt* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpbDgtHexMode:
				A_Obj->ModeHextSet( *((E_GrUiScptPrTySpbDgtHexMode*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbDgtAreaUpdn:
				A_Obj->AreaUpDownSizeSet( *((E_GrUiScptPrTySpbDgtAreaUpdn*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbDgtNextInp:
				A_Obj->NextInputIdSet( *((E_GrUiScptPrTySpbDgtNextInp*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbDgtAreaCipher:
				A_Obj->AreaCipherSizeSet( *((E_GrUiScptPrTySpbDgtAreaCipher*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbDgtCipher:
				A_Obj->CipherSet( *((E_GrUiScptPrTySpbDgtCipher*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpbDgtValMax:
				A_Obj->ValMaxSet( *((E_GrUiScptPrTySpbDgtValMax*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvSmpBtn( Cls_GrUiDoSmpBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSmpbTxtPara:
				A_Obj->TextParaSet( ((E_GrUiScptPrTySmpbTxtPara*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSmpbSkinDis:
			case	E_GrUiScptPrIdSmpbSkinNrm:
			case	E_GrUiScptPrIdSmpbSkinFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSmpbSkinDis;
				A_Obj->StatSkinSet( (__u8)Tv_Idx, ((E_GrUiScptPrTySmpbSkinDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSmpbClrDis:
			case	E_GrUiScptPrIdSmpbClrNrm:
			case	E_GrUiScptPrIdSmpbClrFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSmpbClrDis;
				A_Obj->StatClrSet( (__u8)Tv_Idx, *((E_GrUiScptPrTySmpbClrDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSmpbTclrDis:
			case	E_GrUiScptPrIdSmpbTclrNrm:
			case	E_GrUiScptPrIdSmpbTclrFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSmpbTclrDis;
				A_Obj->TxtClrSet( (__u8)Tv_Idx, *((E_GrUiScptPrTySmpbTclrDis*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvEdit( Cls_GrUiDoEdit* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;
		Tv_Idx			=	0;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdEditNumType:
				A_Obj->EditTypeSet( *((E_GrUiScptPrTyEditNumType*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdEditEvtHostId:
				A_Obj->EvtHostIdSet( *((E_GrUiScptPrTyEditEvtHostId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdEditMaxLen:
				A_Obj->EditMaxLenSet( *((E_GrUiScptPrTyEditMaxLen*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdEditNoKeyVk:
				A_Obj->NoKeyVkbdSet( *((E_GrUiScptPrTyEditNoKeyVk*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdEditSkinEfcs:
				Tv_Idx ++;
			case	E_GrUiScptPrIdEditSkinEnrm:
				Tv_Idx	=	Tv_Idx + E_GrUiDoEditImgIdxEptyNrm;
				A_Obj->StatSkinSet( (__u8)Tv_Idx, ((E_GrUiScptPrTyEditSkinEnrm*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdEditClrEfcs:
				Tv_Idx ++;
			case	E_GrUiScptPrIdEditClrEnrm:
				Tv_Idx	=	Tv_Idx + E_GrUiDoEditImgIdxEptyNrm;
				A_Obj->StatClrSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyEditClrEnrm*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvSysBtn( Cls_GrUiDoSysBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSysBtnTextId:
				A_Obj->TextIdSet( *((E_GrUiScptPrTySysBtnTextId*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvCalendar( Cls_GrUiDoCalendar* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Idx			=	0;
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdCalenTxtPara:
				A_Obj->TextParaSet( ((E_GrUiScptPrTyCalenTxtPara*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenClrMark7:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrMark6:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrMark5:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrMark4:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrMark3:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrMark2:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrMark1:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrMark0:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrCus:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrSel:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrTxtCus:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCalenClrTxtNrm:
				A_Obj->CalendarClrSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyCalenClrTxtNrm*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenItmWidth:
				A_Obj->ItemWidthSet( *((E_GrUiScptPrTyCalenItmWidth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenItmHeight:
				A_Obj->ItemHeightSet( *((E_GrUiScptPrTyCalenItmHeight*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenItmGapX:
				A_Obj->ItemGapxSet( *((E_GrUiScptPrTyCalenItmGapX*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenItmGapY:
				A_Obj->ItemGapySet( *((E_GrUiScptPrTyCalenItmGapY*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenSelGap:
				A_Obj->SelBoxGapSet( *((E_GrUiScptPrTyCalenSelGap*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenSelDepth:
				A_Obj->SelBoxDepthSet( *((E_GrUiScptPrTyCalenSelDepth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenCusGap:
				A_Obj->CusBoxGapSet( *((E_GrUiScptPrTyCalenCusGap*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenCusDepth:
				A_Obj->CusBoxDepthSet( *((E_GrUiScptPrTyCalenCusDepth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenSkinMark:
			case	E_GrUiScptPrIdCalenSkinSel:
			case	E_GrUiScptPrIdCalenSkinCus:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdCalenSkinMark;
				A_Obj->CalendarSkinSet( (__u8)Tv_Idx, ((E_GrUiScptPrTyCalenSkinMark*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenClrDis:
			case	E_GrUiScptPrIdCalenClrNrm:
			case	E_GrUiScptPrIdCalenClrFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdCalenClrDis;
				A_Obj->StatClrSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyCalenClrDis*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenSpinYear:
				A_Obj->SpinYearIdSet( *((E_GrUiScptPrTyCalenSpinYear*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenSpinMonth:
				A_Obj->SpinMonthIdSet( *((E_GrUiScptPrTyCalenSpinMonth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCalenEvtHostId:
				A_Obj->EvtHostIdSet( *((E_GrUiScptPrTyCalenEvtHostId*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvQuad( Cls_GrUiDoQuad* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdQuadTxtPara:
				A_Obj->TextParaSet( ((E_GrUiScptPrTyQuadTxtPara*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadClrTitle:
				A_Obj->TitleColorSet( *((E_GrUiScptPrTyQuadClrTitle*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadClrGrid:
				A_Obj->GridColorSet( *((E_GrUiScptPrTyQuadClrGrid*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadGridShow:
				A_Obj->GridShowSet( *((E_GrUiScptPrTyQuadGridShow*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadTitleShow:
				A_Obj->TitleShowSet( *((E_GrUiScptPrTyQuadTitleShow*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadIconShow:
				A_Obj->IconShowSet( *((E_GrUiScptPrTyQuadIconShow*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadBgImgShow:
				A_Obj->BgImgShowSet( *((E_GrUiScptPrTyQuadBgImgShow*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadChMargin:
				A_Obj->ChMarginSet( *((E_GrUiScptPrTyQuadChMargin*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadIconSizeX:
				A_Obj->IconXsizeSet( *((E_GrUiScptPrTyQuadIconSizeX*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadIconSizeY:
				A_Obj->IconYsizeSet( *((E_GrUiScptPrTyQuadIconSizeY*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadIconGapTop:
				A_Obj->IconGapTopSet( *((E_GrUiScptPrTyQuadIconGapTop*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadIconGapBtw:
				A_Obj->IconGapBtwSet( *((E_GrUiScptPrTyQuadIconGapBtw*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadIconImgRec:
			case	E_GrUiScptPrIdQuadIconImgMtn:
			case	E_GrUiScptPrIdQuadIconImgDi:
			case	E_GrUiScptPrIdQuadIconImgIps:
			case	E_GrUiScptPrIdQuadIconImgNsg:
			case	E_GrUiScptPrIdQuadIconImgCvt:
			case	E_GrUiScptPrIdQuadIconBigNsg:
			case	E_GrUiScptPrIdQuadIconBigCvt:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdQuadIconImgRec;
				A_Obj->IconImgSet( (__u8)Tv_Idx, ((E_GrUiScptPrTyQuadIconImgRec*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadPosMode:
				A_Obj->PosModeSet( *((E_GrUiScptPrTyQuadPosMode*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadExtTtClr:
				//A_Obj->ExtTitleColorSet( *((E_GrUiScptPrTyQuadExtTtClr*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadExtTtPara:
				A_Obj->ExtTxtParaSet( ((E_GrUiScptPrTyQuadExtTtPara*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdQuadGridWidth:
				A_Obj->GridWidthSet( *((E_GrUiScptPrTyQuadGridWidth*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvGrpSel( Cls_GrUiDoGrpSel* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdGrpSelColCnt:
				A_Obj->ColumCountSet( *((E_GrUiScptPrTyGrpSelColCnt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelLineCnt:
				A_Obj->LineCountSet( *((E_GrUiScptPrTyGrpSelLineCnt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelCusClrDis:
			case	E_GrUiScptPrIdGrpSelCusClrNrm:
			case	E_GrUiScptPrIdGrpSelCusClrFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdGrpSelCusClrDis;
				A_Obj->CusColorSet( Tv_Idx, *((E_GrUiScptPrTyGrpSelCusClrFcs*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelAtbClr0:
			case	E_GrUiScptPrIdGrpSelAtbClr1:
			case	E_GrUiScptPrIdGrpSelAtbClr2:
			case	E_GrUiScptPrIdGrpSelAtbClr3:
			case	E_GrUiScptPrIdGrpSelAtbClr4:
			case	E_GrUiScptPrIdGrpSelAtbClr5:
			case	E_GrUiScptPrIdGrpSelAtbClr6:
			case	E_GrUiScptPrIdGrpSelAtbClr7:
			case	E_GrUiScptPrIdGrpSelAtbClr8:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdGrpSelAtbClr0;
				A_Obj->AtbColorSet( Tv_Idx, *((E_GrUiScptPrTyGrpSelAtbClr0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelBitPos0:
			case	E_GrUiScptPrIdGrpSelBitPos1:
			case	E_GrUiScptPrIdGrpSelBitPos2:
			case	E_GrUiScptPrIdGrpSelBitPos3:
			case	E_GrUiScptPrIdGrpSelBitPos4:
			case	E_GrUiScptPrIdGrpSelBitPos5:
			case	E_GrUiScptPrIdGrpSelBitPos6:
			case	E_GrUiScptPrIdGrpSelBitPos7:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdGrpSelBitPos0;
				A_Obj->BitGroupPosSet( Tv_Idx, *((E_GrUiScptPrTyGrpSelBitPos0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelCusColWdt:
				A_Obj->CusColumWidthSet( *((E_GrUiScptPrTyGrpSelCusColWdt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelCusLineWdt:
				A_Obj->CusLineWidthSet( *((E_GrUiScptPrTyGrpSelCusLineWdt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelColItmSize:
				A_Obj->ColumItemSizeSet( *((E_GrUiScptPrTyGrpSelColItmSize*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelLineItmSize:
				A_Obj->LineItemSizeSet( *((E_GrUiScptPrTyGrpSelLineItmSize*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelLineGap:
				A_Obj->LineGapSet( *((E_GrUiScptPrTyGrpSelLineGap*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelBitGrpCnt:
				A_Obj->BitGroupCountSet( *((E_GrUiScptPrTyGrpSelBitGrpCnt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelBitGrpGap:
				A_Obj->BitGroupGapSet( *((E_GrUiScptPrTyGrpSelBitGrpGap*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelRltColLbl:
				A_Obj->RltColumLableIdSet( *((E_GrUiScptPrTyGrpSelRltColLbl*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelRltLineLbl:
				A_Obj->RltLineLableIdSet( *((E_GrUiScptPrTyGrpSelRltLineLbl*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpSelColSelClr:
				A_Obj->ColumSelColorSet( *((E_GrUiScptPrTyGrpSelColSelClr*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvGrpEdit( Cls_GrUiDoGrpEdit* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdGrpEdtColCnt:
				A_Obj->ColumCountSet( *((E_GrUiScptPrTyGrpEdtColCnt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtLineCnt:
				A_Obj->LineCountSet( *((E_GrUiScptPrTyGrpEdtLineCnt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtBitMode:
				A_Obj->BitModeSet( *((E_GrUiScptPrTyGrpEdtBitMode*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtBitPos0:
			case	E_GrUiScptPrIdGrpEdtBitPos1:
			case	E_GrUiScptPrIdGrpEdtBitPos2:
			case	E_GrUiScptPrIdGrpEdtBitPos3:
			case	E_GrUiScptPrIdGrpEdtBitPos4:
			case	E_GrUiScptPrIdGrpEdtBitPos5:
			case	E_GrUiScptPrIdGrpEdtBitPos6:
			case	E_GrUiScptPrIdGrpEdtBitPos7:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdGrpEdtBitPos0;
				A_Obj->BitPosSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyGrpEdtBitPos0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtClrAtb0:
			case	E_GrUiScptPrIdGrpEdtClrAtb1:
			case	E_GrUiScptPrIdGrpEdtClrAtb2:
			case	E_GrUiScptPrIdGrpEdtClrAtb3:
			case	E_GrUiScptPrIdGrpEdtClrAtb4:
			case	E_GrUiScptPrIdGrpEdtClrAtb5:
			case	E_GrUiScptPrIdGrpEdtClrAtb6:
			case	E_GrUiScptPrIdGrpEdtClrAtb7:
			case	E_GrUiScptPrIdGrpEdtClrAtb8:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdGrpEdtClrAtb0;
				A_Obj->ClrAtbSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyGrpEdtClrAtb0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtClrGrid:
				A_Obj->ClrGridSet( *((E_GrUiScptPrTyGrpEdtClrGrid*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtGridWidth:
				A_Obj->GridWidthSet( *((E_GrUiScptPrTyGrpEdtGridWidth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtClrCus:
				A_Obj->ClrCursorSet( *((E_GrUiScptPrTyGrpEdtClrCus*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtCusWidth:
				A_Obj->CursorWidthSet( *((E_GrUiScptPrTyGrpEdtCusWidth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtColItmSize:
				A_Obj->ColumItemSizeSet( *((E_GrUiScptPrTyGrpEdtColItmSize*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtLineItmSize:
				A_Obj->LineItemSizeSet( *((E_GrUiScptPrTyGrpEdtLineItmSize*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtLineGap:
				A_Obj->LineGapSet( *((E_GrUiScptPrTyGrpEdtLineGap*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtRltLblCol:
				A_Obj->RltLblColumIdSet( *((E_GrUiScptPrTyGrpEdtRltLblCol*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtRltLblLine:
				A_Obj->RltLblLineIdSet( *((E_GrUiScptPrTyGrpEdtRltLblLine*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdGrpEdtValCnt:
				A_Obj->ValueCountSet( *((E_GrUiScptPrTyGrpEdtValCnt*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvBitGrid( Cls_GrUiDoBitGrid* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	TRUE;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdBitGridCelSizeX:
				A_Obj->CellXsizeSet( *((E_GrUiScptPrTyBitGridCelSizeX*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridCelSizeY:
				A_Obj->CellYsizeSet( *((E_GrUiScptPrTyBitGridCelSizeY*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridGridCntX:
				A_Obj->GridXcountSet( *((E_GrUiScptPrTyBitGridGridCntX*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridGridCntY:
				A_Obj->GridYcountSet( *((E_GrUiScptPrTyBitGridGridCntY*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridGridWidth:
				A_Obj->GridWidthSet( *((E_GrUiScptPrTyBitGridGridWidth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridGridColor:
				A_Obj->GridColorSet( *((E_GrUiScptPrTyBitGridGridColor*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridMarkWidth:
				A_Obj->MarkWidthSet( *((E_GrUiScptPrTyBitGridMarkWidth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridMarkColor:
				A_Obj->MarkColorSet( *((E_GrUiScptPrTyBitGridMarkColor*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridCusWidth:
				A_Obj->CursorWidthSet( *((E_GrUiScptPrTyBitGridCusWidth*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridCusColor:
				A_Obj->CursorColorSet( *((E_GrUiScptPrTyBitGridCusColor*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdBitGridDispColor:
				A_Obj->DisplayColorSet( *((E_GrUiScptPrTyBitGridDispColor*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvSpinImg( Cls_GrUiDoSpinImg* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpnImgImage0:
			case	E_GrUiScptPrIdSpnImgImage1:
			case	E_GrUiScptPrIdSpnImgImage2:
			case	E_GrUiScptPrIdSpnImgImage3:
			case	E_GrUiScptPrIdSpnImgImage4:
			case	E_GrUiScptPrIdSpnImgImage5:
			case	E_GrUiScptPrIdSpnImgImage6:
			case	E_GrUiScptPrIdSpnImgImage7:
			case	E_GrUiScptPrIdSpnImgImage8:
			case	E_GrUiScptPrIdSpnImgImage9:
			case	E_GrUiScptPrIdSpnImgImage10:
			case	E_GrUiScptPrIdSpnImgImage11:
			case	E_GrUiScptPrIdSpnImgImage12:
			case	E_GrUiScptPrIdSpnImgImage13:
			case	E_GrUiScptPrIdSpnImgImage14:
			case	E_GrUiScptPrIdSpnImgImage15:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSpnImgImage0;
				A_Obj->ImageSet( (__u8)Tv_Idx, ((E_GrUiScptPrTySpnImgImage0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpnImgVertical:
				A_Obj->VerticalTypeSet( *((E_GrUiScptPrTySpnImgVertical*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpnImgImageCnt:
				A_Obj->ImageCountSet( *((E_GrUiScptPrTySpnImgImageCnt*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdSpnImgTxtStId:
				A_Obj->TextStartIdSet( *((E_GrUiScptPrTySpnImgTxtStId*)A_PtrBuf) );
				break;
						
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvPrgsBar( Cls_GrUiDoPrgsbar* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	TRUE;

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdPrgbarBarColor:
				A_Obj->BarColorSet( *((E_GrUiScptPrTyPrgbarBarColor*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdPrgbarBarImage:
				A_Obj->BarImageSet( ((E_GrUiScptPrTyPrgbarBarImage*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdPrgbarValMax:
				A_Obj->ValueMaxSet( *((E_GrUiScptPrTyPrgbarValMax*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdPrgbarVertical:
				A_Obj->VerticalTypeSet( *((E_GrUiScptPrTyPrgbarVertical*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdPrgbarRelCntType:
				A_Obj->RltLblTypeCntSet( *((E_GrUiScptPrTyPrgbarRelCntType*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdPrgbarRelLblId:
				A_Obj->RltLblIdSet( *((E_GrUiScptPrTyPrgbarRelLblId*)A_PtrBuf) );
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvInpTime( Cls_GrUiDoInpTime* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	TRUE;

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdInpTimeYearId:
				A_Obj->RelIdYearSet( *((E_GrUiScptPrTyInpTimeYearId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeMonId:
				A_Obj->RelIdMonthSet( *((E_GrUiScptPrTyInpTimeMonId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeDayId:
				A_Obj->RelIdDaySet( *((E_GrUiScptPrTyInpTimeDayId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeHourId:
				A_Obj->RelIdHourSet( *((E_GrUiScptPrTyInpTimeHourId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeMinId:
				A_Obj->RelIdMinSet( *((E_GrUiScptPrTyInpTimeMinId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeSecId:
				A_Obj->RelIdSecSet( *((E_GrUiScptPrTyInpTimeSecId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeUnYearId:
				A_Obj->RelIdUnitYearSet( *((E_GrUiScptPrTyInpTimeUnYearId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeUnMonId:
				A_Obj->RelIdUnitMonthSet( *((E_GrUiScptPrTyInpTimeUnMonId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeUnDayId:
				A_Obj->RelIdUnitDaySet( *((E_GrUiScptPrTyInpTimeUnDayId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeDiv1Id:
				A_Obj->RelIdDiv1Set( *((E_GrUiScptPrTyInpTimeDiv1Id*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdInpTimeDiv2Id:
				A_Obj->RelIdDiv2Set( *((E_GrUiScptPrTyInpTimeDiv2Id*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvArea( Cls_GrUiDoArea* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
		//not exist
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvCdvArea( Cls_GrUiDoCdvArea* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdCdvAreaSrcIdx31:
			case	E_GrUiScptPrIdCdvAreaSrcIdx30:
			case	E_GrUiScptPrIdCdvAreaSrcIdx29:
			case	E_GrUiScptPrIdCdvAreaSrcIdx28:
			case	E_GrUiScptPrIdCdvAreaSrcIdx27:
			case	E_GrUiScptPrIdCdvAreaSrcIdx26:
			case	E_GrUiScptPrIdCdvAreaSrcIdx25:
			case	E_GrUiScptPrIdCdvAreaSrcIdx24:
			case	E_GrUiScptPrIdCdvAreaSrcIdx23:
			case	E_GrUiScptPrIdCdvAreaSrcIdx22:
			case	E_GrUiScptPrIdCdvAreaSrcIdx21:
			case	E_GrUiScptPrIdCdvAreaSrcIdx20:
			case	E_GrUiScptPrIdCdvAreaSrcIdx19:
			case	E_GrUiScptPrIdCdvAreaSrcIdx18:
			case	E_GrUiScptPrIdCdvAreaSrcIdx17:
			case	E_GrUiScptPrIdCdvAreaSrcIdx16:
			case	E_GrUiScptPrIdCdvAreaSrcIdx15:
			case	E_GrUiScptPrIdCdvAreaSrcIdx14:
			case	E_GrUiScptPrIdCdvAreaSrcIdx13:
			case	E_GrUiScptPrIdCdvAreaSrcIdx12:
			case	E_GrUiScptPrIdCdvAreaSrcIdx11:
			case	E_GrUiScptPrIdCdvAreaSrcIdx10:
			case	E_GrUiScptPrIdCdvAreaSrcIdx9:
			case	E_GrUiScptPrIdCdvAreaSrcIdx8:
			case	E_GrUiScptPrIdCdvAreaSrcIdx7:
			case	E_GrUiScptPrIdCdvAreaSrcIdx6:
			case	E_GrUiScptPrIdCdvAreaSrcIdx5:
			case	E_GrUiScptPrIdCdvAreaSrcIdx4:
			case	E_GrUiScptPrIdCdvAreaSrcIdx3:
			case	E_GrUiScptPrIdCdvAreaSrcIdx2:
			case	E_GrUiScptPrIdCdvAreaSrcIdx1:
			case	E_GrUiScptPrIdCdvAreaSrcIdx0:
				Tv_Idx	= A_ParaId - E_GrUiScptPrIdCdvAreaSrcIdx0;
				A_Obj->CondSrcIdxSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyCdvAreaSrcIdx0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCdvAreaConType0:
			case	E_GrUiScptPrIdCdvAreaConType1:
			case	E_GrUiScptPrIdCdvAreaConType2:
			case	E_GrUiScptPrIdCdvAreaConType3:
			case	E_GrUiScptPrIdCdvAreaConType4:
			case	E_GrUiScptPrIdCdvAreaConType5:
			case	E_GrUiScptPrIdCdvAreaConType6:
			case	E_GrUiScptPrIdCdvAreaConType7:
			case	E_GrUiScptPrIdCdvAreaConType8:
			case	E_GrUiScptPrIdCdvAreaConType9:
			case	E_GrUiScptPrIdCdvAreaConType10:
			case	E_GrUiScptPrIdCdvAreaConType11:
			case	E_GrUiScptPrIdCdvAreaConType12:
			case	E_GrUiScptPrIdCdvAreaConType13:
			case	E_GrUiScptPrIdCdvAreaConType14:
			case	E_GrUiScptPrIdCdvAreaConType15:
			case	E_GrUiScptPrIdCdvAreaConType16:
			case	E_GrUiScptPrIdCdvAreaConType17:
			case	E_GrUiScptPrIdCdvAreaConType18:
			case	E_GrUiScptPrIdCdvAreaConType19:
			case	E_GrUiScptPrIdCdvAreaConType20:
			case	E_GrUiScptPrIdCdvAreaConType21:
			case	E_GrUiScptPrIdCdvAreaConType22:
			case	E_GrUiScptPrIdCdvAreaConType23:
			case	E_GrUiScptPrIdCdvAreaConType24:
			case	E_GrUiScptPrIdCdvAreaConType25:
			case	E_GrUiScptPrIdCdvAreaConType26:
			case	E_GrUiScptPrIdCdvAreaConType27:
			case	E_GrUiScptPrIdCdvAreaConType28:
			case	E_GrUiScptPrIdCdvAreaConType29:
			case	E_GrUiScptPrIdCdvAreaConType30:
			case	E_GrUiScptPrIdCdvAreaConType31:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdCdvAreaConType0;
				A_Obj->CondTypeSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyCdvAreaSrcIdx0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdCdvAreaRefVal0:
			case	E_GrUiScptPrIdCdvAreaRefVal1:
			case	E_GrUiScptPrIdCdvAreaRefVal2:
			case	E_GrUiScptPrIdCdvAreaRefVal3:
			case	E_GrUiScptPrIdCdvAreaRefVal4:
			case	E_GrUiScptPrIdCdvAreaRefVal5:
			case	E_GrUiScptPrIdCdvAreaRefVal6:
			case	E_GrUiScptPrIdCdvAreaRefVal7:
			case	E_GrUiScptPrIdCdvAreaRefVal8:
			case	E_GrUiScptPrIdCdvAreaRefVal9:
			case	E_GrUiScptPrIdCdvAreaRefVal10:
			case	E_GrUiScptPrIdCdvAreaRefVal11:
			case	E_GrUiScptPrIdCdvAreaRefVal12:
			case	E_GrUiScptPrIdCdvAreaRefVal13:
			case	E_GrUiScptPrIdCdvAreaRefVal14:
			case	E_GrUiScptPrIdCdvAreaRefVal15:
			case	E_GrUiScptPrIdCdvAreaRefVal16:
			case	E_GrUiScptPrIdCdvAreaRefVal17:
			case	E_GrUiScptPrIdCdvAreaRefVal18:
			case	E_GrUiScptPrIdCdvAreaRefVal19:
			case	E_GrUiScptPrIdCdvAreaRefVal20:
			case	E_GrUiScptPrIdCdvAreaRefVal21:
			case	E_GrUiScptPrIdCdvAreaRefVal22:
			case	E_GrUiScptPrIdCdvAreaRefVal23:
			case	E_GrUiScptPrIdCdvAreaRefVal24:
			case	E_GrUiScptPrIdCdvAreaRefVal25:
			case	E_GrUiScptPrIdCdvAreaRefVal26:
			case	E_GrUiScptPrIdCdvAreaRefVal27:
			case	E_GrUiScptPrIdCdvAreaRefVal28:
			case	E_GrUiScptPrIdCdvAreaRefVal29:
			case	E_GrUiScptPrIdCdvAreaRefVal30:
			case	E_GrUiScptPrIdCdvAreaRefVal31:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdCdvAreaRefVal0;
				A_Obj->CondRefSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyCdvAreaSrcIdx0*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvAcBtn( Cls_GrUiDoAcBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdAcBtnAcObj0:
			case	E_GrUiScptPrIdAcBtnAcObj1:
			case	E_GrUiScptPrIdAcBtnAcObj2:
			case	E_GrUiScptPrIdAcBtnAcObj3:
			case	E_GrUiScptPrIdAcBtnAcObj4:
			case	E_GrUiScptPrIdAcBtnAcObj5:
			case	E_GrUiScptPrIdAcBtnAcObj6:
			case	E_GrUiScptPrIdAcBtnAcObj7:
			case	E_GrUiScptPrIdAcBtnAcObj8:
			case	E_GrUiScptPrIdAcBtnAcObj9:
			case	E_GrUiScptPrIdAcBtnAcObj10:
			case	E_GrUiScptPrIdAcBtnAcObj11:
			case	E_GrUiScptPrIdAcBtnAcObj12:
			case	E_GrUiScptPrIdAcBtnAcObj13:
			case	E_GrUiScptPrIdAcBtnAcObj14:
			case	E_GrUiScptPrIdAcBtnAcObj15:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdAcBtnAcObj0;
				A_Obj->ActObjSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyAcBtnAcObj0*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdAcBtnAcRvs:
				A_Obj->ActRvsSet( *((E_GrUiScptPrTyAcBtnAcRvs*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvAcSpNum( Cls_GrUiDoAcSpNum* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdAcSpBtnAcObj0:
			case	E_GrUiScptPrIdAcSpBtnAcObj1:
			case	E_GrUiScptPrIdAcSpBtnAcObj2:
			case	E_GrUiScptPrIdAcSpBtnAcObj3:
			case	E_GrUiScptPrIdAcSpBtnAcObj4:
			case	E_GrUiScptPrIdAcSpBtnAcObj5:
			case	E_GrUiScptPrIdAcSpBtnAcObj6:
			case	E_GrUiScptPrIdAcSpBtnAcObj7:
			case	E_GrUiScptPrIdAcSpBtnAcObj8:
			case	E_GrUiScptPrIdAcSpBtnAcObj9:
			case	E_GrUiScptPrIdAcSpBtnAcObj10:
			case	E_GrUiScptPrIdAcSpBtnAcObj11:
			case	E_GrUiScptPrIdAcSpBtnAcObj12:
			case	E_GrUiScptPrIdAcSpBtnAcObj13:
			case	E_GrUiScptPrIdAcSpBtnAcObj14:
			case	E_GrUiScptPrIdAcSpBtnAcObj15:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdAcSpBtnAcObj0;
				A_Obj->ActObjSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyAcSpBtnAcObj0*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvAcSpTxt( Cls_GrUiDoAcSpTxt* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdAcSpTxtAcObj0:
			case	E_GrUiScptPrIdAcSpTxtAcObj1:
			case	E_GrUiScptPrIdAcSpTxtAcObj2:
			case	E_GrUiScptPrIdAcSpTxtAcObj3:
			case	E_GrUiScptPrIdAcSpTxtAcObj4:
			case	E_GrUiScptPrIdAcSpTxtAcObj5:
			case	E_GrUiScptPrIdAcSpTxtAcObj6:
			case	E_GrUiScptPrIdAcSpTxtAcObj7:
			case	E_GrUiScptPrIdAcSpTxtAcObj8:
			case	E_GrUiScptPrIdAcSpTxtAcObj9:
			case	E_GrUiScptPrIdAcSpTxtAcObj10:
			case	E_GrUiScptPrIdAcSpTxtAcObj11:
			case	E_GrUiScptPrIdAcSpTxtAcObj12:
			case	E_GrUiScptPrIdAcSpTxtAcObj13:
			case	E_GrUiScptPrIdAcSpTxtAcObj14:
			case	E_GrUiScptPrIdAcSpTxtAcObj15:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdAcSpTxtAcObj0;
				A_Obj->ActObjSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyAcSpTxtAcObj0*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvAcSpImg( Cls_GrUiDoAcSpImg* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdAcSpImgAcObj0:
			case	E_GrUiScptPrIdAcSpImgAcObj1:
			case	E_GrUiScptPrIdAcSpImgAcObj2:
			case	E_GrUiScptPrIdAcSpImgAcObj3:
			case	E_GrUiScptPrIdAcSpImgAcObj4:
			case	E_GrUiScptPrIdAcSpImgAcObj5:
			case	E_GrUiScptPrIdAcSpImgAcObj6:
			case	E_GrUiScptPrIdAcSpImgAcObj7:
			case	E_GrUiScptPrIdAcSpImgAcObj8:
			case	E_GrUiScptPrIdAcSpImgAcObj9:
			case	E_GrUiScptPrIdAcSpImgAcObj10:
			case	E_GrUiScptPrIdAcSpImgAcObj11:
			case	E_GrUiScptPrIdAcSpImgAcObj12:
			case	E_GrUiScptPrIdAcSpImgAcObj13:
			case	E_GrUiScptPrIdAcSpImgAcObj14:
			case	E_GrUiScptPrIdAcSpImgAcObj15:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdAcSpImgAcObj0;
				A_Obj->ActObjSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyAcSpImgAcObj0*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvAcSpDgt( Cls_GrUiDoAcSpDgt* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result		=	TRUE;

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdAcSpDgtAcObj0:
			case	E_GrUiScptPrIdAcSpDgtAcObj1:
			case	E_GrUiScptPrIdAcSpDgtAcObj2:
			case	E_GrUiScptPrIdAcSpDgtAcObj3:
			case	E_GrUiScptPrIdAcSpDgtAcObj4:
			case	E_GrUiScptPrIdAcSpDgtAcObj5:
			case	E_GrUiScptPrIdAcSpDgtAcObj6:
			case	E_GrUiScptPrIdAcSpDgtAcObj7:
			case	E_GrUiScptPrIdAcSpDgtAcObj8:
			case	E_GrUiScptPrIdAcSpDgtAcObj9:
			case	E_GrUiScptPrIdAcSpDgtAcObj10:
			case	E_GrUiScptPrIdAcSpDgtAcObj11:
			case	E_GrUiScptPrIdAcSpDgtAcObj12:
			case	E_GrUiScptPrIdAcSpDgtAcObj13:
			case	E_GrUiScptPrIdAcSpDgtAcObj14:
			case	E_GrUiScptPrIdAcSpDgtAcObj15:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdAcSpDgtAcObj0;
				A_Obj->ActObjSet( (__u8)Tv_Idx, *((E_GrUiScptPrTyAcSpDgtAcObj0*)A_PtrBuf) );
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiRtlPrStvLbl( Cls_GrUiDoLbl* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	TRUE;
		
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdLblTextId:
				A_Obj->TextIdSet( *((E_GrUiScptPrTyLblTextId*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLblTextColor:
				A_Obj->TextColorSet( *((E_GrUiScptPrTyLblTextColor*)A_PtrBuf) );
				break;
			case	E_GrUiScptPrIdLblTextPara:
				A_Obj->TextParaSet( ((E_GrUiScptPrTyLblTextPara*)A_PtrBuf) );
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	GrUiRtlCreateObj( __u8 A_TypeId, Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsAlocId )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	NULL;
		
		//check exist parent
		if ( NULL != A_ObjPrnt )
		{
			//create
			switch( A_TypeId )
			{
				case	E_GrUiDoTypeIdFrame:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoFrm( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdButton:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoBtn( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdLabel:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoLbl( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdListbox:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoListbox( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdSbar:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoSbar( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdSpbtnNum:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoSpinNum( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdSpbtnTxt:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoSpinTxt( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdSpbtnDgt:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoSpinDgt( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdEdit:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoEdit( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdSysBtn:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoSysBtn( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdCalendar:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoCalendar( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdQuad:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoQuad( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdGrpSel:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoGrpSel( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdGrpEdit:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoGrpEdit( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdBitGrid:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoBitGrid( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdSpbtnImg:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoSpinImg( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdPrgsbar:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoPrgsbar( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdInpTime:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoInpTime( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdArea:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoArea( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdCdvArea:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoCdvArea( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdAcBtn:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoAcBtn( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdAcSpNum:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoAcSpNum( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdAcSpTxt:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoAcSpTxt( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdAcSpImg:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoAcSpImg( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				case	E_GrUiDoTypeIdAcSpDgt:
					Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoAcSpDgt( A_PtrRect, A_ObjPrnt, A_IsAlocId );
					break;
				//case E_GrUiDoTypeIdMosaicGrid:
				//	Tv_Result	=	(Cls_GrUiDoBase*)new Cls_GrUiDoMosaicGrid( A_PtrRect, A_ObjPrnt, A_IsAlocId );
				//	break;//new-120905~mosaic
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
//BOOL8	GrUiRtlPrStvMosaicGrid( Cls_GrUiDoMosaicGrid* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
//{
//	// local -------------------
//	BOOL8	Tv_Result;
//	// code --------------------
//	Tv_Result		=	TRUE;
//	switch( A_ParaId )
//	{
//	case	E_GrUiScptPrIdMosaicGridCelSizeX:
//		A_Obj->CellXsizeSet( *((E_GrUiScptPrTyMosaicGridCelSizeX*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridCelSizeY:
//		A_Obj->CellYsizeSet( *((E_GrUiScptPrTyMosaicGridCelSizeY*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridGridCntX:
//		A_Obj->GridXcountSet( *((E_GrUiScptPrTyMosaicGridGridCntX*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridGridCntY:
//		A_Obj->GridYcountSet( *((E_GrUiScptPrTyMosaicGridGridCntY*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridGridWidth:
//		A_Obj->GridWidthSet( *((E_GrUiScptPrTyMosaicGridGridWidth*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridGridColor:
//		A_Obj->GridColorSet( *((E_GrUiScptPrTyMosaicGridGridColor*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridMarkWidth:
//		A_Obj->MarkWidthSet( *((E_GrUiScptPrTyMosaicGridMarkWidth*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridMarkColor:
//		A_Obj->MarkColorSet( *((E_GrUiScptPrTyMosaicGridMarkColor*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridCusWidth:
//		A_Obj->CursorWidthSet( *((E_GrUiScptPrTyMosaicGridCusWidth*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridCusColor:
//		A_Obj->CursorColorSet( *((E_GrUiScptPrTyMosaicGridCusColor*)A_PtrBuf) );
//		break;
//	case	E_GrUiScptPrIdMosaicGridDispColor:
//		A_Obj->DisplayColorSet( *((E_GrUiScptPrTyMosaicGridDispColor*)A_PtrBuf) );
//		break;
//
//	default:
//		Tv_Result	=	FALSE;
//		break;
//	}
//	return	Tv_Result;
//}//new-120905~mosaic
//--------------------------------------------------------------------

