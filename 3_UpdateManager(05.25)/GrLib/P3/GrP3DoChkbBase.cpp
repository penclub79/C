/*
		platform check button static setup bool8

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoChkbBase.h>
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

#define E_GrP3DoChkbBaseParaCnt		9

St_GrP3DvlDoPrInfoData	V_GrP3DoChkbBaseParaInfo[E_GrP3DoChkbBaseParaCnt]	=	
{
	{	L"Paint FalseDis",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoChkbBasePara)0)->PntStat[E_GrP3DoChkbBaseStatFalseDis],  NULL,	L"Paint false disable status"	},
	{	L"Paint False",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoChkbBasePara)0)->PntStat[E_GrP3DoChkbBaseStatFalse],	 NULL,L"Paint false normal status"	},
	{	L"Paint FalseFcs",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoChkbBasePara)0)->PntStat[E_GrP3DoChkbBaseStatFalseFcs],	 NULL,L"Paint false focus status"	},
	{	L"Paint FalsePush",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoChkbBasePara)0)->PntStat[E_GrP3DoChkbBaseStatFalsePush],	 NULL,L"Paint false push status"	},
	{	L"Paint TrueDis",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoChkbBasePara)0)->PntStat[E_GrP3DoChkbBaseStatTrueDis],	 NULL,L"Paint true disable status"	},
	{	L"Paint True",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoChkbBasePara)0)->PntStat[E_GrP3DoChkbBaseStatTrue],	 NULL,L"Paint true normal status"	},
	{	L"Paint TrueFcs",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoChkbBasePara)0)->PntStat[E_GrP3DoChkbBaseStatTrueFcs],	 NULL,L"Paint true focus status"	},
	{	L"Paint TruePush",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoChkbBasePara)0)->PntStat[E_GrP3DoChkbBaseStatTruePush],	 NULL,L"Paint true push status"	},
	{	L"AsmValChg",					TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoChkbBasePara)0)->AsmChgVal,	 NULL,L"assembly value change event code"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoChkbBase::Cls_GrP3DoChkbBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassChkbBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoChkbBase) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsPush		=	FALSE;
		m_IsVal			=	FALSE;

		GrDumyZeroMem( &m_ParaDoChkbBase, sizeof(m_ParaDoChkbBase) );
		//m_ParaDoChkbBase.PntBg	=	E_GrP3ScptPstIdDflt;

}
//--------------------------------------------------------------------
Cls_GrP3DoChkbBase::~Cls_GrP3DoChkbBase( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbBase::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassChkbBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoChkbBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoChkbBase, sizeof(m_ParaDoChkbBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoChkbBase)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoChkbBase, sizeof(m_ParaDoChkbBase) , m_PtrScpt , E_GrP3DoClassChkbBase );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoChkbBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiValueGet:
				LcValLoad();
				A_PtrInt[0]	=	(__s32)m_IsVal;
				break;
			case E_GrP3UpiValueSet:
				m_IsVal	=	(BOOL8)A_PtrInt[1];
				LcValSave();
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// reload value
		LcValLoad();
		// update status
		Tv_Stat	=	E_GrP3DoChkbBaseStatFalseDis;
		if ( m_IsVal )
		{
			Tv_Stat	=	E_GrP3DoChkbBaseStatTrueDis;
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
void	Cls_GrP3DoChkbBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoChkbBase, V_GrP3DoChkbBaseParaInfo, E_GrP3DoChkbBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoChkbBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3KeyEnter == A_Key )
		{
			m_IsPush	=	TRUE;
			ReqDraw();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoChkbBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3KeyEnter == A_Key )
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
BOOL8	Cls_GrP3DoChkbBase::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	TRUE;
		Cls_GrP3DoBase::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoChkbBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP3DoBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		ReqDraw();
		LcValLoad();
		m_IsVal	=	!m_IsVal;
		LcValSave();
		// do up assembly
		V_GrP3Mng->VmRunCode( m_ParaDoChkbBase.AsmChgVal, this );
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbBase::LcValSave( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbBase::LcNotifyValChg( void )
{
	// local -------------------
	// code --------------------
		// do up assembly
		V_GrP3Mng->VmRunCode( m_ParaDoChkbBase.AsmChgVal, this );
}
//--------------------------------------------------------------------
