/*
 dvr io client WTX1200A

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Io/GrDvrIoCliWtx1200.h>
#include	<DvrRtl.h>
#include	<GrUart.h>
#include	<Ui/GrUiBase.h>

//====================================================================
//local const
#define E_GrDvrioCliWtx1200DirKeyIntv	2
#define E_GrDvrioCliWtx1200FzKeyIntv	4

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrIoCliWtx1200::Cls_GrDvrIoCliWtx1200( __u8 A_UartCh, __u8 A_Id ):
Cls_GrDvrIoCliBase( A_UartCh, A_Id )
{
		m_RecvIn	=	0;
		m_RecvMax	=	1;
		m_DirKeyIntv	=	0;
		m_TimerFz	=	0;
		m_ChNum		=	0;
		m_IsChIn	=	FALSE;
}
//--------------------------------------------------------------------
Cls_GrDvrIoCliWtx1200::~Cls_GrDvrIoCliWtx1200()
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrIoCliWtx1200::RtlProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8*	Tv_PtrRecv;
		__u8	Tv_RecvSize;
		__u8	Tv_Recved;
	// code --------------------
		Tv_Result		=	FALSE;

		//recive
		Tv_PtrRecv	=	&m_RecvBuf[m_RecvIn];
		Tv_RecvSize	=	m_RecvMax - m_RecvIn;
		Tv_Recved		=	(__u8)GrUartRecv( m_UartCh, (void*)Tv_PtrRecv, (__u32)Tv_RecvSize );
		if ( 0 != Tv_Recved )
		{
			m_RecvIn	=	m_RecvIn + Tv_Recved;
			if ( m_RecvMax <= m_RecvIn )
			{
				switch( m_RecvMax )
				{
					case	1:
						if ( 0xA1 == m_RecvBuf[0] )
						{
							//finded
							m_RecvMax	=	5;
						}
						else
						{
							m_RecvIn	=	0;
						}
						break;
					case	5:
						LcParsPrtc();
						break;
				}
			}
			Tv_Result	=	TRUE;
		}
		
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliWtx1200::LcParsPrtc( void )
{
	// local -------------------
		BOOL8	Tv_IsWide;
		__u8	Tv_Speed;
		BOOL8	Tv_IsLeft;
		BOOL8	Tv_IsRight;
		BOOL8	Tv_IsUp;
		BOOL8	Tv_IsDown;
		__u8	Tv_PanSpd;
		__u8	Tv_TiltSpd;
		__u8	Tv_Key;
		__u8	Tv_IsStop;
	// code --------------------
		//check id
		if ( m_Id == m_RecvBuf[1] )
		{
			//check ptz
			if ( 0 != (0x80 & m_RecvBuf[2]) )
			{
				//check ptz mode
				if ( DvrRtlPtzIsView() )
				{
					Tv_IsStop	=	TRUE;
					//check zoom
					if ( 0x80 != m_RecvBuf[2] )
					{
						Tv_IsWide	=	FALSE;
						if ( 0x88 > m_RecvBuf[2] )
						{
							//wide
							Tv_Speed	=	(m_RecvBuf[2] - 0x80 + 1) << 1;
							Tv_IsWide	=	TRUE;
						}
						else
						{
							//tele
							Tv_Speed	=	(m_RecvBuf[2] -0x88 + 1) << 1;
						}
						if ( 4 < Tv_Speed )
						{
							Tv_Speed	=	4;
						}
						DvrRtlPtzDoZoom( DvrRtlQuadViewChGet(0), Tv_IsWide, Tv_Speed );
						Tv_IsStop	=	FALSE;
					}
					//check pantilt
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
					// do ptz
					m_TimerFz		=	0;
					if ( Tv_IsStop )
					{
						DvrRtlPtzDoStop( DvrRtlQuadViewChGet(0) );
					}
					else
					{
						if ( Tv_IsLeft || Tv_IsRight || Tv_IsUp || Tv_IsDown )
						{
							DvrRtlPtzDoPanTilt( DvrRtlQuadViewChGet(0), Tv_IsLeft, Tv_IsRight, Tv_IsUp, Tv_IsDown,
								Tv_PanSpd, Tv_TiltSpd );
						}
					}
				}
				else
				{
					//check interval
					if ( 0 == m_DirKeyIntv )
					{
						//menu key mode
						Tv_Key		=	E_GrUiKeyNone;
						//check max value
						Tv_PanSpd		=	(0xF0 & m_RecvBuf[3]) >> 4;
						Tv_TiltSpd	=	(0x0F & m_RecvBuf[3]);
						if ( (2 <= Tv_PanSpd) && (7 >= Tv_PanSpd) )
						{
							Tv_Key	=	E_GrUiKeyLeft;
						}
						else if ( (0x0A <= Tv_PanSpd) && (0x0F >= Tv_PanSpd) )
						{
							Tv_Key	=	E_GrUiKeyRight;
						}
						else if ( (2 <= Tv_TiltSpd) && (7 >= Tv_TiltSpd) )
						{
							Tv_Key	=	E_GrUiKeyUp;
						}
						else if ( (0x0A <= Tv_TiltSpd) && (0x0F >= Tv_TiltSpd) )
						{
							Tv_Key	=	E_GrUiKeyDown;
						}
						if ( E_GrUiKeyNone != Tv_Key )
						{
							DvrRtlKeyEmu( Tv_Key, TRUE );
							DvrRtlKeyEmu( Tv_Key, FALSE );
							m_TimerFz		=	0;
							m_DirKeyIntv	=	E_GrDvrioCliWtx1200DirKeyIntv;
						}
					}
				}
			}
			else
			{
				//normal key
				Tv_Key	=	E_GrUiKeyNone;
				switch( m_RecvBuf[2] )
				{
					case	0x01:
						m_ChNum	=	(m_ChNum * 10) + 1;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh1;
						break;
					case	0x02:
						m_ChNum	=	(m_ChNum * 10) + 2;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh2;
						break;
					case	0x03:
						m_ChNum	=	(m_ChNum * 10) + 3;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh3;
						break;
					case	0x04:
						m_ChNum	=	(m_ChNum * 10) + 4;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh4;
						break;
					case	0x05:
						m_ChNum	=	(m_ChNum * 10) + 5;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh5;
						break;
					case	0x06:
						m_ChNum	=	(m_ChNum * 10) + 6;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh6;
						break;
					case	0x07:
						m_ChNum	=	(m_ChNum * 10) + 7;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh7;
						break;
					case	0x08:
						m_ChNum	=	(m_ChNum * 10) + 8;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh8;
						break;
					case	0x09:
						m_ChNum	=	(m_ChNum * 10) + 9;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh9;
						break;
					case	0x0A:
						m_ChNum	=	(m_ChNum * 10) + 0;
						m_IsChIn		=	TRUE;
						//Tv_Key	=	E_GrUiKeyCh0;
						break;
					case	0x15:
						Tv_Key	=	E_GrUiKeyMenu;
						break;
					case	0x14:
						Tv_Key	=	E_GrUiKeyAudio;
						break;
					case	0x13:
						Tv_Key	=	E_GrUiKeyPtzMode;
						break;
					case	0x12:
						Tv_Key	=	E_GrUiKeyBackup;
						break;
					case	0x11:
						Tv_Key	=	E_GrUiKeySearch;
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
								Tv_Key	=	E_GrUiKeyCh0 + m_ChNum;
							}
						}
						else
						{
							Tv_Key	=	E_GrUiKeyEnter;
						}
						break;
					case	0x22:
						Tv_Key	=	E_GrUiKeyQuad;
						break;
					case	0x29:
						Tv_Key	=	E_GrUiKeySequence;
						break;
					case	0x23:
						Tv_Key	=	E_GrUiKeyEmgcyOff;
						break;
					case	0x24:
						Tv_Key	=	E_GrUiKeyEmgcyOn;
						break;
					case	0x25:
						DvrRtlPtzDoFocus( DvrRtlQuadViewChGet(0), FALSE );
						m_TimerFz		=	E_GrDvrioCliWtx1200FzKeyIntv;
						m_ChNum			=	0;
						m_IsChIn		=	FALSE;
						break;
					case	0x26:
						DvrRtlPtzDoFocus( DvrRtlQuadViewChGet(0), TRUE );
						m_TimerFz		=	E_GrDvrioCliWtx1200FzKeyIntv;
						m_ChNum			=	0;
						m_IsChIn		=	FALSE;
						break;
					case	0x27:
						DvrRtlPtzDoZoom( DvrRtlQuadViewChGet(0), FALSE );
						m_TimerFz		=	E_GrDvrioCliWtx1200FzKeyIntv;
						m_ChNum			=	0;
						m_IsChIn		=	FALSE;
						break;
					case	0x28:
						DvrRtlPtzDoZoom( DvrRtlQuadViewChGet(0), TRUE );
						m_TimerFz		=	E_GrDvrioCliWtx1200FzKeyIntv;
						m_ChNum			=	0;
						m_IsChIn		=	FALSE;
						break;
				}
				if ( E_GrUiKeyNone != Tv_Key )
				{
					m_TimerFz		=	0;
					DvrRtlKeyEmu( Tv_Key, TRUE );
					DvrRtlKeyEmu( Tv_Key, FALSE );
					m_ChNum			=	0;
					m_IsChIn		=	FALSE;
				}
				//update channel number
				if ( 16 < m_ChNum )
				{
					m_ChNum	=	100;
				}
			}
		}
		//reset buffer
		m_RecvIn	=	0;
		m_RecvMax	=	1;
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliWtx1200::RtlTimer( void )
{
	// local -------------------
	// code --------------------
		// key interval
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
				DvrRtlPtzDoStop( DvrRtlQuadViewChGet(0) );
			}
		}
		
}
//--------------------------------------------------------------------

