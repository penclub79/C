/*
		platform panel PTZ

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P4/GrP4DoPnlPtz.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4ConBase.h>
#include <P4/GrP4AsmBase.h>
#include <P4/GrP4Mng.h>
#include <P4/GrP4UiCmd.h>

#include <P4/GrP4Cfg.h>

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

#define E_GrP4DoPnlPtzParaCnt		3

St_GrP4DvlDoPrInfoData	V_GrP4DoPnlPtzParaInfo[E_GrP4DoPnlPtzParaCnt]	=	
{
	{	L"Paint Pan tilt",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPnlPtzPara)0)->PntBg[E_GrP4DoPnlPtzPantilt],  NULL,	L"Paint PAN TILT mode"	},
	{	L"Paint Zoom",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPnlPtzPara)0)->PntBg[E_GrP4DoPnlPtzZoom],	 NULL,L"Paint ZOOM mode"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPnlPtzPara)0)->PntBg[E_GrP4DoPnlPtzFocus],	 NULL,L"Paint FOCUS mode"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoPnlPtz::Cls_GrP4DoPnlPtz(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassPnlPtz;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaPnlPtz) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		//m_Ch				=	0;
		m_PtzMode		=	E_GrP4DoPnlPtzPantilt;
		m_StrTitle	=	(WCHAR*)V_GrP4Mng->SetupDataPtrGet( E_GrP4CfgOfsCamTitle );

		m_MseMode		=	E_GrP4DoPnlPtzMseStop;

		m_IsKeyLeft		=	FALSE;
		m_IsKeyRight	=	FALSE;
		m_IsKeyDown		=	FALSE;
		m_IsKeyUp			=	FALSE;

		GrDumyZeroMem( &m_ParaPnlPtz, sizeof(m_ParaPnlPtz) );
		//m_ParaPnlPtz.PntBg	=	E_GrP4ScptPstIdDflt;

		m_PtzMng		=	V_GrP4Mng->PtzMngGet();

		m_PtrGvs		=	V_GrP4Mng->GvsPtrGet();
		((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;
		((Ptr_GrP4Gvs)m_PtrGvs)->IsPtzCtrlLock	=	FALSE;	// reset lock

}
//--------------------------------------------------------------------
Cls_GrP4DoPnlPtz::~Cls_GrP4DoPnlPtz( void )
{
	// local -------------------
	// code --------------------
		((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlMode		=	E_GrP4MngPtzModeNone;
		((Ptr_GrP4Gvs)m_PtrGvs)->IsPtzCtrlLock	=	FALSE;

		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoPnlPtz::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassPnlPtz;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaPnlPtz);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaPnlPtz, sizeof(m_ParaPnlPtz) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaPnlPtz)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoPnlPtz::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaPnlPtz, sizeof(m_ParaPnlPtz) , m_PtrScpt , E_GrP4DoClassPnlPtz );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPnlPtz::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiChGet:
				A_PtrInt[0]	=	(__s32)((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh;
				break;
			case E_GrP4UpiChSet:
				if ( V_GrP4Mng->VdoMaxChCntGet() > (__u8)A_PtrInt[1] )
				{
					((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh	=	(__u8)A_PtrInt[1];
					ReqDraw();
				}
				break;
			case E_GrP4UpiValueGet:
				A_PtrInt[0]	=	(__s32)m_PtzMode;
				break;
			case E_GrP4UpiValueSet:
				if ( E_GrP4DoPnlPtzFocus >= (__u8)A_PtrInt[1] )
				{
					m_PtzMode	=	(__u8)A_PtrInt[1];
					ReqDraw();
					// update PTZ control mode
					((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;
				}
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPnlPtz::ProcPaint( void )
{
	// local -------------------
	// code --------------------
		
		PaintScriptPush( m_ParaPnlPtz.PntBg[m_PtzMode], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoPnlPtz::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaPnlPtz, V_GrP4DoPnlPtzParaInfo, E_GrP4DoPnlPtzParaCnt );
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPnlPtz::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_PtzMng )
		{
			Tv_Result	=	TRUE;
			if ( E_GrP4KeyLeft == A_Key )
			{
				m_IsKeyLeft	=	TRUE;
				switch ( m_PtzMode )
				{
					case E_GrP4DoPnlPtzZoom:
						m_PtzMng->SafeDoZoom( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, TRUE, E_GrPtzV2DfltSpeed, 0 );
						break;
					case E_GrP4DoPnlPtzFocus:
						m_PtzMng->SafeDoFocus( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, FALSE, E_GrPtzV2DfltSpeed, 0 );
						break;
					default:
						m_PtzMng->SafeDoPanTilt( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, 
							m_IsKeyLeft, m_IsKeyRight, m_IsKeyUp, m_IsKeyDown, E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );
						break;
				}
			}
			else if ( E_GrP4KeyRight == A_Key )
			{
				m_IsKeyRight	=	TRUE;
				switch ( m_PtzMode )
				{
					case E_GrP4DoPnlPtzZoom:
						m_PtzMng->SafeDoZoom( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, FALSE, E_GrPtzV2DfltSpeed, 0 );
						break;
					case E_GrP4DoPnlPtzFocus:
						m_PtzMng->SafeDoFocus( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, TRUE, E_GrPtzV2DfltSpeed, 0 );
						break;
					default:
						m_PtzMng->SafeDoPanTilt( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, 
							m_IsKeyLeft, m_IsKeyRight, m_IsKeyUp, m_IsKeyDown, E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );
						break;
				}
			}
			else if ( E_GrP4KeyUp == A_Key )
			{
				m_IsKeyUp	=	TRUE;
				switch ( m_PtzMode )
				{
					case E_GrP4DoPnlPtzZoom:
						m_PtzMng->SafeDoZoom( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, FALSE, E_GrPtzV2DfltSpeed, 0 );
						break;
					case E_GrP4DoPnlPtzFocus:
						m_PtzMng->SafeDoFocus( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, TRUE, E_GrPtzV2DfltSpeed, 0 );
						break;
					default:
						m_PtzMng->SafeDoPanTilt( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, 
							m_IsKeyLeft, m_IsKeyRight, m_IsKeyUp, m_IsKeyDown, E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );
						break;
				}
			}
			else if ( E_GrP4KeyDown == A_Key )
			{
				m_IsKeyDown	=	TRUE;
				switch ( m_PtzMode )
				{
					case E_GrP4DoPnlPtzZoom:
						m_PtzMng->SafeDoZoom( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, TRUE, E_GrPtzV2DfltSpeed, 0 );
						break;
					case E_GrP4DoPnlPtzFocus:
						m_PtzMng->SafeDoFocus( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, FALSE, E_GrPtzV2DfltSpeed, 0 );
						break;
					default:
						m_PtzMng->SafeDoPanTilt( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, 
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
BOOL8	Cls_GrP4DoPnlPtz::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Mode;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4KeyEnter == A_Key )
		{
			Tv_Mode	=	m_PtzMode + 1;
			if ( E_GrP4DoPnlPtzFocus < Tv_Mode )
			{
				Tv_Mode	=	E_GrP4DoPnlPtzPantilt;
			}
			m_PtzMode	=	Tv_Mode;
			ReqDraw();
			// update PTZ control mode
			((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;

			Tv_Result	=	TRUE;
		}
		else if ( (E_GrP4KeyLeft <= A_Key) && (E_GrP4KeyDown >= A_Key)  )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
					m_IsKeyLeft	=	FALSE;
					break;
				case E_GrP4KeyRight:
					m_IsKeyRight	=	FALSE;
					break;
				case E_GrP4KeyUp:
					m_IsKeyUp		=	FALSE;
					break;
				case E_GrP4KeyDown:
					m_IsKeyDown	=	FALSE;
					break;
			}
			// check stop
			if ( (!m_IsKeyLeft) && (!m_IsKeyRight) && (!m_IsKeyUp) && (!m_IsKeyDown) )
			{
				m_PtzMng->SafeDoStop( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh );
			}
			else
			{
				if ( E_GrP4DoPnlPtzPantilt == m_PtzMode )
				{
					m_PtzMng->SafeDoPanTilt( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, 
						m_IsKeyLeft, m_IsKeyRight, m_IsKeyUp, m_IsKeyDown, E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );

				}
			}
			
		}
		

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPnlPtz::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		LcMsePtzRun( A_X, A_Y );
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPnlPtz::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		if ( NULL != m_PtzMng )
		{
			m_PtzMng->SafeDoStop( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh );
			m_MseMode	=	E_GrP4DoPnlPtzMseStop;
		}
		return	TRUE;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoPnlPtz::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u32		Tv_Pos;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			if ( E_GrDvrMaxChCnt > ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh )
			{
				Tv_Pos	=	E_GrP4CfgOfsCamTitle + ( ((__u32)((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh) << 5 );
				Tv_Result	=	(WCHAR*)( (__u32)m_StrTitle + Tv_Pos );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPnlPtz::LcMsePtzRun( __u16 A_X, __u16 A_Y )
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
				if ( E_GrP4DoPnlPtzPantilt == m_PtzMode )
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
					if ( E_GrP4DoPnlPtzMseStop != m_MseMode )
					{
						m_PtzMng->SafeDoStop( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh );
						m_MseMode	=	E_GrP4DoPnlPtzMseStop;
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
							case E_GrP4DoPnlPtzZoom:
								m_PtzMng->SafeDoZoom( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, Tv_IsLeft, Tv_SpdX, 0 );
								break;
							case E_GrP4DoPnlPtzFocus:
								m_PtzMng->SafeDoFocus( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, Tv_IsLeft, Tv_SpdX, 0 );
								break;
							default:
								m_PtzMng->SafeDoPanTilt( ((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlCh, Tv_IsLeft, Tv_IsRight, Tv_IsUp, Tv_IsDown, Tv_SpdX, Tv_SpdY, 0 );
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
BOOL8	Cls_GrP4DoPnlPtz::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		if ( 0 != (E_GrP4MouseBmkLeft & A_BtnMap) )
		{
			LcMsePtzRun( A_X, A_Y );
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPnlPtz::ProcEvtWheelUp( void )
{
	// local -------------------
		__u8	Tv_Mode;
	// code --------------------
		// check able mode
		//if ( E_GrP4DoPnlPtzMseStop == m_MseMode )
		{
			Tv_Mode	=	m_PtzMode + 1;
			if ( E_GrP4DoPnlPtzFocus < Tv_Mode )
			{
				Tv_Mode	=	E_GrP4DoPnlPtzPantilt;
			}
			m_PtzMode	=	Tv_Mode;
			ReqDraw();
			// update PTZ control mode
			((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPnlPtz::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		// check able mode
		//if ( E_GrP4DoPnlPtzMseStop == m_MseMode )
		{
			if ( E_GrP4DoPnlPtzPantilt == m_PtzMode )
			{
				m_PtzMode	=	E_GrP4DoPnlPtzFocus;
			}
			else
			{
				m_PtzMode --;
			}
			ReqDraw();
			// update PTZ control mode
			((Ptr_GrP4Gvs)m_PtrGvs)->PtzCtrlMode		=	m_PtzMode + 1;
		}

		return	FALSE;
}
//--------------------------------------------------------------------
