/*
		Check Button Local

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoChkbLoc.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include <P3/GrP3DoWin.h>

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

#define E_GrP3DoChkbLocParaCnt		3

St_GrP3DvlDoPrInfoData	V_GrP3DoChkbLocParaInfo[E_GrP3DoChkbLocParaCnt]	=	
{
	{	L"TxtIdFalse",	TRUE,		E_GrP3DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP3DoChkbLocPara)0)->TxtIdFalse,NULL,	L"text id false"	},
	{	L"TxtIdTrue",		TRUE,		E_GrP3DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP3DoChkbLocPara)0)->TxtIdTrue,NULL,	L"text id true"	},
	{	L"EvtHostId",		TRUE,		E_GrP3DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP3DoChkbLocPara)0)->EvtHostId,NULL,	L"Event host id"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoChkbLoc::Cls_GrP3DoChkbLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoChkbBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassChkbLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaChkbLoc) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaChkbLoc, sizeof(m_ParaChkbLoc) );

}
//--------------------------------------------------------------------
Cls_GrP3DoChkbLoc::~Cls_GrP3DoChkbLoc( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbLoc::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoChkbBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassChkbLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaChkbLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaChkbLoc, sizeof(m_ParaChkbLoc) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaChkbLoc)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoChkbBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaChkbLoc, sizeof(m_ParaChkbLoc) , m_PtrScpt , E_GrP3DoClassChkbLoc );

		LcValLoad();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoChkbLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbLoc.TxtIdFalse;
				break;
			case E_GrP3UpiTextIdSet:
				m_ParaChkbLoc.TxtIdTrue		=	(__u16)A_PtrInt[1];
				m_ParaChkbLoc.TxtIdFalse	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoChkbBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbLoc::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoChkbBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaChkbLoc, V_GrP3DoChkbLocParaInfo, E_GrP3DoChkbLocParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoChkbLoc::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
				Tv_Result	=	Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaChkbLoc.TxtIdTrue );
			}
			else
			{
				Tv_Result	=	Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaChkbLoc.TxtIdFalse );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbLoc::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		Cls_GrP3DoChkbBase::LcNotifyValChg();

		// notify message
		if ( E_GrP3ScptIdNone != m_ParaChkbLoc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdBtnChkChg;
			Tv_Msg.Hd.ConIdx	=	((Cls_GrP3ConBase*)m_ObjCon)->IndexGet();
			Tv_Msg.Hd.WinId		=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_ParaChkbLoc.EvtHostId;
			Tv_Msg.ObjId			=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

}
//--------------------------------------------------------------------
