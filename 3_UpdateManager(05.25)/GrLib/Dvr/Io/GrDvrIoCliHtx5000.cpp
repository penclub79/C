/*
 dvr io client Htx5000

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Io/GrDvrIoCliHtx5000.h>
#include	<DvrRtl.h>
#include	<GrUart.h>
#include	<Ui/GrUiBase.h>
//====================================================================
//local const
#define E_GrDvrioCliHtx5000DirKeyIntv	2

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrIoCliHtx5000::Cls_GrDvrIoCliHtx5000( __u8 A_UartCh, __u8 A_Id ):
Cls_GrDvrIoCliBase( A_UartCh, A_Id )
{
		m_RecvIn	=	0;
		m_RecvMax	=	3;
		m_DirKeyIntv	=	0;
}
//--------------------------------------------------------------------
Cls_GrDvrIoCliHtx5000::~Cls_GrDvrIoCliHtx5000()
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrIoCliHtx5000::RtlProc( void )
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
						if ( 0xAA == m_RecvBuf[0] )
						{
							//finded
							m_RecvMax	=	3;
						}
						else
						{
							m_RecvIn	=	0;
						}
						break;
					case	3:
						m_RecvMax	=	5;
						if ( 0x80 == m_RecvBuf[2] )
						{
							m_RecvMax	=	8;
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
void	Cls_GrDvrIoCliHtx5000::LcParsPrtc( void )
{
	// local -------------------
		__u8	Tv_ChkSum;
		__u32	Tv_PlaySpd;
		BOOL8	Tv_IsWide;
		__u8	Tv_Speed;
		BOOL8	Tv_IsLeft;
		BOOL8	Tv_IsRight;
		BOOL8	Tv_IsUp;
		BOOL8	Tv_IsDown;
		__u8	Tv_PanSpd;
		__u8	Tv_TiltSpd;
		__u8	Tv_Key;
	// code --------------------
		//check id
		if ( m_Id == m_RecvBuf[1] )
		{
			if ( 0x80 == m_RecvBuf[2] )
			{
				//8 byte command
				//check checksum
				Tv_ChkSum	=	m_RecvBuf[0] + m_RecvBuf[1] + m_RecvBuf[2] + m_RecvBuf[3] + m_RecvBuf[4] +
					m_RecvBuf[5] + m_RecvBuf[6];
				if ( m_RecvBuf[7] == Tv_ChkSum )
				{
					//check ptz control
					if ( 0 != (0x80 & m_RecvBuf[4]) )
					{
						if ( DvrRtlPtzIsView() )
						{
							//pantilt mode
							if ( 0 != (0x10 & m_RecvBuf[4]) )
							{
								//zoom
								Tv_IsWide	=	FALSE;
								if ( 0 != (0x08 & m_RecvBuf[4] ) )
								{
									Tv_IsWide	=	TRUE;
								}
								Tv_Speed	=	m_RecvBuf[4] & 0x07;
								if ( 4 < Tv_Speed )
								{
									Tv_Speed	=	4;
								}
								DvrRtlPtzDoZoom( DvrRtlQuadViewChGet(0), Tv_IsWide, Tv_Speed );
							}
							else if ( 0x88 == m_RecvBuf[4] )
							{
								//focus near
								DvrRtlPtzDoFocus( DvrRtlQuadViewChGet(0), FALSE );
							}
							else if ( 0x84 == m_RecvBuf[4] )
							{
								//forcus far
								DvrRtlPtzDoFocus( DvrRtlQuadViewChGet(0), TRUE );
							}
							else if ( 0x82 == m_RecvBuf[4] )
							{
								//iris close
								DvrRtlPtzDoIris( DvrRtlQuadViewChGet(0), FALSE );
							}
							else if ( 0x81 == m_RecvBuf[4] )
							{
								//iris open
								DvrRtlPtzDoIris( DvrRtlQuadViewChGet(0), TRUE );
							}
							else if ( 0x80 == m_RecvBuf[4] )
							{
								//pan and tilt
								if ( (0 == m_RecvBuf[5]) && (0 == m_RecvBuf[6]) )
								{
									DvrRtlPtzDoStop( DvrRtlQuadViewChGet(0) );
								}
								else
								{
									//pantilt
									Tv_IsLeft		=	FALSE;
									Tv_IsRight	=	FALSE;
									Tv_IsUp			=	FALSE;
									Tv_IsDown		=	FALSE;
									Tv_PanSpd		=	0;
									Tv_TiltSpd	=	0;
									if ( 0 != m_RecvBuf[5] )
									{
										if ( 0 != (0x80 & m_RecvBuf[5]) )
										{
											Tv_IsLeft		=	TRUE;
										}
										else
										{
											Tv_IsRight	=	TRUE;
										}
										Tv_PanSpd	=	(m_RecvBuf[5] >> 3) & 0x07;
										if ( 4 < Tv_PanSpd )
										{
											Tv_PanSpd	=	4;
										}
									}
									if ( 0 != m_RecvBuf[6] )
									{
										if ( 0 != (0x80 & m_RecvBuf[6]) )
										{
											Tv_IsUp		=	TRUE;
										}
										else
										{
											Tv_IsDown	=	TRUE;
										}
										Tv_TiltSpd	=	(m_RecvBuf[6] >> 3) & 0x07;
										if ( 4 < Tv_TiltSpd )
										{
											Tv_TiltSpd	=	4;
										}
									}
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
								if (0x10 <= (0x3F & m_RecvBuf[5]) )
								{
									//left right
									Tv_Key	=	E_GrUiKeyRight;
									if ( 0 != (0x80 & m_RecvBuf[5]) )
									{
										Tv_Key	=	E_GrUiKeyLeft;
									}
								}
								else if ( 0x10 <= (0x3F & m_RecvBuf[6]) )
								{
									//up down
									Tv_Key	=	E_GrUiKeyDown;
									if ( 0 != (0x80 & m_RecvBuf[6]) )
									{
										Tv_Key	=	E_GrUiKeyUp;
									}
								}
								if ( E_GrUiKeyNone != Tv_Key )
								{
									DvrRtlKeyEmu( Tv_Key, TRUE );
									DvrRtlKeyEmu( Tv_Key, FALSE );
									m_DirKeyIntv	=	E_GrDvrioCliHtx5000DirKeyIntv;
								}
							}
						}
					}
					else
					{
						//run command
						switch( m_RecvBuf[4] )
						{
							case	0x24:
								DvrRtlKeyEmu( E_GrUiKeyMenu, TRUE );
								DvrRtlKeyEmu( E_GrUiKeyMenu, FALSE );
								break;
							case	0x27://ESC
								DvrRtlKeyEmu( E_GrUiKeyMenu, TRUE );
								DvrRtlKeyEmu( E_GrUiKeyMenu, FALSE );
								break;
							case	0x2C:
								//enterkey only
								DvrRtlKeyEmu( E_GrUiKeyEnter, TRUE );
								DvrRtlKeyEmu( E_GrUiKeyEnter, FALSE );
								break;
						}
					}
				}
			}
			else
			{
				//5 byte command
				//check checksum
				Tv_ChkSum	=	m_RecvBuf[0] + m_RecvBuf[1] + m_RecvBuf[2] + m_RecvBuf[3];
				if ( m_RecvBuf[4] == Tv_ChkSum )
				{
					//run command
					switch( m_RecvBuf[2] )
					{
						case	0x0C:
							LcProcQuadKey( m_RecvBuf[3] );
							break;
						case	0x04:
							//rewind
							if ( DvrRtlIsPlayMode() )
							{
								if ( DvrRtlIsPlayDirBack() )
								{
									Tv_PlaySpd	=	DvrRtlPlaySpeedGet();
									if ( 4 > Tv_PlaySpd )
									{
										DvrRtlPlaySpeedSet( Tv_PlaySpd + 1 );
									}
								}
								else
								{
									DvrRtlPlayDirSet( TRUE );
									DvrRtlPlaySpeedSet( 0 );
								}
								DvrRtlPlayDoPlay();
							}
							break;
						case	0x05:
							//fast foword
							if ( DvrRtlIsPlayMode() )
							{
								if ( !DvrRtlIsPlayDirBack() )
								{
									Tv_PlaySpd	=	DvrRtlPlaySpeedGet();
									if ( 4 > Tv_PlaySpd )
									{
										DvrRtlPlaySpeedSet( Tv_PlaySpd + 1 );
									}
								}
								else
								{
									DvrRtlPlayDirSet( FALSE );
									DvrRtlPlaySpeedSet( 0 );
								}
								DvrRtlPlayDoPlay();
							}
							break;
						case	0x03:
							//play
							if ( DvrRtlIsPlayMode() )
							{
								if ( DvrRtlIsPlayRun() )
								{
									DvrRtlPlayPause();
								}
								else
								{
									DvrRtlPlayDoPlay(); 
								}
							}
							break;
						case	0x02:
							//stop
							if ( DvrRtlIsPlayMode() )
							{
								DvrRtlPlayPause();
							}
							break;
						case	0x08:
							//emergency
							DvrRtlKeyEmu( E_GrUiKeyEmergency, TRUE );
							DvrRtlKeyEmu( E_GrUiKeyEmergency, FALSE );
							break;
						case	0x30:
						case	0x34:
							//cam , number
							DvrRtlKeyEmu( E_GrUiKeyCh0 + m_RecvBuf[3], TRUE );
							DvrRtlKeyEmu( E_GrUiKeyCh0 + m_RecvBuf[3], FALSE );
							break;
						case	0x09:
							DvrRtlKeyEmu( E_GrUiKeySearch, TRUE );
							DvrRtlKeyEmu( E_GrUiKeySearch, FALSE );
							break;
						case	0x01:
							DvrRtlKeyEmu( E_GrUiKeyPtzMode, TRUE );
							DvrRtlKeyEmu( E_GrUiKeyPtzMode, FALSE );
							break;
						case	0x0D:
							DvrRtlKeyEmu( E_GrUiKeyAudio, TRUE );
							DvrRtlKeyEmu( E_GrUiKeyAudio, FALSE );
							break;
						case	0x0B:
							DvrRtlKeyEmu( E_GrUiKeySequence, TRUE );
							DvrRtlKeyEmu( E_GrUiKeySequence, FALSE );
							break;
						case 0x0A:
							DvrRtlKeyEmu( E_GrUiKeyBackup, TRUE );
							DvrRtlKeyEmu( E_GrUiKeyBackup, FALSE );						
					}
				}
			}
		}
		//reset buffer
		m_RecvIn	=	0;
		m_RecvMax	=	1;
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliHtx5000::LcProcQuadKey( __u8 A_Num )
{
	// local -------------------
	// code --------------------
		//change quad
		if ( 0 == A_Num )
		{
			DvrRtlQuadModeNext();
		}
		else
		{
			DvrRtlQuadModeSet( DvrRtlQuadModeGet(), A_Num - 1 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliHtx5000::RtlTimer( void )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_DirKeyIntv )
		{
			m_DirKeyIntv --;
		}
}
//--------------------------------------------------------------------

