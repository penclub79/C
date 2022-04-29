/*
		Check Button Local

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoChkbLoc.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include <P4/GrP4DoWin.h>

#ifdef GR_P4_EDITOR
#include	<P4/Dvl/GrP4DvlBase.h>
#include	<P4/Dvl/GrP4DvlMng.h>
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
extern	Cls_GrP4Mng*	V_GrP4Mng;

#ifdef GR_P4_EDITOR

#define E_GrP4DoChkbLocParaCnt		3

St_GrP4DvlDoPrInfoData	V_GrP4DoChkbLocParaInfo[E_GrP4DoChkbLocParaCnt]	=	
{
	{	L"TxtIdFalse",	TRUE,		E_GrP4DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP4DoChkbLocPara)0)->TxtIdFalse,NULL,	L"text id false"	},
	{	L"TxtIdTrue",		TRUE,		E_GrP4DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP4DoChkbLocPara)0)->TxtIdTrue,NULL,	L"text id true"	},
	{	L"EvtHostId",		TRUE,		E_GrP4DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP4DoChkbLocPara)0)->EvtHostId,NULL,	L"Event host id"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoChkbLoc::Cls_GrP4DoChkbLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoChkbBase( A_ScptId, A_PtrRect, A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassChkbLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaChkbLoc) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaChkbLoc, sizeof(m_ParaChkbLoc) );

}
//--------------------------------------------------------------------
Cls_GrP4DoChkbLoc::~Cls_GrP4DoChkbLoc( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoChkbLoc::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoChkbBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassChkbLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaChkbLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaChkbLoc, sizeof(m_ParaChkbLoc) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaChkbLoc)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoChkbBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaChkbLoc, sizeof(m_ParaChkbLoc) , m_PtrScpt , E_GrP4DoClassChkbLoc );

		LcValLoad();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoChkbLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbLoc.TxtIdFalse;
				break;
			case E_GrP4UpiTextIdSet:
				m_ParaChkbLoc.TxtIdTrue		=	(__u16)A_PtrInt[1];
				m_ParaChkbLoc.TxtIdFalse	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoChkbBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoChkbLoc::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoChkbBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaChkbLoc, V_GrP4DoChkbLocParaInfo, E_GrP4DoChkbLocParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoChkbLoc::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			if ( m_IsVal )
			{
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaChkbLoc.TxtIdTrue );
			}
			else
			{
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaChkbLoc.TxtIdFalse );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbLoc::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		Cls_GrP4DoChkbBase::LcNotifyValChg();

		// notify message
		if ( E_GrP4ScptIdNone != m_ParaChkbLoc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdBtnChkChg;
			Tv_Msg.Hd.ConIdx	=	((Cls_GrP4ConBase*)m_ObjCon)->IndexGet();
			Tv_Msg.Hd.WinId		=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_ParaChkbLoc.EvtHostId;
			Tv_Msg.ObjId			=	m_ScptId;
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

}
//--------------------------------------------------------------------
