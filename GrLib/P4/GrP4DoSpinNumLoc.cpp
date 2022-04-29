/*
		platform spin number local

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinNumLoc.h>
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

#define E_GrP4DoSpinNumLocParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinNumLocParaInfo[E_GrP4DoSpinNumLocParaCnt]	=	
{
	{	L"EvtHostId",			TRUE,		E_GrP4DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP4DoSpinNumLocPara)0)->EvtHostId,NULL,	L"event host object id"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinNumLoc::Cls_GrP4DoSpinNumLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoSpinNumBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinNumLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinNumLoc) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinNumLoc, sizeof(m_ParaSpinNumLoc) );

}
//--------------------------------------------------------------------
Cls_GrP4DoSpinNumLoc::~Cls_GrP4DoSpinNumLoc( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinNumLoc::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoSpinNumBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinNumLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinNumLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinNumLoc, sizeof(m_ParaSpinNumLoc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinNumLoc)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoSpinNumBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinNumLoc, sizeof(m_ParaSpinNumLoc) , m_PtrScpt , E_GrP4DoClassSpinNumLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinNumLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaSpinNumLoc.EvtHostId;
				break;
			case E_GrP4UpiEvtHostIdSet:
				m_ParaSpinNumLoc.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			default:
				Tv_Result	=	Cls_GrP4DoSpinNumBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinNumLoc::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoSpinNumBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinNumLoc, V_GrP4DoSpinNumLocParaInfo, E_GrP4DoSpinNumLocParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumLoc::EvtHostIdSet( __u16 A_ObjId )
{
		m_ParaSpinNumLoc.EvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DoSpinNumLoc::EvtHostIdGet( void )
{
		return	m_ParaSpinNumLoc.EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumLoc::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		if ( E_GrP4ScptIdNone != m_ParaSpinNumLoc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd				=	E_GrP4MsgCmdSpinValChg;
			Tv_Msg.Hd.ConIdx		=	E_GrP4ConIdxFocus;
			Tv_Msg.Hd.WinId			=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId			=	m_ParaSpinNumLoc.EvtHostId;
			Tv_Msg.ObjId				=	m_ScptId;
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		
		Cls_GrP4DoSpinNumBase::LcNotifyValChg();		// inherited
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumLoc::LcValLoad( void )
{
		// not work
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumLoc::LcValSave( void )
{
		// not work
}
//--------------------------------------------------------------------


