/*
 osd develop 

 */

//====================================================================
// uses
#include	<DvlUi/GrUiDvlPara.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiBase.h>
#include	<Ui/GrUiScptDec.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiDoSkin.h>
#include	<GrDumyTool.h>
#include	<GrDrawTool.h>
#include	<GrGdibObj.h>
#include	<Ui/GrUiDoWin.h>
#include	<Ui/GrUiDoFrm.h>
#include	<Ui/GrUiDoBtn.h>
#include	<Ui/GrUiDoLbl.h>
#include	<Ui/GrUiDoListbox.h>
#include	<Ui/GrUiDoSbar.h>
#include	<Ui/GrUiConsole.h>
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

//====================================================================
//local const

#define E_GrUiDvlParaMaxScptTypeCnt			16
#define E_GrUiDvlParaMaxExceptParaCnt		32

//====================================================================
//local type
typedef	struct St_GrUiDvlParaObjAblePara 
{
	__u16		ParaTypeTbl[E_GrUiDvlParaMaxScptTypeCnt];
	__u16		ExcpParaTbl[E_GrUiDvlParaMaxExceptParaCnt];
}	*Ptr_GrUiDvlParaObjAblePara;


//====================================================================
//local function
__u32	GrUiDvlGetPrValBase( Cls_GrUiDoBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValSkin( Cls_GrUiDoSkin* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValWin( Cls_GrUiDoWin* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValFrm( Cls_GrUiDoFrm* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValBtn( Cls_GrUiDoBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValLbl( Cls_GrUiDoLbl* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValListBox( Cls_GrUiDoListbox* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValSbar( Cls_GrUiDoSbar* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValSpinBase( Cls_GrUiDoSpinBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValSpinNum( Cls_GrUiDoSpinNum* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValSpinTxt( Cls_GrUiDoSpinTxt* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValSpinDgt( Cls_GrUiDoSpinDgt* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValSmpBtn( Cls_GrUiDoSmpBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValEdit( Cls_GrUiDoEdit* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValSysBtn( Cls_GrUiDoSysBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValCalendar( Cls_GrUiDoCalendar* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValQuad( Cls_GrUiDoQuad* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValGrpSel( Cls_GrUiDoGrpSel* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValGrpEdit( Cls_GrUiDoGrpEdit* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValBitGrid( Cls_GrUiDoBitGrid* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValSpinImg( Cls_GrUiDoSpinImg* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValPrgsBar( Cls_GrUiDoPrgsbar* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValInpTime( Cls_GrUiDoInpTime* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValArea( Cls_GrUiDoArea* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValCdvArea( Cls_GrUiDoCdvArea* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValAcBtn( Cls_GrUiDoAcBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValAcSpNum( Cls_GrUiDoAcSpNum* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValAcSpTxt( Cls_GrUiDoAcSpTxt* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValAcSpImg( Cls_GrUiDoAcSpImg* A_Obj, __u16 A_ParaId, void* A_PtrBuf );
__u32	GrUiDvlGetPrValAcSpDgt( Cls_GrUiDoAcSpDgt* A_Obj, __u16 A_ParaId, void* A_PtrBuf );

void	GrUiDvlGetValColorText( __u32 A_Val, WCHAR* A_StrRt );
void	GrUiDvlGetValFocusMapText( __u8 A_Map, WCHAR* A_StrRt );
void	GrUiDvlGetValBorderAtbText( __u8 A_Atb, WCHAR* A_StrRt );
void	GrUiDvlGetValTextParaText( Ptr_GrUiDoTextPara A_PtrPara, WCHAR* A_StrRt );
void	GrUiDvlGetValTextIdText( __u32 A_Val, WCHAR* A_StrRt );
void	GrUiDvlGetValSkinText( Ptr_GrUiDoSkinImg A_PtrVal, WCHAR* A_StrRt );
void	GrUiDvlGetValDirText( __u8 A_Val, WCHAR* A_StrRt );
void	GrUiDvlGetValCloseKeyText( __u32 A_Val, WCHAR* A_StrRt );
void	GrUiDvlGetValTimeMsText( __u32 A_Val, WCHAR* A_StrRt );
void	GrUiDvlGetValGvalText( __u8 A_Val, WCHAR* A_StrRt );
void	GrUiDvlGetValCondText( __u8 A_Val, WCHAR* A_StrRt );
void	GrUiDvlGetValBitMaskText( __u32 A_Val, WCHAR* A_StrRt );

//====================================================================
//var

BOOL8	V_IsGrUiDvlParaDbg		=	FALSE;		//debug mode
__u32	V_GrUiDvlOsdFcc;
__u32	V_IsGrUiDvlOsdPlt			=	FALSE;		//pallete mode

__u32	V_GrUiParaTypeSize[E_GrUiParaTypeCnt]	=	
{		
/* 00 */	sizeof(__u32),	sizeof(__u16),	sizeof(__u8),	sizeof(__s32) , sizeof(__s16), sizeof(__s8),
/* 06 */	sizeof(BOOL8),  sizeof(St_GrByteRect), sizeof(E_GrUiScptPrTyFocusMap),
/* 09 */	sizeof(E_GrUiScptPrTyClrBg), sizeof(E_GrUiScptPrTyBorderAtb),
/* 10 */	sizeof(St_GrUiDoTextPara), sizeof(E_GrUiScptPrTyLblTextId),
/* 13 */	sizeof(E_GrUiScptPrTyCloseKeyType), sizeof(E_GrUiScptPrTyCloseTime),
/* 15 */	sizeof(St_GrUiDoSkinImg), sizeof(E_GrUiScptPrTyBtnType), sizeof(E_GrUiScptPrTyLbxIconPos),
/* 18 */	sizeof(E_GrUiScptPrTyLblTextColor),	sizeof(__u8), sizeof(__u8), sizeof(__u32)
};

St_GrUiParaDesc	V_GrUiParaInfo[]	=	
{
/*	paraId												type											name						  */
//base
{	E_GrUiScptPrIdIsEnable,					E_GrUiParaTypeBool8,			{L"Enable"} },
{	E_GrUiScptPrIdFocusBlock,				E_GrUiParaTypeBool8,			{L"FocusBlock"} },
{	E_GrUiScptPrIdFocusMap,					E_GrUiParaTypeFocusMap,		{L"FocusMap"} },
{	E_GrUiScptPrIdFocusBaseId,			E_GrUiParaTypeWord,				{L"FocusBaseCtrl"} },
{	E_GrUiScptPrIdCliMargin,				E_GrUiParaTypeByteRect,		{L"ClientMargin"} },
{	E_GrUiScptPrIdUserData0,				E_GrUiParaTypeDword,			{L"UserData0"} },
{	E_GrUiScptPrIdUserData1,				E_GrUiParaTypeDword,			{L"UserData1"} },
{	E_GrUiScptPrIdUserData2,				E_GrUiParaTypeDword,			{L"UserData2"} },
{	E_GrUiScptPrIdUserData3,				E_GrUiParaTypeDword,			{L"UserData3"} },
{	E_GrUiScptPrIdExtPosY,					E_GrUiParaTypeByte,				{L"PalPosY"} },
{	E_GrUiScptPrIdExtSizeY,					E_GrUiParaTypeByte,				{L"PalSizeY"} },
{	E_GrUiScptPrIdRelFcsLblId,			E_GrUiParaTypeWord,				{L"RelFcsLabel"} },
{	E_GrUiScptPrIdRelFcsTxtId,			E_GrUiParaTypeTxtId,			{L"RelFcsText"} },
{	E_GrUiScptPrIdCndVisMap0,				E_GrUiParaTypeBitMask,		{L"CondVisMap0"} },
{	E_GrUiScptPrIdCndVisMap1,				E_GrUiParaTypeBitMask,		{L"CondVisMap1"} },

//skin
{	E_GrUiScptPrIdIsBgSprite,				E_GrUiParaTypeBool8,			{L"BgSprite"} },
{	E_GrUiScptPrIdClrBg,						E_GrUiParaTypeColor,			{L"ColorBg"} },
{	E_GrUiScptPrIdSkin0,						E_GrUiParaTypeSkin,				{L"Skin0"} },
{	E_GrUiScptPrIdSkin1,						E_GrUiParaTypeSkin,				{L"Skin1"} },
{	E_GrUiScptPrIdSkin2,						E_GrUiParaTypeSkin,				{L"Skin2"} },
{	E_GrUiScptPrIdSkin3,						E_GrUiParaTypeSkin,				{L"Skin3"} },
{	E_GrUiScptPrIdSkin4,						E_GrUiParaTypeSkin,				{L"Skin4"} },
{	E_GrUiScptPrIdSkin5,						E_GrUiParaTypeSkin,				{L"Skin5"} },
{	E_GrUiScptPrIdSkin6,						E_GrUiParaTypeSkin,				{L"Skin6"} },
{	E_GrUiScptPrIdSkin7,						E_GrUiParaTypeSkin,				{L"Skin7"} },
{	E_GrUiScptPrIdSkin8,						E_GrUiParaTypeSkin,				{L"Skin8"} },
{	E_GrUiScptPrIdSkin9,						E_GrUiParaTypeSkin,				{L"Skin9"} },
{	E_GrUiScptPrIdSkin10,						E_GrUiParaTypeSkin,				{L"Skin10"} },
{	E_GrUiScptPrIdSkin11,						E_GrUiParaTypeSkin,				{L"Skin11"} },
{	E_GrUiScptPrIdSkin12,						E_GrUiParaTypeSkin,				{L"Skin12"} },
{	E_GrUiScptPrIdSkin13,						E_GrUiParaTypeSkin,				{L"Skin13"} },
{	E_GrUiScptPrIdSkin14,						E_GrUiParaTypeSkin,				{L"Skin14"} },
{	E_GrUiScptPrIdSkin15,						E_GrUiParaTypeSkin,				{L"Skin15"} },
{	E_GrUiScptPrIdBorderAtb,				E_GrUiParaTypeBorderAtb,	{L"BrdAtb"} },
{	E_GrUiScptPrIdBorderDepth,			E_GrUiParaTypeByte,				{L"BrdDepth"} },
{	E_GrUiScptPrIdBorderClrLt,			E_GrUiParaTypeColor,			{L"BrdClrLight"} },
{	E_GrUiScptPrIdBorderClrDk,			E_GrUiParaTypeColor,			{L"BrdClrDark"} },
{	E_GrUiScptPrIdBorderClrMl,			E_GrUiParaTypeColor,			{L"BrdClrMlight"} },
{	E_GrUiScptPrIdBorderClrMd,			E_GrUiParaTypeColor,			{L"BrdClrMdark"} },

//win
{	E_GrUiScptPrIdIsMoveAble,				E_GrUiParaTypeBool8,			{L"IsMoveAble"} },
{	E_GrUiScptPrIdCloseKeyType,			E_GrUiParaTypeCloseKey,		{L"CloseKey"} },
{	E_GrUiScptPrIdCloseTime,				E_GrUiParaTypeTimeValMs,	{L"CloseTime"} },
{	E_GrUiScptPrIdIsLayerLock,			E_GrUiParaTypeBool8,			{L"IsLayerLock"} },
{	E_GrUiScptPrIdIsFocusAble,			E_GrUiParaTypeBool8,			{L"IsFocusAble"} },
{	E_GrUiScptPrIdWinRelFrmId,			E_GrUiParaTypeWord,				{L"RelFrameId"} },

//frame
{	E_GrUiScptPrIdFrmViewPage,			E_GrUiParaTypeByte,				{L"ViewIndex"} },
{	E_GrUiScptPrIdFrmPageId0,				E_GrUiParaTypeWord,				{L"Page0Id"} },
{	E_GrUiScptPrIdFrmPageId1,				E_GrUiParaTypeWord,				{L"Page1Id"} },
{	E_GrUiScptPrIdFrmPageId2,				E_GrUiParaTypeWord,				{L"Page2Id"} },
{	E_GrUiScptPrIdFrmPageId3,				E_GrUiParaTypeWord,				{L"Page3Id"} },
{	E_GrUiScptPrIdFrmPageId4,				E_GrUiParaTypeWord,				{L"Page4Id"} },
{	E_GrUiScptPrIdFrmPageId5,				E_GrUiParaTypeWord,				{L"Page5Id"} },
{	E_GrUiScptPrIdFrmPageId6,				E_GrUiParaTypeWord,				{L"Page6Id"} },
{	E_GrUiScptPrIdFrmPageId7,				E_GrUiParaTypeWord,				{L"Page7Id"} },
{	E_GrUiScptPrIdFrmPageId8,				E_GrUiParaTypeWord,				{L"Page8Id"} },
{	E_GrUiScptPrIdFrmPageId9,				E_GrUiParaTypeWord,				{L"Page9Id"} },
{	E_GrUiScptPrIdFrmPageId10,			E_GrUiParaTypeWord,				{L"Page10Id"} },
{	E_GrUiScptPrIdFrmPageId11,			E_GrUiParaTypeWord,				{L"Page11Id"} },
{	E_GrUiScptPrIdFrmPageId12,			E_GrUiParaTypeWord,				{L"Page12Id"} },
{	E_GrUiScptPrIdFrmPageId13,			E_GrUiParaTypeWord,				{L"Page13Id"} },
{	E_GrUiScptPrIdFrmPageId14,			E_GrUiParaTypeWord,				{L"Page14Id"} },
{	E_GrUiScptPrIdFrmPageId15,			E_GrUiParaTypeWord,				{L"Page15Id"} },
{	E_GrUiScptPrIdFrmNoFcsMov,			E_GrUiParaTypeBool8,			{L"NoMovFcs"} },
{	E_GrUiScptPrIdFrmRtlLblId,			E_GrUiParaTypeWord,				{L"RelLabelId"} },

//label
{	E_GrUiScptPrIdLblTextId,				E_GrUiParaTypeTxtId,			{L"TextId"} },
{	E_GrUiScptPrIdLblTextColor,			E_GrUiParaTypeTxtClr,			{L"TextColor"} },
{	E_GrUiScptPrIdLblTextPara,			E_GrUiParaTypeTxtPara,		{L"TextPara"} },

//button
{	E_GrUiScptPrIdBtnType,					E_GrUiParaTypeBtnType,		{L"BtnType"} },
{	E_GrUiScptPrIdBtnCheck,					E_GrUiParaTypeBool8,			{L"BtnCheck"} },
{	E_GrUiScptPrIdBtnImgDisable,		E_GrUiParaTypeSkin,				{L"ImgDisable"} },
{	E_GrUiScptPrIdBtnImgNormal,			E_GrUiParaTypeSkin,				{L"ImgNormal"} },
{	E_GrUiScptPrIdBtnImgNrmFcs,			E_GrUiParaTypeSkin,				{L"ImgNrmFocus"} },
{	E_GrUiScptPrIdBtnImgNrmPush,		E_GrUiParaTypeSkin,				{L"ImgNrmPush"} },
{	E_GrUiScptPrIdBtnImgCheck,			E_GrUiParaTypeSkin,				{L"ImgCheck"} },
{	E_GrUiScptPrIdBtnImgChkFcs,			E_GrUiParaTypeSkin,				{L"ImgChkFocus"} },
{	E_GrUiScptPrIdBtnImgChkPush,		E_GrUiParaTypeSkin,				{L"ImgChkPush"} },
{	E_GrUiScptPrIdBtnImgChkDis,			E_GrUiParaTypeSkin,				{L"ImgChkDisable"} },
{	E_GrUiScptPrIdBtnClrDisable,		E_GrUiParaTypeColor,			{L"ClrDisable"} },
{	E_GrUiScptPrIdBtnClrNormal,			E_GrUiParaTypeColor,			{L"ClrNormal"} },
{	E_GrUiScptPrIdBtnClrNrmFcs,			E_GrUiParaTypeColor,			{L"ClrNrmFocus"} },
{	E_GrUiScptPrIdBtnClrNrmPush,		E_GrUiParaTypeColor,			{L"ClrNrmPush"} },
{	E_GrUiScptPrIdBtnClrCheck,			E_GrUiParaTypeColor,			{L"ClrCheck"} },
{	E_GrUiScptPrIdBtnClrChkFcs,			E_GrUiParaTypeColor,			{L"ClrChkFocus"} },
{	E_GrUiScptPrIdBtnClrChkPush,		E_GrUiParaTypeColor,			{L"ClrChkPush"} },
{	E_GrUiScptPrIdBtnClrChkDis,			E_GrUiParaTypeColor,			{L"ClrChkDisable"} },
{	E_GrUiScptPrIdBtnTclrDisable,		E_GrUiParaTypeTxtClr,			{L"TclrDisable"} },
{	E_GrUiScptPrIdBtnTclrNormal,		E_GrUiParaTypeTxtClr,			{L"TclrNormal"} },
{	E_GrUiScptPrIdBtnTclrNrmFcs,		E_GrUiParaTypeTxtClr,			{L"TclrNrmFocus"} },
{	E_GrUiScptPrIdBtnTclrNrmPush,		E_GrUiParaTypeTxtClr,			{L"TclrNrmPush"} },
{	E_GrUiScptPrIdBtnTclrCheck,			E_GrUiParaTypeTxtClr,			{L"TclrCheck"} },
{	E_GrUiScptPrIdBtnTclrChkFcs,		E_GrUiParaTypeTxtClr,			{L"TclrChkFocus"} },
{	E_GrUiScptPrIdBtnTclrChkPush,		E_GrUiParaTypeTxtClr,			{L"TclrChkPush"} },
{	E_GrUiScptPrIdBtnTclrChkDis,		E_GrUiParaTypeTxtClr,			{L"TclrChkDisable"} },
{	E_GrUiScptPrIdBtnTextId,				E_GrUiParaTypeTxtId,			{L"TextId"} },
{	E_GrUiScptPrIdBtnTextPara,			E_GrUiParaTypeTxtPara,		{L"TextPara"} },
{	E_GrUiScptPrIdBtnEvtHostId,			E_GrUiParaTypeWord,				{L"EvtHostId"} },

//listbox
{	E_GrUiScptPrIdLbxEditAble,			E_GrUiParaTypeBool8,			{L"EditAble"} },
{	E_GrUiScptPrIdLbxEditNum,				E_GrUiParaTypeBool8,			{L"EditNumber"} },
{	E_GrUiScptPrIdLbxKeyDel,				E_GrUiParaTypeBool8,			{L"KeyDelAble"} },
{	E_GrUiScptPrIdLbxDelQst,				E_GrUiParaTypeBool8,			{L"QuestKeyDel"} },
{	E_GrUiScptPrIdLbxItmHeight,			E_GrUiParaTypeByte,				{L"ItemHeight"} },
{	E_GrUiScptPrIdLbxItmMargin,			E_GrUiParaTypeByteRect,		{L"ItemMargin"} },
{	E_GrUiScptPrIdLbxItmBrdDep,			E_GrUiParaTypeByte,				{L"ItemBrdDepth"} },
{	E_GrUiScptPrIdLbxItmBrdAtb,			E_GrUiParaTypeBorderAtb,	{L"ItemBrdAtb"} },
{	E_GrUiScptPrIdLbxItmGap,				E_GrUiParaTypeByte,				{L"ItemGap"} },
{	E_GrUiScptPrIdLbxIconWidth,			E_GrUiParaTypeByte,				{L"IconWidth"} },
{	E_GrUiScptPrIdLbxIconCount,			E_GrUiParaTypeByte,				{L"IconCount"} },
{	E_GrUiScptPrIdLbxIconPos,				E_GrUiParaTypeDirection,	{L"IconPos"} },
{	E_GrUiScptPrIdLbxIconGap,				E_GrUiParaTypeByte,				{L"IconGap"} },
{	E_GrUiScptPrIdLbxImgNormal,			E_GrUiParaTypeSkin,				{L"ImgItemNormal"} },
{	E_GrUiScptPrIdLbxImgOnCus,			E_GrUiParaTypeSkin,				{L"ImgItemOnCus"} },
{	E_GrUiScptPrIdLbxImgFcsOut,			E_GrUiParaTypeSkin,				{L"ImgItemFcsOut"} },
{	E_GrUiScptPrIdLbxClrNormal,			E_GrUiParaTypeColor,			{L"ClrItemNormal"} },
{	E_GrUiScptPrIdLbxClrOnCus,			E_GrUiParaTypeColor,			{L"ClrItemOnCus"} },
{	E_GrUiScptPrIdLbxClrFcsOut,			E_GrUiParaTypeColor,			{L"ClrItemFcsOut"} },
{	E_GrUiScptPrIdLbxTclrNormal,		E_GrUiParaTypeTxtClr,			{L"TclrItemNormal"} },
{	E_GrUiScptPrIdLbxTclrOnCus,			E_GrUiParaTypeTxtClr,			{L"TclrItemOnCus"} },
{	E_GrUiScptPrIdLbxTclrFcsOut,		E_GrUiParaTypeTxtClr,			{L"TclrItemFcsOut"} },
{	E_GrUiScptPrIdLbxBgClrDis,			E_GrUiParaTypeColor,			{L"ClrBgDisable"} },
{	E_GrUiScptPrIdLbxBgClrNrm,			E_GrUiParaTypeColor,			{L"ClrBgNormal"} },
{	E_GrUiScptPrIdLbxBgClrFcs,			E_GrUiParaTypeColor,			{L"ClrBgFocus"} },
{	E_GrUiScptPrIdLbxBgClrCtrl,			E_GrUiParaTypeColor,			{L"ClrBgControl"} },
{	E_GrUiScptPrIdLbxIconImg0,			E_GrUiParaTypeSkin,				{L"Icon0"} },
{	E_GrUiScptPrIdLbxIconImg1,			E_GrUiParaTypeSkin,				{L"Icon1"} },
{	E_GrUiScptPrIdLbxIconImg2,			E_GrUiParaTypeSkin,				{L"Icon2"} },
{	E_GrUiScptPrIdLbxIconImg3,			E_GrUiParaTypeSkin,				{L"Icon3"} },
{	E_GrUiScptPrIdLbxIconImg4,			E_GrUiParaTypeSkin,				{L"Icon4"} },
{	E_GrUiScptPrIdLbxIconImg5,			E_GrUiParaTypeSkin,				{L"Icon5"} },
{	E_GrUiScptPrIdLbxIconImg6,			E_GrUiParaTypeSkin,				{L"Icon6"} },
{	E_GrUiScptPrIdLbxIconImg7,			E_GrUiParaTypeSkin,				{L"Icon7"} },
{	E_GrUiScptPrIdLbxIconImg8,			E_GrUiParaTypeSkin,				{L"Icon8"} },
{	E_GrUiScptPrIdLbxIconImg9,			E_GrUiParaTypeSkin,				{L"Icon9"} },
{	E_GrUiScptPrIdLbxIconImg10,			E_GrUiParaTypeSkin,				{L"Icon10"} },
{	E_GrUiScptPrIdLbxIconImg11,			E_GrUiParaTypeSkin,				{L"Icon11"} },
{	E_GrUiScptPrIdLbxIconImg12,			E_GrUiParaTypeSkin,				{L"Icon12"} },
{	E_GrUiScptPrIdLbxIconImg13,			E_GrUiParaTypeSkin,				{L"Icon13"} },
{	E_GrUiScptPrIdLbxIconImg14,			E_GrUiParaTypeSkin,				{L"Icon14"} },
{	E_GrUiScptPrIdLbxIconImg15,			E_GrUiParaTypeSkin,				{L"Icon15"} },
{	E_GrUiScptPrIdLbxSbarId,				E_GrUiParaTypeWord,				{L"SbarId"} },
{	E_GrUiScptPrIdLbxTextPara,			E_GrUiParaTypeTxtPara,		{L"TextPara"} },
{	E_GrUiScptPrIdLbxKeyFcsCtrl,		E_GrUiParaTypeBool8,			{L"KeyFocusCtrl"} },

//sbar
{	E_GrUiScptPrIdSbarDirecton,			E_GrUiParaTypeBool8,			{L"IsHorizontal"} },
{	E_GrUiScptPrIdSbarBtnBrdAtb,		E_GrUiParaTypeBorderAtb,	{L"BtnBrdAtb"} },
{	E_GrUiScptPrIdSbarBtnBrdDepth,	E_GrUiParaTypeByte,				{L"BtnBrdDepth"} },
{	E_GrUiScptPrIdSbarBtnSize,			E_GrUiParaTypeByte,				{L"BtnSize"} },
{	E_GrUiScptPrIdSbarBtnImgDis,		E_GrUiParaTypeSkin,				{L"ImgBtnDisable"} },
{	E_GrUiScptPrIdSbarBtnImgNrm,		E_GrUiParaTypeSkin,				{L"ImgBtnNormal"} },
{	E_GrUiScptPrIdSbarBtnImgFcs,		E_GrUiParaTypeSkin,				{L"ImgBtnFocus"} },
{	E_GrUiScptPrIdSbarBtnImgCtrl,		E_GrUiParaTypeSkin,				{L"ImgBtnControl"} },
{	E_GrUiScptPrIdSbarBtnClrDis,		E_GrUiParaTypeColor,			{L"ClrBtnDisable"} },
{	E_GrUiScptPrIdSbarBtnClrNrm,		E_GrUiParaTypeColor,			{L"ClrBtnNormal"} },
{	E_GrUiScptPrIdSbarBtnClrFcs,		E_GrUiParaTypeColor,			{L"ClrBtnFocus"} },
{	E_GrUiScptPrIdSbarBtnClrCtrl,		E_GrUiParaTypeColor,			{L"ClrBtnControl"} },
{	E_GrUiScptPrIdSbarRngMin,				E_GrUiParaTypeInt,				{L"RangeMin"} },
{	E_GrUiScptPrIdSbarRngMax,				E_GrUiParaTypeInt,				{L"RangeMax"} },
{	E_GrUiScptPrIdSbarValPos,				E_GrUiParaTypeInt,				{L"ValuePos"} },
{	E_GrUiScptPrIdSbarPageUnit,			E_GrUiParaTypeInt,				{L"PageUnit"} },
{	E_GrUiScptPrIdSbarOnKeyCtrl,		E_GrUiParaTypeBool8,			{L"OnKeyCtrl"} },
{	E_GrUiScptPrIdSbarEvtHostId,		E_GrUiParaTypeWord,				{L"EvtHostId"} },

//spin base
{	E_GrUiScptPrIdSpbBaseTxtPara,		E_GrUiParaTypeTxtPara,		{L"TextPara"} },
{	E_GrUiScptPrIdSpbBaseClrDis,		E_GrUiParaTypeColor,			{L"ClrDisable"} },
{	E_GrUiScptPrIdSpbBaseClrNrm,		E_GrUiParaTypeColor,			{L"ClrNormal"} },
{	E_GrUiScptPrIdSpbBaseClrFcs,		E_GrUiParaTypeColor,			{L"ClrFocus"} },
{	E_GrUiScptPrIdSpbBaseClrCtrl,		E_GrUiParaTypeColor,			{L"ClrControl"} },
{	E_GrUiScptPrIdSpbBaseTclrDis,		E_GrUiParaTypeColor,			{L"ClrTxtDisable"} },
{	E_GrUiScptPrIdSpbBaseTclrNrm,		E_GrUiParaTypeColor,			{L"ClrTxtNormal"} },
{	E_GrUiScptPrIdSpbBaseTclrFcs,		E_GrUiParaTypeColor,			{L"ClrTxtFocus"} },
{	E_GrUiScptPrIdSpbBaseTclrCtrl,	E_GrUiParaTypeColor,			{L"ClrTxtControl"} },
{	E_GrUiScptPrIdSpbBaseSkinDis,		E_GrUiParaTypeSkin,				{L"SkinDisable"} },
{	E_GrUiScptPrIdSpbBaseSkinNrm,		E_GrUiParaTypeSkin,				{L"SkinNormal"} },
{	E_GrUiScptPrIdSpbBaseSkinFcs,		E_GrUiParaTypeSkin,				{L"SkinFocus"} },
{	E_GrUiScptPrIdSpbBaseSkinCtrl,	E_GrUiParaTypeSkin,				{L"SkinControl"} },
{	E_GrUiScptPrIdSpbBaseOnKeyCtrl,	E_GrUiParaTypeBool8,			{L"OnKeyCtrl"} },
{	E_GrUiScptPrIdSpbBaseEvtHostId,	E_GrUiParaTypeWord,				{L"EvtHostId"} },

//spin number
{	E_GrUiScptPrIdSpbNumRngMin,			E_GrUiParaTypeInt,				{L"RangeMin"} },
{	E_GrUiScptPrIdSpbNumRngMax,			E_GrUiParaTypeInt,				{L"RangeMax"} },
{	E_GrUiScptPrIdSpbNumValue,			E_GrUiParaTypeInt,				{L"Value"} },
{	E_GrUiScptPrIdSpbNumStep,				E_GrUiParaTypeInt,				{L"Step"} },
{	E_GrUiScptPrIdSpbNumTxtIdZero,	E_GrUiParaTypeTxtId,			{L"ZeroTxtId"} },
{	E_GrUiScptPrIdSpbNumTxtIdPre,		E_GrUiParaTypeTxtId,			{L"PrefixTxtId"} },
{	E_GrUiScptPrIdSpbNumTxtIdSuf,		E_GrUiParaTypeTxtId,			{L"SuffixTxtId"} },
{	E_GrUiScptPrIdSpbNumVertical,		E_GrUiParaTypeBool8,			{L"Vertical"} },
{	E_GrUiScptPrIdSpbNumCipher,			E_GrUiParaTypeByte,				{L"Cipher"} },
{	E_GrUiScptPrIdSpbNumCenterSz,		E_GrUiParaTypeDword,			{L"CenterSize"} },

//spin Text
{	E_GrUiScptPrIdSpbTxtTxtIdSt,		E_GrUiParaTypeTxtId,			{L"TextIdStart"} },
{	E_GrUiScptPrIdSpbTxtTxtIdCnt,		E_GrUiParaTypeByte,				{L"TextIdCount"} },
{	E_GrUiScptPrIdSpbTxtVertical,		E_GrUiParaTypeBool8,			{L"Vertical"} },
{	E_GrUiScptPrIdSpbTxtCenterSz,		E_GrUiParaTypeDword,			{L"CenterSize"} },

//spin digit
{	E_GrUiScptPrIdSpbDgtHexMode,		E_GrUiParaTypeBool8,			{L"HexMode"} },
{	E_GrUiScptPrIdSpbDgtNextInp,		E_GrUiParaTypeWord,				{L"NextObjId"} },
{	E_GrUiScptPrIdSpbDgtCipher,			E_GrUiParaTypeByte,				{L"Cipher"} },
{	E_GrUiScptPrIdSpbDgtAreaUpdn,		E_GrUiParaTypeByte,				{L"AreaUpDown"} },
{	E_GrUiScptPrIdSpbDgtAreaCipher,	E_GrUiParaTypeByte,				{L"AreaCipher"} },
{	E_GrUiScptPrIdSpbDgtValMax,			E_GrUiParaTypeDword,			{L"MaxValue"} },

//simple button
{	E_GrUiScptPrIdSmpbTxtPara,			E_GrUiParaTypeTxtPara,		{L"TextPara"} },
{	E_GrUiScptPrIdSmpbClrDis,				E_GrUiParaTypeColor,			{L"ClrDisable"} },
{	E_GrUiScptPrIdSmpbClrNrm,				E_GrUiParaTypeColor,			{L"ClrNormal"} },
{	E_GrUiScptPrIdSmpbClrFcs,				E_GrUiParaTypeColor,			{L"ClrFocus"} },
{	E_GrUiScptPrIdSmpbTclrDis,			E_GrUiParaTypeColor,			{L"ClrTxtDisable"} },
{	E_GrUiScptPrIdSmpbTclrNrm,			E_GrUiParaTypeColor,			{L"ClrTxtNormal"} },
{	E_GrUiScptPrIdSmpbTclrFcs,			E_GrUiParaTypeColor,			{L"ClrTxtFocus"} },
{	E_GrUiScptPrIdSmpbSkinDis,			E_GrUiParaTypeSkin,				{L"SkinDisable"} },
{	E_GrUiScptPrIdSmpbSkinNrm,			E_GrUiParaTypeSkin,				{L"SkinNormal"} },
{	E_GrUiScptPrIdSmpbSkinFcs,			E_GrUiParaTypeSkin,				{L"SkinFocus"} },

//edit
{	E_GrUiScptPrIdEditSkinEnrm,			E_GrUiParaTypeSkin,				{L"SkinEmpty"} },
{	E_GrUiScptPrIdEditSkinEfcs,			E_GrUiParaTypeSkin,				{L"SkinEfocus"} },
{	E_GrUiScptPrIdEditClrEnrm,			E_GrUiParaTypeColor,			{L"ClrEmpty"} },
{	E_GrUiScptPrIdEditClrEfcs,			E_GrUiParaTypeColor,			{L"ClrEfocus"} },
{	E_GrUiScptPrIdEditNumType,			E_GrUiParaTypeBool8,			{L"NumberType"} },
{	E_GrUiScptPrIdEditNoKeyVk,			E_GrUiParaTypeBool8,			{L"NoKeyVkbd"} },
{	E_GrUiScptPrIdEditMaxLen,				E_GrUiParaTypeDword,			{L"MaxLength"} },
{	E_GrUiScptPrIdEditEvtHostId,		E_GrUiParaTypeWord,				{L"EvtHostId"} },

//sys button
{	E_GrUiScptPrIdSysBtnTextId,			E_GrUiParaTypeTxtId,			{L"TextId"} },

//calendar
{	E_GrUiScptPrIdCalenTxtPara,			E_GrUiParaTypeTxtPara,		{L"TextPara"} },
{	E_GrUiScptPrIdCalenClrTxtNrm,		E_GrUiParaTypeTxtClr,			{L"ClrTxtNormal"} },
{	E_GrUiScptPrIdCalenClrTxtCus,		E_GrUiParaTypeTxtClr,			{L"ClrTxtCursor"} },
{	E_GrUiScptPrIdCalenClrMark0,		E_GrUiParaTypeColor,			{L"ClrMark0"} },
{	E_GrUiScptPrIdCalenClrMark1,		E_GrUiParaTypeColor,			{L"ClrMark1"} },
{	E_GrUiScptPrIdCalenClrMark2,		E_GrUiParaTypeColor,			{L"ClrMark2"} },
{	E_GrUiScptPrIdCalenClrMark3,		E_GrUiParaTypeColor,			{L"ClrMark3"} },
{	E_GrUiScptPrIdCalenClrMark4,		E_GrUiParaTypeColor,			{L"ClrMark4"} },
{	E_GrUiScptPrIdCalenClrMark5,		E_GrUiParaTypeColor,			{L"ClrMark5"} },
{	E_GrUiScptPrIdCalenClrMark6,		E_GrUiParaTypeColor,			{L"ClrMark6"} },
{	E_GrUiScptPrIdCalenClrMark7,		E_GrUiParaTypeColor,			{L"ClrMark7"} },
{	E_GrUiScptPrIdCalenClrSel,			E_GrUiParaTypeColor,			{L"ClrSelect"} },
{	E_GrUiScptPrIdCalenClrCus,			E_GrUiParaTypeColor,			{L"ClrCursor"} },
{	E_GrUiScptPrIdCalenItmWidth,		E_GrUiParaTypeByte,				{L"ItemWidth"} },
{	E_GrUiScptPrIdCalenItmHeight,		E_GrUiParaTypeByte,				{L"ItemHeight"} },
{	E_GrUiScptPrIdCalenItmGapX,			E_GrUiParaTypeByte,				{L"ItemGapX"} },
{	E_GrUiScptPrIdCalenItmGapY,			E_GrUiParaTypeByte,				{L"ItemGapY"} },
{	E_GrUiScptPrIdCalenSelGap,			E_GrUiParaTypeByte,				{L"SelBoxGap"} },
{	E_GrUiScptPrIdCalenSelDepth,		E_GrUiParaTypeByte,				{L"SelBoxDepth"} },
{	E_GrUiScptPrIdCalenCusGap,			E_GrUiParaTypeByte,				{L"CusBoxGap"} },
{	E_GrUiScptPrIdCalenCusDepth,		E_GrUiParaTypeByte,				{L"CusBoxDepth"} },
{	E_GrUiScptPrIdCalenSkinMark,		E_GrUiParaTypeSkin,				{L"SkinMark"} },
{	E_GrUiScptPrIdCalenSkinSel,			E_GrUiParaTypeSkin,				{L"SkinSelect"} },
{	E_GrUiScptPrIdCalenSkinCus,			E_GrUiParaTypeSkin,				{L"SkinCursor"} },
{	E_GrUiScptPrIdCalenClrDis,			E_GrUiParaTypeColor,			{L"ClrDisable"} },
{	E_GrUiScptPrIdCalenClrNrm,			E_GrUiParaTypeColor,			{L"ClrNormal"} },
{	E_GrUiScptPrIdCalenClrFcs,			E_GrUiParaTypeColor,			{L"ClrFocus"} },
//{	E_GrUiScptPrIdCalenClrCtrl,			E_GrUiParaTypeColor,			{L"ClrControl"} },
{	E_GrUiScptPrIdCalenSpinYear,		E_GrUiParaTypeWord,				{L"SpinYearId"} },
{	E_GrUiScptPrIdCalenSpinMonth,		E_GrUiParaTypeWord,				{L"SpinMonthId"} },
{	E_GrUiScptPrIdCalenEvtHostId,		E_GrUiParaTypeWord,				{L"EvtHostId"} },

//quad
{	E_GrUiScptPrIdQuadTxtPara,			E_GrUiParaTypeTxtPara,		{L"TextPara"} },
{	E_GrUiScptPrIdQuadClrTitle,			E_GrUiParaTypeColor,			{L"ClrTitle"} },
{	E_GrUiScptPrIdQuadClrGrid,			E_GrUiParaTypeColor,			{L"ClrGrid"} },
{	E_GrUiScptPrIdQuadGridWidth,		E_GrUiParaTypeByte,				{L"GridWidth"} },
{	E_GrUiScptPrIdQuadGridShow,			E_GrUiParaTypeBool8,			{L"ShowGrid"} },
{	E_GrUiScptPrIdQuadTitleShow,		E_GrUiParaTypeBool8,			{L"ShowTitle"} },
{	E_GrUiScptPrIdQuadIconShow,			E_GrUiParaTypeBool8,			{L"ShowIcon"} },
{	E_GrUiScptPrIdQuadBgImgShow,		E_GrUiParaTypeBool8,			{L"ShowBkImg"} },
{	E_GrUiScptPrIdQuadChMargin,			E_GrUiParaTypeByte,				{L"ChMargin"} },
{	E_GrUiScptPrIdQuadIconSizeX,		E_GrUiParaTypeByte,				{L"IconSizeX"} },
{	E_GrUiScptPrIdQuadIconSizeY,		E_GrUiParaTypeByte,				{L"IconSizeY"} },
{	E_GrUiScptPrIdQuadIconGapTop,		E_GrUiParaTypeByte,				{L"IconGapTop"} },
{	E_GrUiScptPrIdQuadIconGapBtw,		E_GrUiParaTypeByte,				{L"IconGapBtw"} },
{	E_GrUiScptPrIdQuadIconImgRec,		E_GrUiParaTypeSkin,				{L"IconRecord"} },
{	E_GrUiScptPrIdQuadIconImgMtn,		E_GrUiParaTypeSkin,				{L"IconMotion"} },
{	E_GrUiScptPrIdQuadIconImgDi,		E_GrUiParaTypeSkin,				{L"IconSensor"} },
{	E_GrUiScptPrIdQuadIconImgIps,		E_GrUiParaTypeSkin,				{L"IconImgPrs"} },
{	E_GrUiScptPrIdQuadIconImgNsg,		E_GrUiParaTypeSkin,				{L"IconNosig"} },
{	E_GrUiScptPrIdQuadIconImgCvt,		E_GrUiParaTypeSkin,				{L"IconCovert"} },
{	E_GrUiScptPrIdQuadIconBigNsg,		E_GrUiParaTypeSkin,				{L"BgImgNosig"} },
{	E_GrUiScptPrIdQuadIconBigCvt,		E_GrUiParaTypeSkin,				{L"BgImgCovert"} },
{	E_GrUiScptPrIdQuadPosMode,			E_GrUiParaTypeByte,				{L"ViewPosMode"} },
{	E_GrUiScptPrIdQuadExtTtPara,		E_GrUiParaTypeTxtPara,		{L"ExtTxtPara"} },
//{	E_GrUiScptPrIdQuadExtTtClr,			E_GrUiParaTypeColor,			{L"ExtTxtColor"} },

//grpsel
{	E_GrUiScptPrIdGrpSelColCnt,			E_GrUiParaTypeByte,				{L"ColumCnt"} },
{	E_GrUiScptPrIdGrpSelLineCnt,		E_GrUiParaTypeByte,				{L"LineCnt"} },
{	E_GrUiScptPrIdGrpSelCusClrDis,	E_GrUiParaTypeColor,			{L"ClrCusDisable"} },
{	E_GrUiScptPrIdGrpSelCusClrNrm,	E_GrUiParaTypeColor,			{L"ClrCusNormal"} },
{	E_GrUiScptPrIdGrpSelCusClrFcs,	E_GrUiParaTypeColor,			{L"ClrCusFocus"} },
{	E_GrUiScptPrIdGrpSelAtbClr0,		E_GrUiParaTypeColor,			{L"ClrAtb0"} },
{	E_GrUiScptPrIdGrpSelAtbClr1,		E_GrUiParaTypeColor,			{L"ClrAtb1"} },
{	E_GrUiScptPrIdGrpSelAtbClr2,		E_GrUiParaTypeColor,			{L"ClrAtb2"} },
{	E_GrUiScptPrIdGrpSelAtbClr3,		E_GrUiParaTypeColor,			{L"ClrAtb3"} },
{	E_GrUiScptPrIdGrpSelAtbClr4,		E_GrUiParaTypeColor,			{L"ClrAtb4"} },
{	E_GrUiScptPrIdGrpSelAtbClr5,		E_GrUiParaTypeColor,			{L"ClrAtb5"} },
{	E_GrUiScptPrIdGrpSelAtbClr6,		E_GrUiParaTypeColor,			{L"ClrAtb6"} },
{	E_GrUiScptPrIdGrpSelAtbClr7,		E_GrUiParaTypeColor,			{L"ClrAtb7"} },
{	E_GrUiScptPrIdGrpSelAtbClr8,		E_GrUiParaTypeColor,			{L"ClrAtb8"} },
{	E_GrUiScptPrIdGrpSelBitPos0,		E_GrUiParaTypeByte,				{L"BitPos0"} },
{	E_GrUiScptPrIdGrpSelBitPos1,		E_GrUiParaTypeByte,				{L"BitPos1"} },
{	E_GrUiScptPrIdGrpSelBitPos2,		E_GrUiParaTypeByte,				{L"BitPos2"} },
{	E_GrUiScptPrIdGrpSelBitPos3,		E_GrUiParaTypeByte,				{L"BitPos3"} },
{	E_GrUiScptPrIdGrpSelBitPos4,		E_GrUiParaTypeByte,				{L"BitPos4"} },
{	E_GrUiScptPrIdGrpSelBitPos5,		E_GrUiParaTypeByte,				{L"BitPos5"} },
{	E_GrUiScptPrIdGrpSelBitPos6,		E_GrUiParaTypeByte,				{L"BitPos6"} },
{	E_GrUiScptPrIdGrpSelBitPos7,		E_GrUiParaTypeByte,				{L"BitPos7"} },
{	E_GrUiScptPrIdGrpSelCusColWdt,	E_GrUiParaTypeDword,			{L"CusColWidth"} },
{	E_GrUiScptPrIdGrpSelCusLineWdt,	E_GrUiParaTypeDword,			{L"CusLineWidth"} },
{	E_GrUiScptPrIdGrpSelColItmSize,	E_GrUiParaTypeDword,			{L"ColItemSize"} },
{	E_GrUiScptPrIdGrpSelLineItmSize,	E_GrUiParaTypeDword,		{L"LineItemSize"} },
{	E_GrUiScptPrIdGrpSelLineGap,		E_GrUiParaTypeDword,			{L"LineGap"} },
{	E_GrUiScptPrIdGrpSelBitGrpCnt,	E_GrUiParaTypeByte,				{L"BitGroupCnt"} },
{	E_GrUiScptPrIdGrpSelBitGrpGap,	E_GrUiParaTypeDword,			{L"BitGroupGap"} },
{	E_GrUiScptPrIdGrpSelColSelClr,	E_GrUiParaTypeColor,			{L"ColumSelColor"} },
{	E_GrUiScptPrIdGrpSelRltColLbl,	E_GrUiParaTypeWord,				{L"ColumLabl"} },
{	E_GrUiScptPrIdGrpSelRltLineLbl,	E_GrUiParaTypeWord,				{L"LineLabl"} },

//grpedit
{	E_GrUiScptPrIdGrpEdtColCnt,			E_GrUiParaTypeByte,				{L"ColumCnt"} },
{	E_GrUiScptPrIdGrpEdtLineCnt,		E_GrUiParaTypeByte,				{L"LineCnt"} },
{	E_GrUiScptPrIdGrpEdtValCnt,			E_GrUiParaTypeByte,				{L"ValueCnt"} },
{	E_GrUiScptPrIdGrpEdtBitMode,		E_GrUiParaTypeBool8,			{L"BitMode"} },
{	E_GrUiScptPrIdGrpEdtBitPos0,		E_GrUiParaTypeByte,				{L"BitPos0"} },
{	E_GrUiScptPrIdGrpEdtBitPos1,		E_GrUiParaTypeByte,				{L"BitPos1"} },
{	E_GrUiScptPrIdGrpEdtBitPos2,		E_GrUiParaTypeByte,				{L"BitPos2"} },
{	E_GrUiScptPrIdGrpEdtBitPos3,		E_GrUiParaTypeByte,				{L"BitPos3"} },
{	E_GrUiScptPrIdGrpEdtBitPos4,		E_GrUiParaTypeByte,				{L"BitPos4"} },
{	E_GrUiScptPrIdGrpEdtBitPos5,		E_GrUiParaTypeByte,				{L"BitPos5"} },
{	E_GrUiScptPrIdGrpEdtBitPos6,		E_GrUiParaTypeByte,				{L"BitPos6"} },
{	E_GrUiScptPrIdGrpEdtBitPos7,		E_GrUiParaTypeByte,				{L"BitPos7"} },
{	E_GrUiScptPrIdGrpEdtClrAtb0,		E_GrUiParaTypeColor,			{L"ClrAtb0"} },
{	E_GrUiScptPrIdGrpEdtClrAtb1,		E_GrUiParaTypeColor,			{L"ClrAtb1"} },
{	E_GrUiScptPrIdGrpEdtClrAtb2,		E_GrUiParaTypeColor,			{L"ClrAtb2"} },
{	E_GrUiScptPrIdGrpEdtClrAtb3,		E_GrUiParaTypeColor,			{L"ClrAtb3"} },
{	E_GrUiScptPrIdGrpEdtClrAtb4,		E_GrUiParaTypeColor,			{L"ClrAtb4"} },
{	E_GrUiScptPrIdGrpEdtClrAtb5,		E_GrUiParaTypeColor,			{L"ClrAtb5"} },
{	E_GrUiScptPrIdGrpEdtClrAtb6,		E_GrUiParaTypeColor,			{L"ClrAtb6"} },
{	E_GrUiScptPrIdGrpEdtClrAtb7,		E_GrUiParaTypeColor,			{L"ClrAtb7"} },
{	E_GrUiScptPrIdGrpEdtClrAtb8,		E_GrUiParaTypeColor,			{L"ClrAtb8"} },
{	E_GrUiScptPrIdGrpEdtClrGrid,		E_GrUiParaTypeColor,			{L"ClrGrid"} },
{	E_GrUiScptPrIdGrpEdtGridWidth,	E_GrUiParaTypeByte,				{L"GridWidth"} },
{	E_GrUiScptPrIdGrpEdtClrCus,			E_GrUiParaTypeColor,			{L"ClrCursor"} },
{	E_GrUiScptPrIdGrpEdtCusWidth,		E_GrUiParaTypeByte,				{L"CusWidth"} },
{	E_GrUiScptPrIdGrpEdtColItmSize,	E_GrUiParaTypeDword,			{L"ColItmSize"} },
{	E_GrUiScptPrIdGrpEdtLineItmSize,	E_GrUiParaTypeDword,		{L"LineItmSize"} },
{	E_GrUiScptPrIdGrpEdtLineGap,		E_GrUiParaTypeDword,			{L"LineGap"} },
{	E_GrUiScptPrIdGrpEdtRltLblCol,	E_GrUiParaTypeWord,				{L"ColumLbl"} },
{	E_GrUiScptPrIdGrpEdtRltLblLine,	E_GrUiParaTypeWord,				{L"LineLbl"} },

//bit grid
{	E_GrUiScptPrIdBitGridCelSizeX,	E_GrUiParaTypeByte,				{L"CellSizeX"} },
{	E_GrUiScptPrIdBitGridCelSizeY,	E_GrUiParaTypeByte,				{L"CellSizeY"} },
{	E_GrUiScptPrIdBitGridGridCntX,	E_GrUiParaTypeByte,				{L"GridCntX"} },
{	E_GrUiScptPrIdBitGridGridCntY,	E_GrUiParaTypeByte,				{L"GridCntY"} },
{	E_GrUiScptPrIdBitGridGridWidth,	E_GrUiParaTypeByte,				{L"GridWidth"} },
{	E_GrUiScptPrIdBitGridGridColor,	E_GrUiParaTypeColor,			{L"GridColor"} },
{	E_GrUiScptPrIdBitGridMarkWidth,	E_GrUiParaTypeByte,				{L"MarkWidth"} },
{	E_GrUiScptPrIdBitGridMarkColor,	E_GrUiParaTypeColor,			{L"MarkColor"} },
{	E_GrUiScptPrIdBitGridCusWidth,	E_GrUiParaTypeByte,				{L"CusWidth"} },
{	E_GrUiScptPrIdBitGridCusColor,	E_GrUiParaTypeColor,			{L"CusColor"} },
{	E_GrUiScptPrIdBitGridDispColor,	E_GrUiParaTypeColor,			{L"DispColor"} },

//spin image
{	E_GrUiScptPrIdSpnImgImage0,			E_GrUiParaTypeSkin,				{L"Image0"} },
{	E_GrUiScptPrIdSpnImgImage1,			E_GrUiParaTypeSkin,				{L"Image1"} },
{	E_GrUiScptPrIdSpnImgImage2,			E_GrUiParaTypeSkin,				{L"Image2"} },
{	E_GrUiScptPrIdSpnImgImage3,			E_GrUiParaTypeSkin,				{L"Image3"} },
{	E_GrUiScptPrIdSpnImgImage4,			E_GrUiParaTypeSkin,				{L"Image4"} },
{	E_GrUiScptPrIdSpnImgImage5,			E_GrUiParaTypeSkin,				{L"Image5"} },
{	E_GrUiScptPrIdSpnImgImage6,			E_GrUiParaTypeSkin,				{L"Image6"} },
{	E_GrUiScptPrIdSpnImgImage7,			E_GrUiParaTypeSkin,				{L"Image7"} },
{	E_GrUiScptPrIdSpnImgImage8,			E_GrUiParaTypeSkin,				{L"Image8"} },
{	E_GrUiScptPrIdSpnImgImage9,			E_GrUiParaTypeSkin,				{L"Image9"} },
{	E_GrUiScptPrIdSpnImgImage10,		E_GrUiParaTypeSkin,				{L"Image10"} },
{	E_GrUiScptPrIdSpnImgImage11,		E_GrUiParaTypeSkin,				{L"Image11"} },
{	E_GrUiScptPrIdSpnImgImage12,		E_GrUiParaTypeSkin,				{L"Image12"} },
{	E_GrUiScptPrIdSpnImgImage13,		E_GrUiParaTypeSkin,				{L"Image13"} },
{	E_GrUiScptPrIdSpnImgImage14,		E_GrUiParaTypeSkin,				{L"Image14"} },
{	E_GrUiScptPrIdSpnImgImage15,		E_GrUiParaTypeSkin,				{L"Image15"} },
{	E_GrUiScptPrIdSpnImgImageCnt,		E_GrUiParaTypeByte,				{L"ImageCnt"} },
{	E_GrUiScptPrIdSpnImgTxtStId,		E_GrUiParaTypeTxtId,			{L"TextStId"} },
{	E_GrUiScptPrIdSpnImgVertical,		E_GrUiParaTypeBool8,			{L"Vertical"} },

//prograss bar
{	E_GrUiScptPrIdPrgbarBarColor,		E_GrUiParaTypeColor,			{L"BarColor"} },
{	E_GrUiScptPrIdPrgbarBarImage,		E_GrUiParaTypeSkin,				{L"BarImage"} },
{	E_GrUiScptPrIdPrgbarValMax,			E_GrUiParaTypeDword,			{L"ValueMax"} },
{	E_GrUiScptPrIdPrgbarVertical,		E_GrUiParaTypeBool8,			{L"Vertical"} },
{	E_GrUiScptPrIdPrgbarRelCntType,	E_GrUiParaTypeBool8,			{L"RelCntType"} },
{	E_GrUiScptPrIdPrgbarRelLblId,		E_GrUiParaTypeWord,				{L"RelLblId"} },

//input time
{	E_GrUiScptPrIdInpTimeYearId,		E_GrUiParaTypeWord,				{L"YearId"} },
{	E_GrUiScptPrIdInpTimeUnYearId,	E_GrUiParaTypeWord,				{L"UnitYearId"} },
{	E_GrUiScptPrIdInpTimeMonId,			E_GrUiParaTypeWord,				{L"MonthId"} },
{	E_GrUiScptPrIdInpTimeUnMonId,		E_GrUiParaTypeWord,				{L"UnitMonthId"} },
{	E_GrUiScptPrIdInpTimeDayId,			E_GrUiParaTypeWord,				{L"DayId"} },
{	E_GrUiScptPrIdInpTimeUnDayId,		E_GrUiParaTypeWord,				{L"UnitDayId"} },
{	E_GrUiScptPrIdInpTimeHourId,		E_GrUiParaTypeWord,				{L"HourId"} },
{	E_GrUiScptPrIdInpTimeMinId,			E_GrUiParaTypeWord,				{L"MinId"} },
{	E_GrUiScptPrIdInpTimeSecId,			E_GrUiParaTypeWord,				{L"SecId"} },
{	E_GrUiScptPrIdInpTimeDiv1Id,		E_GrUiParaTypeWord,				{L"Div1Id"} },
{	E_GrUiScptPrIdInpTimeDiv2Id,		E_GrUiParaTypeWord,				{L"Div2Id"} },

//area

//cdv area
{	E_GrUiScptPrIdCdvAreaSrcIdx0,		E_GrUiParaTypeGval,				{L"SrcIdx0"} },
{	E_GrUiScptPrIdCdvAreaConType0,	E_GrUiParaTypeCond,				{L"Cond0"} },
{	E_GrUiScptPrIdCdvAreaRefVal0,		E_GrUiParaTypeDword,			{L"Ref0"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx1,		E_GrUiParaTypeGval,				{L"SrcIdx1"} },
{	E_GrUiScptPrIdCdvAreaConType1,	E_GrUiParaTypeCond,				{L"Cond1"} },
{	E_GrUiScptPrIdCdvAreaRefVal1,		E_GrUiParaTypeDword,			{L"Ref1"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx2,		E_GrUiParaTypeGval,				{L"SrcIdx2"} },
{	E_GrUiScptPrIdCdvAreaConType2,	E_GrUiParaTypeCond,				{L"Cond2"} },
{	E_GrUiScptPrIdCdvAreaRefVal2,		E_GrUiParaTypeDword,			{L"Ref2"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx3,		E_GrUiParaTypeGval,				{L"SrcIdx3"} },
{	E_GrUiScptPrIdCdvAreaConType3,	E_GrUiParaTypeCond,				{L"Cond3"} },
{	E_GrUiScptPrIdCdvAreaRefVal3,		E_GrUiParaTypeDword,			{L"Ref3"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx4,		E_GrUiParaTypeGval,				{L"SrcIdx4"} },
{	E_GrUiScptPrIdCdvAreaConType4,	E_GrUiParaTypeCond,				{L"Cond4"} },
{	E_GrUiScptPrIdCdvAreaRefVal4,		E_GrUiParaTypeDword,			{L"Ref4"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx5,		E_GrUiParaTypeGval,				{L"SrcIdx5"} },
{	E_GrUiScptPrIdCdvAreaConType5,	E_GrUiParaTypeCond,				{L"Cond5"} },
{	E_GrUiScptPrIdCdvAreaRefVal5,		E_GrUiParaTypeDword,			{L"Ref5"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx6,		E_GrUiParaTypeGval,				{L"SrcIdx6"} },
{	E_GrUiScptPrIdCdvAreaConType6,	E_GrUiParaTypeCond,				{L"Cond6"} },
{	E_GrUiScptPrIdCdvAreaRefVal6,		E_GrUiParaTypeDword,			{L"Ref6"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx7,		E_GrUiParaTypeGval,				{L"SrcIdx7"} },
{	E_GrUiScptPrIdCdvAreaConType7,	E_GrUiParaTypeCond,				{L"Cond7"} },
{	E_GrUiScptPrIdCdvAreaRefVal7,		E_GrUiParaTypeDword,			{L"Ref7"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx8,		E_GrUiParaTypeGval,				{L"SrcIdx8"} },
{	E_GrUiScptPrIdCdvAreaConType8,	E_GrUiParaTypeCond,				{L"Cond8"} },
{	E_GrUiScptPrIdCdvAreaRefVal8,		E_GrUiParaTypeDword,			{L"Ref8"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx9,		E_GrUiParaTypeGval,				{L"SrcIdx9"} },
{	E_GrUiScptPrIdCdvAreaConType9,	E_GrUiParaTypeCond,				{L"Cond9"} },
{	E_GrUiScptPrIdCdvAreaRefVal9,		E_GrUiParaTypeDword,			{L"Ref9"} },

{	E_GrUiScptPrIdCdvAreaSrcIdx10,	E_GrUiParaTypeGval,				{L"SrcIdx10"} },
{	E_GrUiScptPrIdCdvAreaConType10,	E_GrUiParaTypeCond,				{L"Cond10"} },
{	E_GrUiScptPrIdCdvAreaRefVal10,	E_GrUiParaTypeDword,			{L"Ref10"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx11,	E_GrUiParaTypeGval,				{L"SrcIdx11"} },
{	E_GrUiScptPrIdCdvAreaConType11,	E_GrUiParaTypeCond,				{L"Cond11"} },
{	E_GrUiScptPrIdCdvAreaRefVal11,	E_GrUiParaTypeDword,			{L"Ref111"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx12,	E_GrUiParaTypeGval,				{L"SrcIdx12"} },
{	E_GrUiScptPrIdCdvAreaConType12,	E_GrUiParaTypeCond,				{L"Cond12"} },
{	E_GrUiScptPrIdCdvAreaRefVal12,	E_GrUiParaTypeDword,			{L"Ref12"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx13,	E_GrUiParaTypeGval,				{L"SrcIdx13"} },
{	E_GrUiScptPrIdCdvAreaConType13,	E_GrUiParaTypeCond,				{L"Cond13"} },
{	E_GrUiScptPrIdCdvAreaRefVal13,	E_GrUiParaTypeDword,			{L"Ref13"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx14,	E_GrUiParaTypeGval,				{L"SrcIdx14"} },
{	E_GrUiScptPrIdCdvAreaConType14,	E_GrUiParaTypeCond,				{L"Cond14"} },
{	E_GrUiScptPrIdCdvAreaRefVal14,	E_GrUiParaTypeDword,			{L"Ref14"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx15,	E_GrUiParaTypeGval,				{L"SrcIdx15"} },
{	E_GrUiScptPrIdCdvAreaConType15,	E_GrUiParaTypeCond,				{L"Cond15"} },
{	E_GrUiScptPrIdCdvAreaRefVal15,	E_GrUiParaTypeDword,			{L"Ref15"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx16,	E_GrUiParaTypeGval,				{L"SrcIdx16"} },
{	E_GrUiScptPrIdCdvAreaConType16,	E_GrUiParaTypeCond,				{L"Cond16"} },
{	E_GrUiScptPrIdCdvAreaRefVal16,	E_GrUiParaTypeDword,			{L"Ref16"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx17,	E_GrUiParaTypeGval,				{L"SrcIdx17"} },
{	E_GrUiScptPrIdCdvAreaConType17,	E_GrUiParaTypeCond,				{L"Cond17"} },
{	E_GrUiScptPrIdCdvAreaRefVal17,	E_GrUiParaTypeDword,			{L"Ref17"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx18,	E_GrUiParaTypeGval,				{L"SrcIdx18"} },
{	E_GrUiScptPrIdCdvAreaConType18,	E_GrUiParaTypeCond,				{L"Cond18"} },
{	E_GrUiScptPrIdCdvAreaRefVal18,	E_GrUiParaTypeDword,			{L"Ref18"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx19,	E_GrUiParaTypeGval,				{L"SrcIdx19"} },
{	E_GrUiScptPrIdCdvAreaConType19,	E_GrUiParaTypeCond,				{L"Cond19"} },
{	E_GrUiScptPrIdCdvAreaRefVal19,	E_GrUiParaTypeDword,			{L"Ref19"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx20,	E_GrUiParaTypeGval,				{L"SrcIdx20"} },
{	E_GrUiScptPrIdCdvAreaConType20,	E_GrUiParaTypeCond,				{L"Cond20"} },
{	E_GrUiScptPrIdCdvAreaRefVal20,	E_GrUiParaTypeDword,			{L"Ref20"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx21,	E_GrUiParaTypeGval,				{L"SrcIdx21"} },
{	E_GrUiScptPrIdCdvAreaConType21,	E_GrUiParaTypeCond,				{L"Cond21"} },
{	E_GrUiScptPrIdCdvAreaRefVal21,	E_GrUiParaTypeDword,			{L"Ref21"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx22,	E_GrUiParaTypeGval,				{L"SrcIdx22"} },
{	E_GrUiScptPrIdCdvAreaConType22,	E_GrUiParaTypeCond,				{L"Cond22"} },
{	E_GrUiScptPrIdCdvAreaRefVal22,	E_GrUiParaTypeDword,			{L"Ref22"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx23,	E_GrUiParaTypeGval,				{L"SrcIdx23"} },
{	E_GrUiScptPrIdCdvAreaConType23,	E_GrUiParaTypeCond,				{L"Cond23"} },
{	E_GrUiScptPrIdCdvAreaRefVal23,	E_GrUiParaTypeDword,			{L"Ref23"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx24,	E_GrUiParaTypeGval,				{L"SrcIdx24"} },
{	E_GrUiScptPrIdCdvAreaConType24,	E_GrUiParaTypeCond,				{L"Cond24"} },
{	E_GrUiScptPrIdCdvAreaRefVal24,	E_GrUiParaTypeDword,			{L"Ref24"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx25,	E_GrUiParaTypeGval,				{L"SrcIdx25"} },
{	E_GrUiScptPrIdCdvAreaConType25,	E_GrUiParaTypeCond,				{L"Cond25"} },
{	E_GrUiScptPrIdCdvAreaRefVal25,	E_GrUiParaTypeDword,			{L"Ref25"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx26,	E_GrUiParaTypeGval,				{L"SrcIdx26"} },
{	E_GrUiScptPrIdCdvAreaConType26,	E_GrUiParaTypeCond,				{L"Cond26"} },
{	E_GrUiScptPrIdCdvAreaRefVal26,	E_GrUiParaTypeDword,			{L"Ref26"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx27,	E_GrUiParaTypeGval,				{L"SrcIdx27"} },
{	E_GrUiScptPrIdCdvAreaConType27,	E_GrUiParaTypeCond,				{L"Cond27"} },
{	E_GrUiScptPrIdCdvAreaRefVal27,	E_GrUiParaTypeDword,			{L"Ref27"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx28,	E_GrUiParaTypeGval,				{L"SrcIdx28"} },
{	E_GrUiScptPrIdCdvAreaConType28,	E_GrUiParaTypeCond,				{L"Cond28"} },
{	E_GrUiScptPrIdCdvAreaRefVal28,	E_GrUiParaTypeDword,			{L"Ref28"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx29,	E_GrUiParaTypeGval,				{L"SrcIdx29"} },
{	E_GrUiScptPrIdCdvAreaConType29,	E_GrUiParaTypeCond,				{L"Cond29"} },
{	E_GrUiScptPrIdCdvAreaRefVal29,	E_GrUiParaTypeDword,			{L"Ref29"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx30,	E_GrUiParaTypeGval,				{L"SrcIdx30"} },
{	E_GrUiScptPrIdCdvAreaConType30,	E_GrUiParaTypeCond,				{L"Cond30"} },
{	E_GrUiScptPrIdCdvAreaRefVal30,	E_GrUiParaTypeDword,			{L"Ref30"} },
{	E_GrUiScptPrIdCdvAreaSrcIdx31,	E_GrUiParaTypeGval,				{L"SrcIdx31"} },
{	E_GrUiScptPrIdCdvAreaConType31,	E_GrUiParaTypeCond,				{L"Cond31"} },
{	E_GrUiScptPrIdCdvAreaRefVal31,	E_GrUiParaTypeDword,			{L"Ref31"} },

//action button
{	E_GrUiScptPrIdAcBtnAcRvs,				E_GrUiParaTypeBool8,			{L"Reverse"} },
{	E_GrUiScptPrIdAcBtnAcObj0,			E_GrUiParaTypeWord,				{L"ActObj0"} },
{	E_GrUiScptPrIdAcBtnAcObj1,			E_GrUiParaTypeWord,				{L"ActObj1"} },
{	E_GrUiScptPrIdAcBtnAcObj2,			E_GrUiParaTypeWord,				{L"ActObj2"} },
{	E_GrUiScptPrIdAcBtnAcObj3,			E_GrUiParaTypeWord,				{L"ActObj3"} },
{	E_GrUiScptPrIdAcBtnAcObj4,			E_GrUiParaTypeWord,				{L"ActObj4"} },
{	E_GrUiScptPrIdAcBtnAcObj5,			E_GrUiParaTypeWord,				{L"ActObj5"} },
{	E_GrUiScptPrIdAcBtnAcObj6,			E_GrUiParaTypeWord,				{L"ActObj6"} },
{	E_GrUiScptPrIdAcBtnAcObj7,			E_GrUiParaTypeWord,				{L"ActObj7"} },
{	E_GrUiScptPrIdAcBtnAcObj8,			E_GrUiParaTypeWord,				{L"ActObj8"} },
{	E_GrUiScptPrIdAcBtnAcObj9,			E_GrUiParaTypeWord,				{L"ActObj9"} },
{	E_GrUiScptPrIdAcBtnAcObj10,			E_GrUiParaTypeWord,				{L"ActObj10"} },
{	E_GrUiScptPrIdAcBtnAcObj11,			E_GrUiParaTypeWord,				{L"ActObj11"} },
{	E_GrUiScptPrIdAcBtnAcObj12,			E_GrUiParaTypeWord,				{L"ActObj12"} },
{	E_GrUiScptPrIdAcBtnAcObj13,			E_GrUiParaTypeWord,				{L"ActObj13"} },
{	E_GrUiScptPrIdAcBtnAcObj14,			E_GrUiParaTypeWord,				{L"ActObj14"} },
{	E_GrUiScptPrIdAcBtnAcObj15,			E_GrUiParaTypeWord,				{L"ActObj15"} },

//action spin number
{	E_GrUiScptPrIdAcSpBtnAcObj0,		E_GrUiParaTypeWord,				{L"ActObj0"} },
{	E_GrUiScptPrIdAcSpBtnAcObj1,		E_GrUiParaTypeWord,				{L"ActObj1"} },
{	E_GrUiScptPrIdAcSpBtnAcObj2,		E_GrUiParaTypeWord,				{L"ActObj2"} },
{	E_GrUiScptPrIdAcSpBtnAcObj3,		E_GrUiParaTypeWord,				{L"ActObj3"} },
{	E_GrUiScptPrIdAcSpBtnAcObj4,		E_GrUiParaTypeWord,				{L"ActObj4"} },
{	E_GrUiScptPrIdAcSpBtnAcObj5,		E_GrUiParaTypeWord,				{L"ActObj5"} },
{	E_GrUiScptPrIdAcSpBtnAcObj6,		E_GrUiParaTypeWord,				{L"ActObj6"} },
{	E_GrUiScptPrIdAcSpBtnAcObj7,		E_GrUiParaTypeWord,				{L"ActObj7"} },
{	E_GrUiScptPrIdAcSpBtnAcObj8,		E_GrUiParaTypeWord,				{L"ActObj8"} },
{	E_GrUiScptPrIdAcSpBtnAcObj9,		E_GrUiParaTypeWord,				{L"ActObj9"} },
{	E_GrUiScptPrIdAcSpBtnAcObj10,		E_GrUiParaTypeWord,				{L"ActObj10"} },
{	E_GrUiScptPrIdAcSpBtnAcObj11,		E_GrUiParaTypeWord,				{L"ActObj11"} },
{	E_GrUiScptPrIdAcSpBtnAcObj12,		E_GrUiParaTypeWord,				{L"ActObj12"} },
{	E_GrUiScptPrIdAcSpBtnAcObj13,		E_GrUiParaTypeWord,				{L"ActObj13"} },
{	E_GrUiScptPrIdAcSpBtnAcObj14,		E_GrUiParaTypeWord,				{L"ActObj14"} },
{	E_GrUiScptPrIdAcSpBtnAcObj15,		E_GrUiParaTypeWord,				{L"ActObj15"} },

//action spin text
{	E_GrUiScptPrIdAcSpTxtAcObj0,		E_GrUiParaTypeWord,				{L"ActObj0"} },
{	E_GrUiScptPrIdAcSpTxtAcObj1,		E_GrUiParaTypeWord,				{L"ActObj1"} },
{	E_GrUiScptPrIdAcSpTxtAcObj2,		E_GrUiParaTypeWord,				{L"ActObj2"} },
{	E_GrUiScptPrIdAcSpTxtAcObj3,		E_GrUiParaTypeWord,				{L"ActObj3"} },
{	E_GrUiScptPrIdAcSpTxtAcObj4,		E_GrUiParaTypeWord,				{L"ActObj4"} },
{	E_GrUiScptPrIdAcSpTxtAcObj5,		E_GrUiParaTypeWord,				{L"ActObj5"} },
{	E_GrUiScptPrIdAcSpTxtAcObj6,		E_GrUiParaTypeWord,				{L"ActObj6"} },
{	E_GrUiScptPrIdAcSpTxtAcObj7,		E_GrUiParaTypeWord,				{L"ActObj7"} },
{	E_GrUiScptPrIdAcSpTxtAcObj8,		E_GrUiParaTypeWord,				{L"ActObj8"} },
{	E_GrUiScptPrIdAcSpTxtAcObj9,		E_GrUiParaTypeWord,				{L"ActObj9"} },
{	E_GrUiScptPrIdAcSpTxtAcObj10,		E_GrUiParaTypeWord,				{L"ActObj10"} },
{	E_GrUiScptPrIdAcSpTxtAcObj11,		E_GrUiParaTypeWord,				{L"ActObj11"} },
{	E_GrUiScptPrIdAcSpTxtAcObj12,		E_GrUiParaTypeWord,				{L"ActObj12"} },
{	E_GrUiScptPrIdAcSpTxtAcObj13,		E_GrUiParaTypeWord,				{L"ActObj13"} },
{	E_GrUiScptPrIdAcSpTxtAcObj14,		E_GrUiParaTypeWord,				{L"ActObj14"} },
{	E_GrUiScptPrIdAcSpTxtAcObj15,		E_GrUiParaTypeWord,				{L"ActObj15"} },

//action spin image
{	E_GrUiScptPrIdAcSpImgAcObj0,		E_GrUiParaTypeWord,				{L"ActObj0"} },
{	E_GrUiScptPrIdAcSpImgAcObj1,		E_GrUiParaTypeWord,				{L"ActObj1"} },
{	E_GrUiScptPrIdAcSpImgAcObj2,		E_GrUiParaTypeWord,				{L"ActObj2"} },
{	E_GrUiScptPrIdAcSpImgAcObj3,		E_GrUiParaTypeWord,				{L"ActObj3"} },
{	E_GrUiScptPrIdAcSpImgAcObj4,		E_GrUiParaTypeWord,				{L"ActObj4"} },
{	E_GrUiScptPrIdAcSpImgAcObj5,		E_GrUiParaTypeWord,				{L"ActObj5"} },
{	E_GrUiScptPrIdAcSpImgAcObj6,		E_GrUiParaTypeWord,				{L"ActObj6"} },
{	E_GrUiScptPrIdAcSpImgAcObj7,		E_GrUiParaTypeWord,				{L"ActObj7"} },
{	E_GrUiScptPrIdAcSpImgAcObj8,		E_GrUiParaTypeWord,				{L"ActObj8"} },
{	E_GrUiScptPrIdAcSpImgAcObj9,		E_GrUiParaTypeWord,				{L"ActObj9"} },
{	E_GrUiScptPrIdAcSpImgAcObj10,		E_GrUiParaTypeWord,				{L"ActObj10"} },
{	E_GrUiScptPrIdAcSpImgAcObj11,		E_GrUiParaTypeWord,				{L"ActObj11"} },
{	E_GrUiScptPrIdAcSpImgAcObj12,		E_GrUiParaTypeWord,				{L"ActObj12"} },
{	E_GrUiScptPrIdAcSpImgAcObj13,		E_GrUiParaTypeWord,				{L"ActObj13"} },
{	E_GrUiScptPrIdAcSpImgAcObj14,		E_GrUiParaTypeWord,				{L"ActObj14"} },
{	E_GrUiScptPrIdAcSpImgAcObj15,		E_GrUiParaTypeWord,				{L"ActObj15"} },

//action spin digit
{	E_GrUiScptPrIdAcSpDgtAcObj0,		E_GrUiParaTypeWord,				{L"ActObj0"} },
{	E_GrUiScptPrIdAcSpDgtAcObj1,		E_GrUiParaTypeWord,				{L"ActObj1"} },
{	E_GrUiScptPrIdAcSpDgtAcObj2,		E_GrUiParaTypeWord,				{L"ActObj2"} },
{	E_GrUiScptPrIdAcSpDgtAcObj3,		E_GrUiParaTypeWord,				{L"ActObj3"} },
{	E_GrUiScptPrIdAcSpDgtAcObj4,		E_GrUiParaTypeWord,				{L"ActObj4"} },
{	E_GrUiScptPrIdAcSpDgtAcObj5,		E_GrUiParaTypeWord,				{L"ActObj5"} },
{	E_GrUiScptPrIdAcSpDgtAcObj6,		E_GrUiParaTypeWord,				{L"ActObj6"} },
{	E_GrUiScptPrIdAcSpDgtAcObj7,		E_GrUiParaTypeWord,				{L"ActObj7"} },
{	E_GrUiScptPrIdAcSpDgtAcObj8,		E_GrUiParaTypeWord,				{L"ActObj8"} },
{	E_GrUiScptPrIdAcSpDgtAcObj9,		E_GrUiParaTypeWord,				{L"ActObj9"} },
{	E_GrUiScptPrIdAcSpDgtAcObj10,		E_GrUiParaTypeWord,				{L"ActObj10"} },
{	E_GrUiScptPrIdAcSpDgtAcObj11,		E_GrUiParaTypeWord,				{L"ActObj11"} },
{	E_GrUiScptPrIdAcSpDgtAcObj12,		E_GrUiParaTypeWord,				{L"ActObj12"} },
{	E_GrUiScptPrIdAcSpDgtAcObj13,		E_GrUiParaTypeWord,				{L"ActObj13"} },
{	E_GrUiScptPrIdAcSpDgtAcObj14,		E_GrUiParaTypeWord,				{L"ActObj14"} },
{	E_GrUiScptPrIdAcSpDgtAcObj15,		E_GrUiParaTypeWord,				{L"ActObj15"} },

};

St_GrUiDvlSysIdDesc	V_GrUiDvlSysWinIdTbl[]	=	
{
/*	userId											name						  */
	{	E_GrUiObjIdWinVkbdText,			{L"Text Virtual Keyboard"} },
	{	E_GrUiObjIdWinVkbdNum,			{L"Number Virtual Keyboard"} },
	{	E_GrUiObjIdWinMsgBoxType0,	{L"MessageBox Yes"} },
	{	E_GrUiObjIdWinMsgBoxType1,	{L"MessageBox YesNo"} },
	{	E_GrUiObjIdWinMsgBoxType2,	{L"MessageBox YesNoCancel"} },
};

St_GrUiDvlSysIdDesc	V_GrUiDvlSysPageIdTbl[]	=	
{
	/*	userId										name						  */
	{	E_GrUiObjIdPageVkbdLang0A,		{L"Vk Frame Lang 0 - 1"} },
	{	E_GrUiObjIdPageVkbdLang0B,		{L"Vk Frame Lang 0 - 2"} },
	{	E_GrUiObjIdPageVkbdLang0C,		{L"Vk Frame Lang 0 - 3"} },
	{	E_GrUiObjIdPageVkbdLang0D,		{L"Vk Frame Lang 0 - 4"} },
	{	E_GrUiObjIdPageVkbdLang1A,		{L"Vk Frame Lang 1 - 1"} },
	{	E_GrUiObjIdPageVkbdLang1B,		{L"Vk Frame Lang 1 - 2"} },
	{	E_GrUiObjIdPageVkbdLang1C,		{L"Vk Frame Lang 1 - 3"} },
	{	E_GrUiObjIdPageVkbdLang1D,		{L"Vk Frame Lang 1 - 4"} },
	{	E_GrUiObjIdPageVkbdLang2A,		{L"Vk Frame Lang 2 - 1"} },
	{	E_GrUiObjIdPageVkbdLang2B,		{L"Vk Frame Lang 2 - 2"} },
	{	E_GrUiObjIdPageVkbdLang2C,		{L"Vk Frame Lang 2 - 3"} },
	{	E_GrUiObjIdPageVkbdLang2D,		{L"Vk Frame Lang 2 - 4"} },
	{	E_GrUiObjIdPageVkbdLang3A,		{L"Vk Frame Lang 3 - 1"} },
	{	E_GrUiObjIdPageVkbdLang3B,		{L"Vk Frame Lang 3 - 2"} },
	{	E_GrUiObjIdPageVkbdLang3C,		{L"Vk Frame Lang 3 - 3"} },
	{	E_GrUiObjIdPageVkbdLang3D,		{L"Vk Frame Lang 3 - 4"} },
	{	E_GrUiObjIdPageVkbdLang4A,		{L"Vk Frame Lang 4 - 1"} },
	{	E_GrUiObjIdPageVkbdLang4B,		{L"Vk Frame Lang 4 - 2"} },
	{	E_GrUiObjIdPageVkbdLang4C,		{L"Vk Frame Lang 4 - 3"} },
	{	E_GrUiObjIdPageVkbdLang4D,		{L"Vk Frame Lang 4 - 4"} },
	{	E_GrUiObjIdPageVkbdLang5A,		{L"Vk Frame Lang 5 - 1"} },
	{	E_GrUiObjIdPageVkbdLang5B,		{L"Vk Frame Lang 5 - 2"} },
	{	E_GrUiObjIdPageVkbdLang5C,		{L"Vk Frame Lang 5 - 3"} },
	{	E_GrUiObjIdPageVkbdLang5D,		{L"Vk Frame Lang 5 - 4"} },
	{	E_GrUiObjIdPageVkbdLang6A,		{L"Vk Frame Lang 6 - 1"} },
	{	E_GrUiObjIdPageVkbdLang6B,		{L"Vk Frame Lang 6 - 2"} },
	{	E_GrUiObjIdPageVkbdLang6C,		{L"Vk Frame Lang 6 - 3"} },
	{	E_GrUiObjIdPageVkbdLang6D,		{L"Vk Frame Lang 6 - 4"} },
	{	E_GrUiObjIdPageVkbdLang7A,		{L"Vk Frame Lang 7 - 1"} },
	{	E_GrUiObjIdPageVkbdLang7B,		{L"Vk Frame Lang 7 - 2"} },
	{	E_GrUiObjIdPageVkbdLang7C,		{L"Vk Frame Lang 7 - 3"} },
	{	E_GrUiObjIdPageVkbdLang7D,		{L"Vk Frame Lang 7 - 4"} },
	{	E_GrUiObjIdPageVkbdLang8A,		{L"Vk Frame Lang 8 - 1"} },
	{	E_GrUiObjIdPageVkbdLang8B,		{L"Vk Frame Lang 8 - 2"} },
	{	E_GrUiObjIdPageVkbdLang8C,		{L"Vk Frame Lang 8 - 3"} },
	{	E_GrUiObjIdPageVkbdLang8D,		{L"Vk Frame Lang 8 - 4"} },
	{	E_GrUiObjIdPageVkbdLang9A,		{L"Vk Frame Lang 9 - 1"} },
	{	E_GrUiObjIdPageVkbdLang9B,		{L"Vk Frame Lang 9 - 2"} },
	{	E_GrUiObjIdPageVkbdLang9C,		{L"Vk Frame Lang 9 - 3"} },
	{	E_GrUiObjIdPageVkbdLang9D,		{L"Vk Frame Lang 9 - 4"} },
	{	E_GrUiObjIdPageVkbdLang10A,		{L"Vk Frame Lang 10 - 1"} },
	{	E_GrUiObjIdPageVkbdLang10B,		{L"Vk Frame Lang 10 - 2"} },
	{	E_GrUiObjIdPageVkbdLang10C,		{L"Vk Frame Lang 10 - 3"} },
	{	E_GrUiObjIdPageVkbdLang10D,		{L"Vk Frame Lang 10 - 4"} },
	{	E_GrUiObjIdPageVkbdLang11A,		{L"Vk Frame Lang 11 - 1"} },
	{	E_GrUiObjIdPageVkbdLang11B,		{L"Vk Frame Lang 11 - 2"} },
	{	E_GrUiObjIdPageVkbdLang11C,		{L"Vk Frame Lang 11 - 3"} },
	{	E_GrUiObjIdPageVkbdLang11D,		{L"Vk Frame Lang 11 - 4"} },
	{	E_GrUiObjIdPageVkbdLang12A,		{L"Vk Frame Lang 12 - 1"} },
	{	E_GrUiObjIdPageVkbdLang12B,		{L"Vk Frame Lang 12 - 2"} },
	{	E_GrUiObjIdPageVkbdLang12C,		{L"Vk Frame Lang 12 - 3"} },
	{	E_GrUiObjIdPageVkbdLang12D,		{L"Vk Frame Lang 12 - 4"} },
	{	E_GrUiObjIdPageVkbdLang13A,		{L"Vk Frame Lang 13 - 1"} },
	{	E_GrUiObjIdPageVkbdLang13B,		{L"Vk Frame Lang 13 - 2"} },
	{	E_GrUiObjIdPageVkbdLang13C,		{L"Vk Frame Lang 13 - 3"} },
	{	E_GrUiObjIdPageVkbdLang13D,		{L"Vk Frame Lang 13 - 4"} },
	{	E_GrUiObjIdPageVkbdLang14A,		{L"Vk Frame Lang 14 - 1"} },
	{	E_GrUiObjIdPageVkbdLang14B,		{L"Vk Frame Lang 14 - 2"} },
	{	E_GrUiObjIdPageVkbdLang14C,		{L"Vk Frame Lang 14 - 3"} },
	{	E_GrUiObjIdPageVkbdLang14D,		{L"Vk Frame Lang 14 - 4"} },
	{	E_GrUiObjIdPageVkbdLang15A,		{L"Vk Frame Lang 15 - 1"} },
	{	E_GrUiObjIdPageVkbdLang15B,		{L"Vk Frame Lang 15 - 2"} },
	{	E_GrUiObjIdPageVkbdLang15C,		{L"Vk Frame Lang 15 - 3"} },
	{	E_GrUiObjIdPageVkbdLang15D,		{L"Vk Frame Lang 15 - 4"} },
};

Def_WstrName	V_GrUiDvlSkinPosStr[E_GrUiDoSkinPosValCnt]	=	
{
/*	E_GrUiDoSkinPosCenter	*/			{L"[CC]"},
/*	E_GrUiDoSkinPosMidLeft	*/		{L"[ML]"},
/*	E_GrUiDoSkinPosMidRight	*/		{L"[MR]"},
/*	E_GrUiDoSkinPosTopCenter	*/	{L"[UC]"},
/*	E_GrUiDoSkinPosTopLeft	*/		{L"[UL]"},
/*	E_GrUiDoSkinPosTopRight	*/		{L"[UR]"},
/*	E_GrUiDoSkinPosBtmCenter	*/	{L"[DC]"},
/*	E_GrUiDoSkinPosBtmLeft	*/		{L"[DL]"},
/*	E_GrUiDoSkinPosBtmRight	*/		{L"[DR]"},
/*	E_GrUiDoSkinPosTileAll	*/		{L"[TA]"},
/*	E_GrUiDoSkinPosTileLeft	*/		{L"[TL]"},
/*	E_GrUiDoSkinPosTileTop	*/		{L"[TU]"},
/*	E_GrUiDoSkinPosTileRight	*/	{L"[TR]"},
/*	E_GrUiDoSkinPosTileBottom	*/	{L"[TD]"},
/*	E_GrUiDoSkinPosTileMidHz	*/	{L"[TH]"},
/*	E_GrUiDoSkinPosTileMidVt	*/	{L"[TV]"},

};
Def_WstrName	V_GrUiDvlCloseKeyStr[]	=	
{
	/* E_GrUiDoWinKeyCloseNone */		{L"None"},
	/* E_GrUiDoWinKeyCloseEsc */		{L"ESC key"},
	/* E_GrUiDoWinKeyCloseAnykey */	{L"any key"},
};

Def_WstrName	V_GrUiDvlTypeIdTbl[E_GrUiDoTypeIdCnt]	=	
{ 
/* 0	*/	{L"None"},
/* 1	*/	{L"Window"},
/* 2	*/	{L"Frame"},
/* 3	*/	{L"Button"},
/* 4	*/	{L"Label"},
/* 5	*/	{L"ListBox"},
/* 6	*/	{L"SlideBar"},
/* 7  */	{L"SpinNum"},
/* 8  */	{L"SpinText"},
/* 9  */	{L"SpinDigit"},
/* 10  */	{L"Edit"},
/* 11  */	{L"CloseBtn"},
/* 12  */	{L"Calendar"},
/* 13	*/	{L"Quad"},
/* 14	*/	{L"GraphSel"},
/* 15	*/	{L"GraphEdit"},
/* 16	*/	{L"BitGrid"},
/* 17	*/	{L"SpinImg"},
/* 18	*/	{L"PrgsBar"},
/* 19	*/	{L"InputTime"},
/* 20	*/	{L"Area"},
/* 21	*/	{L"CdvArea"},
/* 22	*/	{L"AllBtn"},
/* 23	*/	{L"AllSpinNum"},
/* 24	*/	{L"AllSpinTxt"},
/* 25	*/	{L"AllSpinImg"},
/* 26	*/	{L"AllSpinDgt"},
};

Def_WstrName	V_GrUiDvlTxtNameTbl[]	=	
{
	{ L"[Prj Date]" },	{ L"[UserVersion]"},
	{ L"[Lang 0]" },		{ L"[Lang 1]" },			{ L"[Lang 2]" },			{ L"[Lang 3]" },
	{ L"[Lang 4]" },		{ L"[Lang 5]" },			{	L"[Lang 6]" },			{ L"[Lang 7]" },
	{ L"[Lang 8]" },		{ L"[Lang 9]" },			{ L"[Lang 10]" },			{ L"[Lang 11]" },
	{ L"[Lang 12]" },		{ L"[Lang 13]" },			{ L"[Lang 14]" },			{ L"[Lang 15]" },
	{ L"[Mon 1]" },			{ L"[Mon 2]" },				{ L"[Mon 3]" },				{ L"[Mon 4]" },
	{ L"[Mon 5]" },			{ L"[Mon 6]" },				{ L"[Mon 7]" },				{ L"[Mon 8]" },
	{ L"[Mon 9]" },			{ L"[Mon 10]" },			{ L"[Mon 11]" },			{ L"[Mon 12]" },
	{ L"[Week Sun]" },	{ L"[Week Mon]" },		{ L"[Week Tue]" },		{ L"[Week Wed]" },
	{ L"[Week Thu]" },	{ L"[Week Fri]" },		{ L"[Week Sat]" },	
	{ L"[Unit Year]" },	{ L"[Unit Month]" },	{ L"[Unit Day]" },	
	{ L"[Unit Hour]" },	{ L"[Unit Min]" },		{ L"[Unit Sec]" },		{ L"[Unit Msec]" },	
	{ L"[Time AM]" },		{ L"[Time PM]" },			{ L"[Lbx Item Del quest]" }
};

Def_WstrName	V_GrUiDvlBtnTypeName[E_GrUiDoBtnTypeCnt]	=	
{
	{ L"normal" },
	{ L"toggle" },
	{ L"radio" }
};

Def_WstrName	V_GrUiDvlStrDirTbl[4]	=	
{
	{L"left"},	{L"up"},	{L"right"},	{L"down"}
};

Def_WstrName	V_GrUiDvlStrGvalTbl[16]	=	
{
	{L"Model"},	{L"VideoCh"},	{L"AudioCh"},	{L"Idx3"},
	{L"Idx4"},	{L"Idx5"},	{L"Idx6"},	{L"Idx7"},
	{L"Idx8"},	{L"Idx9"},	{L"Idx10"},	{L"Idx11"},
	{L"Idx12"},	{L"Idx13"},	{L"Idx14"},	{L"Idx15"},
};

//able table
St_GrUiDvlParaObjAblePara	V_GrUiDvlAbleParaTbl[E_GrUiDoTypeIdCnt-1]	=	
{
		/* window */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdWinObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdRelFcsLblId,	E_GrUiScptPrIdRelFcsTxtId,	E_GrUiScptPrIdFocusBlock,	E_GrUiScptPrIdNone	}
	},
		/* frame */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdFrmObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdRelFcsLblId,	E_GrUiScptPrIdRelFcsTxtId,	E_GrUiScptPrIdFocusBlock,	E_GrUiScptPrIdNone	}
	},
		/* button */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdBtnObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg,	E_GrUiScptPrIdNone	}
	},
		/* label */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdLblObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdRelFcsLblId,	E_GrUiScptPrIdRelFcsTxtId,	E_GrUiScptPrIdFocusBlock,	E_GrUiScptPrIdNone	}
	},
		/* listbox */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdLbxObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* sbar */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSbarObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdNone	}
	},
		/* spin num */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSpbBaseObjType,	E_GrUiScptPrIdSpbNumObjType, E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* spin text */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSpbBaseObjType,	E_GrUiScptPrIdSpbTxtObjType, E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* spin digit */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSpbBaseObjType,	E_GrUiScptPrIdSpbDgtObjType, E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* edit */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSmpbObjType,	E_GrUiScptPrIdEditObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
	/* system button */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSmpbObjType,	E_GrUiScptPrIdSysBtnObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* calendar */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdCalenObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* quad */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdQuadObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* graph sel */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdGrpSelObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdNone	}
	},
		/* graph edit */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdGrpEdtObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdNone	}
	},
		/* bit grid */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdBitGridObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdNone	}
	},
		/* spin image */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSpbBaseObjType,	E_GrUiScptPrIdSpnImgObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* prograss bar */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdPrgbarObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdRelFcsLblId,	E_GrUiScptPrIdRelFcsTxtId,	E_GrUiScptPrIdFocusBlock,	E_GrUiScptPrIdNone	}
	},
		/* input time */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdInpTimeObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdRelFcsLblId,	E_GrUiScptPrIdRelFcsTxtId,	E_GrUiScptPrIdFocusBlock,	E_GrUiScptPrIdNone	}
	},
		/* area */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdAreaObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdNone	}
	},
		/* cdv area */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdCdvAreaObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdNone	}
	},
		/* action button */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdBtnObjType, E_GrUiScptPrIdAcBtnObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg,	E_GrUiScptPrIdNone	}
	},
		/* action spin number */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSpbBaseObjType,	E_GrUiScptPrIdSpbNumObjType, E_GrUiScptPrIdAcSpNumObjType, E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* action spin text */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSpbBaseObjType,	E_GrUiScptPrIdSpbTxtObjType, E_GrUiScptPrIdAcSpTxtObjType, E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* action spin image */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSpbBaseObjType,	E_GrUiScptPrIdSpnImgObjType, E_GrUiScptPrIdAcSpImgObjType,	E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},
		/* action spin digit */
	{
		{	E_GrUiScptPrIdBaseObjType,	E_GrUiScptPrIdSkinObjType,	E_GrUiScptPrIdSpbBaseObjType,	E_GrUiScptPrIdSpbDgtObjType, E_GrUiScptPrIdAcSpDgtObjType, E_GrUiScptPrIdNone	},
		{	E_GrUiScptPrIdClrBg, E_GrUiScptPrIdNone	}
	},

};

//====================================================================
//--------------------------------------------------------------------
Ptr_GrUiParaDesc	GrUiDvlGetParaInfoByPara( __u16 A_ParaId )
{
	// local -------------------
		Ptr_GrUiParaDesc	Tv_Result;
		__u32	Tv_ParaCnt;
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//find
		Tv_ParaCnt	=	GrUiDvlGetTotalParaCnt();
		for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ParaCnt;Tv_WkIdx++ )
		{
			if ( A_ParaId == V_GrUiParaInfo[Tv_WkIdx].Id )
			{
				//finded
				Tv_Result	=	&V_GrUiParaInfo[Tv_WkIdx];
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiDvlIsAblePara( __u8 A_ObjTypeId, __u16 A_ParaId )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrUiDvlParaObjAblePara	Tv_PtrTbl;
		__u16	Tv_ParaType;
		__u8	Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check able para
		if ( (E_GrUiDoTypeIdNone != A_ObjTypeId) && (E_GrUiDoTypeIdCnt > A_ObjTypeId) )
		{
			Tv_PtrTbl	=	&V_GrUiDvlAbleParaTbl[A_ObjTypeId-1];
			//check able
			Tv_ParaType	=	A_ParaId & E_GrUiScptMaskPrIdType;
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlParaMaxScptTypeCnt;Tv_WkIdx++ )
			{
				if ( Tv_ParaType == Tv_PtrTbl->ParaTypeTbl[Tv_WkIdx] )
				{
					//finded
					Tv_Result		=	TRUE;
					break;
				}
				//check finish
				if ( E_GrUiScptPrIdNone == Tv_PtrTbl->ParaTypeTbl[Tv_WkIdx] )
				{
					break;
				}
			}
			//check except
			if ( Tv_Result )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlParaMaxExceptParaCnt;Tv_WkIdx++ )
				{
					if ( A_ParaId == Tv_PtrTbl->ExcpParaTbl[Tv_WkIdx] )
					{
						Tv_Result	=	FALSE;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetDfltVal( __u16 A_ParaId, void* A_PtrRtBuf )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrUiParaDesc	Tv_PtrParaInfo;
		BOOL8		Tv_IsChked;
		__u32		Tv_Idx;
	// code --------------------
		//init
		Tv_Result		=	0;

		Tv_PtrParaInfo	=	GrUiDvlGetParaInfoByPara( A_ParaId );
		if ( NULL != Tv_PtrParaInfo )
		{
			Tv_Result	=	V_GrUiParaTypeSize[Tv_PtrParaInfo->Type];
			//fill zero
			GrDumyZeroMem( A_PtrRtBuf, Tv_Result );
			//check special type
			Tv_IsChked	=	TRUE;
			//check by type
			switch( Tv_PtrParaInfo->Type )
			{
				case	E_GrUiParaTypeTxtPara:
					if ( V_IsGrUiDvlOsdPlt )
					{
						((E_GrUiScptPrTyLblTextPara*)A_PtrRtBuf)->ClrShadow	=	E_GrUiConPltClrBlack;
					}
					else
					{
						((E_GrUiScptPrTyLblTextPara*)A_PtrRtBuf)->ClrShadow	=	GrDrawGetColorBlack( V_GrUiDvlOsdFcc );
					}
					break;
				case	E_GrUiParaTypeTxtClr:
					if ( V_IsGrUiDvlOsdPlt )
					{
						*((E_GrUiScptPrTyLblTextColor*)A_PtrRtBuf)	=	E_GrUiConPltClrWhite;
					}
					else
					{
						*((E_GrUiScptPrTyLblTextColor*)A_PtrRtBuf)	=	GrDrawGetColorWhite( V_GrUiDvlOsdFcc );
					}
					break;
					break;
				default:
					Tv_IsChked	=	FALSE;
					break;
			}
			
			//check by parametter id
			if ( !Tv_IsChked )
			{
				//patch other value
				switch( A_ParaId )
				{
					case	E_GrUiScptPrIdIsEnable:
						*((E_GrUiScptPrTyIsEnable*)A_PtrRtBuf)	=	TRUE;
						break;
					case	E_GrUiScptPrIdFocusMap:
						*((E_GrUiScptPrTyFocusMap*)A_PtrRtBuf)	=	E_GrUiDoFocusMapAll;
						break;
					case	E_GrUiScptPrIdBorderDepth:
					case	E_GrUiScptPrIdSbarBtnBrdDepth:
						*((E_GrUiScptPrTyBorderDepth*)A_PtrRtBuf)	=	E_GrUiDoSkinDfltBrdDepth;
						break;
					case	E_GrUiScptPrIdSbarRngMax:
						*((E_GrUiScptPrTySbarRngMax*)A_PtrRtBuf)	=	E_GrUiDoSbarDfltMax;
						break;
					case	E_GrUiScptPrIdBorderClrLt:
					case	E_GrUiScptPrIdBorderClrMl:
						if ( V_IsGrUiDvlOsdPlt )
						{
							*((E_GrUiScptPrTyBorderClrLt*)A_PtrRtBuf)	=	E_GrUiConPltClrWhite;
						}
						else
						{
							*((E_GrUiScptPrTyBorderClrLt*)A_PtrRtBuf)	=	GrDrawGetColorWhite( V_GrUiDvlOsdFcc );
						}
						break;
					case	E_GrUiScptPrIdBorderClrDk:
					case	E_GrUiScptPrIdBorderClrMd:
						if ( V_IsGrUiDvlOsdPlt )
						{
							*((E_GrUiScptPrTyBorderClrDk*)A_PtrRtBuf)	=	E_GrUiConPltClrBlack;
						}
						else
						{
							*((E_GrUiScptPrTyBorderClrDk*)A_PtrRtBuf)	=	GrDrawGetColorBlack( V_GrUiDvlOsdFcc );
						}
						break;
					case	E_GrUiScptPrIdSbarBtnSize:
						*((E_GrUiScptPrTySbarBtnSize*)A_PtrRtBuf)	=	E_GrUiDoSbarBtnDfltSize;
						break;
					case	E_GrUiScptPrIdLbxItmBrdDep:
						*((E_GrUiScptPrTyLbxItmBrdDep*)A_PtrRtBuf)	=	E_GrUiDoSkinDfltBrdDepth;
						break;
					case	E_GrUiScptPrIdLbxItmHeight:
						*((E_GrUiScptPrTyLbxItmHeight*)A_PtrRtBuf)	=	E_GrUiDoListboxDfltItemHeight;
						break;
					case	E_GrUiScptPrIdLbxIconWidth:
						*((E_GrUiScptPrTyLbxIconWidth*)A_PtrRtBuf)	=	E_GrUiDoListboxDfltIconWidth;
						break;
					case	E_GrUiScptPrIdLbxIconGap:
						*((E_GrUiScptPrTyLbxIconGap*)A_PtrRtBuf)	=	E_GrUiDoListboxDfltIconGap;
						break;
					case	E_GrUiScptPrIdEditMaxLen:
						*((E_GrUiScptPrTyEditMaxLen*)A_PtrRtBuf)	=	E_GrUiVkbdEdtMax;
						break;
					case	E_GrUiScptPrIdQuadClrTitle:
					case	E_GrUiScptPrIdQuadClrGrid:
					case	E_GrUiScptPrIdQuadExtTtClr:
						if ( V_IsGrUiDvlOsdPlt )
						{
							*((E_GrUiScptPrTyQuadClrTitle*)A_PtrRtBuf)	=	E_GrUiConPltClrWhite;
						}
						else
						{
							*((E_GrUiScptPrTyQuadClrTitle*)A_PtrRtBuf)	=	GrDrawGetColorWhite( V_GrUiDvlOsdFcc );
						}
						break;
					case	E_GrUiScptPrIdQuadGridShow:
						*((E_GrUiScptPrTyQuadGridShow*)A_PtrRtBuf)	=	E_GrUiDoQuadDfltGridShow;
						break;
					case	E_GrUiScptPrIdQuadTitleShow:
						*((E_GrUiScptPrTyQuadTitleShow*)A_PtrRtBuf)	=	E_GrUiDoQuadDfltTitleShow;
						break;
					case	E_GrUiScptPrIdQuadIconShow:
						*((E_GrUiScptPrTyQuadIconShow*)A_PtrRtBuf)	=	E_GrUiDoQuadDfltIconShow;
						break;
					case	E_GrUiScptPrIdQuadBgImgShow:
						*((E_GrUiScptPrTyQuadBgImgShow*)A_PtrRtBuf)	=	E_GrUiDoQuadDfltImageShow;
						break;
					case	E_GrUiScptPrIdQuadIconSizeX:
						*((E_GrUiScptPrTyQuadIconSizeX*)A_PtrRtBuf)	=	E_GrUiDoQuadDfltIconSizeX;
						break;
					case	E_GrUiScptPrIdQuadIconSizeY:
						*((E_GrUiScptPrTyQuadIconSizeY*)A_PtrRtBuf)	=	E_GrUiDoQuadDfltIconSizeY;
						break;
					case	E_GrUiScptPrIdQuadIconGapTop:
						*((E_GrUiScptPrTyQuadIconGapTop*)A_PtrRtBuf)	=	E_GrUiDoQuadDfltIconGapTop;
						break;
					case	E_GrUiScptPrIdGrpSelColCnt:
						*((E_GrUiScptPrTyGrpSelColCnt*)A_PtrRtBuf)	=	E_GrUiDoGrpSelDfltColCnt;
						break;
					case	E_GrUiScptPrIdGrpSelLineCnt:
						*((E_GrUiScptPrTyGrpSelLineCnt*)A_PtrRtBuf)	=	E_GrUiDoGrpSelDfltLineCnt;
						break;
					case	E_GrUiScptPrIdGrpSelCusColWdt:
						*((E_GrUiScptPrTyGrpSelCusColWdt*)A_PtrRtBuf)	=	E_GrUiDoGrpSelDfltCusColWidth;
						break;
					case	E_GrUiScptPrIdGrpSelColItmSize:
						*((E_GrUiScptPrTyGrpSelColItmSize*)A_PtrRtBuf)	=	E_GrUiDoGrpSelDfltColItmSize;
						break;
					case	E_GrUiScptPrIdGrpSelLineItmSize:
						*((E_GrUiScptPrTyGrpSelLineItmSize*)A_PtrRtBuf)	=	E_GrUiDoGrpSelDfltLineItmSize;
						break;
					case	E_GrUiScptPrIdGrpSelLineGap:
						*((E_GrUiScptPrTyGrpSelLineGap*)A_PtrRtBuf)	=	E_GrUiDoGrpSelDfltLineGap;
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
						*((E_GrUiScptPrTyGrpSelBitPos0*)A_PtrRtBuf)	=	(E_GrUiScptPrTyGrpSelBitPos0)Tv_Idx;
						break;
					case	E_GrUiScptPrIdGrpEdtColCnt:
						*((E_GrUiScptPrTyGrpEdtColCnt*)A_PtrRtBuf)	=	E_GrUiDoGrpEditDfltColCnt;
						break;
					case	E_GrUiScptPrIdGrpEdtLineCnt:
						*((E_GrUiScptPrTyGrpEdtLineCnt*)A_PtrRtBuf)	=	E_GrUiDoGrpEditDfltLineCnt;
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
						*((E_GrUiScptPrTyGrpEdtBitPos0*)A_PtrRtBuf)	=	(E_GrUiScptPrTyGrpEdtBitPos0)Tv_Idx;
						break;
					case	E_GrUiScptPrIdGrpEdtClrGrid:
						*((E_GrUiScptPrTyGrpEdtClrGrid*)A_PtrRtBuf)	=	GrDrawGetColorWhite( V_GrUiDvlOsdFcc );
						break;
					case	E_GrUiScptPrIdGrpEdtGridWidth:
						*((E_GrUiScptPrTyGrpEdtGridWidth*)A_PtrRtBuf)	=	E_GrUiDoGrpEditDfltGridWidth;
						break;
					case	E_GrUiScptPrIdGrpEdtClrCus:
						*((E_GrUiScptPrTyGrpEdtClrCus*)A_PtrRtBuf)	=	GrDrawGetColorBlack( V_GrUiDvlOsdFcc );
						break;
					case	E_GrUiScptPrIdGrpEdtCusWidth:
						*((E_GrUiScptPrTyGrpEdtCusWidth*)A_PtrRtBuf)	=	E_GrUiDoGrpEditDfltCusWidth;
						break;
					case	E_GrUiScptPrIdGrpEdtColItmSize:
						*((E_GrUiScptPrTyGrpEdtColItmSize*)A_PtrRtBuf)	=	E_GrUiDoGrpEditDfltColItmSize;
						break;
					case	E_GrUiScptPrIdGrpEdtLineItmSize:
						*((E_GrUiScptPrTyGrpEdtLineItmSize*)A_PtrRtBuf)	=	E_GrUiDoGrpEditDfltLineItmSize;
						break;
					case	E_GrUiScptPrIdGrpEdtValCnt:
						*((E_GrUiScptPrTyGrpEdtValCnt*)A_PtrRtBuf)	=	E_GrUiDoGrpEditDfltValCnt;
						break;
					case	E_GrUiScptPrIdBitGridCelSizeX:
						*((E_GrUiScptPrTyBitGridCelSizeX*)A_PtrRtBuf)	=	E_GrUiDoBitGridDfltCellSizeX;
						break;
					case	E_GrUiScptPrIdBitGridCelSizeY:
						*((E_GrUiScptPrTyBitGridCelSizeY*)A_PtrRtBuf)	=	E_GrUiDoBitGridDfltCellSizeY;
						break;
					case	E_GrUiScptPrIdBitGridGridCntX:
						*((E_GrUiScptPrTyBitGridGridCntX*)A_PtrRtBuf)	=	E_GrUiDoBitGridDfltGridCntX;
						break;
					case	E_GrUiScptPrIdBitGridGridCntY:
						*((E_GrUiScptPrTyBitGridGridCntY*)A_PtrRtBuf)	=	E_GrUiDoBitGridDfltGridCntY;
						break;
					case	E_GrUiScptPrIdBitGridGridWidth:
						*((E_GrUiScptPrTyBitGridGridWidth*)A_PtrRtBuf)	=	E_GrUiDoBitGridDfltGridWidth;
						break;
					case	E_GrUiScptPrIdBitGridGridColor:
						*((E_GrUiScptPrTyBitGridGridColor*)A_PtrRtBuf)	=	GrDrawGetColorWhite( V_GrUiDvlOsdFcc );
						break;
					case	E_GrUiScptPrIdBitGridMarkWidth:
						*((E_GrUiScptPrTyBitGridMarkWidth*)A_PtrRtBuf)	=	E_GrUiDoBitGridDfltMarkWidth;
						break;
					case	E_GrUiScptPrIdBitGridMarkColor:
						*((E_GrUiScptPrTyBitGridMarkColor*)A_PtrRtBuf)	=	GrDrawGetColorBlack( V_GrUiDvlOsdFcc );
						break;
					case	E_GrUiScptPrIdBitGridCusWidth:
						*((E_GrUiScptPrTyBitGridCusWidth*)A_PtrRtBuf)	=	E_GrUiDoBitGridDfltCusWidth;
						break;
					case	E_GrUiScptPrIdBitGridCusColor:
						*((E_GrUiScptPrTyBitGridCusColor*)A_PtrRtBuf)	=	GrDrawGetColorWhite( V_GrUiDvlOsdFcc );
						break;
					case	E_GrUiScptPrIdIsFocusAble:
						*((E_GrUiScptPrTyIsFocusAble*)A_PtrRtBuf)	=	TRUE;
						break;
					case	E_GrUiScptPrIdSpnImgImageCnt:
						*((E_GrUiScptPrTySpnImgImageCnt*)A_PtrRtBuf)	=	E_GrUiDoSpinImgDfltImgCnt;
						break;
					case	E_GrUiScptPrIdPrgbarValMax:
						*((E_GrUiScptPrTyPrgbarValMax*)A_PtrRtBuf)	=	E_GrUiDoPrgsbarDfltMaxVal;
						break;
					case	E_GrUiScptPrIdSpbDgtCipher:
						*((E_GrUiScptPrTySpbDgtCipher*)A_PtrRtBuf)	=	E_GrUiDoSpinDgtDfltCipher;
						break;
					case	E_GrUiScptPrIdSpbDgtValMax:
						*((E_GrUiScptPrTySpbDgtValMax*)A_PtrRtBuf)	=	E_GrUiDoSpinDgtDfltMaxVal;
						break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetTotalParaCnt( void )
{
		return	( sizeof(V_GrUiParaInfo) / sizeof(St_GrUiParaDesc) );
}
//--------------------------------------------------------------------
Ptr_GrUiParaDesc	GrUiDvlGetParaInfoByIdx( __u32 A_Idx )
{
	// local -------------------
		Ptr_GrUiParaDesc	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check index
		if ( GrUiDvlGetTotalParaCnt() > A_Idx )
		{
			Tv_Result	=	&V_GrUiParaInfo[A_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValSbar( Cls_GrUiDoSbar* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSbarDirecton:
				*((E_GrUiScptPrTySbarDirecton*)A_PtrBuf)	=	A_Obj->IsBarDirVt();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarDirecton);
				break;
			case	E_GrUiScptPrIdSbarBtnBrdAtb:
				*((E_GrUiScptPrTySbarBtnBrdAtb*)A_PtrBuf)	=	A_Obj->SbtnBrdAtbGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarBtnBrdAtb);
				break;
			case	E_GrUiScptPrIdSbarBtnBrdDepth:
				*((E_GrUiScptPrTySbarBtnBrdDepth*)A_PtrBuf)	=	A_Obj->SbtnBrdDepthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarBtnBrdDepth);
				break;
			case	E_GrUiScptPrIdSbarBtnImgDis:
			case	E_GrUiScptPrIdSbarBtnImgNrm:
			case	E_GrUiScptPrIdSbarBtnImgFcs:
			case	E_GrUiScptPrIdSbarBtnImgCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSbarBtnImgDis;
				A_Obj->SbtnStatImgGet( Tv_Idx, ((E_GrUiScptPrTySbarBtnImgDis*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarBtnImgDis);
				break;
			case	E_GrUiScptPrIdSbarBtnClrDis:
			case	E_GrUiScptPrIdSbarBtnClrNrm:
			case	E_GrUiScptPrIdSbarBtnClrFcs:
			case	E_GrUiScptPrIdSbarBtnClrCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSbarBtnClrDis;
				*((E_GrUiScptPrTySbarBtnClrDis*)A_PtrBuf)	=	A_Obj->SbtnStatClrGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarBtnClrDis);
				break;
			case	E_GrUiScptPrIdSbarRngMin:
				*((E_GrUiScptPrTySbarRngMin*)A_PtrBuf)	=	A_Obj->RangeMinGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarRngMin);
				break;
			case	E_GrUiScptPrIdSbarRngMax:
				*((E_GrUiScptPrTySbarRngMax*)A_PtrBuf)	=	A_Obj->RangeMaxGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarRngMax);
				break;
			case	E_GrUiScptPrIdSbarValPos:
				*((E_GrUiScptPrTySbarValPos*)A_PtrBuf)	=	A_Obj->PosGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarValPos);
				break;
			case	E_GrUiScptPrIdSbarPageUnit:
				*((E_GrUiScptPrTySbarPageUnit*)A_PtrBuf)	=	A_Obj->PageUnitGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarPageUnit);
				break;
			case	E_GrUiScptPrIdSbarEvtHostId:
				*((E_GrUiScptPrTySbarEvtHostId*)A_PtrBuf)	=	A_Obj->EvtHostIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarEvtHostId);
				break;
			case	E_GrUiScptPrIdSbarBtnSize:
				*((E_GrUiScptPrTySbarBtnSize*)A_PtrBuf)	=	A_Obj->SbtnSizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarBtnSize);
				break;
			case	E_GrUiScptPrIdSbarOnKeyCtrl:
				*((E_GrUiScptPrTySbarOnKeyCtrl*)A_PtrBuf)	=	A_Obj->IsOnKyeCtrl();
				Tv_Result	=	sizeof(E_GrUiScptPrTySbarOnKeyCtrl);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValSpinBase( Cls_GrUiDoSpinBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpbBaseTxtPara:
				A_Obj->TextParaGet( ((E_GrUiScptPrTySpbBaseTxtPara*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbBaseTxtPara);
				break;
			case	E_GrUiScptPrIdSpbBaseClrDis:
			case	E_GrUiScptPrIdSpbBaseClrNrm:
			case	E_GrUiScptPrIdSpbBaseClrFcs:
			case	E_GrUiScptPrIdSpbBaseClrCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSpbBaseClrDis;
				*((E_GrUiScptPrTySpbBaseClrDis*)A_PtrBuf)	=	A_Obj->StatClrGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbBaseClrDis);
				break;
			case	E_GrUiScptPrIdSpbBaseTclrDis:
			case	E_GrUiScptPrIdSpbBaseTclrNrm:
			case	E_GrUiScptPrIdSpbBaseTclrFcs:
			case	E_GrUiScptPrIdSpbBaseTclrCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSpbBaseTclrDis;
				*((E_GrUiScptPrTySpbBaseTclrDis*)A_PtrBuf)	=	A_Obj->StatTxtClrGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbBaseTclrDis);
				break;
			case	E_GrUiScptPrIdSpbBaseSkinDis:
			case	E_GrUiScptPrIdSpbBaseSkinNrm:
			case	E_GrUiScptPrIdSpbBaseSkinFcs:
			case	E_GrUiScptPrIdSpbBaseSkinCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSpbBaseSkinDis;
				A_Obj->StatSkinGet( (__u8)Tv_Idx, ((E_GrUiScptPrTySpbBaseSkinDis*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbBaseSkinDis);
				break;
			case	E_GrUiScptPrIdSpbBaseEvtHostId:
				*((E_GrUiScptPrTySpbBaseEvtHostId*)A_PtrBuf)	=	A_Obj->EvtHostIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbBaseEvtHostId);
				break;
			case	E_GrUiScptPrIdSpbBaseOnKeyCtrl:
				*((E_GrUiScptPrTySpbBaseOnKeyCtrl*)A_PtrBuf)	=	A_Obj->IsOnKeyCtrl();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbBaseOnKeyCtrl);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValSpinNum( Cls_GrUiDoSpinNum* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpbNumRngMax:
				*((E_GrUiScptPrTySpbNumRngMax*)A_PtrBuf)	=	A_Obj->RangeMaxGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumRngMax);
				break;
			case	E_GrUiScptPrIdSpbNumRngMin:
				*((E_GrUiScptPrTySpbNumRngMin*)A_PtrBuf)	=	A_Obj->RangeMinGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumRngMin);
				break;
			case	E_GrUiScptPrIdSpbNumValue:
				*((E_GrUiScptPrTySpbNumValue*)A_PtrBuf)	=	A_Obj->ValueGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumValue);
				break;
			case	E_GrUiScptPrIdSpbNumStep:
				*((E_GrUiScptPrTySpbNumStep*)A_PtrBuf)	=	A_Obj->StepGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumStep);
				break;
			case	E_GrUiScptPrIdSpbNumTxtIdZero:
				*((E_GrUiScptPrTySpbNumTxtIdZero*)A_PtrBuf)	=	A_Obj->ZeroTxtIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumTxtIdZero);
				break;
			case	E_GrUiScptPrIdSpbNumVertical:
				*((E_GrUiScptPrTySpbNumVertical*)A_PtrBuf)	=	A_Obj->IsVerticalType();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumVertical);
				break;
			case	E_GrUiScptPrIdSpbNumCipher:
				*((E_GrUiScptPrTySpbNumCipher*)A_PtrBuf)	=	A_Obj->CipherGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumCipher);
				break;
			case	E_GrUiScptPrIdSpbNumTxtIdPre:
				*((E_GrUiScptPrTySpbNumTxtIdPre*)A_PtrBuf)	=	A_Obj->PrefixTxtIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumTxtIdPre);
				break;
			case	E_GrUiScptPrIdSpbNumTxtIdSuf:
				*((E_GrUiScptPrTySpbNumTxtIdSuf*)A_PtrBuf)	=	A_Obj->SuffixTxtIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumTxtIdSuf);
				break;
			case	E_GrUiScptPrIdSpbNumCenterSz:
				*((E_GrUiScptPrTySpbNumCenterSz*)A_PtrBuf)	=	A_Obj->CenterSizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbNumCenterSz);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValSpinTxt( Cls_GrUiDoSpinTxt* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpbTxtTxtIdSt:
				*((E_GrUiScptPrTySpbTxtTxtIdSt*)A_PtrBuf)	=	A_Obj->TextIdStartGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbTxtTxtIdSt);
				break;
			case	E_GrUiScptPrIdSpbTxtTxtIdCnt:
				*((E_GrUiScptPrTySpbTxtTxtIdCnt*)A_PtrBuf)	=	A_Obj->TextIdCountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbTxtTxtIdCnt);
				break;
			case	E_GrUiScptPrIdSpbTxtVertical:
				*((E_GrUiScptPrTySpbTxtVertical*)A_PtrBuf)	=	A_Obj->IsVerticalType();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbTxtVertical);
				break;
			case	E_GrUiScptPrIdSpbTxtCenterSz:
				*((E_GrUiScptPrTySpbTxtCenterSz*)A_PtrBuf)	=	A_Obj->CenterSizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbTxtCenterSz);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValSpinDgt( Cls_GrUiDoSpinDgt* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpbDgtHexMode:
				*((E_GrUiScptPrTySpbDgtHexMode*)A_PtrBuf)	=	A_Obj->IsModeHex();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbDgtHexMode);
				break;
			case	E_GrUiScptPrIdSpbDgtNextInp:
				*((E_GrUiScptPrTySpbDgtNextInp*)A_PtrBuf)	=	A_Obj->NextInputIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbDgtNextInp);
				break;
			case	E_GrUiScptPrIdSpbDgtAreaUpdn:
				*((E_GrUiScptPrTySpbDgtAreaUpdn*)A_PtrBuf)	=	A_Obj->AreaUpDownSizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbDgtAreaUpdn);
				break;
			case	E_GrUiScptPrIdSpbDgtAreaCipher:
				*((E_GrUiScptPrTySpbDgtAreaCipher*)A_PtrBuf)	=	A_Obj->AreaCipherSizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbDgtAreaCipher);
				break;
			case	E_GrUiScptPrIdSpbDgtCipher:
				*((E_GrUiScptPrTySpbDgtCipher*)A_PtrBuf)	=	A_Obj->CipherGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbDgtCipher);
				break;
			case	E_GrUiScptPrIdSpbDgtValMax:
				*((E_GrUiScptPrTySpbDgtValMax*)A_PtrBuf)	=	A_Obj->ValMaxGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpbDgtValMax);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValSmpBtn( Cls_GrUiDoSmpBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSmpbTxtPara:
				A_Obj->TextParaGet( ((E_GrUiScptPrTySmpbTxtPara*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTySmpbTxtPara);
				break;
			case	E_GrUiScptPrIdSmpbClrDis:
			case	E_GrUiScptPrIdSmpbClrNrm:
			case	E_GrUiScptPrIdSmpbClrFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSmpbClrDis;
				*((E_GrUiScptPrTySmpbClrDis*)A_PtrBuf)	=	A_Obj->StatClrGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTySmpbClrDis);
				break;
			case	E_GrUiScptPrIdSmpbTclrDis:
			case	E_GrUiScptPrIdSmpbTclrNrm:
			case	E_GrUiScptPrIdSmpbTclrFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSmpbTclrDis;
				*((E_GrUiScptPrTySmpbTclrDis*)A_PtrBuf)	=	A_Obj->TxtClrGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTySmpbTclrDis);
				break;
			case	E_GrUiScptPrIdSmpbSkinDis:
			case	E_GrUiScptPrIdSmpbSkinNrm:
			case	E_GrUiScptPrIdSmpbSkinFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdSmpbSkinDis;
				A_Obj->StatSkinGet( (__u8)Tv_Idx, ((E_GrUiScptPrTySmpbSkinDis*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTySmpbSkinDis);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValEdit( Cls_GrUiDoEdit* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		Tv_Idx	=	0;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdEditNumType:
				*((E_GrUiScptPrTyEditNumType*)A_PtrBuf)	=	A_Obj->IsEditTypeNumber();
				Tv_Result	=	sizeof(E_GrUiScptPrTyEditNumType);
				break;
			case	E_GrUiScptPrIdEditEvtHostId:
				*((E_GrUiScptPrTyEditEvtHostId*)A_PtrBuf)	=	A_Obj->EvtHostIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyEditEvtHostId);
				break;
			case	E_GrUiScptPrIdEditMaxLen:
				*((E_GrUiScptPrTyEditMaxLen*)A_PtrBuf)	=	A_Obj->EditMaxLenGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyEditMaxLen);
				break;
			case	E_GrUiScptPrIdEditNoKeyVk:
				*((E_GrUiScptPrTyEditNoKeyVk*)A_PtrBuf)	=	A_Obj->IsNoKeyVkbd();
				Tv_Result	=	sizeof(E_GrUiScptPrTyEditNoKeyVk);
				break;
			case	E_GrUiScptPrIdEditSkinEfcs:
				Tv_Idx ++;
			case	E_GrUiScptPrIdEditSkinEnrm:
				Tv_Idx	=	Tv_Idx + E_GrUiDoEditImgIdxEptyNrm;
				A_Obj->StatSkinGet( (__u8)Tv_Idx, ((E_GrUiScptPrTyEditSkinEnrm*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyEditSkinEnrm);
				break;
			case	E_GrUiScptPrIdEditClrEfcs:
				Tv_Idx ++;
			case	E_GrUiScptPrIdEditClrEnrm:
				Tv_Idx	=	Tv_Idx + E_GrUiDoEditImgIdxEptyNrm;
				*((E_GrUiScptPrTyEditClrEnrm*)A_PtrBuf)	=	A_Obj->StatClrGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyEditClrEnrm);
				break;
			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValSysBtn( Cls_GrUiDoSysBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSysBtnTextId:
				*((E_GrUiScptPrTySysBtnTextId*)A_PtrBuf)	=	A_Obj->TextIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySysBtnTextId);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValCalendar( Cls_GrUiDoCalendar* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		Tv_Idx	=	0;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdCalenTxtPara:
				A_Obj->TextParaGet( ((E_GrUiScptPrTyCalenTxtPara*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenTxtPara);
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
				*((E_GrUiScptPrTyCalenClrTxtNrm*)A_PtrBuf)	=	A_Obj->CalendarClrGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenClrTxtNrm);
				break;
			case	E_GrUiScptPrIdCalenItmWidth:
				*((E_GrUiScptPrTyCalenItmWidth*)A_PtrBuf)	=	A_Obj->ItemWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenItmWidth);
				break;
			case	E_GrUiScptPrIdCalenItmHeight:
				*((E_GrUiScptPrTyCalenItmHeight*)A_PtrBuf)	=	A_Obj->ItemHeightGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenItmHeight);
				break;
			case	E_GrUiScptPrIdCalenItmGapX:
				*((E_GrUiScptPrTyCalenItmGapX*)A_PtrBuf)	=	A_Obj->ItemGapxGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenItmGapX);
				break;
			case	E_GrUiScptPrIdCalenItmGapY:
				*((E_GrUiScptPrTyCalenItmGapY*)A_PtrBuf)	=	A_Obj->ItemGapyGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenItmGapY);
				break;
			case	E_GrUiScptPrIdCalenSelGap:
				*((E_GrUiScptPrTyCalenSelGap*)A_PtrBuf)	=	A_Obj->SelBoxGapGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenSelGap);
				break;
			case	E_GrUiScptPrIdCalenSelDepth:
				*((E_GrUiScptPrTyCalenSelDepth*)A_PtrBuf)	=	A_Obj->SelBoxDepthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenSelDepth);
				break;
			case	E_GrUiScptPrIdCalenCusGap:
				*((E_GrUiScptPrTyCalenCusGap*)A_PtrBuf)	=	A_Obj->CusBoxGapGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenCusGap);
				break;
			case	E_GrUiScptPrIdCalenCusDepth:
				*((E_GrUiScptPrTyCalenCusDepth*)A_PtrBuf)	=	A_Obj->CusBoxDepthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenCusDepth);
				break;
			case	E_GrUiScptPrIdCalenSkinMark:
			case	E_GrUiScptPrIdCalenSkinSel:
			case	E_GrUiScptPrIdCalenSkinCus:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdCalenSkinMark;
				A_Obj->CalendarSkinGet( (__u8)Tv_Idx, ((E_GrUiScptPrTyCalenSkinMark*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenSkinMark);
				break;
			case	E_GrUiScptPrIdCalenClrDis:
			case	E_GrUiScptPrIdCalenClrNrm:
			case	E_GrUiScptPrIdCalenClrFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdCalenClrDis;
				*((E_GrUiScptPrTyCalenClrDis*)A_PtrBuf)	=	A_Obj->StatClrGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenClrDis);
				break;
			case	E_GrUiScptPrIdCalenSpinYear:
				*((E_GrUiScptPrTyCalenSpinYear*)A_PtrBuf)	=	A_Obj->SpinYearIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenSpinYear);
				break;
			case	E_GrUiScptPrIdCalenSpinMonth:
				*((E_GrUiScptPrTyCalenSpinMonth*)A_PtrBuf)	=	A_Obj->SpinMonthIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenSpinMonth);
				break;
			case	E_GrUiScptPrIdCalenEvtHostId:
				*((E_GrUiScptPrTyCalenEvtHostId*)A_PtrBuf)	=	A_Obj->EvtHostIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCalenEvtHostId);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValQuad( Cls_GrUiDoQuad* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdQuadTxtPara:
				A_Obj->TextParaGet( ((E_GrUiScptPrTyQuadTxtPara*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadTxtPara);
				break;
			case	E_GrUiScptPrIdQuadClrTitle:
				*((E_GrUiScptPrTyQuadClrTitle*)A_PtrBuf)	=	A_Obj->TitleColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadClrTitle);
				break;
			case	E_GrUiScptPrIdQuadClrGrid:
				*((E_GrUiScptPrTyQuadClrGrid*)A_PtrBuf)	=	A_Obj->GridColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadClrGrid);
				break;
			case	E_GrUiScptPrIdQuadGridShow:
				*((E_GrUiScptPrTyQuadGridShow*)A_PtrBuf)	=	A_Obj->IsGridShow();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadGridShow);
				break;
			case	E_GrUiScptPrIdQuadTitleShow:
				*((E_GrUiScptPrTyQuadTitleShow*)A_PtrBuf)	=	A_Obj->IsTitleShow();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadTitleShow);
				break;
			case	E_GrUiScptPrIdQuadIconShow:
				*((E_GrUiScptPrTyQuadIconShow*)A_PtrBuf)	=	A_Obj->IsIconShow();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadIconShow);
				break;
			case	E_GrUiScptPrIdQuadBgImgShow:
				*((E_GrUiScptPrTyQuadBgImgShow*)A_PtrBuf)	=	A_Obj->IsBgImgShow();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadBgImgShow);
				break;
			case	E_GrUiScptPrIdQuadChMargin:
				*((E_GrUiScptPrTyQuadChMargin*)A_PtrBuf)	=	A_Obj->ChMarginGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadChMargin);
				break;
			case	E_GrUiScptPrIdQuadIconSizeX:
				*((E_GrUiScptPrTyQuadIconSizeX*)A_PtrBuf)	=	A_Obj->IconXsizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadIconSizeX);
				break;
			case	E_GrUiScptPrIdQuadIconSizeY:
				*((E_GrUiScptPrTyQuadIconSizeY*)A_PtrBuf)	=	A_Obj->IconYsizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadIconSizeY);
				break;
			case	E_GrUiScptPrIdQuadIconGapTop:
				*((E_GrUiScptPrTyQuadIconGapTop*)A_PtrBuf)	=	A_Obj->IconGapTopGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadIconGapTop);
				break;
			case	E_GrUiScptPrIdQuadIconGapBtw:
				*((E_GrUiScptPrTyQuadIconGapBtw*)A_PtrBuf)	=	A_Obj->IconGapBtwGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadIconGapBtw);
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
				A_Obj->IconImgGet( (__u8)Tv_Idx, ((E_GrUiScptPrTyQuadIconImgRec*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadIconImgRec);
				break;
			case	E_GrUiScptPrIdQuadPosMode:
				*((E_GrUiScptPrTyQuadPosMode*)A_PtrBuf)	=	A_Obj->PosModeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadPosMode);
				break;
			case	E_GrUiScptPrIdQuadExtTtClr:
				//*((E_GrUiScptPrTyQuadExtTtClr*)A_PtrBuf)	=	A_Obj->ExtTitleColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadExtTtClr);
				break;
			case	E_GrUiScptPrIdQuadExtTtPara:
				A_Obj->ExtTxtParaGet( ((E_GrUiScptPrTyQuadExtTtPara*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadExtTtPara);
				break;
			case	E_GrUiScptPrIdQuadGridWidth:
				*((E_GrUiScptPrTyQuadGridWidth*)A_PtrBuf)	=	A_Obj->GridWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyQuadGridWidth);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValGrpSel( Cls_GrUiDoGrpSel* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdGrpSelColCnt:
				*((E_GrUiScptPrTyGrpSelColCnt*)A_PtrBuf)	=	A_Obj->ColumCountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelColCnt);
				break;
			case	E_GrUiScptPrIdGrpSelLineCnt:
				*((E_GrUiScptPrTyGrpSelLineCnt*)A_PtrBuf)	=	A_Obj->LineCountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelLineCnt);
				break;
			case	E_GrUiScptPrIdGrpSelCusClrDis:
			case	E_GrUiScptPrIdGrpSelCusClrNrm:
			case	E_GrUiScptPrIdGrpSelCusClrFcs:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdGrpSelCusClrDis;
				*((E_GrUiScptPrTyGrpSelCusClrDis*)A_PtrBuf)	=	A_Obj->CusColorGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelCusClrDis);
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
				*((E_GrUiScptPrTyGrpSelAtbClr0*)A_PtrBuf)	=	A_Obj->AtbColorGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelAtbClr0);
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
				*((E_GrUiScptPrTyGrpSelBitPos0*)A_PtrBuf)	=	A_Obj->BitGroupPosGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelBitPos0);
				break;
			case	E_GrUiScptPrIdGrpSelCusColWdt:
				*((E_GrUiScptPrTyGrpSelCusColWdt*)A_PtrBuf)	=	A_Obj->CusColumWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelCusColWdt);
				break;
			case	E_GrUiScptPrIdGrpSelCusLineWdt:
				*((E_GrUiScptPrTyGrpSelCusLineWdt*)A_PtrBuf)	=	A_Obj->CusLineWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelCusLineWdt);
				break;
			case	E_GrUiScptPrIdGrpSelColItmSize:
				*((E_GrUiScptPrTyGrpSelColItmSize*)A_PtrBuf)	=	A_Obj->ColumItemSizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelColItmSize);
				break;
			case	E_GrUiScptPrIdGrpSelLineItmSize:
				*((E_GrUiScptPrTyGrpSelLineItmSize*)A_PtrBuf)	=	A_Obj->LineItemSizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelLineItmSize);
				break;
			case	E_GrUiScptPrIdGrpSelLineGap:
				*((E_GrUiScptPrTyGrpSelLineGap*)A_PtrBuf)	=	A_Obj->LineGapGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelLineGap);
				break;
			case	E_GrUiScptPrIdGrpSelBitGrpCnt:
				*((E_GrUiScptPrTyGrpSelBitGrpCnt*)A_PtrBuf)	=	A_Obj->BitGroupCountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelBitGrpCnt);
				break;
			case	E_GrUiScptPrIdGrpSelBitGrpGap:
				*((E_GrUiScptPrTyGrpSelBitGrpGap*)A_PtrBuf)	=	A_Obj->BitGroupGapGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelBitGrpGap);
				break;
			case	E_GrUiScptPrIdGrpSelRltColLbl:
				*((E_GrUiScptPrTyGrpSelRltColLbl*)A_PtrBuf)	=	A_Obj->RltColumLableIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelRltColLbl);
				break;
			case	E_GrUiScptPrIdGrpSelRltLineLbl:
				*((E_GrUiScptPrTyGrpSelRltLineLbl*)A_PtrBuf)	=	A_Obj->RltLineLableIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelRltLineLbl);
				break;
			case	E_GrUiScptPrIdGrpSelColSelClr:
				*((E_GrUiScptPrTyGrpSelColSelClr*)A_PtrBuf)	=	A_Obj->ColumSelColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpSelColSelClr);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValGrpEdit( Cls_GrUiDoGrpEdit* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdGrpEdtColCnt:
				*((E_GrUiScptPrTyGrpEdtColCnt*)A_PtrBuf)	=	A_Obj->ColumCountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtColCnt);
				break;
			case	E_GrUiScptPrIdGrpEdtLineCnt:
				*((E_GrUiScptPrTyGrpEdtLineCnt*)A_PtrBuf)	=	A_Obj->LineCountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtLineCnt);
				break;
			case	E_GrUiScptPrIdGrpEdtBitMode:
				*((E_GrUiScptPrTyGrpEdtBitMode*)A_PtrBuf)	=	A_Obj->IsBitMode();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtBitMode);
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
				*((E_GrUiScptPrTyGrpEdtBitPos0*)A_PtrBuf)	=	A_Obj->BitPosGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtBitPos0);
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
				*((E_GrUiScptPrTyGrpEdtClrAtb0*)A_PtrBuf)	=	A_Obj->ClrAtbGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtClrAtb0);
				break;
			case	E_GrUiScptPrIdGrpEdtClrGrid:
				*((E_GrUiScptPrTyGrpEdtClrGrid*)A_PtrBuf)	=	A_Obj->ClrGridGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtClrGrid);
				break;
			case	E_GrUiScptPrIdGrpEdtGridWidth:
				*((E_GrUiScptPrTyGrpEdtGridWidth*)A_PtrBuf)	=	A_Obj->GridWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtGridWidth);
				break;
			case	E_GrUiScptPrIdGrpEdtClrCus:
				*((E_GrUiScptPrTyGrpEdtClrCus*)A_PtrBuf)	=	A_Obj->ClrCursorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtClrCus);
				break;
			case	E_GrUiScptPrIdGrpEdtCusWidth:
				*((E_GrUiScptPrTyGrpEdtCusWidth*)A_PtrBuf)	=	A_Obj->CursorWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtCusWidth);
				break;
			case	E_GrUiScptPrIdGrpEdtColItmSize:
				*((E_GrUiScptPrTyGrpEdtColItmSize*)A_PtrBuf)	=	A_Obj->ColumItemSizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtColItmSize);
				break;
			case	E_GrUiScptPrIdGrpEdtLineItmSize:
				*((E_GrUiScptPrTyGrpEdtLineItmSize*)A_PtrBuf)	=	A_Obj->LineItemSizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtLineItmSize);
				break;
			case	E_GrUiScptPrIdGrpEdtLineGap:
				*((E_GrUiScptPrTyGrpEdtLineGap*)A_PtrBuf)	=	A_Obj->LineGapGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtLineGap);
				break;
			case	E_GrUiScptPrIdGrpEdtRltLblCol:
				*((E_GrUiScptPrTyGrpEdtRltLblCol*)A_PtrBuf)	=	A_Obj->RltLblColumIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtRltLblCol);
				break;
			case	E_GrUiScptPrIdGrpEdtRltLblLine:
				*((E_GrUiScptPrTyGrpEdtRltLblLine*)A_PtrBuf)	=	A_Obj->RltLblLineIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtRltLblLine);
				break;
			case	E_GrUiScptPrIdGrpEdtValCnt:
				*((E_GrUiScptPrTyGrpEdtValCnt*)A_PtrBuf)	=	A_Obj->ValueCountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyGrpEdtValCnt);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValBitGrid( Cls_GrUiDoBitGrid* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdBitGridCelSizeX:
				*((E_GrUiScptPrTyBitGridCelSizeX*)A_PtrBuf)	=	A_Obj->CellXsizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridCelSizeX);
				break;
			case	E_GrUiScptPrIdBitGridCelSizeY:
				*((E_GrUiScptPrTyBitGridCelSizeY*)A_PtrBuf)	=	A_Obj->CellYsizeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridCelSizeY);
				break;
			case	E_GrUiScptPrIdBitGridGridCntX:
				*((E_GrUiScptPrTyBitGridGridCntX*)A_PtrBuf)	=	A_Obj->GridXcountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridGridCntX);
				break;
			case	E_GrUiScptPrIdBitGridGridCntY:
				*((E_GrUiScptPrTyBitGridGridCntY*)A_PtrBuf)	=	A_Obj->GridYcountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridGridCntY);
				break;
			case	E_GrUiScptPrIdBitGridGridWidth:
				*((E_GrUiScptPrTyBitGridGridWidth*)A_PtrBuf)	=	A_Obj->GridWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridGridWidth);
				break;
			case	E_GrUiScptPrIdBitGridGridColor:
				*((E_GrUiScptPrTyBitGridGridColor*)A_PtrBuf)	=	A_Obj->GridColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridGridColor);
				break;
			case	E_GrUiScptPrIdBitGridMarkWidth:
				*((E_GrUiScptPrTyBitGridMarkWidth*)A_PtrBuf)	=	A_Obj->MarkWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridMarkWidth);
				break;
			case	E_GrUiScptPrIdBitGridMarkColor:
				*((E_GrUiScptPrTyBitGridMarkColor*)A_PtrBuf)	=	A_Obj->MarkColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridMarkColor);
				break;
			case	E_GrUiScptPrIdBitGridCusWidth:
				*((E_GrUiScptPrTyBitGridCusWidth*)A_PtrBuf)	=	A_Obj->CursorWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridCusWidth);
				break;
			case	E_GrUiScptPrIdBitGridCusColor:
				*((E_GrUiScptPrTyBitGridCusColor*)A_PtrBuf)	=	A_Obj->CursorColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridCusColor);
				break;
			case	E_GrUiScptPrIdBitGridDispColor:
				*((E_GrUiScptPrTyBitGridDispColor*)A_PtrBuf)	=	A_Obj->DisplayColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBitGridDispColor);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValSpinImg( Cls_GrUiDoSpinImg* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdSpnImgVertical:
				*((E_GrUiScptPrTySpnImgVertical*)A_PtrBuf)	=	A_Obj->IsVerticalType();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpnImgVertical);
				break;
			case	E_GrUiScptPrIdSpnImgImageCnt:
				*((E_GrUiScptPrTySpnImgImageCnt*)A_PtrBuf)	=	A_Obj->ImageCountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpnImgImageCnt);
				break;
			case	E_GrUiScptPrIdSpnImgTxtStId:
				*((E_GrUiScptPrTySpnImgTxtStId*)A_PtrBuf)	=	A_Obj->TextStartIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTySpnImgTxtStId);
				break;

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
				A_Obj->ImageGet( (__u8)Tv_Idx, ((E_GrUiScptPrTySpnImgImage0*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTySpnImgImage0);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValPrgsBar( Cls_GrUiDoPrgsbar* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdPrgbarBarColor:
				*((E_GrUiScptPrTyPrgbarBarColor*)A_PtrBuf)	=	A_Obj->BarColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyPrgbarBarColor);
				break;
			case	E_GrUiScptPrIdPrgbarBarImage:
				A_Obj->BarImageGet( ((E_GrUiScptPrTyPrgbarBarImage*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyPrgbarBarImage);
				break;
			case	E_GrUiScptPrIdPrgbarValMax:
				*((E_GrUiScptPrTyPrgbarValMax*)A_PtrBuf)	=	A_Obj->ValueMaxGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyPrgbarValMax);
				break;
			case	E_GrUiScptPrIdPrgbarVertical:
				*((E_GrUiScptPrTyPrgbarVertical*)A_PtrBuf)	=	A_Obj->IsVerticalType();
				Tv_Result	=	sizeof(E_GrUiScptPrTyPrgbarVertical);
				break;
			case	E_GrUiScptPrIdPrgbarRelCntType:
				*((E_GrUiScptPrTyPrgbarRelCntType*)A_PtrBuf)	=	A_Obj->IsRltLblTypeCnt();
				Tv_Result	=	sizeof(E_GrUiScptPrTyPrgbarRelCntType);
				break;
			case	E_GrUiScptPrIdPrgbarRelLblId:
				*((E_GrUiScptPrTyPrgbarRelLblId*)A_PtrBuf)	=	A_Obj->RltLblIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyPrgbarRelLblId);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValArea( Cls_GrUiDoArea* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
		return	0;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValCdvArea( Cls_GrUiDoCdvArea* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u16		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdCdvAreaSrcIdx0:
			case	E_GrUiScptPrIdCdvAreaSrcIdx1:
			case	E_GrUiScptPrIdCdvAreaSrcIdx2:
			case	E_GrUiScptPrIdCdvAreaSrcIdx3:
			case	E_GrUiScptPrIdCdvAreaSrcIdx4:
			case	E_GrUiScptPrIdCdvAreaSrcIdx5:
			case	E_GrUiScptPrIdCdvAreaSrcIdx6:
			case	E_GrUiScptPrIdCdvAreaSrcIdx7:
			case	E_GrUiScptPrIdCdvAreaSrcIdx8:
			case	E_GrUiScptPrIdCdvAreaSrcIdx9:
			case	E_GrUiScptPrIdCdvAreaSrcIdx10:
			case	E_GrUiScptPrIdCdvAreaSrcIdx11:
			case	E_GrUiScptPrIdCdvAreaSrcIdx12:
			case	E_GrUiScptPrIdCdvAreaSrcIdx13:
			case	E_GrUiScptPrIdCdvAreaSrcIdx14:
			case	E_GrUiScptPrIdCdvAreaSrcIdx15:
			case	E_GrUiScptPrIdCdvAreaSrcIdx16:
			case	E_GrUiScptPrIdCdvAreaSrcIdx17:
			case	E_GrUiScptPrIdCdvAreaSrcIdx18:
			case	E_GrUiScptPrIdCdvAreaSrcIdx19:
			case	E_GrUiScptPrIdCdvAreaSrcIdx20:
			case	E_GrUiScptPrIdCdvAreaSrcIdx21:
			case	E_GrUiScptPrIdCdvAreaSrcIdx22:
			case	E_GrUiScptPrIdCdvAreaSrcIdx23:
			case	E_GrUiScptPrIdCdvAreaSrcIdx24:
			case	E_GrUiScptPrIdCdvAreaSrcIdx25:
			case	E_GrUiScptPrIdCdvAreaSrcIdx26:
			case	E_GrUiScptPrIdCdvAreaSrcIdx27:
			case	E_GrUiScptPrIdCdvAreaSrcIdx28:
			case	E_GrUiScptPrIdCdvAreaSrcIdx29:
			case	E_GrUiScptPrIdCdvAreaSrcIdx30:
			case	E_GrUiScptPrIdCdvAreaSrcIdx31:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdCdvAreaSrcIdx0;
				*((E_GrUiScptPrTyCdvAreaSrcIdx0*)A_PtrBuf)	=	A_Obj->CondSrcIdxGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyCdvAreaSrcIdx0);
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
				*((E_GrUiScptPrTyCdvAreaConType0*)A_PtrBuf)	=	A_Obj->CondTypeGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyCdvAreaConType0);
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
				*((E_GrUiScptPrTyCdvAreaRefVal0*)A_PtrBuf)	=	A_Obj->CondRefGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyCdvAreaRefVal0);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValAcBtn( Cls_GrUiDoAcBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u16		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdAcBtnAcRvs:
				*((E_GrUiScptPrTyAcBtnAcRvs*)A_PtrBuf)	=	A_Obj->IsActRvs();
				Tv_Result	=	sizeof(E_GrUiScptPrTyAcBtnAcRvs);
				break;

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
				*((E_GrUiScptPrTyAcBtnAcObj0*)A_PtrBuf)	=	A_Obj->ActObjGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyAcBtnAcObj0);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValAcSpNum( Cls_GrUiDoAcSpNum* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u16		Tv_Idx;
	// code --------------------
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
				*((E_GrUiScptPrTyAcSpBtnAcObj0*)A_PtrBuf)	=	A_Obj->ActObjGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyAcSpBtnAcObj0);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValAcSpTxt( Cls_GrUiDoAcSpTxt* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u16		Tv_Idx;
	// code --------------------
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
				*((E_GrUiScptPrTyAcSpTxtAcObj0*)A_PtrBuf)	=	A_Obj->ActObjGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyAcSpTxtAcObj0);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValAcSpImg( Cls_GrUiDoAcSpImg* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u16		Tv_Idx;
	// code --------------------
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
				*((E_GrUiScptPrTyAcSpImgAcObj0*)A_PtrBuf)	=	A_Obj->ActObjGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyAcSpImgAcObj0);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValAcSpDgt( Cls_GrUiDoAcSpDgt* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u16		Tv_Idx;
	// code --------------------
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
				*((E_GrUiScptPrTyAcSpDgtAcObj0*)A_PtrBuf)	=	A_Obj->ActObjGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyAcSpDgtAcObj0);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValInpTime( Cls_GrUiDoInpTime* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdInpTimeYearId:
				*((E_GrUiScptPrTyInpTimeYearId*)A_PtrBuf)	=	A_Obj->RelIdYearGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeYearId);
				break;
			case	E_GrUiScptPrIdInpTimeMonId:
				*((E_GrUiScptPrTyInpTimeMonId*)A_PtrBuf)	=	A_Obj->RelIdMonthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeMonId);
				break;
			case	E_GrUiScptPrIdInpTimeDayId:
				*((E_GrUiScptPrTyInpTimeDayId*)A_PtrBuf)	=	A_Obj->RelIdDayGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeDayId);
				break;
			case	E_GrUiScptPrIdInpTimeHourId:
				*((E_GrUiScptPrTyInpTimeHourId*)A_PtrBuf)	=	A_Obj->RelIdHourGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeHourId);
				break;
			case	E_GrUiScptPrIdInpTimeMinId:
				*((E_GrUiScptPrTyInpTimeMinId*)A_PtrBuf)	=	A_Obj->RelIdMinGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeMinId);
				break;
			case	E_GrUiScptPrIdInpTimeSecId:
				*((E_GrUiScptPrTyInpTimeSecId*)A_PtrBuf)	=	A_Obj->RelIdSecGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeSecId);
				break;
			case	E_GrUiScptPrIdInpTimeUnYearId:
				*((E_GrUiScptPrTyInpTimeUnYearId*)A_PtrBuf)	=	A_Obj->RelIdUnitYearGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeUnYearId);
				break;
			case	E_GrUiScptPrIdInpTimeUnMonId:
				*((E_GrUiScptPrTyInpTimeUnMonId*)A_PtrBuf)	=	A_Obj->RelIdUnitMonthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeUnMonId);
				break;
			case	E_GrUiScptPrIdInpTimeUnDayId:
				*((E_GrUiScptPrTyInpTimeUnDayId*)A_PtrBuf)	=	A_Obj->RelIdUnitDayGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeUnDayId);
				break;
			case	E_GrUiScptPrIdInpTimeDiv1Id:
				*((E_GrUiScptPrTyInpTimeDiv1Id*)A_PtrBuf)	=	A_Obj->RelIdDiv1Get();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeDiv1Id);
				break;
			case	E_GrUiScptPrIdInpTimeDiv2Id:
				*((E_GrUiScptPrTyInpTimeDiv2Id*)A_PtrBuf)	=	A_Obj->RelIdDiv2Get();
				Tv_Result	=	sizeof(E_GrUiScptPrTyInpTimeDiv2Id);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetParaVal( Cls_GrUiDoBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u16		Tv_ParaType;
		Def_WstrShort	Tv_StrMsg;
		Def_WstrNum		Tv_StrNum;
	// code --------------------
		Tv_Result		=	0;
		//get para value
		Tv_ParaType	=	E_GrUiScptMaskPrIdType & A_ParaId;
		switch( Tv_ParaType )
		{
			case	E_GrUiScptPrIdBaseObjType:
				Tv_Result	=	GrUiDvlGetPrValBase( A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSkinObjType:
				Tv_Result	=	GrUiDvlGetPrValSkin( (Cls_GrUiDoSkin*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdWinObjType:
				Tv_Result	=	GrUiDvlGetPrValWin( (Cls_GrUiDoWin*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdFrmObjType:
				Tv_Result	=	GrUiDvlGetPrValFrm( (Cls_GrUiDoFrm*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdLblObjType:
				Tv_Result	=	GrUiDvlGetPrValLbl( (Cls_GrUiDoLbl*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdBtnObjType:
				Tv_Result	=	GrUiDvlGetPrValBtn( (Cls_GrUiDoBtn*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdLbxObjType:
				Tv_Result	=	GrUiDvlGetPrValListBox( (Cls_GrUiDoListbox*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSbarObjType:
				Tv_Result	=	GrUiDvlGetPrValSbar( (Cls_GrUiDoSbar*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpbBaseObjType:
				Tv_Result	=	GrUiDvlGetPrValSpinBase( (Cls_GrUiDoSpinBase*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpbNumObjType:
				Tv_Result	=	GrUiDvlGetPrValSpinNum( (Cls_GrUiDoSpinNum*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpbTxtObjType:
				Tv_Result	=	GrUiDvlGetPrValSpinTxt( (Cls_GrUiDoSpinTxt*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpbDgtObjType:
				Tv_Result	=	GrUiDvlGetPrValSpinDgt( (Cls_GrUiDoSpinDgt*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSmpbObjType:
				Tv_Result	=	GrUiDvlGetPrValSmpBtn( (Cls_GrUiDoSmpBtn*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdEditObjType:
				Tv_Result	=	GrUiDvlGetPrValEdit( (Cls_GrUiDoEdit*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSysBtnObjType:
				Tv_Result	=	GrUiDvlGetPrValSysBtn( (Cls_GrUiDoSysBtn*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdCalenObjType:
				Tv_Result	=	GrUiDvlGetPrValCalendar( (Cls_GrUiDoCalendar*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdQuadObjType:
				Tv_Result	=	GrUiDvlGetPrValQuad( (Cls_GrUiDoQuad*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdGrpSelObjType:
				Tv_Result	=	GrUiDvlGetPrValGrpSel( (Cls_GrUiDoGrpSel*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdGrpEdtObjType:
				Tv_Result	=	GrUiDvlGetPrValGrpEdit( (Cls_GrUiDoGrpEdit*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdBitGridObjType:
				Tv_Result	=	GrUiDvlGetPrValBitGrid( (Cls_GrUiDoBitGrid*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdSpnImgObjType:
				Tv_Result	=	GrUiDvlGetPrValSpinImg( (Cls_GrUiDoSpinImg*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdPrgbarObjType:
				Tv_Result	=	GrUiDvlGetPrValPrgsBar( (Cls_GrUiDoPrgsbar*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdInpTimeObjType:
				Tv_Result	=	GrUiDvlGetPrValInpTime( (Cls_GrUiDoInpTime*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAreaObjType:
				Tv_Result	=	GrUiDvlGetPrValArea( (Cls_GrUiDoArea*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdCdvAreaObjType:
				Tv_Result	=	GrUiDvlGetPrValCdvArea( (Cls_GrUiDoCdvArea*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcBtnObjType:
				Tv_Result	=	GrUiDvlGetPrValAcBtn( (Cls_GrUiDoAcBtn*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcSpNumObjType:
				Tv_Result	=	GrUiDvlGetPrValAcSpNum( (Cls_GrUiDoAcSpNum*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcSpTxtObjType:
				Tv_Result	=	GrUiDvlGetPrValAcSpTxt( (Cls_GrUiDoAcSpTxt*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcSpImgObjType:
				Tv_Result	=	GrUiDvlGetPrValAcSpImg( (Cls_GrUiDoAcSpImg*)A_Obj, A_ParaId, A_PtrBuf );
				break;
			case	E_GrUiScptPrIdAcSpDgtObjType:
				Tv_Result	=	GrUiDvlGetPrValAcSpDgt( (Cls_GrUiDoAcSpDgt*)A_Obj, A_ParaId, A_PtrBuf );
				break;
		}
		//debug
		if ( V_IsGrUiDvlParaDbg && (0 != Tv_Result) )
		{
			if ( GrUiDvlGetParaSizeById( A_ParaId ) != Tv_Result )
			{
				//bug message
				GrStrWcopy( Tv_StrMsg, L"Script bug! Parametter Id = " );
				GrStrIntToWstr( Tv_StrNum, A_ParaId );
				GrStrWcat( Tv_StrMsg, Tv_StrNum );
#ifndef LINUX_APP
				MessageBox( NULL, Tv_StrMsg, L"BUG!" , MB_OK | MB_ICONERROR );
#endif
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValBase( Cls_GrUiDoBase* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u8		Tv_Idx;
	// code --------------------
		Tv_Idx	=	0;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdIsEnable:
				*((E_GrUiScptPrTyIsEnable*)A_PtrBuf)	=	A_Obj->IsEnable();
				Tv_Result	=	sizeof(E_GrUiScptPrTyIsEnable);
				break;
			case	E_GrUiScptPrIdFocusMap:
				*((E_GrUiScptPrTyFocusMap*)A_PtrBuf)	=	(E_GrUiScptPrTyFocusMap)A_Obj->FocusMapGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyFocusMap);
				break;
			case	E_GrUiScptPrIdCliMargin:
				A_Obj->ClientMarginGet( ((E_GrUiScptPrTyCliMargin*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyCliMargin);
				break;
			case	E_GrUiScptPrIdUserData0:
				*((E_GrUiScptPrTyUserData0*)A_PtrBuf)	=	A_Obj->UserDataGet( 0 );
				Tv_Result	=	sizeof(E_GrUiScptPrTyUserData0);
				break;
			case	E_GrUiScptPrIdUserData1:
				*((E_GrUiScptPrTyUserData1*)A_PtrBuf)	=	A_Obj->UserDataGet( 1 );
				Tv_Result	=	sizeof(E_GrUiScptPrTyUserData1);
				break;
			case	E_GrUiScptPrIdUserData2:
				*((E_GrUiScptPrTyUserData2*)A_PtrBuf)	=	A_Obj->UserDataGet( 2 );
				Tv_Result	=	sizeof(E_GrUiScptPrTyUserData2);
				break;
			case	E_GrUiScptPrIdUserData3:
				*((E_GrUiScptPrTyUserData3*)A_PtrBuf)	=	A_Obj->UserDataGet( 3 );
				Tv_Result	=	sizeof(E_GrUiScptPrTyUserData3);
				break;
			case	E_GrUiScptPrIdExtPosY:
				*((E_GrUiScptPrTyExtPosY*)A_PtrBuf)	=	A_Obj->PalExtPosyGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyExtPosY);
				break;
			case	E_GrUiScptPrIdExtSizeY:
				*((E_GrUiScptPrTyExtSizeY*)A_PtrBuf)	=	A_Obj->PalExtSizeyGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyExtSizeY);
				break;
			case	E_GrUiScptPrIdRelFcsLblId:
				*((E_GrUiScptPrTyRelFcsLblId*)A_PtrBuf)	=	A_Obj->RelFocusLabelIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyRelFcsLblId);
				break;
			case	E_GrUiScptPrIdRelFcsTxtId:
				*((E_GrUiScptPrTyRelFcsTxtId*)A_PtrBuf)	=	A_Obj->RelFocusTextIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyRelFcsTxtId);
				break;
			case	E_GrUiScptPrIdFocusBlock:
				*((E_GrUiScptPrTyFocusBlock*)A_PtrBuf)	=	A_Obj->IsFocusBlock();
				Tv_Result	=	sizeof(E_GrUiScptPrTyFocusBlock);
				break;
			case	E_GrUiScptPrIdFocusBaseId:
				*((E_GrUiScptPrTyFocusBaseId*)A_PtrBuf)	=	A_Obj->FocusBaseIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyFocusBaseId);
				break;
			case	E_GrUiScptPrIdCndVisMap1:
				Tv_Idx ++;
			case	E_GrUiScptPrIdCndVisMap0:
				*((E_GrUiScptPrTyCndVisMap0*)A_PtrBuf)	=	A_Obj->CondVisibleMapGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyCndVisMap0);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValSkin( Cls_GrUiDoSkin* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdIsBgSprite:
				*((E_GrUiScptPrTyIsBgSprite*)A_PtrBuf)	=	A_Obj->IsBgSpriteMode();
				Tv_Result	=	sizeof(E_GrUiScptPrTyIsBgSprite);
				break;
			case	E_GrUiScptPrIdClrBg:
				*((E_GrUiScptPrTyClrBg*)A_PtrBuf)	=	A_Obj->BgColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyClrBg);
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
				A_Obj->BgSkinGet( Tv_Idx,((E_GrUiScptPrTySkin0*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTySkin0);
				break;
			case	E_GrUiScptPrIdBorderAtb:
				*((E_GrUiScptPrTyBorderAtb*)A_PtrBuf)	=	A_Obj->BorderAtbGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBorderAtb);
				break;
			case	E_GrUiScptPrIdBorderDepth:
				*((E_GrUiScptPrTyBorderDepth*)A_PtrBuf)	=	(E_GrUiScptPrTyBorderDepth)A_Obj->BorderDepthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBorderDepth);
				break;
			case	E_GrUiScptPrIdBorderClrLt:
			case	E_GrUiScptPrIdBorderClrDk:
			case	E_GrUiScptPrIdBorderClrMl:
			case	E_GrUiScptPrIdBorderClrMd:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdBorderClrLt;
				*((E_GrUiScptPrTyBorderClrLt*)A_PtrBuf)	=	A_Obj->BorderColorGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyBorderClrLt);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValWin( Cls_GrUiDoWin* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdIsMoveAble:
				*((E_GrUiScptPrTyIsMoveAble*)A_PtrBuf)	=	A_Obj->IsMoveable();
				Tv_Result	=	sizeof(E_GrUiScptPrTyIsMoveAble);
				break;
			case	E_GrUiScptPrIdCloseKeyType:
				*((E_GrUiScptPrTyCloseKeyType*)A_PtrBuf)	=	(E_GrUiScptPrTyCloseKeyType)A_Obj->CloseKeyTypeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCloseKeyType);
				break;
			case	E_GrUiScptPrIdCloseTime:
				*((E_GrUiScptPrTyCloseTime*)A_PtrBuf)	=	A_Obj->CloseTimeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyCloseTime);
				break;
			case	E_GrUiScptPrIdIsLayerLock:
				*((E_GrUiScptPrTyIsLayerLock*)A_PtrBuf)	=	A_Obj->IsLayerFcsLock();
				Tv_Result	=	sizeof(E_GrUiScptPrTyIsLayerLock);
				break;
			case	E_GrUiScptPrIdIsFocusAble:
				*((E_GrUiScptPrTyIsFocusAble*)A_PtrBuf)	=	A_Obj->IsFocusAble();
				Tv_Result	=	sizeof(E_GrUiScptPrTyIsFocusAble);
				break;
			case	E_GrUiScptPrIdWinRelFrmId:
				*((E_GrUiScptPrTyWinRelFrmId*)A_PtrBuf)	=	A_Obj->RelateFrameIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyWinRelFrmId);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValFrm( Cls_GrUiDoFrm* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
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
				*((E_GrUiScptPrTyFrmPageId0*)A_PtrBuf)	=	A_Obj->FrmPageObjIdGet( (__u8)Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyFrmPageId0);
				break;
			case	E_GrUiScptPrIdFrmViewPage:
				*((E_GrUiScptPrTyFrmViewPage*)A_PtrBuf)	=	A_Obj->FrmPageViewIdxGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyFrmViewPage);
				break;
			case	E_GrUiScptPrIdFrmRtlLblId:
				*((E_GrUiScptPrTyFrmRtlLblId*)A_PtrBuf)	=	A_Obj->FrmRelLabelIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyFrmRtlLblId);
				break;
			case	E_GrUiScptPrIdFrmNoFcsMov:
				*((E_GrUiScptPrTyFrmNoFcsMov*)A_PtrBuf)	=	A_Obj->IsFrmNoMoveFocus();
				Tv_Result	=	sizeof(E_GrUiScptPrTyFrmNoFcsMov);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValListBox( Cls_GrUiDoListbox* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		Tv_Idx	=	0;
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdLbxEditAble:
				*((E_GrUiScptPrTyLbxEditAble*)A_PtrBuf)	=	A_Obj->IsEditAble();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxEditAble);
				break;
			case	E_GrUiScptPrIdLbxEditNum:
				*((E_GrUiScptPrTyLbxEditNum*)A_PtrBuf)	=	A_Obj->IsEditNum();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxEditNum);
				break;
			case	E_GrUiScptPrIdLbxKeyDel:
				*((E_GrUiScptPrTyLbxKeyDel*)A_PtrBuf)	=	A_Obj->IsKeyDelAble();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxKeyDel);
				break;
			case	E_GrUiScptPrIdLbxDelQst:
				*((E_GrUiScptPrTyLbxDelQst*)A_PtrBuf)	=	A_Obj->IsKeyDelQuest();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxDelQst);
				break;
			case	E_GrUiScptPrIdLbxItmHeight:
				*((E_GrUiScptPrTyLbxItmHeight*)A_PtrBuf)	=	A_Obj->ItemHeightGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxItmHeight);
				break;
			case	E_GrUiScptPrIdLbxItmMargin:
				A_Obj->ItemMarginGet( ((E_GrUiScptPrTyLbxItmMargin*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxItmMargin);
				break;
			case	E_GrUiScptPrIdLbxItmBrdDep:
				*((E_GrUiScptPrTyLbxItmBrdDep*)A_PtrBuf)	=	A_Obj->ItemBrdDepthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxItmBrdDep);
				break;
			case	E_GrUiScptPrIdLbxItmBrdAtb:
				*((E_GrUiScptPrTyLbxItmBrdAtb*)A_PtrBuf)	=	A_Obj->ItemBrdAtbGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxItmBrdAtb);
				break;
			case	E_GrUiScptPrIdLbxItmGap:
				*((E_GrUiScptPrTyLbxItmGap*)A_PtrBuf)	=	A_Obj->ItemGapGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxItmGap);
				break;
			case	E_GrUiScptPrIdLbxIconWidth:
				*((E_GrUiScptPrTyLbxIconWidth*)A_PtrBuf)	=	A_Obj->IconWidthGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxIconWidth);
				break;
			case	E_GrUiScptPrIdLbxIconCount:
				*((E_GrUiScptPrTyLbxIconCount*)A_PtrBuf)	=	A_Obj->IconCountGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxIconCount);
				break;
			case	E_GrUiScptPrIdLbxIconPos:
				*((E_GrUiScptPrTyLbxIconPos*)A_PtrBuf)	=	A_Obj->IconPosGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxIconPos);
				break;
			case	E_GrUiScptPrIdLbxIconGap:
				*((E_GrUiScptPrTyLbxIconGap*)A_PtrBuf)	=	A_Obj->IconGapGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxIconGap);
				break;
			case	E_GrUiScptPrIdLbxImgFcsOut:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxImgOnCus:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxImgNormal:
				A_Obj->ItemSkinGet( Tv_Idx, ((E_GrUiScptPrTyLbxImgNormal*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxImgNormal);
				break;
			case	E_GrUiScptPrIdLbxClrFcsOut:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxClrOnCus:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxClrNormal:
				*((E_GrUiScptPrTyLbxClrNormal*)A_PtrBuf)	=	A_Obj->ItemColorGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxClrNormal);
				break;
			case	E_GrUiScptPrIdLbxTclrFcsOut:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxTclrOnCus:
				Tv_Idx ++;
			case	E_GrUiScptPrIdLbxTclrNormal:
				*((E_GrUiScptPrTyLbxTclrNormal*)A_PtrBuf)	=	A_Obj->TextColorGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxTclrNormal);
				break;
			case	E_GrUiScptPrIdLbxBgClrDis:
			case	E_GrUiScptPrIdLbxBgClrNrm:
			case	E_GrUiScptPrIdLbxBgClrFcs:
			case	E_GrUiScptPrIdLbxBgClrCtrl:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdLbxBgClrDis;
				*((E_GrUiScptPrTyLbxBgClrDis*)A_PtrBuf)	=	A_Obj->LbxColorGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxBgClrDis);
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
				A_Obj->IconGet( Tv_Idx, ((E_GrUiScptPrTyLbxIconImg0*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxIconImg0);
				break;
			case	E_GrUiScptPrIdLbxSbarId:
				*((E_GrUiScptPrTyLbxSbarId*)A_PtrBuf)	=	A_Obj->ScrlSbarIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxSbarId);
				break;
			case	E_GrUiScptPrIdLbxTextPara:
				A_Obj->TextParaGet( ((E_GrUiScptPrTyLbxTextPara*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxTextPara);
				break;
			case	E_GrUiScptPrIdLbxKeyFcsCtrl:
				*((E_GrUiScptPrTyLbxKeyFcsCtrl*)A_PtrBuf)	=	A_Obj->IsKeyFocusCtrl();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLbxKeyFcsCtrl);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValBtn( Cls_GrUiDoBtn* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdBtnType:
				*((E_GrUiScptPrTyBtnType*)A_PtrBuf)	=	A_Obj->BtnTypeGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnType);
				break;
			case	E_GrUiScptPrIdBtnCheck:
				*((E_GrUiScptPrTyBtnCheck*)A_PtrBuf)	=	A_Obj->IsBtnChecked();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnCheck);
				break;
			case	E_GrUiScptPrIdBtnImgDisable:
			case	E_GrUiScptPrIdBtnImgNormal:
			case	E_GrUiScptPrIdBtnImgNrmFcs:
			case	E_GrUiScptPrIdBtnImgNrmPush:
			case	E_GrUiScptPrIdBtnImgCheck:
			case	E_GrUiScptPrIdBtnImgChkFcs:
			case	E_GrUiScptPrIdBtnImgChkPush:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdBtnImgDisable;
				A_Obj->BtnStatImgGet( Tv_Idx, ((E_GrUiScptPrTyBtnImgDisable*)A_PtrBuf)	);
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnImgDisable);
				break;
			case	E_GrUiScptPrIdBtnClrDisable:
			case	E_GrUiScptPrIdBtnClrNormal:
			case	E_GrUiScptPrIdBtnClrNrmFcs:
			case	E_GrUiScptPrIdBtnClrNrmPush:
			case	E_GrUiScptPrIdBtnClrCheck:
			case	E_GrUiScptPrIdBtnClrChkFcs:
			case	E_GrUiScptPrIdBtnClrChkPush:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdBtnClrDisable;
				*((E_GrUiScptPrTyBtnClrDisable*)A_PtrBuf)	=	A_Obj->BtnStatClrGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnClrDisable);
				break;
			case	E_GrUiScptPrIdBtnTclrDisable:
			case	E_GrUiScptPrIdBtnTclrNormal:
			case	E_GrUiScptPrIdBtnTclrNrmFcs:
			case	E_GrUiScptPrIdBtnTclrNrmPush:
			case	E_GrUiScptPrIdBtnTclrCheck:
			case	E_GrUiScptPrIdBtnTclrChkFcs:
			case	E_GrUiScptPrIdBtnTclrChkPush:
				Tv_Idx	=	A_ParaId - E_GrUiScptPrIdBtnTclrDisable;
				*((E_GrUiScptPrTyBtnTclrDisable*)A_PtrBuf)	=	A_Obj->BtnTxtClrGet( Tv_Idx );
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnTclrDisable);
				break;
			case	E_GrUiScptPrIdBtnTextId:
				*((E_GrUiScptPrTyBtnTextId*)A_PtrBuf)	=	(E_GrUiScptPrTyBtnTextId)A_Obj->TextIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnTextId);
				break;
			case	E_GrUiScptPrIdBtnTextPara:
				A_Obj->TextParaGet( ((E_GrUiScptPrTyBtnTextPara*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnTextPara);
				break;
			case	E_GrUiScptPrIdBtnEvtHostId:
				*((E_GrUiScptPrTyBtnEvtHostId*)A_PtrBuf)	=	A_Obj->BtnEvtHostIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnEvtHostId);
				break;
			case	E_GrUiScptPrIdBtnImgChkDis:
				A_Obj->BtnStatImgGet( E_GrUiDoBtnImgIdxChkDis, ((E_GrUiScptPrTyBtnImgChkDis*)A_PtrBuf)	);
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnImgChkDis);
				break;
			case	E_GrUiScptPrIdBtnClrChkDis:
				*((E_GrUiScptPrTyBtnClrChkDis*)A_PtrBuf)	=	A_Obj->BtnStatClrGet( E_GrUiDoBtnImgIdxChkDis );
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnClrChkDis);
				break;
			case	E_GrUiScptPrIdBtnTclrChkDis:
				*((E_GrUiScptPrTyBtnTclrChkDis*)A_PtrBuf)	=	A_Obj->BtnTxtClrGet( E_GrUiDoBtnImgIdxChkDis );
				Tv_Result	=	sizeof(E_GrUiScptPrTyBtnTclrChkDis);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetPrValLbl( Cls_GrUiDoLbl* A_Obj, __u16 A_ParaId, void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------

		switch( A_ParaId )
		{
			case	E_GrUiScptPrIdLblTextId:
				*((E_GrUiScptPrTyLblTextId*)A_PtrBuf)	=	A_Obj->TextIdGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLblTextId);
				break;
			case	E_GrUiScptPrIdLblTextColor:
				*((E_GrUiScptPrTyLblTextColor*)A_PtrBuf)	=	A_Obj->TextColorGet();
				Tv_Result	=	sizeof(E_GrUiScptPrTyLblTextColor);
				break;
			case	E_GrUiScptPrIdLblTextPara:
				A_Obj->TextParaGet( ((E_GrUiScptPrTyLblTextPara*)A_PtrBuf) );
				Tv_Result	=	sizeof(E_GrUiScptPrTyLblTextPara);
				break;

			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetSysObjUserIdCnt( BOOL8 A_IsPage )
{
		if ( A_IsPage )
		{
			return		(sizeof(V_GrUiDvlSysPageIdTbl) / sizeof(St_GrUiDvlSysIdDesc) );
		}
		else
		{
			return		(sizeof(V_GrUiDvlSysWinIdTbl) / sizeof(St_GrUiDvlSysIdDesc) );
		}
}
//--------------------------------------------------------------------
Ptr_GrUiDvlSysIdDesc	GrUiDvlGetSysObjUserIdInfo( __u32 A_Idx, BOOL8 A_IsPage )
{
		if ( A_IsPage )
		{
			return	&V_GrUiDvlSysPageIdTbl[A_Idx];
		}
		else
		{
			return	&V_GrUiDvlSysWinIdTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
Ptr_GrUiDvlSysIdDesc	GrUiDvlFindSysObjInfoById( __u16 A_Id )
{
	// local -------------------
		Ptr_GrUiDvlSysIdDesc	Tv_Result;
		__u32		Tv_WkCnt;
		__u32		Tv_WkIdx;
		Ptr_GrUiDvlSysIdDesc	Tv_PtrWk;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//find window area
		Tv_WkCnt	=	GrUiDvlGetSysObjUserIdCnt( FALSE );
		Tv_PtrWk	=	V_GrUiDvlSysWinIdTbl;
		for ( Tv_WkIdx=0;Tv_WkIdx < Tv_WkCnt;Tv_WkIdx++ )
		{
			if ( A_Id == Tv_PtrWk->Id )
			{
				//finded
				Tv_Result	=	Tv_PtrWk;
				break;
			}
			//next
			Tv_PtrWk ++;
		}
		//check finded
		if ( NULL == Tv_Result )
		{
			//find frame area
			Tv_PtrWk	=	V_GrUiDvlSysPageIdTbl;
			Tv_WkCnt	=	GrUiDvlGetSysObjUserIdCnt( TRUE );
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_WkCnt;Tv_WkIdx++ )
			{
				if ( A_Id == Tv_PtrWk->Id )
				{
					//finded
					Tv_Result	=	Tv_PtrWk;
					break;
				}
				//next
				Tv_PtrWk ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	GrUiDvlTypeIdNameGet( __u32 A_TypeId )
{
		return	V_GrUiDvlTypeIdTbl[A_TypeId];
}
//--------------------------------------------------------------------
__u32	GrUiDvlGetParaSizeById( __u16 A_ParaId )
{
	// local -------------------
		__u32	Tv_Result;
		Ptr_GrUiParaDesc	Tv_PtrDesc;
	// code --------------------
		//init
		Tv_Result	=	0;
		//find para
		Tv_PtrDesc	=	GrUiDvlGetParaInfoByPara( A_ParaId );
		if ( NULL != Tv_PtrDesc )
		{
			Tv_Result	=	V_GrUiParaTypeSize[Tv_PtrDesc->Type];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrUiDvlGetValText( void* A_PtrVal, __u32 A_ValType, WCHAR* A_StrRt )
{
	// local -------------------
		__u32		Tv_TmpVal;
		__u32		Tv_ValType;
		Def_WstrNum	Tv_StrNum;
	// code --------------------
		GrStrClear( A_StrRt );
		Tv_ValType	=	A_ValType;
		switch( Tv_ValType )
		{
			case	E_GrUiParaTypeDword:
			case	E_GrUiParaTypeInt:
				Tv_TmpVal	=	*((__u32*)A_PtrVal);
				GrStrIntToWstr( A_StrRt, Tv_TmpVal  );
				break;
			case	E_GrUiParaTypeWord:
			case	E_GrUiParaTypeShort:
				Tv_TmpVal	=	*((__u16*)A_PtrVal);
				GrStrIntToWstr( A_StrRt, Tv_TmpVal  );
				break;
			case	E_GrUiParaTypeByte:
			case	E_GrUiParaTypeChar:
				Tv_TmpVal	=	*((__u8*)A_PtrVal);
				GrStrIntToWstr( A_StrRt, Tv_TmpVal  );
				break;
			case	E_GrUiParaTypeBool8:
				if ( *((BOOL8*)A_PtrVal) )
				{
					GrStrWcopy( A_StrRt, L"true" );
				}
				else
				{
					GrStrWcopy( A_StrRt, L"false" );
				}
				break;
			case	E_GrUiParaTypeByteRect:
				GrStrWcopy( A_StrRt, L"L:" );		
				GrStrIntToWstr( Tv_StrNum, ((Ptr_GrByteRect)A_PtrVal)->left );
				GrStrWcat( A_StrRt, Tv_StrNum );
				GrStrWcat( A_StrRt, L", T:" );
				GrStrIntToWstr( Tv_StrNum, ((Ptr_GrByteRect)A_PtrVal)->top );
				GrStrWcat( A_StrRt, Tv_StrNum );
				GrStrWcat( A_StrRt, L", R:" );
				GrStrIntToWstr( Tv_StrNum, ((Ptr_GrByteRect)A_PtrVal)->right );
				GrStrWcat( A_StrRt, Tv_StrNum );
				GrStrWcat( A_StrRt, L", B:" );
				GrStrIntToWstr( Tv_StrNum, ((Ptr_GrByteRect)A_PtrVal)->bottom );
				GrStrWcat( A_StrRt, Tv_StrNum );
				break;
			case	E_GrUiParaTypeFocusMap:
				GrUiDvlGetValFocusMapText( *((E_GrUiScptPrTyFocusMap*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeColor:
			case	E_GrUiParaTypeTxtClr:
				GrUiDvlGetValColorText( *((E_GrUiScptPrTyClrBg*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeBorderAtb:
				GrUiDvlGetValBorderAtbText( *((E_GrUiScptPrTyBorderAtb*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeTxtPara:
				GrUiDvlGetValTextParaText( ((E_GrUiScptPrTyLblTextPara*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeTxtId:
				GrUiDvlGetValTextIdText( *((__u32*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeCloseKey:
				GrUiDvlGetValCloseKeyText( *((E_GrUiScptPrTyCloseKeyType*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeTimeValMs:
				GrUiDvlGetValTimeMsText( *((E_GrUiScptPrTyCloseTime*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeSkin:
				GrUiDvlGetValSkinText( ((E_GrUiScptPrTySkin0*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeBtnType:
				GrUiDvlGetValBtnTypeText( *((E_GrUiScptPrTyBtnType*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeDirection:
				GrUiDvlGetValDirText( *((E_GrUiScptPrTyLbxIconPos*)A_PtrVal), A_StrRt );
				break;
			case	E_GrUiParaTypeGval:
				GrUiDvlGetValGvalText( *((__u8*)A_PtrVal) , A_StrRt );
				break;
			case	E_GrUiParaTypeCond:
				GrUiDvlGetValCondText( *((__u8*)A_PtrVal) , A_StrRt );
				break;
			case	E_GrUiParaTypeBitMask:
				GrUiDvlGetValBitMaskText( *((__u32*)A_PtrVal) , A_StrRt );
				break;
				
			default:
#ifndef LINUX_APP
				MessageBox( NULL, L"[GrUiDvlGetValText] Bad parametter id", L"error", MB_OK );
#endif
				break;
		}
}
//--------------------------------------------------------------------
void	GrUiDvlGetValTextIdText( __u32 A_Val, WCHAR* A_StrRt )
{
	// local -------------------
		Def_WstrNum	Tv_StrNum;
	// code --------------------
		switch( A_Val )
		{
			case	E_GrUiTxtRosMakeDate:
				GrStrWcopy( A_StrRt, L"ProjectSaveTime" );
				break;
			case	E_GrUiTxtIdLang0:
			case	E_GrUiTxtIdLang1:
			case	E_GrUiTxtIdLang2:
			case	E_GrUiTxtIdLang3:
			case	E_GrUiTxtIdLang4:
			case	E_GrUiTxtIdLang5:
			case	E_GrUiTxtIdLang6:
			case	E_GrUiTxtIdLang7:
			case	E_GrUiTxtIdLang8:
			case	E_GrUiTxtIdLang9:
			case	E_GrUiTxtIdLang10:
			case	E_GrUiTxtIdLang11:
			case	E_GrUiTxtIdLang12:
			case	E_GrUiTxtIdLang13:
			case	E_GrUiTxtIdLang14:
			case	E_GrUiTxtIdLang15:
				GrStrWcopy( A_StrRt, L"language" );
				GrStrIntToWstr( Tv_StrNum, (__s32)A_Val - E_GrUiTxtIdLang0 );
				GrStrWcat( A_StrRt, Tv_StrNum );
				break;
			case	E_GrUiTxtIdMonth1:
			case	E_GrUiTxtIdMonth2:
			case	E_GrUiTxtIdMonth3:
			case	E_GrUiTxtIdMonth4:
			case	E_GrUiTxtIdMonth5:
			case	E_GrUiTxtIdMonth6:
			case	E_GrUiTxtIdMonth7:
			case	E_GrUiTxtIdMonth8:
			case	E_GrUiTxtIdMonth9:
			case	E_GrUiTxtIdMonth10:
			case	E_GrUiTxtIdMonth11:
			case	E_GrUiTxtIdMonth12:
				GrStrWcopy( A_StrRt, L"month - " );
				GrStrIntToWstr( Tv_StrNum, (__s32)A_Val - E_GrUiTxtIdMonth1 + 1 );
				GrStrWcat( A_StrRt, Tv_StrNum );
				break;
			case	E_GrUiTxtIdWeekSun:
				GrStrWcopy( A_StrRt, L"week - sun" );
				break;
			case	E_GrUiTxtIdWeekMon:
				GrStrWcopy( A_StrRt, L"week - mon" );
				break;
			case	E_GrUiTxtIdWeekTue:
				GrStrWcopy( A_StrRt, L"week - tue" );
				break;
			case	E_GrUiTxtIdWeekWed:
				GrStrWcopy( A_StrRt, L"week - wed" );
				break;
			case	E_GrUiTxtIdWeekThu:
				GrStrWcopy( A_StrRt, L"week - thu" );
				break;
			case	E_GrUiTxtIdWeekFri:
				GrStrWcopy( A_StrRt, L"week - fri" );
				break;
			case	E_GrUiTxtIdWeekSat:
				GrStrWcopy( A_StrRt, L"week - sat" );
				break;
			case	E_GrUiTxtIdYear:
				GrStrWcopy( A_StrRt, L"Year" );
				break;
			case	E_GrUiTxtIdMonth:
				GrStrWcopy( A_StrRt, L"Month" );
				break;
			case	E_GrUiTxtIdDay:
				GrStrWcopy( A_StrRt, L"Day" );
				break;
			case	E_GrUiTxtIdHour:
				GrStrWcopy( A_StrRt, L"Hour" );
				break;
			case	E_GrUiTxtIdMin:
				GrStrWcopy( A_StrRt, L"Miniute" );
				break;
			case	E_GrUiTxtIdSec:
				GrStrWcopy( A_StrRt, L"second" );
				break;
			case	E_GrUiTxtIdMsec:
				GrStrWcopy( A_StrRt, L"millisecond" );
				break;
			case	E_GrUiTxtIdAm:
				GrStrWcopy( A_StrRt, L"AM" );
				break;
			case	E_GrUiTxtIdPm:
				GrStrWcopy( A_StrRt, L"PM" );
				break;
			default:
				GrStrWcopy( A_StrRt, L"[" );
				GrStrIntToWstr( Tv_StrNum, A_Val  );
				GrStrWcat( A_StrRt,Tv_StrNum );
				GrStrWcat( A_StrRt, L"]" );
				break;
		}
}
//--------------------------------------------------------------------
void	GrUiDvlGetValSkinText( Ptr_GrUiDoSkinImg A_PtrVal, WCHAR* A_StrRt )
{
	// local -------------------
		__u8		Tv_Pos;
		Def_WstrNum	Tv_StrNum;
	// code --------------------
		//image id
		if ( E_GrUiImgIdNone == A_PtrVal->Id )
		{
			GrStrWcopy( A_StrRt, L"[None" );	
		}
		else
		{
			GrStrWcopy( A_StrRt, L"[" );
			GrStrIntToWstr( Tv_StrNum, A_PtrVal->Id );
			GrStrWcat( A_StrRt, Tv_StrNum );
		}
		GrStrWcat( A_StrRt, L"]" );	
		//position
		Tv_Pos	=	E_GrUiDoSkinPosValMask & A_PtrVal->Atb;
		GrStrWcat( A_StrRt, V_GrUiDvlSkinPosStr[Tv_Pos] );
		//sprite
		if ( 0 != (E_GrUiDoSkibAtbSprite & A_PtrVal->Atb) )
		{
			GrStrWcat( A_StrRt, L" [S]" );
		}
}
//--------------------------------------------------------------------
void	GrUiDvlGetValFocusMapText( __u8 A_Map, WCHAR* A_StrRt )
{
	// local -------------------
	// code --------------------
		GrStrClear( A_StrRt );
		//check
		if ( 0 != ( E_GrUiDoFocusMapUp & A_Map ) )
		{
			if ( 0 != ( (E_GrUiDoFocusMapUp << E_GrUiDoFindWidePos) & A_Map ) )
			{
				GrStrWcat( A_StrRt, L":UW:" );
			}
			else
			{
				GrStrWcat( A_StrRt, L":U:" );
			}
		}
		if ( 0 != ( E_GrUiDoFocusMapDown & A_Map ) )
		{
			if ( 0 != ( (E_GrUiDoFocusMapDown << E_GrUiDoFindWidePos) & A_Map ) )
			{
				GrStrWcat( A_StrRt, L":DW:" );
			}
			else
			{
				GrStrWcat( A_StrRt, L":D:" );
			}
		}
		if ( 0 != ( E_GrUiDoFocusMapLeft & A_Map ) )
		{
			if ( 0 != ( (E_GrUiDoFocusMapLeft << E_GrUiDoFindWidePos) & A_Map ) )
			{
				GrStrWcat( A_StrRt, L":LW:" );
			}
			else
			{
				GrStrWcat( A_StrRt, L":L:" );
			}
		}
		if ( 0 != ( E_GrUiDoFocusMapRight & A_Map ) )
		{
			if ( 0 != ( (E_GrUiDoFocusMapRight << E_GrUiDoFindWidePos) & A_Map ) )
			{
				GrStrWcat( A_StrRt, L":RW:" );
			}
			else
			{
				GrStrWcat( A_StrRt, L":R:" );
			}
		}
}
//--------------------------------------------------------------------
void	GrUiDvlGetValColorText( __u32 A_Val, WCHAR* A_StrRt )
{
	// local -------------------
	// code --------------------
		if ( 0 == A_Val )
		{
			GrStrWcopy( A_StrRt, L"[SPRITE]" );
		}
		else
		{
			GrStrDwordToHexWstr( A_StrRt, A_Val, 8, TRUE );
			//check alpha
			if ( (E_GrFccUYVYF == V_GrUiDvlOsdFcc) && !V_IsGrUiDvlOsdPlt )
			{
				if ( 0 != (0x01000100 & A_Val) )
				{
					GrStrWcat( A_StrRt, L"[A]" );
				}
			}
		}
}
//--------------------------------------------------------------------
void	GrUiDvlGetValCloseKeyText( __u32 A_Val, WCHAR* A_StrRt )
{
		GrStrWcopy( A_StrRt, V_GrUiDvlCloseKeyStr[A_Val] );		
}
//--------------------------------------------------------------------
void	GrUiDvlGetValTimeMsText( __u32 A_Val, WCHAR* A_StrRt )
{
		if ( 0 == A_Val )
		{
			GrStrWcopy( A_StrRt, L"[not use]" );
		}
		else
		{
			GrStrIntToWstr( A_StrRt, A_Val  );
			GrStrWcat( A_StrRt, L" ms" );
		}
}
//--------------------------------------------------------------------
void	GrUiDvlGetValGvalText( __u8 A_Val, WCHAR* A_StrRt )
{
		GrStrClear( A_StrRt );
		if ( 16 > A_Val )
		{
			GrStrWcopy( A_StrRt, V_GrUiDvlStrGvalTbl[A_Val] );
		}
}
//--------------------------------------------------------------------
void	GrUiDvlGetValCondText( __u8 A_Val, WCHAR* A_StrRt )
{
		switch( A_Val )
		{
			case	0:
				GrStrWcopy( A_StrRt, L"none" );
				break;
			case	1:
				GrStrWcopy( A_StrRt, L"==" );
				break;
			case	2:
				GrStrWcopy( A_StrRt, L"!=" );
				break;
			case	3:
				GrStrWcopy( A_StrRt, L">" );
				break;
			case	4:
				GrStrWcopy( A_StrRt, L"<" );
				break;
			case	5:
				GrStrWcopy( A_StrRt, L">=" );
				break;
			case	6:
				GrStrWcopy( A_StrRt, L"<=" );
				break;
			default:
				GrStrClear(A_StrRt);
#ifndef LINUX_APP
				MessageBox( NULL, L"ERROR!", L"Bad condition value!", MB_OK );
#endif
				break;
		}
}
//--------------------------------------------------------------------
WCHAR*	GrUiDvlSysTxtNameGet( __u32 A_TxtId )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u32		Tv_SysCnt;
	// code --------------------
		//init
		Tv_Result		=	NULL;
		Tv_SysCnt	=		GrUiDvlSysTxtCnt();
		//check range
		if ( (E_GrUiTxtIdSysStart <= A_TxtId) && ((E_GrUiTxtIdSysStart + Tv_SysCnt) > A_TxtId) )
		{
			Tv_Result	=	V_GrUiDvlTxtNameTbl[A_TxtId-E_GrUiTxtIdSysStart];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiDvlSysTxtCnt( void )
{
		return	sizeof(V_GrUiDvlTxtNameTbl) / sizeof(Def_WstrName);
}
//--------------------------------------------------------------------
void	GrUiDvlGetValBtnTypeText( __u8 A_Val, WCHAR* A_StrRt )
{
		GrStrWcopy( A_StrRt, V_GrUiDvlBtnTypeName[A_Val] );		
}
//--------------------------------------------------------------------
void	GrUiDvlOsdFccSet( __u32 A_Fcc, BOOL8 A_IsPltMode )
{
		V_GrUiDvlOsdFcc		=	A_Fcc;
		V_IsGrUiDvlOsdPlt	=	A_IsPltMode;
}
//--------------------------------------------------------------------
void	GrUiDvlGetValBorderAtbText( __u8 A_Atb, WCHAR* A_StrRt )
{
	// local -------------------
	// code --------------------
		GrStrClear( A_StrRt );
		if ( 0 != (E_GrGdibBrdAtbFlat & A_Atb) )
		{
			//flat
			GrStrWcat( A_StrRt, L"[FLAT]" );
		}
		if ( 0 != (E_GrGdibBrdAtbInner & A_Atb) )
		{
			//flat
			GrStrWcat( A_StrRt, L"[INNER]" );
		}
}
//--------------------------------------------------------------------
void	GrUiDvlGetValTextParaText( Ptr_GrUiDoTextPara A_PtrPara, WCHAR* A_StrRt )
{
	// local -------------------
		Def_WstrNum	Tv_StrNum;
		__u16		Tv_Atb;
	// code --------------------
		//fontid
		GrStrIntToWstr( A_StrRt, (__s32)A_PtrPara->FontId );
		//shadow color
		GrStrWcat( A_StrRt,L"[" );
		GrStrDwordToHexWstr( Tv_StrNum, A_PtrPara->ClrShadow, 8, TRUE );
		GrStrWcat( A_StrRt,Tv_StrNum );
		GrStrWcat( A_StrRt,L"][" );
		//attribute
		if ( 0 != (E_GrUiDoTextAtbWidthDraw & A_PtrPara->Atb) )
		{
			GrStrWcat( A_StrRt,L"W" );
		}
		if ( 0 != (E_GrUiDoTextAtbDrawShadow & A_PtrPara->Atb) )
		{
			GrStrWcat( A_StrRt,L"S" );
		}
		if ( 0 != (E_GrUiDoTextAtbMultiLine & A_PtrPara->Atb) )
		{
			GrStrWcat( A_StrRt,L"M" );
		}
		if ( 0 != (E_GrUiDoTextAtbWordwarp & A_PtrPara->Atb) )
		{
			GrStrWcat( A_StrRt,L"O" );
		}
		if ( 0 != (E_GrUiDoTextAtbPasswd & A_PtrPara->Atb) )
		{
			GrStrWcat( A_StrRt,L"P" );
		}
		//align horizontal
		Tv_Atb	=	A_PtrPara->Atb & E_GrUiDoTextAtbAlignHzMask;
		switch( Tv_Atb )
		{
			case	E_GrUiDoTextAtbAlignHzCenter:
				GrStrWcat( A_StrRt, L"C" );
				break;
			case	E_GrUiDoTextAtbAlignHzLeft:
				GrStrWcat( A_StrRt, L"L" );
				break;
			case	E_GrUiDoTextAtbAlignHzRight:
				GrStrWcat( A_StrRt, L"R" );
				break;
		}
		//align vertical
		Tv_Atb	=	A_PtrPara->Atb & E_GrUiDoTextAtbAlignVtMask;
		switch( Tv_Atb )
		{
		case	E_GrUiDoTextAtbAlignVtCenter:
			GrStrWcat( A_StrRt, L"C" );
			break;
		case	E_GrUiDoTextAtbAlignVtTop:
			GrStrWcat( A_StrRt, L"T" );
			break;
		case	E_GrUiDoTextAtbAlignVtBottom:
			GrStrWcat( A_StrRt, L"B" );
			break;
		}
		GrStrWcat( A_StrRt,L"][" );
		//pitch gap
		GrStrIntToWstr( Tv_StrNum, (__s32)A_PtrPara->PitchGap );
		GrStrWcat( A_StrRt,Tv_StrNum );
		GrStrWcat( A_StrRt,L"][" );
		//line gap
		GrStrIntToWstr( Tv_StrNum, (__s32)A_PtrPara->LineGap );
		GrStrWcat( A_StrRt,Tv_StrNum );
		GrStrWcat( A_StrRt,L"]" );
}
//--------------------------------------------------------------------
void	GrUiDvlGetValDirText( __u8 A_Val, WCHAR* A_StrRt )
{
	// local -------------------
	// code --------------------
		GrStrWcopy( A_StrRt, V_GrUiDvlStrDirTbl[A_Val] );
}
//--------------------------------------------------------------------
void	GrUiDvlGetValBitMaskText( __u32 A_Val, WCHAR* A_StrRt )
{
	// local -------------------
		__s32	Tv_WkIdx;
		__u32	Tv_Mask;
		WCHAR*	Tv_StrWk;
		BOOL8	Tv_IsComa;
	// code --------------------
		GrStrClear( A_StrRt );
		Tv_StrWk	=	A_StrRt;
		Tv_Mask		=	1;
		Tv_IsComa	=	FALSE;
		for ( Tv_WkIdx=0;Tv_WkIdx < 32;Tv_WkIdx++ )
		{
			if ( 0 != (Tv_Mask & A_Val) )
			{
				if ( Tv_IsComa )
				{
					Tv_StrWk	=	GrStrWcat( Tv_StrWk, L"," );
				}
				GrStrIntToWstr( Tv_StrWk, Tv_WkIdx );

				Tv_IsComa	=	TRUE;
			}
			Tv_Mask	=	Tv_Mask << 1;
		}
}
//--------------------------------------------------------------------

