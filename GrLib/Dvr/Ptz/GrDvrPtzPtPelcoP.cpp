/*
 dvr pantilt protocol pelco-p

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtPelcoP.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtPelcoPChksumIdx		7

#define E_GrDvrPtzPtPelcoPPktSize			8


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8	V_DvrPtzPtPelcoPPanSpdTbl[5]	=	
{
	0x0F, 0x1F,	0x2F, 0x3F, 0x40
};
__u8	V_DvrPtzPtPelcoPTiltSpdTbl[5]	=	
{
	0x08, 0x10,	0x20, 0x30, 0x3F
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtPelcoP::Cls_GrDvrPtzPtPelcoP( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
		//patch id
		if ( 0 < m_Id )
		{
			m_Id --;
		}
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtPelcoP::~Cls_GrDvrPtzPtPelcoP()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPelcoP::LcSendPkt( __u8 A_Data1, __u8 A_Data2, __u8 A_Data3, __u8 A_Data4 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtPelcoPPktSize];
		__u8	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xA0;
		Tv_PktBuf[1]		=	m_Id;
		Tv_PktBuf[2]		=	A_Data1;
		Tv_PktBuf[3]		=	A_Data2;
		Tv_PktBuf[4]		=	A_Data3;
		Tv_PktBuf[5]		=	A_Data4;
		Tv_PktBuf[6]		=	0xAF;
		//build checksum
		Tv_Chksum	=	Tv_PktBuf[0] ^ Tv_PktBuf[1] ^ Tv_PktBuf[2] ^ Tv_PktBuf[3] ^ Tv_PktBuf[4] ^ Tv_PktBuf[5] ^ Tv_PktBuf[6];
		Tv_PktBuf[E_GrDvrPtzPtPelcoPChksumIdx]	=	Tv_Chksum;
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtPelcoP::LcCvtPanSpeed( __u8 A_Spd )
{
		return	V_DvrPtzPtPelcoPPanSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtPelcoP::LcCvtTiltSpeed( __u8 A_Spd )
{
		return	V_DvrPtzPtPelcoPTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtPelcoP::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data2;
	// code --------------------
		Tv_Data2	=	0;
		//pan
		if ( A_IsLeft )
		{
			Tv_Data2	=	Tv_Data2 | 0x04;
		}
		if ( A_IsRight )
		{
			Tv_Data2	=	Tv_Data2 | 0x02;
		}
		if ( A_IsUp )
		{
			Tv_Data2	=	Tv_Data2 | 0x08;
		}
		if ( A_IsDown )
		{
			Tv_Data2	=	Tv_Data2 | 0x10;
		}
		LcSendPkt( 0x00, Tv_Data2, LcCvtPanSpeed(A_SpdPan), LcCvtTiltSpeed(A_SpdTilt) );
		
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtPelcoP::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data2;
	// code --------------------
		//set zoom		
		Tv_Data2		=	0x20;
		if ( A_IsWide )
		{
			Tv_Data2	=	0x40;
		}
		LcSendPkt( 0x00, Tv_Data2, 0x00, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtPelcoP::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data1;
	// code --------------------
		Tv_Data1	=	0x02;
		if ( A_IsFar )
		{
			Tv_Data1	=	0x01;
		}
		LcSendPkt( Tv_Data1, 0, 0x00, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtPelcoP::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data1;
	// code --------------------
		Tv_Data1	=	0x08;
		if ( A_IsOpen )
		{
			Tv_Data1	=	0x04;
		}
		LcSendPkt( Tv_Data1, 0, 0x00, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPelcoP::DoStop( void )
{
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPelcoP::DoAutoFocus( void )
{
		LcSendPkt( 0x40, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPelcoP::DoPresetSet( __u8 A_Id )
{
	// local -------------------
	// code --------------------
		if ( 0 < A_Id )
		{
			LcSendPkt( 0x00, 0x03, 0x00, A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPelcoP::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		if ( 0 < A_Id )
		{
			LcSendPkt( 0x00, 0x07, 0x00, A_Id );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtPelcoP::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPelcoP::DoMenuCall( void )
{
		DoPresetMove( 95, 2 );
}
//--------------------------------------------------------------------

