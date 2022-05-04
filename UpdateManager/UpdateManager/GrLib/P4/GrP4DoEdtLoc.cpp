/*
		platform edit setup string

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoEdtLoc.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>

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

#define E_GrP4DoEdtLocParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoEdtLocParaInfo[E_GrP4DoEdtLocParaCnt]	=	
{
	{	L"EvtHostId",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoEdtLocPara)0)->EvtHostId,NULL,	L"event host"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoEdtLoc::Cls_GrP4DoEdtLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoEdtBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassEdtLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaEdtLoc) + sizeof(St_GrP4ScptParaHd);

		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrStrClear( m_StrTxt );

		GrDumyZeroMem( &m_ParaEdtLoc, sizeof(m_ParaEdtLoc) );
		
}
//--------------------------------------------------------------------
Cls_GrP4DoEdtLoc::~Cls_GrP4DoEdtLoc( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoEdtLoc::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoEdtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassEdtLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaEdtLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaEdtLoc, sizeof(m_ParaEdtLoc) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaEdtLoc)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoEdtLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoEdtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaEdtLoc, sizeof(m_ParaEdtLoc) , m_PtrScpt , E_GrP4DoClassEdtLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoEdtLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoEdtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoEdtLoc::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoEdtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaEdtLoc, V_GrP4DoEdtLocParaInfo, E_GrP4DoEdtLocParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoEdtLoc::LcValAdrGet( void )
{
	// local -------------------
	// code --------------------
		return	m_StrTxt;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoEdtLoc::ProcValChg( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		if ( E_GrP4ScptIdNone != m_ParaEdtLoc.EvtHostId)
		{
			Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdEditValChg;
			Tv_Msg.Hd.ConIdx	=	((Cls_GrP4ConBase*)m_ObjCon)->IndexGet();
			Tv_Msg.Hd.WinId		=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_ParaEdtLoc.EvtHostId;
			Tv_Msg.ObjId			=	m_ScptId;
			
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		
}
//--------------------------------------------------------------------
