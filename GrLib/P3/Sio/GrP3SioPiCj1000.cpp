/*
 DVR SIO CJ1000

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<P3/Sio/GrP3SioPiCj1000.h>
#include	<GrUart.h>
#include <Dvr/PtzV2/GrPtzV2Mng.h>
#include <P3/GrP3Base.h>
#include <P3/GrP3Bios.h>
#include <P3/GrP3ApiBase.h>

//====================================================================
//local const
#define E_GrP3SioPiCj1000DirKeyIntv	2

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP3Mng*	V_GrP3Mng;

//====================================================================

//--------------------------------------------------------------------
Cls_GrP3SioPiCj1000::Cls_GrP3SioPiCj1000( void* A_SioMng, __u8 A_Uart, __u8 A_Id ):
Cls_GrP3SioPiBase( A_SioMng, A_Uart, A_Id )
{

		m_RecvIn			=	0;
		m_RecvMax			=	3;
		m_DirKeyIntv	=	0;

		m_PtrGvs			=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet( );
}
//--------------------------------------------------------------------
Cls_GrP3SioPiCj1000::~Cls_GrP3SioPiCj1000()
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3SioPiCj1000::RtlBkgProc( void )
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
							//found
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
void	Cls_GrP3SioPiCj1000::LcParsPrtc( void )
{
	// local -------------------
		__u8	Tv_ChkSum;
		__u8	Tv_Key;
		__s32		Tv_Rint[4];
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
					//check PTZ control
					if ( 0 != (0x80 & m_RecvBuf[4]) )
					{
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
								if (0x10 <= (0x3F & m_RecvBuf[5]) )
								{
									//left right
									Tv_Key	=	E_GrP3KeyRight;
									if ( 0 != (0x80 & m_RecvBuf[5]) )
									{
										Tv_Key	=	E_GrP3KeyLeft;
									}
								}
								else if ( 0x10 <= (0x3F & m_RecvBuf[6]) )
								{
									//up down
									Tv_Key	=	E_GrP3KeyDown;
									if ( 0 != (0x80 & m_RecvBuf[6]) )
									{
										Tv_Key	=	E_GrP3KeyUp;
									}
								}
								if ( E_GrP3KeyNone != Tv_Key )
								{
									V_GrP3Mng->KeyInput( Tv_Key, TRUE, E_GrP3InDevPtzKbd );
									V_GrP3Mng->KeyInput( Tv_Key, FALSE, E_GrP3InDevPtzKbd );
									m_DirKeyIntv	=	E_GrP3SioPiCj1000DirKeyIntv;
								}
							}
						}
					}
					else
					{
						//run command
						switch( m_RecvBuf[4] )
						{
							/*
							case	0x21:
								DvrRtlKeyEmu( E_GrP3KeySequence, TRUE );
								DvrRtlKeyEmu( E_GrP3KeySequence, FALSE );
								break;
								*/
								/*
							case	0x23:
								DvrRtlKeyEmu( E_GrP3KeyPtzMode, TRUE );
								DvrRtlKeyEmu( E_GrP3KeyPtzMode, FALSE );
								break;
								*/
							case	0x24:
								V_GrP3Mng->KeyInput( E_GrP3KeyMenu, TRUE, E_GrP3InDevPtzKbd );
								V_GrP3Mng->KeyInput( E_GrP3KeyMenu, FALSE, E_GrP3InDevPtzKbd );
								break;
								/*
							case	0x25:
								DvrRtlKeyEmu( E_GrP3KeyAudio, TRUE );
								DvrRtlKeyEmu( E_GrP3KeyAudio, FALSE );
								break;
								*/
								/*
							case	0x26:
								DvrRtlKeyEmu( E_GrP3KeySearch, TRUE );
								DvrRtlKeyEmu( E_GrP3KeySearch, FALSE );
								break;
								*/
							case	0x27:
								V_GrP3Mng->KeyInput( E_GrP3KeyMenu, TRUE, E_GrP3InDevPtzKbd );
								V_GrP3Mng->KeyInput( E_GrP3KeyMenu, FALSE, E_GrP3InDevPtzKbd );
								break;
							case	0x2C:
								//enter key only
								V_GrP3Mng->KeyInput( E_GrP3KeyEnter, TRUE, E_GrP3InDevPtzKbd );
								V_GrP3Mng->KeyInput( E_GrP3KeyEnter, FALSE, E_GrP3InDevPtzKbd );
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
							/*
						case	0x07:
							DvrRtlKeyEmu( E_GrP3KeyMenu, TRUE );
							DvrRtlKeyEmu( E_GrP3KeyMenu, FALSE );
							break;
							*/
						case	0x03:
							// rewind
							if ( m_PtrGvs->TblIsPlay[0] )
							{
								if ( m_PtrGvs->IsPlayBack )
								{
									if ( 4 > m_PtrGvs->PlaySpd )
									{
										Tv_Rint[1]	=	(__s32)m_PtrGvs->PlaySpd + 1;
										GrP3BiosPlaySpeedSet( Tv_Rint, NULL );
									}
								}
								else
								{
									Tv_Rint[1]	=	(__s32)TRUE;
									GrP3BiosPlayDirSet( Tv_Rint, NULL );
									Tv_Rint[1]	=	0;
									GrP3BiosPlaySpeedSet( Tv_Rint, NULL );
								}
								GrP3BiosPlayDo( Tv_Rint, NULL );
							}
							break;
						case	0x04:
							//fast forward
							if ( m_PtrGvs->TblIsPlay[0] )
							{
								if ( !m_PtrGvs->IsPlayBack )
								{
									if ( 4 > m_PtrGvs->PlaySpd )
									{
										Tv_Rint[1]	=	(__s32)m_PtrGvs->PlaySpd + 1;
										GrP3BiosPlaySpeedSet( Tv_Rint, NULL );
									}
								}
								else
								{
									Tv_Rint[1]	=	(__s32)FALSE;
									GrP3BiosPlayDirSet( Tv_Rint, NULL );
									Tv_Rint[1]	=	0;
									GrP3BiosPlaySpeedSet( Tv_Rint, NULL );
								}
								GrP3BiosPlayDo( Tv_Rint, NULL );
							}
							break;
						case	0x02:
							//play
							if ( m_PtrGvs->TblIsPlay[0] )
							{
								if ( E_GrDvrPlayStatStop != m_PtrGvs->PlayStat )
								{
									GrP3BiosPlayPause( Tv_Rint, NULL );
								}
								else
								{
									GrP3BiosPlayDo( Tv_Rint, NULL );
								}
							}
							break;
						case	0x05:
							//stop
							if ( m_PtrGvs->TblIsPlay[0] )
							{
								GrP3BiosPlayPause( Tv_Rint, NULL );
							}
							break;
						case	0x06:
							//emergency
							V_GrP3Mng->KeyInput( E_GrP3KeyEmergency, TRUE, E_GrP3InDevPtzKbd );
							V_GrP3Mng->KeyInput( E_GrP3KeyEmergency, FALSE, E_GrP3InDevPtzKbd );
							break;
						case	0x35:
							//backup
							V_GrP3Mng->KeyInput( E_GrP3KeyBackup, TRUE, E_GrP3InDevPtzKbd );
							V_GrP3Mng->KeyInput( E_GrP3KeyBackup, FALSE, E_GrP3InDevPtzKbd );
							break;
						case	0x01:
							//lock
							GrP3ApiLogOut( Tv_Rint, NULL );
							break;
						case	0x30:
						case	0x34:
							//cam , number
							Tv_Key	=	E_GrP3KeyCh0 + m_RecvBuf[3];
							V_GrP3Mng->KeyInput( Tv_Key, TRUE, E_GrP3InDevPtzKbd );
							V_GrP3Mng->KeyInput( Tv_Key, FALSE, E_GrP3InDevPtzKbd );
							break;
						case	0x38:
							V_GrP3Mng->KeyInput( E_GrP3KeySearch, TRUE, E_GrP3InDevPtzKbd );
							V_GrP3Mng->KeyInput( E_GrP3KeySearch, FALSE, E_GrP3InDevPtzKbd );
							break;
						case	0x0A:
							V_GrP3Mng->KeyInput( E_GrP3KeyPtzMode, TRUE, E_GrP3InDevPtzKbd );
							V_GrP3Mng->KeyInput( E_GrP3KeyPtzMode, FALSE, E_GrP3InDevPtzKbd );
							break;
						case	0x3A:
							V_GrP3Mng->KeyInput( E_GrP3KeySequence, TRUE, E_GrP3InDevPtzKbd );
							V_GrP3Mng->KeyInput( E_GrP3KeySequence, FALSE, E_GrP3InDevPtzKbd );
							break;
						case	0x08:
						case	0x36:
							V_GrP3Mng->KeyInput( E_GrP3KeyAudio, TRUE, E_GrP3InDevPtzKbd );
							V_GrP3Mng->KeyInput( E_GrP3KeyAudio, FALSE, E_GrP3InDevPtzKbd );
							break;
						case	0x09:
							V_GrP3Mng->KeyInput( E_GrP3KeySequence, TRUE, E_GrP3InDevPtzKbd );
							V_GrP3Mng->KeyInput( E_GrP3KeySequence, FALSE, E_GrP3InDevPtzKbd );
							break;
					}
				}
			}
		}
		//reset buffer
		m_RecvIn	=	0;
		m_RecvMax	=	1;
}
//--------------------------------------------------------------------
void	Cls_GrP3SioPiCj1000::LcProcQuadKey( __u8 A_Num )
{
	// local -------------------
		__s32	Tv_Rint[4];
	// code --------------------
		//change quad
		if ( 0 == A_Num )
		{
			Tv_Rint[1]	=	0;
			GrP3ApiQuadModeNext( Tv_Rint, NULL );
		}
		else
		{
			Tv_Rint[1]	=	0;
			GrP3ApiQuadModeGet( Tv_Rint, NULL );
			Tv_Rint[1]	=	0;
			Tv_Rint[2]	=	Tv_Rint[0];
			Tv_Rint[3]	=	(__s32)A_Num - 1;
			GrP3ApiQuadModeSet( Tv_Rint, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3SioPiCj1000::RtlTimer( void )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_DirKeyIntv )
		{
			m_DirKeyIntv --;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3SioPiCj1000::LcProcPtzCtrl( void )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		BOOL8	Tv_IsWide;
		__u8	Tv_Speed;
		BOOL8	Tv_IsLeft;
		BOOL8	Tv_IsRight;
		BOOL8	Tv_IsUp;
		BOOL8	Tv_IsDown;
		__u8	Tv_SpdPan;
		__u8	Tv_SpdTilt;
	// code --------------------
		// get PTZ control
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			//pan tilt mode
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
				Tv_PtzMng->SafeDoZoom( m_PtrGvs->PtzCtrlCh, Tv_IsWide, Tv_Speed, 0 );
			}
			else if ( 0x88 == m_RecvBuf[4] )
			{
				//focus near
				Tv_PtzMng->SafeDoFocus( m_PtrGvs->PtzCtrlCh, FALSE, 0xFF, 0 );
			}
			else if ( 0x84 == m_RecvBuf[4] )
			{
				// focus far
				Tv_PtzMng->SafeDoFocus( m_PtrGvs->PtzCtrlCh, TRUE, 0xFF, 0 );
			}
			else if ( 0x80 == m_RecvBuf[4] )
			{
				//pan and tilt
				if ( (0 == m_RecvBuf[5]) && (0 == m_RecvBuf[6]) )
				{
					Tv_PtzMng->SafeDoStop( m_PtrGvs->PtzCtrlCh );
				}
				else
				{
					// PANTILT
					Tv_IsLeft		=	FALSE;
					Tv_IsRight	=	FALSE;
					Tv_IsUp			=	FALSE;
					Tv_IsDown		=	FALSE;
					Tv_SpdPan		=	0;
					Tv_SpdTilt	=	0;
					if ( 0 != m_RecvBuf[5] )
					{
						if ( 0 != (0x80 & m_RecvBuf[5]) )
						{
							Tv_IsLeft	=	TRUE;
						}
						else
						{
							Tv_IsRight	=	TRUE;
						}
						Tv_SpdPan	=	(m_RecvBuf[5] >> 3) & 0x07;
						if ( 4 < Tv_SpdPan )
						{
							Tv_SpdPan	=	4;
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
						Tv_SpdTilt	=	(m_RecvBuf[6] >> 3) & 0x07;
						if ( 4 < Tv_SpdTilt )
						{
							Tv_SpdTilt	=	4;
						}
					}

					Tv_PtzMng->SafeDoPanTilt( m_PtrGvs->PtzCtrlCh, Tv_IsLeft, Tv_IsRight, Tv_IsUp, Tv_IsDown, Tv_SpdPan, Tv_SpdTilt, 0 );
				}
			}
		}
}
//--------------------------------------------------------------------

