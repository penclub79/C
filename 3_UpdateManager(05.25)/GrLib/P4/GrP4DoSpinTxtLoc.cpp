/*
		platform spin text local

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinTxtLoc.h>
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

#define E_GrP4DoSpinTxtLocParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinTxtLocParaInfo[E_GrP4DoSpinTxtLocParaCnt]	=	
{
	{	L"EvtHostId",			TRUE,		E_GrP4DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP4DoSpinTxtLocPara)0)->EvtHostId,NULL,	L"event host object id"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinTxtLoc::Cls_GrP4DoSpinTxtLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoSpinTxtBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinTxtLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinTxtLoc) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinTxtLoc, sizeof(m_ParaSpinTxtLoc) );

}
//--------------------------------------------------------------------
Cls_GrP4DoSpinTxtLoc::~Cls_GrP4DoSpinTxtLoc( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinTxtLoc::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoSpinTxtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinTxtLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinTxtLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinTxtLoc, sizeof(m_ParaSpinTxtLoc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinTxtLoc)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoSpinTxtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinTxtLoc, sizeof(m_ParaSpinTxtLoc) , m_PtrScpt , E_GrP4DoClassSpinTxtLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinTxtLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaSpinTxtLoc.EvtHostId;
				break;
			case E_GrP4UpiEvtHostIdSet:
				m_ParaSpinTxtLoc.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			default:
				Tv_Result	=	Cls_GrP4DoSpinTxtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinTxtLoc::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoSpinTxtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinTxtLoc, V_GrP4DoSpinTxtLocParaInfo, E_GrP4DoSpinTxtLocParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtLoc::EvtHostIdSet( __u16 A_ObjId )
{
	m_ParaSpinTxtLoc.EvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DoSpinTxtLoc::EvtHostIdGet( void )
{
	return	m_ParaSpinTxtLoc.EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtLoc::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		if ( E_GrP4ScptIdNone != m_ParaSpinTxtLoc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd				=	E_GrP4MsgCmdSpinValChg;
			Tv_Msg.Hd.ConIdx		=	E_GrP4ConIdxFocus;
			Tv_Msg.Hd.WinId			=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId			=	m_ParaSpinTxtLoc.EvtHostId;
			Tv_Msg.ObjId				=	m_ScptId;
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

		Cls_GrP4DoSpinTxtBase::LcNotifyValChg();		// inherited
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtLoc::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtLoc::LcValSave( void )
{

}
//--------------------------------------------------------------------

