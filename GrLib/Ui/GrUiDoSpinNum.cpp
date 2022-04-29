/*
 spin number button

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoSpinNum.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>

//====================================================================
//local const
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoSpinNum::Cls_GrUiDoSpinNum( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSpinBase( A_PtrRect, A_ObjPrnt, A_IsIdAlloc )
{
	// local -------------------
	// code --------------------
		//init
		m_TypeId				=	E_GrUiDoTypeIdSpbtnNum;

		m_RngMax			=	E_GrUiDoSpinNumMaxDflt;
		m_RngMin			=	E_GrUiDoSpinNumMinDflt;
		m_Value				=	0;
		m_Step				=	1;
		m_TxtIdZero		=	E_GrUiTxtIdNone;
		m_TxtIdPrefix	=	E_GrUiTxtIdNone;
		m_TxtIdSuffix	=	E_GrUiTxtIdNone;
		m_IsVertical	=	FALSE;
		m_Cipher			=	0;
		m_CenterSize	=	0;
		GrStrIntToWstr( m_StrText, m_Value );
}
//--------------------------------------------------------------------
Cls_GrUiDoSpinNum::~Cls_GrUiDoSpinNum()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::ProcBgSkinTblDraw( void )
{
	// local -------------------
		WCHAR*	Tv_StrTxt;
	// code --------------------
		//inherited
		Cls_GrUiDoSpinBase::ProcBgSkinTblDraw();
		//set clipper
		if ( PaintClipperSet( TRUE ) )
		{
			//check value
			if ( (0 == m_Value) && (E_GrUiTxtIdNone != m_TxtIdZero) )
			{
				Tv_StrTxt	=	V_GrUiMngObj->TextPtrGet( m_TxtIdZero );
				if ( NULL != Tv_StrTxt )
				{
					TextDraw( Tv_StrTxt, &m_TxtPara, m_StatTxtClrTbl[m_SpbStatIdx], &m_PntRcClient );
				}
			}
			else
			{
				//draw text
				TextDraw( m_StrText, &m_TxtPara, m_StatTxtClrTbl[m_SpbStatIdx], &m_PntRcClient );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinNum::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;
		switch( A_Key )
		{
			case	E_GrUiKeyRight:
				if ( !m_IsVertical )
				{
					Tv_Result	=	LcProcEvtKeyPushUp();
				}
				break;
			case	E_GrUiKeyLeft:
				if ( !m_IsVertical )
				{
					Tv_Result	=	LcProcEvtKeyPushDown();
				}
				break;
			case	E_GrUiKeyUp:
				if ( m_IsVertical )
				{
					Tv_Result	=	LcProcEvtKeyPushUp();
				}
				break;
			case	E_GrUiKeyDown:
				if ( m_IsVertical )
				{
					Tv_Result	=	LcProcEvtKeyPushDown();
				}
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinNum::LcProcEvtKeyPushUp( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//check able
		if ( m_IsCtrlMode )
		{
			LcValueUp();
			Tv_Result		=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinNum::LcProcEvtKeyPushDown( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//check able
		if ( m_IsCtrlMode )
		{
			LcValueDown();
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::LcValueUp( void )
{
	// local -------------------
		__s32		Tv_Value;
	// code --------------------
		Tv_Value	=	m_Value + m_Step;
		if ( m_RngMax < Tv_Value )
		{
			Tv_Value	=	m_RngMin;
		}
		ValueSet( Tv_Value );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::LcValueDown( void )
{
	// local -------------------
		__s32		Tv_Value;
	// code --------------------
		Tv_Value	=	m_Value - m_Step;
		if ( m_RngMin > Tv_Value )
		{
			Tv_Value	=	m_RngMax;
		}
		ValueSet( Tv_Value );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::ValueSet( __s32 A_Val, BOOL8 A_IsNotify )
{
	// local -------------------
	// code --------------------
		//update value
		m_Value	=	A_Val;
		if ( m_RngMin > m_Value )
		{
			m_Value	=	m_RngMin;
		}
		if ( m_RngMax < m_Value )
		{
			m_Value	=	m_RngMax;
		}
		//change contents
		LcValTextUpdt();

		//notify message
		if ( A_IsNotify )
		{
			LcNotifyValChg();
		}
		//redraw
		ReqDraw();
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSpinNum::ValueGet( void )
{
		return	m_Value;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::StepSet( __s32 A_Step )
{
		if ( 0 < A_Step  )
		{
			m_Step		=	A_Step;
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSpinNum::StepGet( void )
{
		return	m_Step;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::RangeMaxSet( __s32 A_Max )
{
		m_RngMax		=	A_Max;
		if ( m_Value > m_RngMax )
		{
			ValueSet( m_RngMax );
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSpinNum::RangeMaxGet( void )
{
		return	m_RngMax;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::RangeMinSet( __s32 A_Min )
{
		m_RngMin	=	A_Min;
		if ( m_Value < m_RngMin )
		{
			ValueSet( m_RngMin );
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSpinNum::RangeMinGet( void )
{
		return	m_RngMin;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinNum::ProcEvtWheelUp( void )
{
		LcValueUp();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinNum::ProcEvtWheelDown( void )
{
		LcValueDown();
		return	TRUE;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoSpinNum::ZeroTxtIdSet( __u32 A_TxtId )
{
		m_TxtIdZero	=	A_TxtId;
		ReqDraw();
}
//--------------------------------------------------------------------
__u32		Cls_GrUiDoSpinNum::ZeroTxtIdGet( void )
{
		return	m_TxtIdZero;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::PrefixTxtIdSet( __u32 A_TxtId )
{
		m_TxtIdPrefix	=	A_TxtId;
		LcValTextUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSpinNum::PrefixTxtIdGet( void )
{
		return	m_TxtIdPrefix;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::SuffixTxtIdSet( __u32 A_TxtId )
{
		m_TxtIdSuffix	=	A_TxtId;
		LcValTextUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSpinNum::SuffixTxtIdGet( void )
{
		return	m_TxtIdSuffix;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinNum::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect		Tv_Rc;
		__s32		Tv_PosUp;
		__s32		Tv_PosDn;
	// code --------------------
		//inherited
		Cls_GrUiDoSpinBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		
		//check position
		//if ( ScrRectGet( &Tv_Rc ) )
		if ( ClientRectGet( &Tv_Rc ) )
		{
			if ( m_IsVertical )
			{
				//vertical type
				Tv_PosUp		=	((Tv_Rc.bottom - Tv_Rc.top) >> 1) + Tv_Rc.top - (__s32)m_CenterSize;
				Tv_PosDn		=	Tv_PosUp + (__s32)(m_CenterSize << 1);
				if ( Tv_PosDn < A_Y )
				{
					LcValueDown();
				}
				else if ( Tv_PosUp >= A_Y )
				{
					LcValueUp();
				}
				else if ( 0 != m_CenterSize )
				{
					//make click event
					LcNotifyClick();
				}
			}
			else
			{
				//horizontal type
				Tv_PosUp		=	((Tv_Rc.right - Tv_Rc.left) >> 1) + Tv_Rc.left - (__s32)m_CenterSize;
				Tv_PosDn		=	Tv_PosUp + (__s32)(m_CenterSize << 1);
				if ( Tv_PosUp >= A_X )
				{
					LcValueDown();
				}
				else if ( Tv_PosDn < A_X )
				{
					LcValueUp();
				}
				else if (0 != m_CenterSize)
				{
					//make click event
					LcNotifyClick();
				}
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::VerticalTypeSet( BOOL8 A_IsUse )
{
		m_IsVertical	=	A_IsUse;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinNum::IsVerticalType( void )
{
		return	m_IsVertical;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::CipherSet( __u8 A_Val )
{
	// local -------------------
	// code --------------------
		m_Cipher	=	A_Val;
		//cange contents
		LcValTextUpdt();
		//redraw
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSpinNum::CipherGet( void )
{
		return	m_Cipher;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::LcValTextUpdt( void )
{
	// local -------------------
		BOOL8	Tv_IsFix;
		WCHAR*	Tv_StrWk;
		WCHAR*	Tv_StrAdd;
	// code --------------------
		Tv_StrWk	=	m_StrText;
		//add prefix
		if ( E_GrUiTxtIdNone != m_TxtIdPrefix )
		{
			Tv_StrAdd	=	V_GrUiMngObj->TextPtrGet( m_TxtIdPrefix );
			if ( NULL != Tv_StrAdd )
			{
				Tv_StrWk	=	GrStrWcopy( Tv_StrWk, Tv_StrAdd, 12 );
			}
		}
		
		Tv_IsFix	=	FALSE;
		if ( 0 != m_Cipher )
		{
			Tv_IsFix	=	TRUE;
		}
		GrStrIntToWstr( Tv_StrWk, m_Value, (__u32)m_Cipher, Tv_IsFix );

		//add suffix
		if ( E_GrUiTxtIdNone != m_TxtIdSuffix )
		{
			Tv_StrAdd	=	V_GrUiMngObj->TextPtrGet( m_TxtIdSuffix );
			if ( NULL != Tv_StrAdd )
			{
				GrStrWcat( Tv_StrWk, Tv_StrAdd, 12 );
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSpinNum::CenterSizeGet( void )
{
		return	m_CenterSize;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::CenterSizeSet( __u32 A_Size )
{
		m_CenterSize	=	A_Size;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinNum::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;	
		switch( A_Key )
		{
			case	E_GrUiKeyEnter:
				if ( m_IsOnKeyCtrl && (0 != m_CenterSize) )
				{
					LcNotifyClick();
					Tv_Result	=	TRUE;
				}
				else
				{
					Tv_Result	=	LcProcEvtKeyPullEnter();
				}
				break;
		}
		if ( !Tv_Result )
		{
			Tv_Result	=	Cls_GrUiDoSpinBase::ProcEvtKeyUp( A_Key );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinNum::ProcEvtGlbStatChg( __u8 A_Type )
{
	// local -------------------
	// code --------------------
		if ( E_GrUiChgLanguage == A_Type )
		{
			LcValTextUpdt();	//	language change
		}
		// inherited
		Cls_GrUiDoSpinBase::ProcEvtGlbStatChg( A_Type );
}
//--------------------------------------------------------------------

