/*
		platform label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoQuad.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include	<P3/GrP3Cfg.h>
#include <Dvr/GrDvrBase.h>
#include <P3/GrP3Bios.h>
#include <GrDebug.h>

#ifdef GR_P3_EDITOR
#include	<P3/Dvl/GrP3DvlBase.h>
#include	<P3/Dvl/GrP3DvlMng.h>
#endif

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP3Mng*	V_GrP3Mng;

#ifdef GR_P3_EDITOR

#define E_GrP3DoQuadParaCnt		105

St_GrP3DvlDoPrInfoData	V_GrP3DoQuadParaInfo[E_GrP3DoQuadParaCnt]	=	
{
	{	L"Rec Icon Pnt",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntEvtTbl[E_GrP3DoQuadEvtRec],NULL,			L"Paint record icon"	},
	{	L"Rec Area left",				TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtRec],NULL,			L"record icon area left"	},
	{	L"Rec Area left val",		FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtRec].left,NULL,			L"record icon area left value"	},
	{	L"Rec Area top",				TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtRec],NULL,			L"record icon area top"	},
	{	L"Rec Area top val",		FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtRec].top,NULL,			L"record icon area top value"	},
	{	L"Rec Area right",			TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtRec],NULL,			L"record icon area right"	},
	{	L"Rec Area right val",	FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtRec].right,NULL,			L"record icon area right value"	},
	{	L"Rec Area bottom",			TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtRec],NULL,			L"record icon area bottom"	},
	{	L"Rec Area bottom val",	FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtRec].bottom,NULL,			L"record icon area bottom value"	},

	{	L"motion Icon Pnt",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntEvtTbl[E_GrP3DoQuadEvtMotion],NULL,			L"Paint motion icon"	},
	{	L"motion Area left",		TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtMotion],NULL,			L"motion icon area left"	},
	{	L"motion Area left val",FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtMotion].left,NULL,			L"motion icon area left value"	},
	{	L"motion Area top",			TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtMotion],NULL,			L"motion icon area top"	},
	{	L"motion Area top val",	FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtMotion].top,NULL,			L"motion icon area top value"	},
	{	L"motion Area right",		TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtMotion],NULL,			L"motion icon area right"	},
	{	L"motion Area right val",	FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtMotion].right,NULL,			L"motion icon area right value"	},
	{	L"motion Area bottom",	TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtMotion],NULL,			L"motion icon area bottom"	},
	{	L"motion Area bottom val",	FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtMotion].bottom,NULL,			L"motion icon area bottom value"	},

	{	L"sensor Icon Pnt",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntEvtTbl[E_GrP3DoQuadEvtSensor],NULL,			L"Paint sensor icon"	},
	{	L"sensor Area left",		TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtSensor],NULL,			L"sensor icon area left"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtSensor].left,NULL,			L""	},
	{	L"sensor Area top",			TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtSensor],NULL,			L"sensor icon area top"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtSensor].top,NULL,			L""	},
	{	L"sensor Area right",		TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtSensor],NULL,			L"sensor icon area right"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtSensor].right,NULL,			L""	},
	{	L"sensor Area bottom",	TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtBssTbl[E_GrP3DoQuadEvtSensor],NULL,			L"sensor icon area bottom"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->EvtPosTbl[E_GrP3DoQuadEvtSensor].bottom,NULL,			L""	},

	{	L"covert Icon Pnt",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntCorvert,NULL,				L"Paint sensor icon"	},
	{	L"covert Area left",		TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->CovertBss,NULL,					L"sensor icon area left"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->CovertPos.left,NULL,		L""	},
	{	L"covert Area top",			TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->CovertBss,NULL,					L"sensor icon area top"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->CovertPos.top,NULL,			L""	},
	{	L"covert Area right",		TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->CovertBss,NULL,					L"sensor icon area right"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->CovertPos.right,NULL,		L""	},
	{	L"covert Area bottom",	TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->CovertBss,NULL,					L"sensor icon area bottom"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->CovertPos.bottom,NULL,	L""	},

	{	L"vloss Icon Pnt",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntVloss,NULL,					L"Paint vloss icon"	},
	{	L"vloss Area left",			TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->VlossBss,NULL,					L"vloss icon area left"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->VlossPos.left,NULL,			L""	},
	{	L"vloss Area top",			TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->VlossBss,NULL,					L"vloss icon area top"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->VlossPos.top,NULL,			L""	},
	{	L"vloss Area right",		TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->VlossBss,NULL,					L"vloss icon area right"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->VlossPos.right,NULL,		L""	},
	{	L"vloss Area bottom",		TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->VlossBss,NULL,					L"vloss icon area bottom"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->VlossPos.bottom,NULL,		L""	},

	{	L"off Icon Pnt",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntChOff,NULL,					L"Paint off icon"	},
	{	L"off Area left",				TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ChOffBss,NULL,					L"off icon area left"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ChOffPos.left,NULL,			L""	},
	{	L"off Area top",				TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ChOffBss,NULL,					L"off icon area top"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ChOffPos.top,NULL,			L""	},
	{	L"off Area right",			TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ChOffBss,NULL,					L"off icon area right"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ChOffPos.right,NULL,		L""	},
	{	L"off Area bottom",			TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ChOffBss,NULL,					L"off icon area bottom"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ChOffPos.bottom,NULL,		L""	},

	{	L"title Area left",			TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->TitleBss,NULL,					L"title icon area left"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->TItlePos.left,NULL,			L""	},
	{	L"title Area top",			TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->TitleBss,NULL,					L"title icon area top"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->TItlePos.top,NULL,			L""	},
	{	L"title Area right",		TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->TitleBss,NULL,					L"title icon area right"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->TItlePos.right,NULL,		L""	},
	{	L"title Area bottom",		TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->TitleBss,NULL,					L"title icon area bottom"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->TItlePos.bottom,NULL,		L""	},

	{	L"title 0 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[0],NULL,		L"Paint title 0"	},
	{	L"title 1 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[1],NULL,		L"Paint title 1"	},
	{	L"title 2 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[2],NULL,		L"Paint title 2"	},
	{	L"title 3 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[3],NULL,		L"Paint title 3"	},
	{	L"title 4 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[4],NULL,		L"Paint title 4"	},
	{	L"title 5 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[5],NULL,		L"Paint title 5"	},
	{	L"title 6 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[6],NULL,		L"Paint title 6"	},
	{	L"title 7 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[7],NULL,		L"Paint title 7"	},
	{	L"title 8 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[8],NULL,		L"Paint title 8"	},
	{	L"title 9 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[9],NULL,		L"Paint title 9"	},
	{	L"title 10 Pnt",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[10],NULL,		L"Paint title 10"	},
	{	L"title 11 Pnt",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[11],NULL,		L"Paint title 11"	},
	{	L"title 12 Pnt",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[12],NULL,		L"Paint title 12"	},
	{	L"title 13 Pnt",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[13],NULL,		L"Paint title 13"	},
	{	L"title 14 Pnt",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[14],NULL,		L"Paint title 14"	},
	{	L"title 15 Pnt",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntTitleTbl[15],NULL,		L"Paint title 15"	},

	{	L"cont Area left",			TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ContBss,NULL,						L"cont icon area left"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ContPos.left,NULL,			L""	},
	{	L"cont Area top",				TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ContBss,NULL,						L"cont icon area top"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ContPos.top,NULL,				L""	},
	{	L"cont Area right",			TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ContBss,NULL,						L"cont icon area right"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ContPos.right,NULL,			L""	},
	{	L"cont Area bottom",		TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ContBss,NULL,						L"cont icon area bottom"	},
	{	L"",										FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ContPos.bottom,NULL,		L""	},

	{	L"cont 0 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[0],NULL,		L"Paint contents 0"	},
	{	L"cont 1 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[1],NULL,		L"Paint contents 1"	},
	{	L"cont 2 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[2],NULL,		L"Paint contents 2"	},
	{	L"cont 3 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[3],NULL,		L"Paint contents 3"	},
	{	L"cont 4 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[4],NULL,		L"Paint contents 4"	},
	{	L"cont 5 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[5],NULL,		L"Paint contents 5"	},
	{	L"cont 6 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[6],NULL,		L"Paint contents 6"	},
	{	L"cont 7 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[7],NULL,		L"Paint contents 7"	},
	{	L"cont 8 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[8],NULL,		L"Paint contents 8"	},
	{	L"cont 9 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[9],NULL,		L"Paint contents 9"	},
	{	L"cont 10 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[10],NULL,	L"Paint contents 10"	},
	{	L"cont 11 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[11],NULL,	L"Paint contents 11"	},
	{	L"cont 12 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[12],NULL,	L"Paint contents 12"	},
	{	L"cont 13 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[13],NULL,	L"Paint contents 13"	},
	{	L"cont 14 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[14],NULL,	L"Paint contents 14"	},
	{	L"cont 15 Pnt",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoQuadPara)0)->PntContTbl[15],NULL,	L"Paint contents 15"	},

	{	L"Grid Width",					TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->GridWidth,NULL,				L"grid width"	},
	{	L"Grid Color",					TRUE,		E_GrP3DvlParaTypeColor,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->ClrGrid,NULL,					L"Paint sensor icon"	},

	{	L"IsPlayIcon",					TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoQuadPara)0)->IsPlayIcon,NULL,			L"show event icon when playing"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoQuad::Cls_GrP3DoQuad( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassQuad;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoQuad) + sizeof(St_GrP3ScptParaHd);

		m_IsKeyAble	=	TRUE;		// key able
		m_IsMouseAble	=	TRUE;

		m_KeyChCmbTime	=	0;
		
		GrDumyZeroMem( &m_ParaDoQuad, sizeof(m_ParaDoQuad) );
		//m_ParaDoQuad.PntBg		=	E_GrP3ScptPstIdDflt;

		GrDumyZeroMem( m_DispAtbTbl, sizeof(m_DispAtbTbl) );

		m_ViewChTbl	=	NULL;
		m_MgnHz			=	0;
		m_MgnVt			=	0;
		m_RcCli			=	*A_PtrRect;

		m_IsGridShow	=	TRUE;
		m_IsContShow	=	FALSE;
		m_IsTitleShow	=	TRUE;
		m_IsIconShow	=	TRUE;

		m_OldQdMode		=	E_GrDvrQuadMode1;

		// setup GVS
		m_PtrGvs			=	V_GrP3Mng->GvsPtrGet( );

		// cam map
		m_PtrStpCamUse	=	(__u16*)V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsCamUseMap );

		m_VdoChCnt		=	V_GrP3Mng->VdoMaxChCntGet();

		LcLivePlayUpdt();
		LcQuadStatUpdt();
		LcSetupUpdt();
}
//--------------------------------------------------------------------
Cls_GrP3DoQuad::~Cls_GrP3DoQuad( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassQuad;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoQuad);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoQuad, sizeof(m_ParaDoQuad) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoQuad)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoQuad, sizeof(m_ParaDoQuad) , m_PtrScpt , E_GrP3DoClassQuad );

		LcQuadStatUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoQuad::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		if ( !Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr ) )
		{
			switch ( A_Cmd )
			{
				case E_GrP3UpiSubShowGet:
					A_PtrInt[0]	=	LcCmdSubShowGet( A_PtrInt[1] );
					break;
				case E_GrP3UpiSubShowSet:
					LcCmdSubShowSet( A_PtrInt[1], A_PtrInt[2] );
					break;
				default:
					Tv_Result	=	FALSE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcPaintGrid( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		if ( m_IsGridShow )
		{
			if ( 0 != m_GridCnt )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < m_GridCnt;Tv_WkIdx++ )
				{
					PaintFillClrPush( m_ParaDoQuad.ClrGrid, &m_GridTbl[Tv_WkIdx] );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcPaintView( void )
{
	// local -------------------
		St_GrRect	Tv_RcVw;
		St_GrRect	Tv_RcIc;
		__u8	Tv_VwIdx;
		__u8	Tv_EvtIdx;
		__u8	Tv_EvtMsk;
		__u16	Tv_MaskCh;
	// code --------------------
		// view
		for ( Tv_VwIdx=0;Tv_VwIdx < m_ViewCnt;Tv_VwIdx++ )
		{
			// init
			m_PntVwCh	=	m_ViewChTbl[Tv_VwIdx];
			Tv_MaskCh	=	1 << m_PntVwCh;
			// calculate icon rect
			Tv_RcVw.left	=	m_ViewRcTbl[Tv_VwIdx].left;
			if ( Tv_RcVw.left < m_RcCli.left )
			{
				Tv_RcVw.left	=	m_RcCli.left;
			}
			Tv_RcVw.top		=	m_ViewRcTbl[Tv_VwIdx].top;
			if ( Tv_RcVw.top < m_RcCli.top )
			{
				Tv_RcVw.top	=	m_RcCli.top;
			}
			Tv_RcVw.right	=	m_ViewRcTbl[Tv_VwIdx].right;
			if ( Tv_RcVw.right > m_RcCli.right )
			{
				Tv_RcVw.right	=	m_RcCli.right;
			}
			Tv_RcVw.bottom	=	m_ViewRcTbl[Tv_VwIdx].bottom;
			if ( Tv_RcVw.bottom > m_RcCli.bottom )
			{
				Tv_RcVw.bottom	=	m_RcCli.bottom;
			}
			// event icon
			if ( m_IsIconShow && ( (!m_IsPlayMode) || m_ParaDoQuad.IsPlayIcon ) )
			{
				Tv_EvtMsk	=	1;
				for ( Tv_EvtIdx=0;Tv_EvtIdx < E_GrP3DoQuadEvtCnt;Tv_EvtIdx++ )
				{
					// check exist event
					if ( 0 != (Tv_EvtMsk & m_DispAtbTbl[m_PntVwCh]) )
					{
						if ( BasisRectCalc( &Tv_RcVw, m_ParaDoQuad.EvtBssTbl[Tv_EvtIdx], 
							m_ParaDoQuad.EvtPosTbl[Tv_EvtIdx].left, m_ParaDoQuad.EvtPosTbl[Tv_EvtIdx].top, 
							m_ParaDoQuad.EvtPosTbl[Tv_EvtIdx].right, m_ParaDoQuad.EvtPosTbl[Tv_EvtIdx].bottom,
							&Tv_RcIc ) )
						{
							// exist paint code
							PaintScriptPush( m_ParaDoQuad.PntEvtTbl[Tv_EvtIdx], &Tv_RcIc );
						}
					}
					// next
					Tv_EvtMsk	=	Tv_EvtMsk << 1;
				}
			}
			// channel title
			if ( m_IsTitleShow )
			{
				if ( BasisRectCalc( &Tv_RcVw, m_ParaDoQuad.TitleBss, 
					m_ParaDoQuad.TItlePos.left, m_ParaDoQuad.TItlePos.top, m_ParaDoQuad.TItlePos.right, m_ParaDoQuad.TItlePos.bottom,
					&Tv_RcIc ) )
				{
					// exist paint code
					PaintScriptPush( m_ParaDoQuad.PntTitleTbl[m_PntVwCh], &Tv_RcIc );
				}
			}
			// contents
			if ( m_IsContShow )
			{
				if ( BasisRectCalc( &Tv_RcVw, m_ParaDoQuad.ContBss, 
					m_ParaDoQuad.ContPos.left, m_ParaDoQuad.ContPos.top, m_ParaDoQuad.ContPos.right, m_ParaDoQuad.ContPos.bottom,
					&Tv_RcIc ) )
				{
					// exist paint code
					PaintScriptPush( m_ParaDoQuad.PntContTbl[m_PntVwCh], &Tv_RcIc );
				}
			}
			// covert , video loss, not exist
			if ( !m_IsPlayMode )
			{
				// check not use
				if ( 0 == (Tv_MaskCh & (*m_PtrStpCamUse)) )
				{
					if ( BasisRectCalc( &Tv_RcVw, m_ParaDoQuad.ChOffBss, 
						m_ParaDoQuad.ChOffPos.left, m_ParaDoQuad.ChOffPos.top, m_ParaDoQuad.ChOffPos.right, m_ParaDoQuad.ChOffPos.bottom,
						&Tv_RcIc ) )
					{
						// exist paint code
						PaintScriptPush( m_ParaDoQuad.PntChOff, &Tv_RcIc );
					}
				}
				else if ( 0 != (Tv_MaskCh & ((Ptr_GrP3MngGvs)m_PtrGvs)->CovertMap) )
				{
					// covert
					if ( BasisRectCalc( &Tv_RcVw, m_ParaDoQuad.CovertBss, 
						m_ParaDoQuad.CovertPos.left, m_ParaDoQuad.CovertPos.top, m_ParaDoQuad.CovertPos.right, m_ParaDoQuad.CovertPos.bottom,
						&Tv_RcIc ) )
					{
						// exist paint code
						PaintScriptPush( m_ParaDoQuad.PntCorvert, &Tv_RcIc );
					}
				}
				else if ( 0 != (Tv_MaskCh & ((Ptr_GrP3MngGvs)m_PtrGvs)->VlossMap ) )
				{
					// no signal
					if ( BasisRectCalc( &Tv_RcVw, m_ParaDoQuad.VlossBss, 
						m_ParaDoQuad.VlossPos.left, m_ParaDoQuad.VlossPos.top, m_ParaDoQuad.VlossPos.right, m_ParaDoQuad.VlossPos.bottom,
						&Tv_RcIc ) )
					{
						// exist paint code
						PaintScriptPush( m_ParaDoQuad.PntVloss, &Tv_RcIc );
					}
				}
				
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::ProcPaint( void )
{
	// local -------------------
	// code --------------------
		// quad line
		LcPaintGrid();
		// view
		LcPaintView();
		// back ground
		PaintFillClrPush( 0, &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoQuad, V_GrP3DoQuadParaInfo, E_GrP3DoQuadParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
	// code --------------------
		LcEvtDispUpdt();
		LcTmrKeyChCombine();	
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcQuadStatUpdt( void )
{
	// local -------------------
	// code --------------------
		m_GridCnt	=	0;
		if ( 0 != m_ParaDoQuad.GridWidth )
		{
			m_GridCnt	=	((Cls_GrP3ConBase*)m_ObjCon)->QuadGridTblBuild( m_GridTbl, m_ParaDoQuad.GridWidth );
		}
		m_ViewChTbl	=	((Cls_GrP3ConBase*)m_ObjCon)->QuadViewChMapPtrGet();
		m_ViewCnt		=	((Cls_GrP3ConBase*)m_ObjCon)->QuadViewRectBuild( m_ViewRcTbl );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcLivePlayUpdt( void )
{
	// local -------------------
	// code --------------------
		m_IsPlayMode	=	V_GrP3Mng->IsPlayMode( ((Cls_GrP3ConBase*)m_ObjCon)->IndexGet() );		// update play mode
		if ( m_IsPlayMode )
		{
			m_SysAtbTbl	=	V_GrP3Mng->PlayAtbTblPtrGet();
		}
		else
		{
			m_SysAtbTbl	=	V_GrP3Mng->LiveAtbTblPtrGet();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::ProcEvtSysStat( __u8 A_Type )
{
	// local -------------------
	// code --------------------
		switch ( A_Type )
		{
			case E_GrP3SysStatQuadMode:
				LcQuadStatUpdt();
				ReqDraw();
				break;
			case E_GrP3SysStatLivePlay:
				LcLivePlayUpdt();
				break;
			case E_GrP3SysStatSetupChg:
				LcSetupUpdt();
				break;
			case E_GrP3SysStatAccountChg:
				LcAccountUpdt();
				break;
			case E_GrP3SysStatVlossChg:
				ReqDraw();
				break;
			default:
				Cls_GrP3DoBase::ProcEvtSysStat( A_Type );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcRcCliUpdt( void )
{
	// local -------------------
	// code --------------------
		m_RcCli.left		=	m_Rect.left + m_MgnHz;
		m_RcCli.top			=	m_Rect.top + m_MgnVt;
		m_RcCli.right		=	m_Rect.right - m_MgnHz;
		m_RcCli.bottom	=	m_Rect.bottom - m_MgnVt;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::RectSet( Ptr_GrRect A_PtrRect )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RectSet( A_PtrRect );
		// update client rect
		LcRcCliUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::MarginSet( __s32 A_Hz, __s32 A_Vt )
{
	// local -------------------
	// code --------------------
		m_MgnHz		=	A_Hz;
		m_MgnVt		=	A_Vt;
		LcRcCliUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcSetupUpdt( void )
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcAccountUpdt( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoQuad::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Rint[4];
		__s32		Tv_PlaySpd;
	// code --------------------
		Tv_Result		=	TRUE;
		Tv_PlaySpd	=	0;
		switch ( A_Key )
		{
			case E_GrP3KeySplit:
				((Cls_GrP3ConBase*)m_ObjCon)->QuadModeNext();
				break;
			case E_GrP3KeyNextQuad:
				((Cls_GrP3ConBase*)m_ObjCon)->QuadPageNext();
				break;
			case E_GrP3KeyJogLeft:
				if ( m_IsPlayMode )
				{
					GrP3BiosPlayPrevFrm( Tv_Rint, NULL );
				}
				break;
			case E_GrP3KeyJogRight:
				if ( m_IsPlayMode )
				{
					GrP3BiosPlayNextFrm( Tv_Rint, NULL );
				}
				break;
			case E_GrP3KeyShtlL7:
			case E_GrP3KeyShtlL6:
			case E_GrP3KeyShtlL5:
				Tv_PlaySpd ++;
			case E_GrP3KeyShtlL4:
				Tv_PlaySpd ++;
			case E_GrP3KeyShtlL3:
				Tv_PlaySpd ++;
			case E_GrP3KeyShtlL2:
				Tv_PlaySpd ++;
			case E_GrP3KeyShtlL1:
				if ( m_IsPlayMode )
				{
					if ( !((Ptr_GrP3MngGvs)m_PtrGvs)->IsPlayBack )
					{
						Tv_Rint[1]	=	TRUE;
						GrP3BiosPlayDirSet( Tv_Rint, NULL );
					}
					if ( Tv_PlaySpd != ((Ptr_GrP3MngGvs)m_PtrGvs)->PlaySpd )
					{
						Tv_Rint[1]	=	Tv_PlaySpd;
						GrP3BiosPlaySpeedSet( Tv_Rint, NULL );
					}
					if ( E_GrDvrPlayStatStop == ((Ptr_GrP3MngGvs)m_PtrGvs)->PlayStat )
					{
						GrP3BiosPlayDo( Tv_Rint, NULL );
					}
				}
				break;
			case E_GrP3KeyShtlR7:
			case E_GrP3KeyShtlR6:
			case E_GrP3KeyShtlR5:
				Tv_PlaySpd ++;
			case E_GrP3KeyShtlR4:
				Tv_PlaySpd ++;
			case E_GrP3KeyShtlR3:
				Tv_PlaySpd ++;
			case E_GrP3KeyShtlR2:
				Tv_PlaySpd ++;
			case E_GrP3KeyShtlR1:
				if ( m_IsPlayMode )
				{
					if ( ((Ptr_GrP3MngGvs)m_PtrGvs)->IsPlayBack )
					{
						Tv_Rint[1]	=	FALSE;
						GrP3BiosPlayDirSet( Tv_Rint, NULL );
					}
					if ( Tv_PlaySpd != ((Ptr_GrP3MngGvs)m_PtrGvs)->PlaySpd )
					{
						Tv_Rint[1]	=	Tv_PlaySpd;
						GrP3BiosPlaySpeedSet( Tv_Rint, NULL );
					}
					if ( E_GrDvrPlayStatStop == ((Ptr_GrP3MngGvs)m_PtrGvs)->PlayStat )
					{
						GrP3BiosPlayDo( Tv_Rint, NULL );
					}
				}
				break;
			case E_GrP3KeyShtlR0:
			case E_GrP3KeyShtlL0:
				if ( m_IsPlayMode )
				{
					if ( E_GrDvrPlayStatStop != ((Ptr_GrP3MngGvs)m_PtrGvs)->PlayStat )
					{
						GrP3BiosPlayPause( Tv_Rint, NULL );
					}
				}
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoQuad::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( 0 == A_Idx )
		{
			*A_PtrRtCus	=	-1;		// no cursor
			Tv_Result	=	(WCHAR*)( (__u32)V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsCamTitle ) + ( (__u32)m_PntVwCh * 32) );
		}
		/*
		else if ( 1 == A_Idx )
		{
			
		}
		*/
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::ProcEvtMseEnter( void )
{
		m_IsOnMouse	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::ProcEvtMseLeave( void )
{
		m_IsOnMouse	=	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcEvtDispUpdt( void )
{
	// local -------------------
		__u8	Tv_ViewIdx;
		__u8	Tv_Ch;
		__u8	Tv_Atb;
	// code --------------------
		if ( 0 != m_ViewCnt )
		{
			for ( Tv_ViewIdx=0;Tv_ViewIdx < m_ViewCnt;Tv_ViewIdx++ )
			{
				// get channel
				Tv_Atb	=	0;
				Tv_Ch		=	m_ViewChTbl[Tv_ViewIdx];
				if ( NULL != m_SysAtbTbl )
				{
					Tv_Atb	=	m_SysAtbTbl[Tv_Ch];
				}
				// check changed
				if ( Tv_Atb != m_DispAtbTbl[Tv_Ch] )
				{
					m_DispAtbTbl[Tv_Ch]	=	Tv_Atb;
					// request redraw
					ReqPartDraw( &m_ViewRcTbl[Tv_ViewIdx] );
				}
				
			}
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrP3DoQuad::LcCmdSubShowGet( __s32 A_Type )
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		switch ( A_Type )
		{
			case 0:
				Tv_Result	=	(__s32)m_IsGridShow;
				break;
			case 1:
				Tv_Result	=	(__s32)m_IsIconShow;
				break;
			case 2:
				Tv_Result	=	(__s32)m_IsTitleShow;
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcCmdSubShowSet( __s32 A_Type, __s32 A_Val )
{
	// local -------------------
	// code --------------------
		switch ( A_Type )
		{
			case 0:
				m_IsGridShow	=	(BOOL8)A_Val;
				break;
			case 1:
				m_IsIconShow	=	(BOOL8)A_Val;
				break;
			case 2:
				m_IsTitleShow	=	(BOOL8)A_Val;
				break;
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DoQuad::LcViewIdxByPos( __u16 A_X, __u16 A_Y )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_ViewCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_ViewCnt;Tv_WkIdx++ )
			{
				if (	( (__s32)A_X >= m_ViewRcTbl[Tv_WkIdx].left ) && ( (__s32)A_X < m_ViewRcTbl[Tv_WkIdx].right ) && 
							( (__s32)A_Y >= m_ViewRcTbl[Tv_WkIdx].top ) && ( (__s32)A_Y < m_ViewRcTbl[Tv_WkIdx].bottom )
						)
				{
					Tv_Result	=	Tv_WkIdx;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoQuad::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u8	Tv_Mode;
		__u8	Tv_Page;
		__u8	Tv_ViewCnt;
		__u8	Tv_NewPage;
		__u8	Tv_ViewIdx;
	// code --------------------
		// check now mode
		((Cls_GrP3ConBase*)m_ObjCon)->QuadModeGet( &Tv_Mode, &Tv_Page );
		if ( E_GrDvrQuadMode1 == Tv_Mode )
		{
			// check old quad mode
			if ( E_GrDvrQuadMode1 == m_OldQdMode )
			{
				switch ( V_GrP3Mng->VdoMaxChCntGet() )
				{
					case 16:
						m_OldQdMode	=	E_GrDvrQuadMode16;
						break;
					case 9:
					case 8:
						m_OldQdMode	=	E_GrDvrQuadMode9;
						break;
					default:
						m_OldQdMode	=	E_GrDvrQuadMode4;
						break;
				}
			}
			Tv_ViewCnt	=	GrDvrQuadViewCount( m_OldQdMode );
			Tv_NewPage	=	Tv_Page / Tv_ViewCnt;
			((Cls_GrP3ConBase*)m_ObjCon)->QuadModeSet( m_OldQdMode, Tv_NewPage );

		}
		else
		{
			m_OldQdMode	=	Tv_Mode;
			Tv_ViewIdx	=	LcViewIdxByPos( A_X, A_Y );
			((Cls_GrP3ConBase*)m_ObjCon)->QuadModeSet( E_GrDvrQuadMode1, m_ViewChTbl[Tv_ViewIdx] );
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoQuad::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_QuadCh;
		__u8*	Tv_PtrStp;
		__u8	Tv_QdMode;
		__u8	Tv_QdPage;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( (E_GrP3KeyCh0 <= A_Key) && (E_GrP3KeyCh16 >= A_Key) )
		{
			Tv_QuadCh	=	A_Key - E_GrP3KeyCh0;
			if ( 0 != m_KeyChCmbTime )
			{
				if ( 10 > Tv_QuadCh )
				{
					Tv_QuadCh	=	Tv_QuadCh + 10;
				}
				m_KeyChCmbTime	=	0;
			}
			else
			{
				if ( E_GrP3KeyCh1 == A_Key )
				{
					if ( 10 <= m_VdoChCnt )
					{
						m_KeyChCmbTime	=	E_GrP3DoQuadKeyCmbTimeOut;
					}
				}
			}
			// update quad
			if ( 0 != Tv_QuadCh )
			{
				Tv_QuadCh --;
				((Cls_GrP3ConBase*)m_ObjCon)->QuadModeSet( E_GrDvrQuadMode1, Tv_QuadCh );
			}
			Tv_Result	=	TRUE;
		}
		else
		{
			m_KeyChCmbTime	=	0;

			switch ( A_Key )
			{
				case E_GrP3KeyQuad:
					// change quad mode
					Tv_PtrStp	=	(__u8*)V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsDisSplitMode );
					((Cls_GrP3ConBase*)m_ObjCon)->QuadModeSet( *Tv_PtrStp, 0 );
					break;
				case E_GrP3KeyQdMode4:
					((Cls_GrP3ConBase*)m_ObjCon)->QuadModeGet( &Tv_QdMode, &Tv_QdPage );
					if ( E_GrDvrQuadMode4 == Tv_QdMode )
					{
						((Cls_GrP3ConBase*)m_ObjCon)->QuadPageNext();
					}
					else
					{
						((Cls_GrP3ConBase*)m_ObjCon)->QuadModeSet( E_GrDvrQuadMode4, 0 );
					}
					break;
				case E_GrP3KeyQdMode9:
					((Cls_GrP3ConBase*)m_ObjCon)->QuadModeGet( &Tv_QdMode, &Tv_QdPage );
					if ( E_GrDvrQuadMode9 == Tv_QdMode )
					{
						((Cls_GrP3ConBase*)m_ObjCon)->QuadPageNext();
					}
					else
					{
						((Cls_GrP3ConBase*)m_ObjCon)->QuadModeSet( E_GrDvrQuadMode9, 0 );
					}
					break;
				case E_GrP3KeyQdMode16:
					((Cls_GrP3ConBase*)m_ObjCon)->QuadModeGet( &Tv_QdMode, &Tv_QdPage );
					if ( E_GrDvrQuadMode16 == Tv_QdMode )
					{
						((Cls_GrP3ConBase*)m_ObjCon)->QuadPageNext();
					}
					else
					{
						((Cls_GrP3ConBase*)m_ObjCon)->QuadModeSet( E_GrDvrQuadMode16, 0 );
					}
					break;
			}
			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuad::LcTmrKeyChCombine( void )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_KeyChCmbTime )
		{
			m_KeyChCmbTime --;
		}
}
//--------------------------------------------------------------------
