/*
 dvr pantilt protocol base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtFastraxIIE.h>
#include	<GrUart.h>

//====================================================================
//local const

#define GrDvrPtzPtFastraxIIEChksumIdx		6
#define GrDvrPtzPtFastraxIIEPktSize			7	

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
__u8	V_DvrPtzPtFastraxIIEPanTiltSpdTbl[5]	=	
{
		//0x01, 0x0C, 0x18, 0x24, 0x3F
		0x14, 0x1E, 0x28, 0x32, 0x3F
};

__u8	V_DvrPtzPtFastraxIIEZoomSpdTbl[5]	=	
{
		0x01, 0x03,	0x04, 0x06, 0x07
};
//====================================================================
//--------------------------------------------------------------------
Cls_GrDvrPtzPtFastraxIIE::Cls_GrDvrPtzPtFastraxIIE( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtFastraxIIE::~Cls_GrDvrPtzPtFastraxIIE()
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::LcSendPkt( __u8 A_Cmd, __u8 A_Data1, __u8 A_Data2 )
{
	// local -------------------
		__u8	Tv_PktBuf[GrDvrPtzPtFastraxIIEPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xFA;
		Tv_PktBuf[1]		= ( m_Id / 256 ) + 0x50;
		Tv_PktBuf[2]		=	m_Id % 256;
		Tv_PktBuf[3]		=	A_Cmd;
		Tv_PktBuf[4]		=	A_Data1;
		Tv_PktBuf[5]		=	A_Data2;		
		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[0] + (__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4] + (__u32)Tv_PktBuf[5];
		Tv_PktBuf[GrDvrPtzPtFastraxIIEChksumIdx]	=	(__u8)( 0xFF & Tv_Chksum );
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxIIE::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtFastraxIIEPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxIIE::LcCvtZoomSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtFastraxIIEZoomSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxIIE::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data1;
		__u8	Tv_Data2;
		__u8	Tv_CMD;
	// code --------------------
		Tv_Data1 = 0;
		Tv_Data2 = 0;
		Tv_CMD = 0x80;
		//pan
		if ( A_IsLeft )
		{
			Tv_Data1	=	Tv_Data1 | 0x00;
			Tv_Data1	=	Tv_Data1 | LcCvtPanTiltSpeed(A_SpdPan);
		}
		if ( A_IsRight )
		{
			Tv_Data1	=	Tv_Data1 | 0x80;
			Tv_Data1	=	Tv_Data1 | LcCvtPanTiltSpeed(A_SpdPan);
		}
		if ( A_IsUp )
		{
			Tv_Data2	=	Tv_Data2 | 0x80;
			Tv_Data2	=	Tv_Data2 | LcCvtPanTiltSpeed(A_SpdTilt);
		}
		if ( A_IsDown )
		{
			Tv_Data2	=	Tv_Data2 | 0x00;
			Tv_Data2	=	Tv_Data2 | LcCvtPanTiltSpeed(A_SpdTilt);
		}
		LcSendPkt( Tv_CMD, Tv_Data1, Tv_Data2 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxIIE::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_CMD;
	// code --------------------
		Tv_CMD = 0x90;
		if( A_IsWide )
		{
			Tv_CMD = Tv_CMD | 0x18;
			Tv_CMD = Tv_CMD | LcCvtZoomSpeed(A_Speed);
		}
		else
		{
			Tv_CMD = Tv_CMD | 0x10;
			Tv_CMD = Tv_CMD | LcCvtZoomSpeed(A_Speed);
		}
		LcSendPkt( Tv_CMD, 0x00, 0x00 );
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxIIE::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_CMD;
	// code --------------------
		Tv_CMD = 0x80;
		if( A_IsFar )
		{
			Tv_CMD = Tv_CMD | 0x04;
		}
		else
		{
			Tv_CMD = Tv_CMD | 0x08;
		}
		LcSendPkt( Tv_CMD, 0x00, 0x00 );
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxIIE::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_CMD;
	// code --------------------
		Tv_CMD = 0x80;
		if( A_IsOpen )
		{
			Tv_CMD = Tv_CMD | 0x01;
		}
		else
		{
			Tv_CMD = Tv_CMD | 0x02;
		}
		LcSendPkt( Tv_CMD, 0x00, 0x00 );
		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoStop( void )
{
	// local -------------------
	// code --------------------
		LcSendPkt( 0x10, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoAutoFocus( void )
{
	// local -------------------
		__u8	Tv_PktBuf[5];
		__u32	Tv_Chksum;
	// code --------------------
	//build id
		Tv_PktBuf[0]		=	0xFA;
		Tv_PktBuf[1]		= ( m_Id / 256 ) + 0x50;
		Tv_PktBuf[2]		=	m_Id % 256;
		Tv_PktBuf[3]		=	0x41;

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[0] + (__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3];
		Tv_PktBuf[4]	=	(__u8)( 0xFF & Tv_Chksum );
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoMenuCall( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoMenuExit( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoPresetSet( __u8 A_Id )
{
	// local -------------------
	// code --------------------
		LcSendPkt( 0x17, A_Id, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		LcSendPkt( 0x11, A_Id, 0x00 );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtFastraxIIE::IsMenuAble( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoMenuOk( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoMenuCancel( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::ExIdView( BOOL8 A_IsOn )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::ExIdSet( __u8 A_IdNew )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::TourRun( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::TourStop( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoMenuLeft( void )
{
		//DoPanTilt( TRUE, FALSE, FALSE, FALSE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoMenuRight( void )
{
		//DoPanTilt( FALSE, TRUE, FALSE, FALSE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoMenuUp( void )
{
		//DoPanTilt( FALSE, FALSE, TRUE, FALSE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoMenuDown( void )
{
		//DoPanTilt( FALSE, FALSE, FALSE, TRUE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxIIE::DoTimer( void )
{
		//void
}
//--------------------------------------------------------------------


