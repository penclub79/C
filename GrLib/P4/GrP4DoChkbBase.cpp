/*
		platform check button static setup bool8

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoChkbBase.h>
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

#define E_GrP4DoChkbBaseParaCnt		9

St_GrP4DvlDoPrInfoData	V_GrP4DoChkbBaseParaInfo[E_GrP4DoChkbBaseParaCnt]	=	
{
	{	L"Paint FalseDis",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoChkbBasePara)0)->PntStat[E_GrP4DoChkbBaseStatFalseDis],  NULL,	L"Paint false disable status"	},
	{	L"Paint False",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoChkbBasePara)0)->PntStat[E_GrP4DoChkbBaseStatFalse],	 NULL,L"Paint false normal status"	},
	{	L"Paint FalseFcs",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoChkbBasePara)0)->PntStat[E_GrP4DoChkbBaseStatFalseFcs],	 NULL,L"Paint false focus status"	},
	{	L"Paint FalsePush",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoChkbBasePara)0)->PntStat[E_GrP4DoChkbBaseStatFalsePush],	 NULL,L"Paint false push status"	},
	{	L"Paint TrueDis",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoChkbBasePara)0)->PntStat[E_GrP4DoChkbBaseStatTrueDis],	 NULL,L"Paint true disable status"	},
	{	L"Paint True",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoChkbBasePara)0)->PntStat[E_GrP4DoChkbBaseStatTrue],	 NULL,L"Paint true normal status"	},
	{	L"Paint TrueFcs",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoChkbBasePara)0)->PntStat[E_GrP4DoChkbBaseStatTrueFcs],	 NULL,L"Paint true focus status"	},
	{	L"Paint TruePush",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoChkbBasePara)0)->PntStat[E_GrP4DoChkbBaseStatTruePush],	 NULL,L"Paint true push status"	},
	{	L"AsmValChg",					TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoChkbBasePara)0)->AsmChgVal,	 NULL,L"assembly value change event code"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoChkbBase::Cls_GrP4DoChkbBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassChkbBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoChkbBase) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsPush		=	FALSE;
		m_IsVal			=	FALSE;

		GrDumyZeroMem( &m_ParaDoChkbBase, sizeof(m_ParaDoChkbBase) );
		//m_ParaDoChkbBase.PntBg	=	E_GrP4ScptPstIdDflt;

}
//--------------------------------------------------------------------
Cls_GrP4DoChkbBase::~Cls_GrP4DoChkbBase( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoChkbBase::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassChkbBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoChkbBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoChkbBase, sizeof(m_ParaDoChkbBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoChkbBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoChkbBase, sizeof(m_ParaDoChkbBase) , m_PtrScpt , E_GrP4DoClassChkbBase );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoChkbBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiValueGet:
				LcValLoad();
				A_PtrInt[0]	=	(__s32)m_IsVal;
				break;
			case E_GrP4UpiValueSet:
				m_IsVal	=	(BOOL8)A_PtrInt[1];
				LcValSave();
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// reload value
		LcValLoad();
		// update status
		Tv_Stat	=	E_GrP4DoChkbBaseStatFalseDis;
		if ( m_IsVal )
		{
			Tv_Stat	=	E_GrP4DoChkbBaseStatTrueDis;
		}
		
		if ( IsCanEnable() )
		{
			Tv_Stat	++;		// normal
			if ( m_IsPush )
			{
				Tv_Stat	=	Tv_Stat + 2;		// push
			}
			else
			{
				if ( m_IsOnMouse || IsFocused() )
				{
					Tv_Stat	++;	// focus
				}
			}
		}
		
		PaintScriptPush( m_ParaDoChkbBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoChkbBase::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoChkbBase, V_GrP4DoChkbBaseParaInfo, E_GrP4DoChkbBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoChkbBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4KeyEnter == A_Key )
		{
			m_IsPush	=	TRUE;
			ReqDraw();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoChkbBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4KeyEnter == A_Key )
		{
			m_IsPush	=	FALSE;
			ReqDraw();
			LcValLoad();
			m_IsVal	=	!m_IsVal;
			LcValSave();
			// notify
			LcNotifyValChg();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoChkbBase::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	TRUE;
		Cls_GrP4DoBase::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoChkbBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		ReqDraw();
		LcValLoad();
		m_IsVal	=	!m_IsVal;
		LcValSave();
		// do up assembly
		V_GrP4Mng->VmRunCode( m_ParaDoChkbBase.AsmChgVal, this );
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbBase::LcValSave( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbBase::LcNotifyValChg( void )
{
	// local -------------------
	// code --------------------
		// do up assembly
		V_GrP4Mng->VmRunCode( m_ParaDoChkbBase.AsmChgVal, this );
}
//--------------------------------------------------------------------
