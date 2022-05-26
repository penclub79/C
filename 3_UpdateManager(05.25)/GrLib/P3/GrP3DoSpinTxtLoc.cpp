/*
		platform spin text local

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinTxtLoc.h>
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

#define E_GrP3DoSpinTxtLocParaCnt		1

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinTxtLocParaInfo[E_GrP3DoSpinTxtLocParaCnt]	=	
{
	{	L"EvtHostId",			TRUE,		E_GrP3DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP3DoSpinTxtLocPara)0)->EvtHostId,NULL,	L"event host object id"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinTxtLoc::Cls_GrP3DoSpinTxtLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoSpinTxtBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinTxtLoc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinTxtLoc) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinTxtLoc, sizeof(m_ParaSpinTxtLoc) );

}
//--------------------------------------------------------------------
Cls_GrP3DoSpinTxtLoc::~Cls_GrP3DoSpinTxtLoc( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtLoc::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoSpinTxtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinTxtLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinTxtLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinTxtLoc, sizeof(m_ParaSpinTxtLoc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinTxtLoc)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoSpinTxtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinTxtLoc, sizeof(m_ParaSpinTxtLoc) , m_PtrScpt , E_GrP3DoClassSpinTxtLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinTxtLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaSpinTxtLoc.EvtHostId;
				break;
			case E_GrP3UpiEvtHostIdSet:
				m_ParaSpinTxtLoc.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			default:
				Tv_Result	=	Cls_GrP3DoSpinTxtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtLoc::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoSpinTxtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinTxtLoc, V_GrP3DoSpinTxtLocParaInfo, E_GrP3DoSpinTxtLocParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtLoc::EvtHostIdSet( __u16 A_ObjId )
{
	m_ParaSpinTxtLoc.EvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoSpinTxtLoc::EvtHostIdGet( void )
{
	return	m_ParaSpinTxtLoc.EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtLoc::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		if ( E_GrP3ScptIdNone != m_ParaSpinTxtLoc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd				=	E_GrP3MsgCmdSpinValChg;
			Tv_Msg.Hd.ConIdx		=	E_GrP3ConIdxFocus;
			Tv_Msg.Hd.WinId			=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId			=	m_ParaSpinTxtLoc.EvtHostId;
			Tv_Msg.ObjId				=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

		Cls_GrP3DoSpinTxtBase::LcNotifyValChg();		// inherited
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtLoc::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtLoc::LcValSave( void )
{

}
//--------------------------------------------------------------------

