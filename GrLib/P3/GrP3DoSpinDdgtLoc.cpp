/*
		platform spin decimal digit setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinDdgtLoc.h>
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

#define E_GrP3DoSpinDdgtLocParaCnt		1

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinDdgtLocParaInfo[E_GrP3DoSpinDdgtLocParaCnt]	=	
{
	{	L"EvtHostId",			TRUE,		E_GrP3DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP3DoSpinDdgtLocPara)0)->EvtHostId,NULL,	L"event host object id"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinDdgtLoc::Cls_GrP3DoSpinDdgtLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoSpinDdgtBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinDdgtLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinDdgtLoc) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinDdgtLoc, sizeof(m_ParaSpinDdgtLoc) );

}
//--------------------------------------------------------------------
Cls_GrP3DoSpinDdgtLoc::~Cls_GrP3DoSpinDdgtLoc( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtLoc::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoSpinDdgtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinDdgtLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinDdgtLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinDdgtLoc, sizeof(m_ParaSpinDdgtLoc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinDdgtLoc)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoSpinDdgtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinDdgtLoc, sizeof(m_ParaSpinDdgtLoc) , m_PtrScpt , E_GrP3DoClassSpinDdgtLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case	E_GrP3UpiEvtHostIdGet:
				A_PtrInt[0]	=	m_ParaSpinDdgtLoc.EvtHostId;
				break;
			case E_GrP3UpiEvtHostIdSet:
				m_ParaSpinDdgtLoc.EvtHostId	=	A_PtrInt[1];
				break;
			default:
				Tv_Result	=	Cls_GrP3DoSpinDdgtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtLoc::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoSpinDdgtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinDdgtLoc, V_GrP3DoSpinDdgtLocParaInfo, E_GrP3DoSpinDdgtLocParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtLoc::EvtHostIdSet( __u16 A_ObjId )
{
		m_ParaSpinDdgtLoc.EvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoSpinDdgtLoc::EvtHostIdGet( void )
{
		return	m_ParaSpinDdgtLoc.EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtLoc::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		if ( E_GrP3ScptIdNone != m_ParaSpinDdgtLoc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd				=	E_GrP3MsgCmdSpinValChg;
			Tv_Msg.Hd.ConIdx		=	E_GrP3ConIdxFocus;
			Tv_Msg.Hd.WinId			=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId			=	m_ParaSpinDdgtLoc.EvtHostId;
			Tv_Msg.ObjId				=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

		Cls_GrP3DoSpinDdgtBase::LcNotifyValChg();		// inherited
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtLoc::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtLoc::LcValSave( void )
{

}
//--------------------------------------------------------------------


