/*
		platform edit setup string

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include <GrDebug.h>
#include	<P4/GrP4DoEdtBase.h>
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

#define E_GrP4DoEdtBaseParaCnt		10

St_GrP4DvlDoPrInfoData	V_GrP4DoEdtBaseParaInfo[E_GrP4DoEdtBaseParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->PntStat[E_GrP4DoEdtBaseStatDisable],NULL,		L"Paint disable status"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->PntStat[E_GrP4DoEdtBaseStatNoraml],NULL,		L"Paint normal status"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->PntStat[E_GrP4DoEdtBaseStatFocus],NULL,	L"Paint focus status"	},
	{	L"Max length",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->MaxLen,NULL,	L"text max length"	},
	{	L"IsResetOver",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->IsResetOver,NULL,	L"when overflow input text, do reset"	},

	{	L"IsKeyVkbd",					TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->IsKeyVkbd,NULL,	L"virtual keyboard show by enter key"	},
	{	L"IsVkbdNum",					TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->IsVkbdNum,NULL,	L"virtual keyboard number type"	},
	{	L"IsPassword",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->IsPasswd,NULL,	L"hid input text"	},
	{	L"IsAdvPasswd",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->IsAdvPasswd,NULL,	L"Advance password"	},
	{	L"AsmVkbdEdtEnd",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoEdtBasePara)0)->AsmVkbdEdtEnd,NULL,	L"Virtual Keyboard Edit End Fucntion" },
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoEdtBase::Cls_GrP4DoEdtBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt ):
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassEdtBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaEdtBase) + sizeof(St_GrP4ScptParaHd);

		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaEdtBase, sizeof(m_ParaEdtBase) );
		
}
//--------------------------------------------------------------------
Cls_GrP4DoEdtBase::~Cls_GrP4DoEdtBase( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoEdtBase::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassEdtBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaEdtBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaEdtBase, sizeof(m_ParaEdtBase) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaEdtBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoEdtBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaEdtBase, sizeof(m_ParaEdtBase) , m_PtrScpt , E_GrP4DoClassEdtBase );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoEdtBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Tidx;
		WCHAR*	Tv_StrVal;
		WCHAR*	Tv_StrWk;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiTextGet:
				Tv_Tidx	=	A_PtrInt[1];
				if ( (0 <= Tv_Tidx) && (E_GrP4AsmMaxTxtRegCnt > Tv_Tidx) )
				{
					Tv_StrVal	=	LcValAdrGet();
					if ( NULL != Tv_StrVal )
					{
						GrStrWcopy( A_PtrStr[Tv_Tidx], Tv_StrVal );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DoEdtBase::RtlCommand - [UPI:TextGet] text register index over.\n" );
				}
				break;
			case E_GrP4UpiTextSet:
				Tv_Tidx	=	A_PtrInt[1];
				if ( (0 <= Tv_Tidx) && (E_GrP4AsmMaxTxtRegCnt > Tv_Tidx) )
				{
					Tv_StrVal	=	LcValAdrGet();
					if ( NULL != Tv_StrVal )
					{
						GrStrWcopy( Tv_StrVal, A_PtrStr[Tv_Tidx] );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DoEdtBase::RtlCommand - [UPI:TextSet] text register index over.\n" );
				}
				break;
			case E_GrP4UpiTextIdSet:
				Tv_StrWk	=	V_GrP4Mng->TextPtrGet((__u16)A_PtrInt[1]);
				if (NULL != Tv_StrWk)
				{
					Tv_StrVal	=	LcValAdrGet();
					if (NULL != Tv_StrVal)
					{
						GrStrWcopy(Tv_StrVal, Tv_StrWk);
					}
				}
				break;
				
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoEdtBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP4DoEdtBaseStatDisable;
		// check disable
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoEdtBaseStatNoraml;
			if ( m_IsOnMouse || IsFocused() )
			{
				Tv_Stat	=	E_GrP4DoEdtBaseStatFocus;
			}
		}
		PaintScriptPush( m_ParaEdtBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoEdtBase::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaEdtBase, V_GrP4DoEdtBaseParaInfo, E_GrP4DoEdtBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoEdtBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	LcValAdrGet();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoEdtBase::LcVkbdRun( void )
{
	// local -------------------
		WCHAR*	Tv_StrVal;
	// code --------------------

		Tv_StrVal	=	LcValAdrGet();
		if ( NULL != Tv_StrVal )
		{
			V_GrP4Mng->VkbdRun( this, Tv_StrVal, m_ParaEdtBase.MaxLen, 
				m_ParaEdtBase.IsPasswd, m_ParaEdtBase.IsVkbdNum, m_ParaEdtBase.IsAdvPasswd );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoEdtBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_StrTxt;
		__u32	Tv_Len;
		WCHAR	Tv_StrIns[2];
	// code --------------------
		Tv_Result	=	FALSE;

		if ( (E_GrP4KeyCh0 <= A_Key) && (E_GrP4KeyCh9 >= A_Key ) )
		{
			Tv_StrTxt	=	LcValAdrGet();
			if ( NULL != Tv_StrTxt )
			{
				Tv_Len	=	GrStrWlen( Tv_StrTxt );
				if ( Tv_Len < m_ParaEdtBase.MaxLen )
				{
					Tv_StrIns[0]	=	0x30 + (WCHAR)(A_Key - E_GrP4KeyCh0);
					Tv_StrIns[1]	=	0;
					GrStrWcat( Tv_StrTxt, Tv_StrIns );

					ProcValChg();
					ReqDraw();
					Tv_Result	=	TRUE;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoEdtBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( !m_ParaDoBase.IsKeyBlock )
		{
			if ( E_GrP4KeyEnter == A_Key )
			{
				if ( m_ParaEdtBase.IsKeyVkbd )
				{
					LcVkbdRun();
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoEdtBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		LcVkbdRun();
		return	TRUE;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoEdtBase::LcValAdrGet( void )
{
		return	NULL;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoEdtBase::ProcEvtVkbdEditEnd( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtVkbdEditEnd();
		ProcValChg();

		if(E_GrP4AsmIdNone != m_ParaEdtBase.AsmVkbdEdtEnd)
		{
			V_GrP4Mng->VmRunCode(m_ParaEdtBase.AsmVkbdEdtEnd, this);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoEdtBase::ProcValChg( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
