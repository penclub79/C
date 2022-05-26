/*
 spin button text

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoSpinDgt.h>
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
Cls_GrUiDoSpinDgt::Cls_GrUiDoSpinDgt( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSpinBase( A_PtrRect, A_ObjPrnt, A_IsIdAlloc )
{
	// local -------------------
	// code --------------------
		//init
		m_TypeId				=	E_GrUiDoTypeIdSpbtnDgt;

		m_IsModeHex		=	FALSE;
		m_NextInpId		=	E_GrUiObjIdNone;
		m_Cipher			=	E_GrUiDoSpinDgtDfltCipher;
		m_ValMax			=	255;
		m_AreaCipherSize	=	0;
		m_AreaUpDnSize		=	0;

		GrStrIntToWstr( m_StrText, 0, m_Cipher, TRUE );
		m_Digit				=	0;
}
//--------------------------------------------------------------------
Cls_GrUiDoSpinDgt::~Cls_GrUiDoSpinDgt()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::ProcBgSkinTblDraw( void )
{
	// local -------------------
		BOOL8				Tv_IsCusOn;
	// code --------------------
		//inherited
		Cls_GrUiDoSpinBase::ProcBgSkinTblDraw();
		//update image
		//set clipper
		if ( PaintClipperSet( TRUE ) )
		{
			//draw text
			Tv_IsCusOn	=	m_IsCtrlMode;
			if ( (IsOnMouse() && (0 != m_AreaCipherSize)) || IsFocused() )
			{
				Tv_IsCusOn	=	TRUE;
			}
			TextDraw( m_StrText, &m_TxtPara, m_StatTxtClrTbl[m_SpbStatIdx], &m_PntRcClient, Tv_IsCusOn, (__s32)m_Digit );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Num;
	// code --------------------
		Tv_Result		=	FALSE;
		switch( A_Key )
		{
			case	E_GrUiKeyUp:
				Tv_Result	=	LcProcEvtKeyPushUp();
				break;
			case	E_GrUiKeyDown:
				Tv_Result	=	LcProcEvtKeyPushDown();
				break;
			case	E_GrUiKeyLeft:
				Tv_Result	=	LcProcEvtKeyPushLeft();
				break;
			case	E_GrUiKeyRight:
				Tv_Result	=	LcProcEvtKeyPushRight();
				break;
			case	E_GrUiKeyCh0:
			case	E_GrUiKeyCh1:
			case	E_GrUiKeyCh2:
			case	E_GrUiKeyCh3:
			case	E_GrUiKeyCh4:
			case	E_GrUiKeyCh5:
			case	E_GrUiKeyCh6:
			case	E_GrUiKeyCh7:
			case	E_GrUiKeyCh8:
			case	E_GrUiKeyCh9:
			case	E_GrUiKeyCh10:
				Tv_Num	=	(A_Key - E_GrUiKeyCh0);
				if ( 10 <= Tv_Num )
				{
					Tv_Num	=	0;
				}
				Tv_Result	=	LcProcEvtKeyPushNum( Tv_Num );
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::LcProcEvtKeyPushUp( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//check able
		if ( m_IsCtrlMode )
		{
			LcCusValInc();
			Tv_Result		=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::LcProcEvtKeyPushDown( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//check able
		if ( m_IsCtrlMode )
		{
			LcCusValDec();
			Tv_Result		=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::LcProcEvtKeyPushLeft( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8		Tv_Digit;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsCtrlMode )
		{
			if ( 0 != m_Digit )
			{
				Tv_Digit	=	m_Digit - 1;
				DigitSet( Tv_Digit );
			}
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::LcProcEvtKeyPushRight( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8		Tv_Digit;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsCtrlMode )
		{
			Tv_Digit	=	m_Digit + 1;
			if ( m_Cipher > Tv_Digit )
			{
				DigitSet( Tv_Digit );
			}
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::ProcEvtWheelUp( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_AreaCipherSize )
		{
			LcCusValInc();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::ProcEvtWheelDown( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_AreaCipherSize )
		{
			LcCusValDec();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSpinDgt::ValueGet( void )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result		=	0;

		if ( m_IsModeHex )
		{
			GrStrHexWstrToDword( m_StrText, &Tv_Result );
		}
		else
		{
			GrStrWstrToInt( m_StrText, (__s32*)&Tv_Result );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::ValueSet( __u32 A_Val, BOOL8 A_IsNotify )
{
	// local -------------------
		__u32	Tv_Val;
	// code --------------------
		Tv_Val	=	A_Val;
		if ( m_ValMax <= Tv_Val )
		{
			Tv_Val	=	m_ValMax;
		}
		if ( m_IsModeHex )
		{
			GrStrDwordToHexWstr( m_StrText, (__u32)Tv_Val, (__u32)m_Cipher, TRUE );
		}
		else
		{
			GrStrIntToWstr( m_StrText, (__s32)Tv_Val, (__u32)m_Cipher, TRUE );
		}
		if ( A_IsNotify )
		{
			LcNotifyValChg();
		}
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSpinDgt::DigitGet( void )
{
		return	m_Digit;		
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::DigitSet( __u8 A_Idx )
{
		if ( m_Cipher > A_Idx )
		{
			m_Digit		=	A_Idx;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::ModeHextSet( BOOL8 A_IsHex )
{
		m_IsModeHex	=	A_IsHex;
		m_Digit	=	0;
		if ( m_IsModeHex )
		{
			GrStrDwordToHexWstr( m_StrText, 0, (__u32)m_Cipher, TRUE );
		}
		else
		{
			GrStrIntToWstr( m_StrText, 0, (__u32)m_Cipher, TRUE );
		}
		
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::IsModeHex( void )
{
		return	m_IsModeHex;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect		Tv_Rc;
		__u8	Tv_Digit;
	// code --------------------
		//inherited
		Cls_GrUiDoSpinBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		Tv_Result	=	FALSE;

		//check mouse area
		if ( ScrRectGet( &Tv_Rc ) )
		{
			//check lbutton
			if ( 0 != m_AreaCipherSize )
			{
				if ( (Tv_Rc.left + (__s32)m_AreaCipherSize) > (__s32)A_X )
				{
					if ( 0 == m_Digit )
					{
						Tv_Digit	=	m_Cipher - 1;
					}
					else
					{
						Tv_Digit	=	m_Digit - 1;
					}
					DigitSet( Tv_Digit );
					Tv_Result	=	TRUE;
				}
				else if ( (Tv_Rc.right - (__s32)m_AreaCipherSize) < (__s32)A_X )
				{
					Tv_Digit	=	m_Digit + 1;
					if ( m_Cipher <= Tv_Digit )
					{
						Tv_Digit	=	0;
					}
					DigitSet( Tv_Digit );
					Tv_Result	=	TRUE;
				}
			}
			//check updown able
			if ( !Tv_Result )
			{
				if ( 0 != m_AreaUpDnSize )
				{
					if ( (Tv_Rc.top + (__s32)m_AreaUpDnSize) > (__s32)A_Y )
					{
						LcCusValInc();
						Tv_Result	=	TRUE;
					}
					else if ( (Tv_Rc.bottom - (__s32)m_AreaUpDnSize) < (__s32)A_Y )
					{
						LcCusValDec();
						Tv_Result	=	TRUE;
					}
				}
			}
		}
		//check virtual keyboard
		if ( !Tv_Result )
		{
			if ( !m_IsModeHex )
			{
				VirtKbdCreate( TRUE, FALSE, 0, m_StrText, m_Cipher );
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinDgt::LcProcEvtKeyPushNum( __u8 A_Num )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Digit;
		BOOL8	Tv_IsDigitOver;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_IsDigitOver	=	FALSE;

		if ( 9 >= A_Num )
		{
			m_StrText[m_Digit]	=	(WCHAR)A_Num + L'0';
			//next digit
			Tv_Digit	=	m_Digit + 1;
			if ( m_Cipher <= Tv_Digit )
			{
				Tv_Digit	=	0;
				Tv_IsDigitOver	=	TRUE;
			}
			DigitSet( Tv_Digit );
			//check value is over
			LcCorrectValOver();
			//notify
			LcNotifyValChg();
			//success
			Tv_Result	=	TRUE;
		}

		//check digit over
		if ( Tv_IsDigitOver )
		{
			LcGoNextInput();
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::LcCorrectValOver( void )
{
	// local -------------------
		__u32	Tv_Val;
	// code --------------------
		if ( m_IsModeHex )
		{
			GrStrHexWstrToDword( m_StrText, &Tv_Val );
			if ( m_ValMax < Tv_Val )
			{
				Tv_Val	=	m_ValMax;
				GrStrDwordToHexWstr( m_StrText, Tv_Val, m_Cipher, TRUE );
			}
		}
		else
		{
			GrStrWstrToInt( m_StrText, (__s32*)&Tv_Val );
			if ( m_ValMax < Tv_Val )
			{
				Tv_Val	=	m_ValMax;
				GrStrIntToWstr( m_StrText, Tv_Val, m_Cipher, TRUE );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::ProcEvtVkbd( __u32 A_VkId	)
{
	// local -------------------
		__u32	Tv_Val;
	// code --------------------
		//currect value
		if ( m_IsModeHex )
		{
			GrStrHexWstrToDword( m_StrText, &Tv_Val );
			if ( m_ValMax < Tv_Val )
			{
				Tv_Val	=	m_ValMax;
			}
			GrStrDwordToHexWstr( m_StrText, Tv_Val, m_Cipher, TRUE );
		}
		else
		{
			GrStrWstrToInt( m_StrText, (__s32*)&Tv_Val );
			if ( m_ValMax < Tv_Val )
			{
				Tv_Val	=	m_ValMax;
			}
			GrStrIntToWstr( m_StrText, Tv_Val, m_Cipher, TRUE );
		}
		//update change value
		LcNotifyValChg();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::LcGoNextInput( void )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_ObjNext;
	// code --------------------
		if ( E_GrUiObjIdNone != m_NextInpId )
		{
			//get next object
			Tv_ObjNext	=	m_ObjWin->FindSlaveObjByObjId( m_NextInpId );
			if ( NULL != Tv_ObjNext )
			{
				//change foucs
				m_ObjConsole->FocusObjSet( Tv_ObjNext );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::ProcEvtFocusGet( void )
{
		m_Digit		=	0;
		//inheritd
		Cls_GrUiDoSpinBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::NextInputIdSet( __u16 A_ObjId )
{
		m_NextInpId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoSpinDgt::NextInputIdGet( void )
{
		return	m_NextInpId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::CipherSet( __u8 A_Cipher )
{
	// local -------------------
	// code --------------------
		if ( 0 != A_Cipher )
		{
			m_Cipher		=	A_Cipher;
			GrStrIntToWstr( m_StrText, 0, m_Cipher, TRUE );
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSpinDgt::CipherGet( void )
{
		return	m_Cipher;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::AreaUpDownSizeSet( __u8 A_Size )
{
		m_AreaUpDnSize	=	A_Size;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSpinDgt::AreaUpDownSizeGet( void )
{
		return	m_AreaUpDnSize;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::AreaCipherSizeSet( __u8 A_Size )
{
		m_AreaCipherSize	=	A_Size;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSpinDgt::AreaCipherSizeGet( void )
{
		return	m_AreaCipherSize;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::LcCusValInc( void )
{
	// local -------------------
	// code --------------------
		//check able
		m_StrText[m_Digit] ++;
		if ( m_IsModeHex )
		{
			if ( 0x3A == m_StrText[m_Digit] )
			{
				m_StrText[m_Digit]	=	L'A';
			}
			else if ( 0x47 == m_StrText[m_Digit] )
			{
				m_StrText[m_Digit]	=	L'0';
			}
		}
		else
		{
			if ( 0x3A == m_StrText[m_Digit] )
			{
				m_StrText[m_Digit]	=	L'0';
			}
		}
		//check value over
		LcCorrectValOver();
		//notify change
		LcNotifyValChg();
		//redraw
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinDgt::LcCusValDec( void )
{
	// local -------------------
	// code --------------------
		//check able
		m_StrText[m_Digit] --;
		if ( m_IsModeHex )
		{
			if ( 0x2F == m_StrText[m_Digit] )
			{
				m_StrText[m_Digit]	=	L'F';
			}
			else if ( 0x40 == m_StrText[m_Digit] )
			{
				m_StrText[m_Digit]	=	L'9';
			}
		}
		else
		{
			if ( 0x2F == m_StrText[m_Digit] )
			{
				m_StrText[m_Digit]	=	L'9';
			}
		}
		//check value over
		LcCorrectValOver();
		//notify change
		LcNotifyValChg();
		//redraw
		ReqDraw();
}
//--------------------------------------------------------------------
void		Cls_GrUiDoSpinDgt::ValMaxSet( __u32 A_Val )
{
		m_ValMax	=	A_Val;
		LcCorrectValOver();
}
//--------------------------------------------------------------------
__u32		Cls_GrUiDoSpinDgt::ValMaxGet( void )
{
		return	m_ValMax;
}
//--------------------------------------------------------------------


