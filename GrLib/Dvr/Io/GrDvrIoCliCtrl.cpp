/*
	DvrCtrlPrtc
 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<DvrRtl.h>
#include	<GrUart.h>
#include	<Ui/GrUiBase.h>
#include	<Dvr/Io/GrDvrIoCliCtrl.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
#define E_GrDvrCtrlPrtcNum0					0x01
#define E_GrDvrCtrlPrtcNum1					0x02
#define E_GrDvrCtrlPrtcNum2					0x03
#define E_GrDvrCtrlPrtcNum3					0x04
#define E_GrDvrCtrlPrtcNum4					0x05
#define E_GrDvrCtrlPrtcNum5					0x06
#define E_GrDvrCtrlPrtcNum6					0x07
#define E_GrDvrCtrlPrtcNum7					0x08
#define E_GrDvrCtrlPrtcNum8					0x09
#define E_GrDvrCtrlPrtcNum9					0x0A
#define E_GrDvrCtrlPrtcNum10				0x0B
#define E_GrDvrCtrlPrtcNum11				0x0C
#define E_GrDvrCtrlPrtcNum12				0x0D
#define E_GrDvrCtrlPrtcNum13				0x0E
#define E_GrDvrCtrlPrtcNum14				0x0F
#define E_GrDvrCtrlPrtcNum15				0x10
#define E_GrDvrCtrlPrtcNum16				0x11
#define E_GrDvrCtrlPrtcEnter				0x12
#define E_GrDvrCtrlPrtcEsc					0x13
#define E_GrDvrCtrlPrtcDelete				0x14
#define E_GrDvrCtrlPrtcLock					0x15
#define E_GrDvrCtrlPrtcLeft					0x16
#define E_GrDvrCtrlPrtcUp						0x17
#define E_GrDvrCtrlPrtcRight				0x18
#define E_GrDvrCtrlPrtcDown					0x19
#define E_GrDvrCtrlPrtcEject				0x1C
#define E_GrDvrCtrlPrtcMenu					0x35
#define E_GrDvrCtrlPrtcBackup				0x36
#define E_GrDvrCtrlPrtcCapture			0x37
#define E_GrDvrCtrlPrtcSearch				0x38
#define E_GrDvrCtrlPrtcDisplay			0x39
#define E_GrDvrCtrlPrtcPtzMode			0x3A
#define E_GrDvrCtrlPrtcEmergency		0x3B
#define E_GrDvrCtrlPrtcQuad					0x3C
#define E_GrDvrCtrlPrtcSequence			0x3D
#define E_GrDvrCtrlPrtcAudio				0x3E
#define E_GrDvrCtrlPrtcSplit				0x3F
#define E_GrDvrCtrlPrtcEmergencyOn	0x40
#define E_GrDvrCtrlPrtcEmergencyOff	0x41
#define E_GrDvrCtrlPrtcNextQuad			0x42
#define E_GrDvrCtrlPrtcPreQuad			0x43
#define E_GrDvrCtrlPrtcPtzTele			0x50
#define E_GrDvrCtrlPrtcPtzWide			0x51
#define E_GrDvrCtrlPrtcPtzFar				0x52
#define E_GrDvrCtrlPrtcPtzNear			0x53
#define E_GrDvrCtrlPrtcReboot				0x55
#define E_GrDvrCtrlPrtcZoomIn				0x56
#define E_GrDvrCtrlPrtcZoomOut			0x57
#define E_GrDvrCtrlPrtcFocusIn			0x58
#define E_GrDvrCtrlPrtcFocusOut			0x59
#define E_GrDvrCtrlPrtcPtzSpeed			0x5A
#define E_GrDvrCtrlPrtcPtzTour			0x5C
#define E_GrDvrCtrlPrtcQuad1				0x60
#define E_GrDvrCtrlPrtcQuad4				0x61
#define E_GrDvrCtrlPrtcQuad6				0x62
#define E_GrDvrCtrlPrtcQuad8				0x63
#define E_GrDvrCtrlPrtcQuad9				0x64
#define E_GrDvrCtrlPrtcQuad10				0x65
#define E_GrDvrCtrlPrtcQuad13				0x66
#define E_GrDvrCtrlPrtcQuad16				0x67
#define E_GrDvrCtrlPrtcMenuLive			0x70
#define E_GrDvrCtrlPrtcExit					0x7F
//====================================================================
//--------------------------------------------------------------------
Cls_GrDvrIoCliCtrl::Cls_GrDvrIoCliCtrl( __u8 A_UartCh, __u8 A_Id ):
Cls_GrDvrIoCliBase( A_UartCh, A_Id )
{
	// local -------------------		
		__u8	Tv_WkIdx;
	// code --------------------
		m_RecvIn	= 0;
		m_UartCh	= A_UartCh;
		m_PkDidx	= 0;
		m_PkCmd		= 0;
		m_PkData1	= 0;
		m_PkDataSz	= 0;
		m_IsRevd	= FALSE;
		for ( Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrCtrlBufSize; Tv_WkIdx++ )
		{
			m_PkData[Tv_WkIdx] = 0;
		}
}
//--------------------------------------------------------------------
Cls_GrDvrIoCliCtrl::~Cls_GrDvrIoCliCtrl()
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliCtrl::RtlTimer( void )
{
	// local -------------------
		__u8	Tv_Key;
	// code --------------------
		Tv_Key = 0x00;
		if( m_IsRevd )
		{
			if( 0 == m_PkCmd )
			{
				switch ( m_PkData1 )
				{
				case	E_GrDvrCtrlPrtcNum0:
					Tv_Key = E_GrUiKeyCh0;
					break;
				case	E_GrDvrCtrlPrtcNum1:
					Tv_Key = E_GrUiKeyCh1;
					break;
				case	E_GrDvrCtrlPrtcNum2:
					Tv_Key = E_GrUiKeyCh2;
					break;
				case	E_GrDvrCtrlPrtcNum3:
					Tv_Key = E_GrUiKeyCh3;
					break;
				case	E_GrDvrCtrlPrtcNum4:
					Tv_Key = E_GrUiKeyCh4;
					break;
				case	E_GrDvrCtrlPrtcNum5:
					Tv_Key = E_GrUiKeyCh5;
					break;
				case	E_GrDvrCtrlPrtcNum6:
					Tv_Key = E_GrUiKeyCh6;
					break;
				case	E_GrDvrCtrlPrtcNum7:						
					Tv_Key = E_GrUiKeyCh7;
					break;
				case	E_GrDvrCtrlPrtcNum8:
					Tv_Key = E_GrUiKeyCh8;
					break;
				case	E_GrDvrCtrlPrtcNum9:
					Tv_Key = E_GrUiKeyCh9;
					break;
				case	E_GrDvrCtrlPrtcEnter:
					Tv_Key = E_GrUiKeyEnter;
					break;
				case 	E_GrDvrCtrlPrtcLeft:
					Tv_Key = E_GrUiKeyLeft;
					break;
				case 	E_GrDvrCtrlPrtcUp:
					Tv_Key = E_GrUiKeyUp;
					break;
				case 	E_GrDvrCtrlPrtcRight:
					Tv_Key = E_GrUiKeyRight;
					break;
				case 	E_GrDvrCtrlPrtcDown:
					Tv_Key = E_GrUiKeyDown;
					break;
				case 	E_GrDvrCtrlPrtcMenu:
					Tv_Key = E_GrUiKeyMenu;
					break;
				case 	E_GrDvrCtrlPrtcBackup:
					Tv_Key = E_GrUiKeyBackup;
					break;
				case 	E_GrDvrCtrlPrtcSearch:
					Tv_Key = E_GrUiKeySearch;
					break;
				case 	E_GrDvrCtrlPrtcPtzMode:
					Tv_Key = E_GrUiKeyPtzMode;
					break;
				case E_GrDvrCtrlPrtcEmergencyOn:
					Tv_Key = E_GrUiKeyEmgcyOn;
					break;
				case E_GrDvrCtrlPrtcEmergencyOff:
					Tv_Key = E_GrUiKeyEmgcyOff;
					break;
				case 	E_GrDvrCtrlPrtcQuad:
					Tv_Key = E_GrUiKeyQuad;
					break;
				case 	E_GrDvrCtrlPrtcSequence:
					Tv_Key = E_GrUiKeySequence;
					break;
				case 	E_GrDvrCtrlPrtcAudio:
					Tv_Key = E_GrUiKeyAudio;
					break;
				case 	E_GrDvrCtrlPrtcSplit:
					Tv_Key = E_GrUiKeySplit;
					break;
				}
				if( E_GrUiKeyNone != Tv_Key )
				{
					DvrRtlKeyEmu( Tv_Key, TRUE );
					DvrRtlKeyEmu( Tv_Key, FALSE );
					m_IsRevd = FALSE;
				}
			}
			else
			{
				//not define
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrIoCliCtrl::RtlProc( void )
{
	// local -------------------
		__u8	Tv_ReqSize;
		BOOL8	Tv_Result;
		__u8*	Tv_PtrRecv;
		__u8	Tv_WkIdx;
		__u8	Tv_CkSum;
		__u8	Tv_Etx;
		__u8	Tv_Recved;
	// code --------------------
		Tv_Result 	= FALSE;
		Tv_Etx 			= 0;
		Tv_CkSum		= 0;
		if ( 0 == m_RecvIn )
		{
			Tv_ReqSize = 1;
		}
		else
		{
			Tv_ReqSize = E_GrDvrCtrlBufSize - m_RecvIn;
		}

		Tv_PtrRecv	=	&m_RecvBuf[m_RecvIn];
		Tv_Recved	=	(__u8)GrUartRecv( m_UartCh, (void*)Tv_PtrRecv, (__u32)Tv_ReqSize );

		if ( 0 != Tv_Recved )
		{
			m_RecvIn = m_RecvIn + Tv_Recved;
			if ( 1 == m_RecvIn )
			{
				if ( E_GrDvrCtrlPrtcStx != m_RecvBuf[0] )
				{
					m_RecvIn	=	0;
				}
			}
			else
			{
				m_PkDataSz 	= m_RecvBuf[3];
				Tv_Etx			= m_RecvBuf[6];
				if ( ( 2 == m_PkDataSz ) & ( 0x56 == Tv_Etx ) )
				{
					if ( E_GrDvrCtrlBufSize == m_RecvIn )
					{
						for( Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrCtrlBufSize-1; Tv_WkIdx++ )
						{
							Tv_CkSum = Tv_CkSum + m_RecvBuf[Tv_WkIdx];
						}
						if ( (Tv_CkSum & 0xFF) == m_RecvBuf[E_GrDvrCtrlBufSize-1] )
						{
							m_IsRevd 	= TRUE;
							Tv_Result	= TRUE; 
							m_PkDidx	= m_RecvBuf[1];
							m_PkCmd		= m_RecvBuf[2];
							m_PkData1	= m_RecvBuf[4];
						}
						m_RecvIn = 0;
					}
					else
					{
						m_RecvIn = 0;
					}
				}
				else
				{
					m_RecvIn = 0;
				}
			}
		}	
		return	Tv_Result;
}
//--------------------------------------------------------------------

