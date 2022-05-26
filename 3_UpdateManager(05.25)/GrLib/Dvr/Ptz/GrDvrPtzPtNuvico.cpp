/*
 dvr pantilt protocol nuvico ez

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtNuvico.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtNuvicoChksumIdx			6

#define E_GrDvrPtzPtNuvicoPktSize				7	


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8	V_DvrPtzPtNuvicoPanTiltSpdTbl[5]	=	
{
	0x01, 0x10,	0x20, 0x30, 0x3F
};

__u8	V_DvrPtzPtNuvicoDZoomFcsSpdTbl[5]	=	
{
	0x01, 0x02,	0x03, 0x05, 0x07
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtNuvico::Cls_GrDvrPtzPtNuvico( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtNuvico::~Cls_GrDvrPtzPtNuvico()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::LcSendPkt( __u8 A_Cmd, __u8 A_Data1, __u8 A_Data2 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtNuvicoPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xE5;
		Tv_PktBuf[1]		=	0x10;		//dome
		Tv_PktBuf[2]		=	m_Id;
		Tv_PktBuf[3]		=	A_Cmd;
		Tv_PktBuf[4]		=	A_Data1;
		Tv_PktBuf[5]		=	A_Data2;

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[0] + (__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4] + (__u32)Tv_PktBuf[5];
		Tv_PktBuf[E_GrDvrPtzPtNuvicoChksumIdx]	=	0xA5 ^ (__u8)( 0xFF & Tv_Chksum );
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtNuvico::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtNuvicoPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtNuvico::LcCvtZoomFcsSpeed( __u8 A_Spd )
{
		return	V_DvrPtzPtNuvicoDZoomFcsSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtNuvico::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
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
		if ( A_IsUp )
		{
			Tv_Data2	=	LcCvtPanTiltSpeed(A_SpdTilt);
		}
		if ( A_IsDown )
		{
			Tv_Data2	=	0x80 | LcCvtPanTiltSpeed(A_SpdTilt);
		}
		LcSendPkt( 0x40, Tv_Data1, Tv_Data2 );
		
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtNuvico::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//set zoom		
		Tv_Cmd		=	0x50 | LcCvtZoomFcsSpeed(A_Speed);
		if ( A_IsWide )
		{
			Tv_Cmd	=	Tv_Cmd | 0x08;
		}
		LcSendPkt( Tv_Cmd, 0, 0 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtNuvico::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//set zoom		
		Tv_Cmd		=	0x48;
		if ( A_IsFar )
		{
			Tv_Cmd		=	0x44;
		}
		LcSendPkt( Tv_Cmd, 0, 0 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtNuvico::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//set zoom		
		Tv_Cmd		=	0x42;
		if ( A_IsOpen )
		{
			Tv_Cmd		=	0x41;
		}
		LcSendPkt( Tv_Cmd, 0, 0 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::DoStop( void )
{
		LcSendPkt( 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::DoAutoFocus( void )
{
		LcSendPkt( 0xBC, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::DoPresetSet( __u8 A_Id )
{
	// local -------------------
	// code --------------------
		LcSendPkt( 0x82, A_Id, 0x00 );
		GrPrcSleep( 500 );
		LcSendPkt( 0xA2, A_Id, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		LcSendPkt( 0x81, A_Id, 0x00 );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtNuvico::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::DoMenuCall( void )
{
		LcSendPkt( 0xA3, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::DoMenuExit( void )
{
		LcSendPkt( 0xA1, 0x00, 0x00 );
		GrPrcSleep( 500 );
		LcSendPkt( 0xA1, 0x00, 0x00 );
		GrPrcSleep( 500 );
		LcSendPkt( 0xA1, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::DoMenuOk( void )
{
		DoZoom( FALSE, 2, 0 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::DoMenuCancel( void )
{
		DoZoom( TRUE, 2, 0 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::TourRun( void )
{
		LcSendPkt( 0x86, 0x01, 0x00 );	
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtNuvico::TourStop( void )
{
		LcSendPkt( 0xA1, 0x00, 0x00 );
}
//--------------------------------------------------------------------

