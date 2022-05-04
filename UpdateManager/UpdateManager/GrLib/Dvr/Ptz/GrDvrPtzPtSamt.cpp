/*
 dvr pantilt protocol Honeywell
 +---------------------------------------------------------------------------------------------+
 |  byte1  | byte2 |  byte3  | byte4 | byte5 | byte6 | byte7 | byte8 | byte9 |  byte10 | byte11|
 +---------+-------+---------+-------+-------+-------+-------+-------+-------+---------+-------+
 |STX(0xA0)| CamID | Host(0) |  CMD1 |  CMD2 | DATA1 | DATA2 | DATA3 | DATA4 |ETX(0xAF)|  C/S  |
 +---------------------------------------------------------------------------------------------+
 check sum = byte2 ~ byte9
 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtSamt.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtSamtPktSize				11

//format type

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

__u8	V_DvrPtzPtSamtPanTiltSpdTbl[5]	=	
{
	0x20, 0x28,	0x30, 0x38, 0x3F
};

__u8	V_DvrPtzPtSamtDZoomFcsSpdTbl[5]	=	
{
	0x01, 0x03,	0x05, 0x07, 0x09
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtSamt::Cls_GrDvrPtzPtSamt( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch, BOOL8 A_MaxPtzSpd ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
		m_IsMaxPtzSpd	=	A_MaxPtzSpd;
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtSamt::~Cls_GrDvrPtzPtSamt()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::LcSendPkt( __u8 A_Cmd1, __u8 A_Cmd2, __u8 A_PanSpd, __u8 A_TiltSpd, __u8 A_ZmFcsSpd )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtSamtPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xA0;		//stx
		Tv_PktBuf[1]		=	m_Id;
		Tv_PktBuf[2]		=	0x00;
		Tv_PktBuf[3]		=	A_Cmd1;
		Tv_PktBuf[4]		=	A_Cmd2;
		Tv_PktBuf[5]		=	A_PanSpd;
		Tv_PktBuf[6]		=	A_TiltSpd;
		Tv_PktBuf[7]		=	A_ZmFcsSpd;
		Tv_PktBuf[8]		=	0;
		Tv_PktBuf[9]		=	0xAF;		//etx

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4]
							+ (__u32)Tv_PktBuf[5] + (__u32)Tv_PktBuf[6] + (__u32)Tv_PktBuf[7] + (__u32)Tv_PktBuf[8];

		Tv_PktBuf[10]	=	~((__u8)( 0xFF & Tv_Chksum ));

		//send
		GrUartSend( m_UartCh, Tv_PktBuf, E_GrDvrPtzPtSamtPktSize, TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSamt::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtSamtPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSamt::LcCvtZoomFcsSpeed( __u8 A_Spd )
{
		return	V_DvrPtzPtSamtDZoomFcsSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSamt::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
		__u8	Tv_PanSpd;
		__u8	Tv_TiltSpd;
	// code --------------------
		Tv_Cmd	=	0;

		//pan
		if ( A_IsLeft )
		{
			Tv_Cmd	=	Tv_Cmd | 0x04;
		}
		if ( A_IsRight )
		{
			Tv_Cmd	=	Tv_Cmd | 0x02;
		}

		//tilt
		if ( A_IsUp )
		{
			Tv_Cmd	=	Tv_Cmd | 0x08;
		}
		if ( A_IsDown )
		{
			Tv_Cmd	=	Tv_Cmd | 0x10;
		}

		if ( m_IsMaxPtzSpd )
		{
			Tv_PanSpd		=	LcCvtPanTiltSpeed(4);
			Tv_TiltSpd	=	LcCvtPanTiltSpeed(4);
		}
		else
		{
			Tv_PanSpd		=	LcCvtPanTiltSpeed(A_SpdPan);
			Tv_TiltSpd	=	LcCvtPanTiltSpeed(A_SpdTilt);
		}
		LcSendPkt( 0, Tv_Cmd, Tv_PanSpd, Tv_TiltSpd, 0 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSamt::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
		__u8	Tv_Speed;
	// code --------------------
		//set zoom		
		Tv_Cmd	=	0x20;
		if ( A_IsWide )
		{
			Tv_Cmd	=	0x40;
		}
		if ( m_IsMaxPtzSpd )
		{
			Tv_Speed	=	LcCvtZoomFcsSpeed(4);
		}
		else
		{
			Tv_Speed	=	LcCvtZoomFcsSpeed(A_Speed);
		}
		LcSendPkt( 0, Tv_Cmd, 0, 0, Tv_Speed );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSamt::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//set focus		
		Tv_Cmd	=	0;
		if ( A_IsFar )
		{
			Tv_Cmd	=	0x01;
		}
		else
		{
			Tv_Cmd	=	0x02;
		}

		LcSendPkt( Tv_Cmd, 0, 0, 0, 0xD0 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSamt::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//set iris		
		Tv_Cmd		=	0;
		if ( A_IsOpen )
		{
			Tv_Cmd		=	Tv_Cmd | 0x10;
		}
		else
		{
			Tv_Cmd		=	Tv_Cmd | 0x20;
		}
		LcSendPkt( Tv_Cmd, 0, 0, 0, 0 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::DoStop( void )
{
		LcSendPkt( 0, 0, 0, 0, 0 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::DoAutoFocus( void )
{
	// local -------------------
	// code --------------------
		LcSendPkt( 0x02, 0, 0, 0, LcCvtZoomFcsSpeed(0) );
		GrPrcSleep( 500 );
		LcSendPkt( 0x04, 0, 0, 0, LcCvtZoomFcsSpeed(0) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::DoPresetSet( __u8 A_Id )
{
	// local -------------------
	// code --------------------
		if ( 0 != A_Id )
		{
			LcSendPkt( 0x00, 0x03, A_Id - 1, 0, 0 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		if ( 0 != A_Id )
		{
			LcSendPkt( 0x00, 0x07, A_Id - 1, 0, 0 );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtSamt::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::DoMenuCall( void )
{
		LcSendPkt( 0x00, 0xB1, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::DoMenuExit( void )
{
		LcSendPkt( 0x00, 0xB1, 0x01, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::DoMenuOk( void )
{
		LcSendPkt( 0x01, 0x00, 0x06, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::DoMenuCancel( void )
{
		LcSendPkt( 0x02, 0x00, 0x07, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::TourRun( void )
{
		LcSendPkt( 0x00, 0x25, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSamt::TourStop( void )
{
		LcSendPkt( 0x00, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------

