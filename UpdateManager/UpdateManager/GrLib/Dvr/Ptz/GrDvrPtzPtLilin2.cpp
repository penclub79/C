/*
 dvr pantilt protocol lilin v2

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtLilin2.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtLilin2ChksumIdx			6

#define E_GrDvrPtzPtLilin2PktSize				7	


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8	V_DvrPtzPtLilin2PanTiltSpdTbl[5]	=	
{
	0x08, 0x09,	0x0A, 0x0C, 0x0F
};

__u8	V_DvrPtzPtLilin2DZoomFcsSpdTbl[5]	=	
{
	0x06, 0x08,	0x0A, 0x0C, 0x0F
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtLilin2::Cls_GrDvrPtzPtLilin2( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch)
{
		//init
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtLilin2::~Cls_GrDvrPtzPtLilin2()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::LcSendPkt( __u8 A_Cmd1, __u8 A_Cmd2, __u8 A_Para1, __u8 A_Para2 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtLilin2PktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xE0;
		Tv_PktBuf[1]		=	m_Id - 1;		//dome
		Tv_PktBuf[2]		=	A_Cmd1;
		Tv_PktBuf[3]		=	A_Cmd2;
		Tv_PktBuf[4]		=	A_Para1;
		Tv_PktBuf[5]		=	A_Para2;

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4] + (__u32)Tv_PktBuf[5];
		Tv_PktBuf[E_GrDvrPtzPtLilin2ChksumIdx]	=	(__u8)( 0xFF & Tv_Chksum );
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtLilin2::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtLilin2PanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtLilin2::LcCvtZoomFcsSpeed( __u8 A_Spd )
{
		return	V_DvrPtzPtLilin2DZoomFcsSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtLilin2::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd2;
		__u8	Tv_Para1;
	// code --------------------
		Tv_Cmd2		=	0;
		Tv_Para1	=	(LcCvtPanTiltSpeed(A_SpdTilt) << 4) | LcCvtPanTiltSpeed(A_SpdPan);

		//pan
		if ( A_IsLeft )
		{
			Tv_Cmd2	=	0x40 | 0x02;
		}
		if ( A_IsRight )
		{
			Tv_Cmd2	=	0x40 | 0x01;
		}
		if ( A_IsUp )
		{
			Tv_Cmd2	=	0x40 | 0x04;
		}
		if ( A_IsDown )
		{
			Tv_Cmd2	=	0x40 | 0x08;
		}
		LcSendPkt( 0x00, Tv_Cmd2, Tv_Para1, 0 );
		
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtLilin2::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd2;
		__u8	Tv_Para2;
	// code --------------------
		//set zoom		
		Tv_Cmd2		=	0x80 | 0x10;
		Tv_Para2	=	LcCvtZoomFcsSpeed(A_Speed);
		if ( A_IsWide )
		{
			Tv_Cmd2		=	0x80 | 0x20;
		}
		LcSendPkt( 0x00, Tv_Cmd2, 0, Tv_Para2 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtLilin2::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd1;
	// code --------------------
		//set zoom		
		Tv_Cmd1		=	0x01;
		if ( A_IsFar )
		{
			Tv_Cmd1		=	0x02;
		}
		LcSendPkt( Tv_Cmd1, 0x00, 0x00, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtLilin2::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd1;
	// code --------------------
		//set zoom		
		Tv_Cmd1		=	0x08;
		if ( A_IsOpen )
		{
			Tv_Cmd1		=	0x04;
		}
		LcSendPkt( Tv_Cmd1, 0x00, 0x00, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::DoStop( void )
{
		LcSendPkt( 0x00, 0x00, 0x00, 0xFF );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::DoAutoFocus( void )
{
		LcSendPkt( 0x00, 0x14, 0x03, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::DoPresetSet( __u8 A_Id )
{
	// local -------------------
	// code --------------------
		if ( 0 < A_Id )
		{
			LcSendPkt( 0x80, A_Id - 1, 0x00, 0xFF );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		if ( 0 < A_Id )
		{
			LcSendPkt( 0x00, 0x04, 0x00, A_Id - 1 );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtLilin2::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::DoMenuCall( void )
{
		LcSendPkt( 0x00, 0x1A, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::DoMenuExit( void )
{
		LcSendPkt( 0x00, 0x1B, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::DoMenuOk( void )
{
		LcSendPkt( 0x00, 0x1A, 0x00, 0x00 );
		//LcSendPkt( 0x00, 0x36, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::DoMenuCancel( void )
{
		LcSendPkt( 0x00, 0x1B, 0x00, 0x00 );
		//LcSendPkt( 0x00, 0x37, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::TourRun( void )
{
		LcSendPkt( 0xC4, 0x00, 0x00, 0xFF );	
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtLilin2::TourStop( void )
{
		LcSendPkt( 0x00, 0x00, 0x00, 0xFF );
}
//--------------------------------------------------------------------

