/*
 dvr pantilt protocol Honeywell
 +------------------------------------------------------------+
 |   byte1    | byte2 | byte3 | byte4 | byte5 | byte6 | byte6 |
 +------------------------------------------------------------+
 |Header(0xFA)|  ID1  |  ID2  |  CMD  | DATA1 | DATA2 |  C/S  |
 +------------------------------------------------------------+
 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtHoneywell.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtHoneywellChksumIdx			6
#define E_GrDvrPtzPtHoneywellPktSize				7	

//format type
#define E_GrDvrPtzPtHoneywell7Byte		0
#define E_GrDvrPtzPtHoneywell5Byte		1
//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

__u8	V_DvrPtzPtHoneywellPanTiltSpdTbl[5]	=	
{
	0x01, 0x10,	0x20, 0x30, 0x3F
};

__u8	V_DvrPtzPtHoneywellDZoomFcsSpdTbl[5]	=	
{
	0x01, 0x02,	0x03, 0x05, 0x07
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtHoneywell::Cls_GrDvrPtzPtHoneywell( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtHoneywell::~Cls_GrDvrPtzPtHoneywell()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::LcSendPkt( __u8 A_Cmd, __u8 A_Data1, __u8 A_Data2, __u8 A_Type )
{
	// local -------------------
		__u8	Tv_ChkIdx;
		__u8	Tv_PktBuf[E_GrDvrPtzPtHoneywellPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_ChkIdx	=	4;
		Tv_PktBuf[0]		=	0xFA;
		Tv_PktBuf[1]		=	((__u32)m_Id >> 8) + 0x50;
		Tv_PktBuf[2]		=	m_Id & 0xFF;
		Tv_PktBuf[3]		=	A_Cmd;
		if ( E_GrDvrPtzPtHoneywell7Byte == A_Type )
		{
			Tv_PktBuf[4]		=	A_Data1;
			Tv_PktBuf[5]		=	A_Data2;
			Tv_ChkIdx	=	E_GrDvrPtzPtHoneywellChksumIdx;
		}

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[0] + (__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3];
		if ( E_GrDvrPtzPtHoneywell7Byte == A_Type )
		{
			Tv_Chksum	=	Tv_Chksum + (__u32)Tv_PktBuf[4] + (__u32)Tv_PktBuf[5];
		}

		Tv_PktBuf[Tv_ChkIdx]	=	(__u8)( 0xFF & Tv_Chksum );

		//send
		GrUartSend( m_UartCh, Tv_PktBuf, Tv_ChkIdx + 1, TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtHoneywell::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtHoneywellPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtHoneywell::LcCvtZoomFcsSpeed( __u8 A_Spd )
{
		return	V_DvrPtzPtHoneywellDZoomFcsSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtHoneywell::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data1;
		__u8	Tv_Data2;
	// code --------------------
		Tv_Data1	=	0;
		Tv_Data2	=	0;

		//pan
		if ( A_IsLeft )
		{
			Tv_Data1	=	0x80 | LcCvtPanTiltSpeed(A_SpdPan);
		}
		if ( A_IsRight )
		{
			Tv_Data1	=	LcCvtPanTiltSpeed(A_SpdPan);
		}
		//tilt
		if ( A_IsUp )
		{
			Tv_Data2	=	0x80 | LcCvtPanTiltSpeed(A_SpdTilt);
		}
		if ( A_IsDown )
		{
			Tv_Data2	=	LcCvtPanTiltSpeed(A_SpdTilt);
		}
		LcSendPkt( 0x80, Tv_Data1, Tv_Data2, E_GrDvrPtzPtHoneywell7Byte );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtHoneywell::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//set zoom		
		Tv_Cmd		=	0x90 | LcCvtZoomFcsSpeed(A_Speed);
		if ( A_IsWide )
		{
			Tv_Cmd	=	Tv_Cmd | 0x08;
		}
		LcSendPkt( Tv_Cmd, 0, 0, E_GrDvrPtzPtHoneywell7Byte );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtHoneywell::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//set focus		
		Tv_Cmd		=	0x80;
		if ( A_IsFar )
		{
			Tv_Cmd		=	Tv_Cmd | 0x04;
		}
		else
		{
			Tv_Cmd		=	Tv_Cmd | 0x08;
		}
		LcSendPkt( Tv_Cmd, 0, 0, E_GrDvrPtzPtHoneywell7Byte );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtHoneywell::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//set iris		
		Tv_Cmd		=	0x80;
		if ( A_IsOpen )
		{
			Tv_Cmd		=	Tv_Cmd | 0x01;
		}
		else
		{
			Tv_Cmd		=	Tv_Cmd | 0x02;
		}
		LcSendPkt( Tv_Cmd, 0, 0, E_GrDvrPtzPtHoneywell7Byte );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::DoStop( void )
{
		LcSendPkt( 0x00, 0x00, 0x00, E_GrDvrPtzPtHoneywell7Byte );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::DoAutoFocus( void )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//LcSendPkt( 0x41, 0x00, 0x00, E_GrDvrPtzPtHoneywell5Byte );
		Tv_Cmd		=	0x90 | LcCvtZoomFcsSpeed(0);
		LcSendPkt( Tv_Cmd, 0, 0, E_GrDvrPtzPtHoneywell7Byte );
		GrPrcSleep( 500 );
		Tv_Cmd		=	0x98 | LcCvtZoomFcsSpeed(0);;
		LcSendPkt( Tv_Cmd, 0, 0, E_GrDvrPtzPtHoneywell7Byte );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::DoPresetSet( __u8 A_Id )
{
	// local -------------------
	// code --------------------
		LcSendPkt( 0x17, A_Id, 0x00, E_GrDvrPtzPtHoneywell7Byte );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		LcSendPkt( 0x11, A_Id, 0x00, E_GrDvrPtzPtHoneywell7Byte );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtHoneywell::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::DoMenuCall( void )
{
		LcSendPkt( 0x24, 0x00, 0x00, E_GrDvrPtzPtHoneywell5Byte );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::DoMenuExit( void )
{
		LcSendPkt( 0x27, 0x00, 0x00, E_GrDvrPtzPtHoneywell5Byte );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::DoMenuOk( void )
{
		LcSendPkt( 0x2C, 0x00, 0x00, E_GrDvrPtzPtHoneywell5Byte );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::DoMenuCancel( void )
{
		LcSendPkt( 0x27, 0x00, 0x00, E_GrDvrPtzPtHoneywell5Byte );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::TourRun( void )
{
		LcSendPkt( 0x12, 0x00, 0x00, E_GrDvrPtzPtHoneywell7Byte );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtHoneywell::TourStop( void )
{
		LcSendPkt( 0x27, 0x00, 0x00, E_GrDvrPtzPtHoneywell7Byte );
}
//--------------------------------------------------------------------

