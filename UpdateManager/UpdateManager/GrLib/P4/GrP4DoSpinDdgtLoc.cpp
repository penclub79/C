/*
		platform spin decimal digit setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinDdgtLoc.h>
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

#define E_GrP4DoSpinDdgtLocParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinDdgtLocParaInfo[E_GrP4DoSpinDdgtLocParaCnt]	=	
{
	{	L"EvtHostId",			TRUE,		E_GrP4DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP4DoSpinDdgtLocPara)0)->EvtHostId,NULL,	L"event host object id"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinDdgtLoc::Cls_GrP4DoSpinDdgtLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoSpinDdgtBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinDdgtLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinDdgtLoc) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinDdgtLoc, sizeof(m_ParaSpinDdgtLoc) );

}
//--------------------------------------------------------------------
Cls_GrP4DoSpinDdgtLoc::~Cls_GrP4DoSpinDdgtLoc( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinDdgtLoc::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoSpinDdgtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinDdgtLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinDdgtLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinDdgtLoc, sizeof(m_ParaSpinDdgtLoc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinDdgtLoc)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoSpinDdgtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinDdgtLoc, sizeof(m_ParaSpinDdgtLoc) , m_PtrScpt , E_GrP4DoClassSpinDdgtLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinDdgtLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case	E_GrP4UpiEvtHostIdGet:
				A_PtrInt[0]	=	m_ParaSpinDdgtLoc.EvtHostId;
				break;
			case E_GrP4UpiEvtHostIdSet:
				m_ParaSpinDdgtLoc.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			default:
				Tv_Result	=	Cls_GrP4DoSpinDdgtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinDdgtLoc::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoSpinDdgtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinDdgtLoc, V_GrP4DoSpinDdgtLocParaInfo, E_GrP4DoSpinDdgtLocParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtLoc::EvtHostIdSet( __u16 A_ObjId )
{
		m_ParaSpinDdgtLoc.EvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DoSpinDdgtLoc::EvtHostIdGet( void )
{
		return	m_ParaSpinDdgtLoc.EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtLoc::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		if ( E_GrP4ScptIdNone != m_ParaSpinDdgtLoc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd				=	E_GrP4MsgCmdSpinValChg;
			Tv_Msg.Hd.ConIdx		=	E_GrP4ConIdxFocus;
			Tv_Msg.Hd.WinId			=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId			=	m_ParaSpinDdgtLoc.EvtHostId;
			Tv_Msg.ObjId				=	m_ScptId;
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

		Cls_GrP4DoSpinDdgtBase::LcNotifyValChg();		// inherited
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtLoc::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtLoc::LcValSave( void )
{

}
//--------------------------------------------------------------------


