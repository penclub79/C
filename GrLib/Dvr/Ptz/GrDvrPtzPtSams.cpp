/*
 dvr pantilt protocol pelco-d

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtSams.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtSamsChksumIdx			10

#define E_GrDvrPtzPtSamsPktSize				11	


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8	V_DvrPtzPtSamsPanTiltSpdTbl[5]	=	
{
	0x01, 0x10,	0x20, 0x30, 0x40
};

__u8	V_DvrPtzPtSamsDZoomFcsSpdTbl[5]	=	
{
	0x11, 0x22,	0x44, 0x66, 0x88
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtSams::Cls_GrDvrPtzPtSams( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtSams::~Cls_GrDvrPtzPtSams()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSams::LcSendPkt( __u8 A_Cmd1, __u8 A_Cmd2, __u8 A_Data1, __u8 A_Data2, __u8 A_Data3 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtSamsPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xA0;
		Tv_PktBuf[1]		=	m_Id;
		Tv_PktBuf[2]		=	0;
		Tv_PktBuf[3]		=	A_Cmd1;
		Tv_PktBuf[4]		=	A_Cmd2;
		Tv_PktBuf[5]		=	A_Data1;
		Tv_PktBuf[6]		=	A_Data2;
		Tv_PktBuf[7]		=	A_Data3;
		Tv_PktBuf[8]		=	0xFF;
		Tv_PktBuf[9]		=	0xAF;

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4] + (__u32)Tv_PktBuf[5] + 
			(__u32)Tv_PktBuf[6] + (__u32)Tv_PktBuf[7] + (__u32)Tv_PktBuf[8];
		Tv_PktBuf[E_GrDvrPtzPtSamsChksumIdx]	=	~((__u8)( 0xFF & Tv_Chksum ));
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSams::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtSamsPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSams::LcCvtZoomFcsSpeed( __u8 A_Spd )
{
		return	V_DvrPtzPtSamsDZoomFcsSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSams::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd2;
	// code --------------------
		Tv_Cmd2	=	0;
		//pan
		if ( A_IsLeft )
		{
			Tv_Cmd2	=	Tv_Cmd2 | 0x04;
		}
		if ( A_IsRight )
		{
			Tv_Cmd2	=	Tv_Cmd2 | 0x02;
		}
		if ( A_IsUp )
		{
			Tv_Cmd2	=	Tv_Cmd2 | 0x08;
		}
		if ( A_IsDown )
		{
			Tv_Cmd2	=	Tv_Cmd2 | 0x10;
		}
		LcSendPkt( 0x00, Tv_Cmd2, LcCvtPanTiltSpeed(A_SpdPan), LcCvtPanTiltSpeed(A_SpdTilt), 0x11 );
		
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSams::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd2;
	// code --------------------
		//set zoom		
		Tv_Cmd2		=	0x20;
		if ( A_IsWide )
		{
			Tv_Cmd2	=	0x40;
		}
		LcSendPkt( 0x00, Tv_Cmd2, 0x00, 0x00, LcCvtZoomFcsSpeed( A_Speed ) );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtSams::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd1;
	// code --------------------
		Tv_Cmd1	=	0x02;
		if ( A_IsFar )
		{
			Tv_Cmd1	=	0x01;
		}
		LcSendPkt( Tv_Cmd1, 0x00, 0x00, 0x00, LcCvtZoomFcsSpeed( A_Speed ) );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSams::DoStop( void )
{
		LcSendPkt( 0x00, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSams::DoAutoFocus( void )
{
		LcSendPkt( 0xE0, 0x15, 0x00, 0x00, 0x88 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSams::DoPresetSet( __u8 A_Id )
{
	// local -------------------
	// code --------------------
		if ( 0x7F > A_Id )
		{
			LcSendPkt( 0x00, 0x03, A_Id, 0x00, 0x00 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtSams::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		if ( 0x7F > A_Id )
		{
			LcSendPkt( 0x00, 0x07, A_Id, 0x00, 0x00 );
		}
}
//--------------------------------------------------------------------
