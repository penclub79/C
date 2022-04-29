/*
		platform edit setup string

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoEdtLoc.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>

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

#define E_GrP3DoEdtLocParaCnt		1

St_GrP3DvlDoPrInfoData	V_GrP3DoEdtLocParaInfo[E_GrP3DoEdtLocParaCnt]	=	
{
	{	L"EvtHostId",					TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoEdtLocPara)0)->EvtHostId,NULL,	L"event host"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoEdtLoc::Cls_GrP3DoEdtLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoEdtBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassEdtLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaEdtLoc) + sizeof(St_GrP3ScptParaHd);

		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrStrClear( m_StrTxt );

		GrDumyZeroMem( &m_ParaEdtLoc, sizeof(m_ParaEdtLoc) );
		
}
//--------------------------------------------------------------------
Cls_GrP3DoEdtLoc::~Cls_GrP3DoEdtLoc( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoEdtLoc::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoEdtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassEdtLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaEdtLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaEdtLoc, sizeof(m_ParaEdtLoc) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaEdtLoc)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoEdtLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoEdtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaEdtLoc, sizeof(m_ParaEdtLoc) , m_PtrScpt , E_GrP3DoClassEdtLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoEdtLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoEdtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoEdtLoc::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoEdtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaEdtLoc, V_GrP3DoEdtLocParaInfo, E_GrP3DoEdtLocParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoEdtLoc::LcValAdrGet( void )
{
	// local -------------------
	// code --------------------
		return	m_StrTxt;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoEdtLoc::ProcValChg( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		if ( E_GrP3ScptIdNone != m_ParaEdtLoc.EvtHostId)
		{
			Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdEditValChg;
			Tv_Msg.Hd.ConIdx	=	((Cls_GrP3ConBase*)m_ObjCon)->IndexGet();
			Tv_Msg.Hd.WinId		=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_ParaEdtLoc.EvtHostId;
			Tv_Msg.ObjId			=	m_ScptId;
			
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		
}
//--------------------------------------------------------------------
