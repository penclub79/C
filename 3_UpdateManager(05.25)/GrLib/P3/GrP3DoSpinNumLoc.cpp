/*
		platform spin number local

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinNumLoc.h>
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

#define E_GrP3DoSpinNumLocParaCnt		1

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinNumLocParaInfo[E_GrP3DoSpinNumLocParaCnt]	=	
{
	{	L"EvtHostId",			TRUE,		E_GrP3DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP3DoSpinNumLocPara)0)->EvtHostId,NULL,	L"event host object id"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinNumLoc::Cls_GrP3DoSpinNumLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoSpinNumBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinNumLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinNumLoc) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinNumLoc, sizeof(m_ParaSpinNumLoc) );

}
//--------------------------------------------------------------------
Cls_GrP3DoSpinNumLoc::~Cls_GrP3DoSpinNumLoc( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumLoc::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoSpinNumBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinNumLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinNumLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinNumLoc, sizeof(m_ParaSpinNumLoc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinNumLoc)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoSpinNumBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinNumLoc, sizeof(m_ParaSpinNumLoc) , m_PtrScpt , E_GrP3DoClassSpinNumLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinNumLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaSpinNumLoc.EvtHostId;
				break;
			case E_GrP3UpiEvtHostIdSet:
				m_ParaSpinNumLoc.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			default:
				Tv_Result	=	Cls_GrP3DoSpinNumBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumLoc::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoSpinNumBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinNumLoc, V_GrP3DoSpinNumLocParaInfo, E_GrP3DoSpinNumLocParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumLoc::EvtHostIdSet( __u16 A_ObjId )
{
		m_ParaSpinNumLoc.EvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoSpinNumLoc::EvtHostIdGet( void )
{
		return	m_ParaSpinNumLoc.EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumLoc::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		if ( E_GrP3ScptIdNone != m_ParaSpinNumLoc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd				=	E_GrP3MsgCmdSpinValChg;
			Tv_Msg.Hd.ConIdx		=	E_GrP3ConIdxFocus;
			Tv_Msg.Hd.WinId			=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId			=	m_ParaSpinNumLoc.EvtHostId;
			Tv_Msg.ObjId				=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		
		Cls_GrP3DoSpinNumBase::LcNotifyValChg();		// inherited
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumLoc::LcValLoad( void )
{
		// not work
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumLoc::LcValSave( void )
{
		// not work
}
//--------------------------------------------------------------------


