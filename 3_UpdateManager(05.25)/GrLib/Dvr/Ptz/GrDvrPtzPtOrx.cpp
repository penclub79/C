/*
 dvr pantilt protocol orx

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtOrx.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtOrxChksumIdx			5

#define E_GrDvrPtzPtOrxPktSize				6	

#define E_GrDvrPtzPtOrxDoTimeCnt			2

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtOrx::Cls_GrDvrPtzPtOrx( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
		m_LastDoMap		=	0;
		m_LastDoTime	=	0;
		LcSendPkt( 0x63, 0x40 );		//power on
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtOrx::~Cls_GrDvrPtzPtOrx()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtOrx::LcSendPkt( __u8 A_Data4, __u8 A_Data5 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtOrxPktSize];
		__u8	Tv_Mod;
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_Mod					=	m_Id;
		Tv_PktBuf[0]		=	(Tv_Mod / 100) | 0x10;
		Tv_Mod					=	Tv_Mod % 100;
		Tv_PktBuf[2]		=	(Tv_Mod / 10) | 0x30;
		Tv_Mod					=	Tv_Mod % 10;
		Tv_PktBuf[1]		=	Tv_Mod | 0x20;

		Tv_PktBuf[3]		=	A_Data4;
		Tv_PktBuf[4]		=	A_Data5;

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[0] + (__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4];
		Tv_PktBuf[E_GrDvrPtzPtOrxChksumIdx]	=	((__u8)( 0xFF & Tv_Chksum ));
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtOrx::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd5;
		__u8	Tv_DoMap;
	// code --------------------
		Tv_DoMap	=	0;
		Tv_Cmd5		=	0x40;
		//pan
		if ( A_IsLeft )
		{
			Tv_Cmd5		=	Tv_Cmd5 | 0x04;
			Tv_DoMap	=	Tv_DoMap | E_GrDvrPtzPtOrxDoLeft;
		}
		if ( A_IsRight )
		{
			Tv_Cmd5	=	Tv_Cmd5 | 0x08;
			Tv_DoMap	=	Tv_DoMap | E_GrDvrPtzPtOrxDoRight;
		}
		if ( A_IsUp )
		{
			Tv_Cmd5	=	Tv_Cmd5 | 0x01;
			Tv_DoMap	=	Tv_DoMap | E_GrDvrPtzPtOrxDoUp;
		}
		if ( A_IsDown )
		{
			Tv_Cmd5	=	Tv_Cmd5 | 0x02;
			Tv_DoMap	=	Tv_DoMap | E_GrDvrPtzPtOrxDoDown;
		}
		LcSendPkt( 0x00, Tv_Cmd5 );
		m_LastDoMap		=	Tv_DoMap;
		m_LastDoTime	=	E_GrDvrPtzPtOrxDoTimeCnt;
		
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtOrx::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd4;
		__u8	Tv_DoMap;
	// code --------------------
		//set zoom		
		Tv_DoMap	=	E_GrDvrPtzPtOrxDoTele;
		Tv_Cmd4		=	0xA3;
		if ( A_IsWide )
		{
			Tv_Cmd4		=	0xA0;
			Tv_DoMap	=	E_GrDvrPtzPtOrxDoWide;
		}
		LcSendPkt( Tv_Cmd4, 0x40 );
		m_LastDoMap		=	Tv_DoMap;
		m_LastDoTime	=	E_GrDvrPtzPtOrxDoTimeCnt;

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtOrx::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd4;
		__u8	Tv_DoMap;
	// code --------------------
		Tv_Cmd4	=	0xAC;
		Tv_DoMap	=	E_GrDvrPtzPtOrxDoNear;
		if ( A_IsFar )
		{
			Tv_Cmd4	=	0xAF;
			Tv_DoMap	=	E_GrDvrPtzPtOrxDoFar;
		}
		LcSendPkt( Tv_Cmd4, 0x40 );
		m_LastDoMap		=	Tv_DoMap;
		m_LastDoTime	=	E_GrDvrPtzPtOrxDoTimeCnt;

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtOrx::DoStop( void )
{
		LcSendPkt( 0x00, 0x40 );
		m_LastDoMap	=	0;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtOrx::DoTimer( void )
{
	// local -------------------
		__u8	Tv_Cmd4;
		__u8	Tv_Cmd5;
	// code --------------------
		if ( 0 != m_LastDoMap )
		{
			m_LastDoTime --;
			if ( 0 == m_LastDoTime )
			{
				Tv_Cmd4		=	0;
				Tv_Cmd5		=	0x40;
				//redo
				if ( 0 != (E_GrDvrPtzPtOrxDoLeft & m_LastDoMap) )
				{
					Tv_Cmd5		=	Tv_Cmd5 | 0x04;
				}
				if ( 0 != (E_GrDvrPtzPtOrxDoRight & m_LastDoMap) )
				{
					Tv_Cmd5	=	Tv_Cmd5 | 0x08;
				}
				if ( 0 != (E_GrDvrPtzPtOrxDoUp & m_LastDoMap) )
				{
					Tv_Cmd5	=	Tv_Cmd5 | 0x01;
				}
				if ( 0 != (E_GrDvrPtzPtOrxDoDown & m_LastDoMap) )
				{
					Tv_Cmd5	=	Tv_Cmd5 | 0x02;
				}
				if ( 0 != (E_GrDvrPtzPtOrxDoNear & m_LastDoMap) )
				{
					Tv_Cmd4	=	0xAC;
				}
				if ( 0 != (E_GrDvrPtzPtOrxDoFar & m_LastDoMap) )
				{
					Tv_Cmd4	=	0xAF;
				}
				if ( 0 != (E_GrDvrPtzPtOrxDoTele & m_LastDoMap) )
				{
					Tv_Cmd4	=	0xA3;
				}
				if ( 0 != (E_GrDvrPtzPtOrxDoWide & m_LastDoMap) )
				{
					Tv_Cmd4	=	0xA0;
				}
				LcSendPkt( Tv_Cmd4, Tv_Cmd5 );

				//timer update
				m_LastDoTime	=	E_GrDvrPtzPtOrxDoTimeCnt;
			}
		}
}
//--------------------------------------------------------------------
