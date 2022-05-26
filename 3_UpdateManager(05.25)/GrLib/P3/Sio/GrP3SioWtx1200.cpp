/*
 DVR SIO WTX1200

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<P3/Sio/GrP3SioWtx1200.h>
#include	<GrUart.h>
#include <Dvr/PtzV2/GrPtzV2Mng.h>
#include <P3/GrP3Base.h>
#include <P3/GrP3Bios.h>
#include <P3/GrP3ApiBase.h>

//====================================================================
//local const
#define E_GrP3SioWtx1200DirKeyIntv	2
#define E_GrP3SioWtx1200FzKeyIntv		4

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP3Mng*	V_GrP3Mng;

//====================================================================

//--------------------------------------------------------------------
Cls_GrP3SioWtx1200::Cls_GrP3SioWtx1200( void* A_SioMng, __u8 A_Uart, __u8 A_Id ):
Cls_GrP3SioPiBase( A_SioMng, A_Uart, A_Id )
{

		m_RecvIn			=	0;
		m_RecvMax			=	1;
		m_DirKeyIntv	=	0;
		m_TimerFz			=	0;
		m_ChNum				=	0;
		m_IsChIn			=	FALSE;

		m_PtrGvs			=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet( );
}
//--------------------------------------------------------------------
Cls_GrP3SioWtx1200::~Cls_GrP3SioWtx1200()
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3SioWtx1200::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8*	Tv_PtrRecv;
		__u8	Tv_RecvSize;
		__u8	Tv_Recved;
	// code --------------------
		Tv_Result		=	FALSE;

		//receive
		Tv_PtrRecv	=	&m_RecvBuf[m_RecvIn];
		Tv_RecvSize	=	m_RecvMax - m_RecvIn;
		//DbgMsgPrint( "WTX req max %d sz %d\n", m_RecvMax, Tv_RecvSize );
		Tv_Recved		=	(__u8)GrUartRecv( m_UartCh, (void*)Tv_PtrRecv, (__u32)Tv_RecvSize );
		if ( 0 != Tv_Recved )
		{
			//DbgMsgPrint( "WTX recv max %d sz %d ed %d\n", m_RecvMax, Tv_RecvSize, Tv_Recved );
			m_RecvIn	=	m_RecvIn + Tv_Recved;
			if ( m_RecvMax <= m_RecvIn )
			{
				switch( m_RecvMax )
				{
					case	1:
						if ( 0xA1 == m_RecvBuf[0] )
						{
							//found
							m_RecvMax	=	5;
						}
						else
						{
							m_RecvIn	=	0;
						}
						break;
					default:
						LcParsPrtc();
						break;
				}
			}
			Tv_Result	=	TRUE;
		}
		
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP3SioWtx1200::LcParsPrtc( void )
{
	// local -------------------
		__u8	Tv_Key;
		__u8	Tv_PanSpd;
		__u8	Tv_TiltSpd;
		__s32		Tv_Rint[4];
	// code --------------------
		//check id
		if ( m_Id == m_RecvBuf[1] )
		{
			DbgMsgPrint( "WTX key in!\n" );
			// check PTZ control key
			if ( 0 != (0x80 & m_RecvBuf[2]) )
			{
				// check PTZ control mode
				if ( (E_GrP3MngPtzModeNone != m_PtrGvs->PtzCtrlMode) && (!m_PtrGvs->IsPtzCtrlLock) )
				{
					LcProcPtzCtrl();
				}
				else
				{
					//check interval
					if ( 0 == m_DirKeyIntv )
					{
						//menu key mode
						Tv_Key	=	E_GrP3KeyNone;
						//check max value
						Tv_PanSpd		=	(0xF0 & m_RecvBuf[3]) >> 4;
						Tv_TiltSpd	=	(0x0F & m_RecvBuf[3]);
						if ( (2 <= Tv_PanSpd) && (7 >= Tv_PanSpd) )
						{
							Tv_Key	=	E_GrP3KeyLeft;
						}
						else if ( (0x0A <= Tv_PanSpd) && (0x0F >= Tv_PanSpd) )
						{
							Tv_Key	=	E_GrP3KeyRight;
						}
						else if ( (2 <= Tv_TiltSpd) && (7 >= Tv_TiltSpd) )
						{
							Tv_Key	=	E_GrP3KeyUp;
						}
						else if ( (0x0A <= Tv_TiltSpd) && (0x0F >= Tv_TiltSpd) )
						{
							Tv_Key	=	E_GrP3KeyDown;
						}
						if ( E_GrP3KeyNone != Tv_Key )
						{
							DbgMsgPrint( "WTX key &X!\n", Tv_Key );

							V_GrP3Mng->KeyInput( Tv_Key, TRUE, E_GrP3InDevPtzKbd );
							V_GrP3Mng->KeyInput( Tv_Key, FALSE, E_GrP3InDevPtzKbd );
							m_TimerFz		=	0;
							m_DirKeyIntv	=	E_GrP3SioWtx1200DirKeyIntv;
						}
					}
				}
			}	//	if ( 0 != (0x80 & m_RecvBuf[2]) )
			else
			{
				//normal key
				Tv_Key	=	E_GrP3KeyNone;
				switch( m_RecvBuf[2] )
				{
					case	0x01:
						m_ChNum	=	(m_ChNum * 10) + 1;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh1;
						break;
					case	0x02:
						m_ChNum	=	(m_ChNum * 10) + 2;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh2;
						break;
					case	0x03:
						m_ChNum	=	(m_ChNum * 10) + 3;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh3;
						break;
					case	0x04:
						m_ChNum	=	(m_ChNum * 10) + 4;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh4;
						break;
					case	0x05:
						m_ChNum	=	(m_ChNum * 10) + 5;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh5;
						break;
					case	0x06:
						m_ChNum	=	(m_ChNum * 10) + 6;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh6;
						break;
					case	0x07:
						m_ChNum	=	(m_ChNum * 10) + 7;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh7;
						break;
					case	0x08:
						m_ChNum	=	(m_ChNum * 10) + 8;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh8;
						break;
					case	0x09:
						m_ChNum	=	(m_ChNum * 10) + 9;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh9;
						break;
					case	0x0A:
						m_ChNum	=	(m_ChNum * 10) + 0;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrP3KeyCh0;
						break;
					case	0x15:
						Tv_Key	=	E_GrP3KeyMenu;
						break;
					case	0x14:
						Tv_Key	=	E_GrP3KeyAudio;
						break;
					case	0x13:
						Tv_Key	=	E_GrP3KeyPtzMode;
						break;
					case	0x12:
						Tv_Key	=	E_GrP3KeyBackup;
						break;
					case	0x11:
						Tv_Key	=	E_GrP3KeySearch;
						break;
					case	0x21:
						if ( m_IsChIn )
						{
							// change id
							if ( (16 < m_ChNum) || (2 == m_RecvBuf[3]) )
							{
								m_ChNum			=	0;
								m_IsChIn		=	FALSE;
							}
							else
							{
								Tv_Key	=	E_GrP3KeyCh0 + m_ChNum;
							}
						}
						else
						{
							Tv_Key	=	E_GrP3KeyEnter;
						}
						break;
					case	0x22:
						Tv_Key	=	E_GrP3KeyQuad;
						break;
					case	0x29:
						Tv_Key	=	E_GrP3KeySequence;
						break;
					case	0x23:
						Tv_Key	=	E_GrP3KeyEmgcyOff;
						break;
					case	0x24:
						Tv_Key	=	E_GrP3KeyEmgcyOn;
						break;
					case	0x25:
						GrP3ApiQuadModeGet( Tv_Rint, NULL );
						Tv_Rint[2]	=	FALSE;
						GrP3ApiPtzFocus( Tv_Rint, NULL );
						m_TimerFz		=	E_GrP3SioWtx1200FzKeyIntv;
						m_ChNum			=	0;
						m_IsChIn		=	FALSE;
						break;
					case	0x26:
						GrP3ApiQuadModeGet( Tv_Rint, NULL );
						Tv_Rint[2]	=	TRUE;
						GrP3ApiPtzFocus( Tv_Rint, NULL );
						m_TimerFz		=	E_GrP3SioWtx1200FzKeyIntv;
						m_ChNum			=	0;
						m_IsChIn		=	FALSE;
						break;
					case	0x27:
						GrP3ApiQuadModeGet( Tv_Rint, NULL );
						Tv_Rint[2]	=	FALSE;
						GrP3ApiPtzZoom( Tv_Rint, NULL );
						m_TimerFz		=	E_GrP3SioWtx1200FzKeyIntv;
						m_ChNum			=	0;
						m_IsChIn		=	FALSE;
						break;
					case	0x28:
						GrP3ApiQuadModeGet( Tv_Rint, NULL );
						Tv_Rint[2]	=	TRUE;
						GrP3ApiPtzZoom( Tv_Rint, NULL );
						m_TimerFz		=	E_GrP3SioWtx1200FzKeyIntv;
						m_ChNum			=	0;
						m_IsChIn		=	FALSE;
						break;
				}
				if ( E_GrP3KeyNone != Tv_Key )
				{
					m_TimerFz		=	0;
					V_GrP3Mng->KeyInput( Tv_Key, TRUE, E_GrP3InDevPtzKbd );
					V_GrP3Mng->KeyInput( Tv_Key, FALSE, E_GrP3InDevPtzKbd );
					m_ChNum			=	0;
					m_IsChIn		=	FALSE;
				}
				//update channel number
				if ( 16 < m_ChNum )
				{
					m_ChNum	=	100;
				}
			}	//	if ( 0 != (0x80 & m_RecvBuf[2]) )
		}	//	if ( m_Id == m_RecvBuf[1] )
		//reset buffer
		m_RecvIn	=	0;
		m_RecvMax	=	1;
}
//--------------------------------------------------------------------
void	Cls_GrP3SioWtx1200::RtlTimer( void )
{
	// local -------------------
		__s32		Tv_Rint[4];
	// code --------------------
		if ( 0 != m_DirKeyIntv )
		{
			m_DirKeyIntv --;
		}
		// focus zoom stop timer
		if ( 0 != m_TimerFz )
		{
			m_TimerFz --;
			if ( 0 == m_TimerFz )
			{
				GrP3ApiQuadModeGet( Tv_Rint, NULL );
				GrP3ApiPtzStop( Tv_Rint, NULL );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3SioWtx1200::LcProcPtzCtrl( void )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		__u8	Tv_Ch;
		BOOL8	Tv_IsStop;
		BOOL8	Tv_IsWide;
		__u8	Tv_Speed;
		BOOL8	Tv_IsLeft;
		BOOL8	Tv_IsRight;
		BOOL8	Tv_IsUp;
		BOOL8	Tv_IsDown;
		__u8	Tv_PanSpd;
		__u8	Tv_TiltSpd;
		__s32		Tv_Rint[4];
	// code --------------------
		// get PTZ control
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			// init
			GrP3ApiQuadModeGet( Tv_Rint, NULL );
			Tv_Ch			=	(__u8)Tv_Rint[1];
			Tv_IsStop	=	TRUE;
			// check zoom mode
			if ( 0x80 != m_RecvBuf[2] )
			{
				//zoom
				Tv_IsWide	=	FALSE;
				if ( 0x88 > m_RecvBuf[2] )
				{
					// wide
					Tv_Speed	=	(m_RecvBuf[2] - 0x80 + 1) << 1;
					Tv_IsWide	=	TRUE;
				}
				else
				{
					// TELE
					Tv_Speed	=	(m_RecvBuf[2] -0x88 + 1) << 1;
				}
				if ( 4 < Tv_Speed )
				{
					Tv_Speed	=	4;
				}
				Tv_PtzMng->SafeDoZoom( Tv_Ch, Tv_IsWide, Tv_Speed, 0 );
				Tv_IsStop	=	FALSE;
			}

			//check PANTILT
			Tv_IsLeft		=	FALSE;
			Tv_IsRight	=	FALSE;
			Tv_IsUp			=	FALSE;
			Tv_IsDown		=	FALSE;
			Tv_PanSpd		=	0;
			Tv_TiltSpd	=	0;
			// pan
			if ( 0 != (0xF0 & m_RecvBuf[3]) )
			{
				if ( 0x80 > (0xF0 & m_RecvBuf[3]) )
				{
					//left
					Tv_IsLeft	=	TRUE;
					Tv_PanSpd	=	((m_RecvBuf[3] >> 4) + 1) >> 1;
				}
				else
				{
					//right
					Tv_IsRight	=	TRUE;
					Tv_PanSpd	=	((m_RecvBuf[3] >> 4) - 8 + 1) >> 1;
				}
				Tv_IsStop	=	FALSE;
			}
			// tilt
			if ( 0 != (0x0F & m_RecvBuf[3]) )
			{
				if ( 0x08 > (0x0F & m_RecvBuf[3]) )
				{
					//up
					Tv_IsUp			=	TRUE;
					Tv_TiltSpd	=	((m_RecvBuf[3] & 0x0F) + 1) >> 1;
				}
				else
				{
					//down
					Tv_IsDown		=	TRUE;
					Tv_TiltSpd	=	((m_RecvBuf[3] & 0x0F) - 8 + 1) >> 1;
				}
				Tv_IsStop	=	FALSE;
			}
			// do PTZ
			m_TimerFz		=	0;
			if ( Tv_IsStop )
			{
				Tv_PtzMng->SafeDoStop( Tv_Ch );
			}
			else
			{
				if ( Tv_IsLeft || Tv_IsRight || Tv_IsUp || Tv_IsDown )
				{
					Tv_PtzMng->SafeDoPanTilt( Tv_Ch, Tv_IsLeft, Tv_IsRight, Tv_IsUp, Tv_IsDown, Tv_PanSpd, Tv_TiltSpd, 0 );
				}
			}
		}
}
//--------------------------------------------------------------------

