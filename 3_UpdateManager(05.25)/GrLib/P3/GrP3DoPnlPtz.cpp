/*
		platform panel PTZ

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P3/GrP3DoPnlPtz.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P3/GrP3Script.h>
#include <P3/GrP3ConBase.h>
#include <P3/GrP3AsmBase.h>
#include <P3/GrP3Mng.h>
#include <P3/GrP3UiCmd.h>

#include <P3/GrP3Cfg.h>

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

#define E_GrP3DoPnlPtzParaCnt		3

St_GrP3DvlDoPrInfoData	V_GrP3DoPnlPtzParaInfo[E_GrP3DoPnlPtzParaCnt]	=	
{
	{	L"Paint Pan tilt",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoPnlPtzPara)0)->PntBg[E_GrP3DoPnlPtzPantilt],  NULL,	L"Paint PAN TILT mode"	},
	{	L"Paint Zoom",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoPnlPtzPara)0)->PntBg[E_GrP3DoPnlPtzZoom],	 NULL,L"Paint ZOOM mode"	},
	{	L"Paint Focus",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoPnlPtzPara)0)->PntBg[E_GrP3DoPnlPtzFocus],	 NULL,L"Paint FOCUS mode"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoPnlPtz::Cls_GrP3DoPnlPtz( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassPnlPtz;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaPnlPtz) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		//m_Ch				=	0;
		m_PtzMode		=	E_GrP3DoPnlPtzPantilt;
		m_StrTitle	=	(WCHAR*)V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsCamTitle );

		m_MseMode		=	E_GrP3DoPnlPtzMseStop;

		m_IsKeyLeft		=	FALSE;
		m_IsKeyRight	=	FALSE;
		m_IsKeyDown		=	FALSE;
		m_IsKeyUp			=	FALSE;

		GrDumyZeroMem( &m_ParaPnlPtz, sizeof(m_ParaPnlPtz) );
		//m_ParaPnlPtz.PntBg	=	E_GrP3ScptPstIdDflt;

		m_PtzMng		=	V_GrP3Mng->PtzMngGet();

		m_PtrGvs		=	V_GrP3Mng->GvsPtrGet();
		((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;
		((Ptr_GrP3MngGvs)m_PtrGvs)->IsPtzCtrlLock	=	FALSE;	// reset lock

}
//--------------------------------------------------------------------
Cls_GrP3DoPnlPtz::~Cls_GrP3DoPnlPtz( void )
{
	// local -------------------
	// code --------------------
		((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlMode		=	E_GrP3MngPtzModeNone;
		((Ptr_GrP3MngGvs)m_PtrGvs)->IsPtzCtrlLock	=	FALSE;

		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPnlPtz::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassPnlPtz;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaPnlPtz);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaPnlPtz, sizeof(m_ParaPnlPtz) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaPnlPtz)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPnlPtz::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaPnlPtz, sizeof(m_ParaPnlPtz) , m_PtrScpt , E_GrP3DoClassPnlPtz );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPnlPtz::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiChGet:
				A_PtrInt[0]	=	(__s32)((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh;
				break;
			case E_GrP3UpiChSet:
				if ( V_GrP3Mng->VdoMaxChCntGet() > (__u8)A_PtrInt[1] )
				{
					((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh	=	(__u8)A_PtrInt[1];
					ReqDraw();
				}
				break;
			case E_GrP3UpiValueGet:
				A_PtrInt[0]	=	(__s32)m_PtzMode;
				break;
			case E_GrP3UpiValueSet:
				if ( E_GrP3DoPnlPtzFocus >= (__u8)A_PtrInt[1] )
				{
					m_PtzMode	=	(__u8)A_PtrInt[1];
					ReqDraw();
					// update PTZ control mode
					((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;
				}
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPnlPtz::ProcPaint( void )
{
	// local -------------------
	// code --------------------
		
		PaintScriptPush( m_ParaPnlPtz.PntBg[m_PtzMode], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPnlPtz::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaPnlPtz, V_GrP3DoPnlPtzParaInfo, E_GrP3DoPnlPtzParaCnt );
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPnlPtz::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_PtzMng )
		{
			Tv_Result	=	TRUE;
			if ( E_GrP3KeyLeft == A_Key )
			{
				m_IsKeyLeft	=	TRUE;
				switch ( m_PtzMode )
				{
					case E_GrP3DoPnlPtzZoom:
						m_PtzMng->SafeDoZoom( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, TRUE, E_GrPtzV2DfltSpeed, 0 );
						break;
					case E_GrP3DoPnlPtzFocus:
						m_PtzMng->SafeDoFocus( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, FALSE, E_GrPtzV2DfltSpeed, 0 );
						break;
					default:
						m_PtzMng->SafeDoPanTilt( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, 
							m_IsKeyLeft, m_IsKeyRight, m_IsKeyUp, m_IsKeyDown, E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );
						break;
				}
			}
			else if ( E_GrP3KeyRight == A_Key )
			{
				m_IsKeyRight	=	TRUE;
				switch ( m_PtzMode )
				{
					case E_GrP3DoPnlPtzZoom:
						m_PtzMng->SafeDoZoom( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, FALSE, E_GrPtzV2DfltSpeed, 0 );
						break;
					case E_GrP3DoPnlPtzFocus:
						m_PtzMng->SafeDoFocus( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, TRUE, E_GrPtzV2DfltSpeed, 0 );
						break;
					default:
						m_PtzMng->SafeDoPanTilt( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, 
							m_IsKeyLeft, m_IsKeyRight, m_IsKeyUp, m_IsKeyDown, E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );
						break;
				}
			}
			else if ( E_GrP3KeyUp == A_Key )
			{
				m_IsKeyUp	=	TRUE;
				switch ( m_PtzMode )
				{
					case E_GrP3DoPnlPtzZoom:
						m_PtzMng->SafeDoZoom( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, FALSE, E_GrPtzV2DfltSpeed, 0 );
						break;
					case E_GrP3DoPnlPtzFocus:
						m_PtzMng->SafeDoFocus( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, TRUE, E_GrPtzV2DfltSpeed, 0 );
						break;
					default:
						m_PtzMng->SafeDoPanTilt( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, 
							m_IsKeyLeft, m_IsKeyRight, m_IsKeyUp, m_IsKeyDown, E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );
						break;
				}
			}
			else if ( E_GrP3KeyDown == A_Key )
			{
				m_IsKeyDown	=	TRUE;
				switch ( m_PtzMode )
				{
					case E_GrP3DoPnlPtzZoom:
						m_PtzMng->SafeDoZoom( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, TRUE, E_GrPtzV2DfltSpeed, 0 );
						break;
					case E_GrP3DoPnlPtzFocus:
						m_PtzMng->SafeDoFocus( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, FALSE, E_GrPtzV2DfltSpeed, 0 );
						break;
					default:
						m_PtzMng->SafeDoPanTilt( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, 
							m_IsKeyLeft, m_IsKeyRight, m_IsKeyUp, m_IsKeyDown, E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );
						break;
				}
			}
			else
			{
				Tv_Result	=	FALSE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPnlPtz::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Mode;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP3KeyEnter == A_Key )
		{
			Tv_Mode	=	m_PtzMode + 1;
			if ( E_GrP3DoPnlPtzFocus < Tv_Mode )
			{
				Tv_Mode	=	E_GrP3DoPnlPtzPantilt;
			}
			m_PtzMode	=	Tv_Mode;
			ReqDraw();
			// update PTZ control mode
			((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;

			Tv_Result	=	TRUE;
		}
		else if ( (E_GrP3KeyLeft <= A_Key) && (E_GrP3KeyDown >= A_Key)  )
		{
			switch ( A_Key )
			{
				case E_GrP3KeyLeft:
					m_IsKeyLeft	=	FALSE;
					break;
				case E_GrP3KeyRight:
					m_IsKeyRight	=	FALSE;
					break;
				case E_GrP3KeyUp:
					m_IsKeyUp		=	FALSE;
					break;
				case E_GrP3KeyDown:
					m_IsKeyDown	=	FALSE;
					break;
			}
			// check stop
			if ( (!m_IsKeyLeft) && (!m_IsKeyRight) && (!m_IsKeyUp) && (!m_IsKeyDown) )
			{
				m_PtzMng->SafeDoStop( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh );
			}
			else
			{
				if ( E_GrP3DoPnlPtzPantilt == m_PtzMode )
				{
					m_PtzMng->SafeDoPanTilt( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, 
						m_IsKeyLeft, m_IsKeyRight, m_IsKeyUp, m_IsKeyDown, E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );

				}
			}
			
		}
		

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPnlPtz::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		LcMsePtzRun( A_X, A_Y );
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPnlPtz::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		if ( NULL != m_PtzMng )
		{
			m_PtzMng->SafeDoStop( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh );
			m_MseMode	=	E_GrP3DoPnlPtzMseStop;
		}
		return	TRUE;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoPnlPtz::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u32		Tv_Pos;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			if ( E_GrDvrMaxChCnt > ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh )
			{
				Tv_Pos	=	E_GrP3CfgOfsCamTitle + ( ((__u32)((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh) << 5 );
				Tv_Result	=	(WCHAR*)( (__u32)m_StrTitle + Tv_Pos );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPnlPtz::LcMsePtzRun( __u16 A_X, __u16 A_Y )
{
	// local -------------------
		BOOL8	Tv_IsLeft;
		BOOL8	Tv_IsRight;
		BOOL8	Tv_IsUp;
		BOOL8	Tv_IsDown;
		__u8	Tv_SpdX;
		__u8	Tv_SpdY;


		__s32		Tv_DltX;
		__s32		Tv_DltY;
		St_GrRect	Tv_RcScr;
		__s32		Tv_CtX;
		__s32		Tv_CtY;
		BOOL8	Tv_IsStop;
	// code --------------------
		if ( NULL != m_PtzMng )
		{
			// calculate speed
			if ( ScrRectGet( &Tv_RcScr ) )
			{
				// init
				Tv_IsLeft		=	FALSE;
				Tv_IsRight	=	FALSE;
				Tv_IsUp			=	FALSE;
				Tv_IsDown		=	FALSE;
				Tv_SpdX			=	0;
				Tv_SpdY			=	0;
				Tv_IsStop		=	FALSE;

				// get center
				Tv_CtX	=	Tv_RcScr.left + ((Tv_RcScr.right - Tv_RcScr.left) / 2);
				Tv_CtY	=	Tv_RcScr.top + ((Tv_RcScr.bottom - Tv_RcScr.top) / 2);
				// calculate delta X
				Tv_DltX			=	0;
				if ( (__s32)A_X < Tv_CtX )
				{
					Tv_DltX		=	Tv_CtX - (__s32)A_X;
					Tv_IsLeft	=	TRUE;
				}
				else if ( (__s32)A_X > Tv_CtX )
				{
					Tv_DltX		=	(__s32)A_X - Tv_CtX;
					Tv_IsRight	=	TRUE;
				}
				Tv_DltX	=	(Tv_DltX * 4) / ( (Tv_RcScr.right - Tv_RcScr.left) / 2 );
				Tv_SpdX	=	(__u8)Tv_DltX;
				if ( 0 == Tv_SpdX )
				{
					Tv_IsStop		=	TRUE;
					Tv_IsLeft		=	FALSE;
					Tv_IsRight	=	FALSE;
				}
				// calculate delta Y
				if ( E_GrP3DoPnlPtzPantilt == m_PtzMode )
				{
					Tv_DltY			=	0;
					if ( (__s32)A_Y < Tv_CtY )
					{
						Tv_DltY		=	Tv_CtY - (__s32)A_Y;
						Tv_IsUp		=	TRUE;
					}
					else if ( (__s32)A_Y > Tv_CtY )
					{
						Tv_DltY		=	(__s32)A_Y - Tv_CtY;
						Tv_IsDown	=	TRUE;
					}
					Tv_DltY	=	(Tv_DltY * 4) / ( (Tv_RcScr.bottom - Tv_RcScr.top) / 2 );
					Tv_SpdY	=	(__u8)Tv_DltY;
					if ( 0 == Tv_SpdY )
					{
						Tv_IsUp		=	FALSE;
						Tv_IsDown	=	FALSE;
					}
					else
					{
						Tv_IsStop		=	FALSE;
					}
				}
				// check mode
				if ( Tv_IsStop )
				{
					if ( E_GrP3DoPnlPtzMseStop != m_MseMode )
					{
						m_PtzMng->SafeDoStop( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh );
						m_MseMode	=	E_GrP3DoPnlPtzMseStop;
					}
				}
				else
				{
					// check change
					if (	(m_MseMode != m_PtzMode) || (m_MseSpdX != Tv_SpdX) || (m_MseSpdY != Tv_SpdY) || 
								(m_IsMseLeft != Tv_IsLeft) || (m_IsMseRight != Tv_IsRight) || (m_IsMseUp != Tv_IsUp) || (m_IsMseDown != Tv_IsDown)	)
					{
						switch ( m_PtzMode )
						{
							case E_GrP3DoPnlPtzZoom:
								m_PtzMng->SafeDoZoom( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, Tv_IsLeft, Tv_SpdX, 0 );
								break;
							case E_GrP3DoPnlPtzFocus:
								m_PtzMng->SafeDoFocus( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, Tv_IsLeft, Tv_SpdX, 0 );
								break;
							default:
								m_PtzMng->SafeDoPanTilt( ((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlCh, Tv_IsLeft, Tv_IsRight, Tv_IsUp, Tv_IsDown, Tv_SpdX, Tv_SpdY, 0 );
									break;
						}
						// update
						m_MseMode			=	m_PtzMode;
						m_MseSpdX			=	Tv_SpdX;
						m_MseSpdY			=	Tv_SpdY;
						m_IsMseLeft		=	Tv_IsLeft;
						m_IsMseRight	=	Tv_IsRight;
						m_IsMseUp			=	Tv_IsUp;
						m_IsMseDown		=	Tv_IsDown;
					}
				}
			}
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPnlPtz::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		if ( 0 != (E_GrP3MouseBmkkLeft & A_BtnMap) )
		{
			LcMsePtzRun( A_X, A_Y );
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPnlPtz::ProcEvtWheelUp( void )
{
	// local -------------------
		__u8	Tv_Mode;
	// code --------------------
		// check able mode
		//if ( E_GrP3DoPnlPtzMseStop == m_MseMode )
		{
			Tv_Mode	=	m_PtzMode + 1;
			if ( E_GrP3DoPnlPtzFocus < Tv_Mode )
			{
				Tv_Mode	=	E_GrP3DoPnlPtzPantilt;
			}
			m_PtzMode	=	Tv_Mode;
			ReqDraw();
			// update PTZ control mode
			((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPnlPtz::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		// check able mode
		//if ( E_GrP3DoPnlPtzMseStop == m_MseMode )
		{
			if ( E_GrP3DoPnlPtzPantilt == m_PtzMode )
			{
				m_PtzMode	=	E_GrP3DoPnlPtzFocus;
			}
			else
			{
				m_PtzMode --;
			}
			ReqDraw();
			// update PTZ control mode
			((Ptr_GrP3MngGvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;
		}

		return	FALSE;
}
//--------------------------------------------------------------------
