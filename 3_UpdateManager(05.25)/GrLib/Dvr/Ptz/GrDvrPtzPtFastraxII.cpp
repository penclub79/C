/*
 dvr pantilt protocol base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtFastraxII.h>
#include	<GrUart.h>

//====================================================================
//local const

#define GrDvrPtzPtFastraxIIChksumIdx		6
#define GrDvrPtzPtFastraxIIPktSize			7	

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
__u8	V_DvrPtzPtFastraxIIPanTiltSpdTbl[5]	=	
{
		0x01, 0x03, 0x04, 0x05, 0x07
};

__u8	V_DvrPtzPtFastraxIIZoomSpdTbl[5]	=	
{
		0x01, 0x03,	0x04, 0x06, 0x07
};
//====================================================================
//--------------------------------------------------------------------
Cls_GrDvrPtzPtFastraxII::Cls_GrDvrPtzPtFastraxII( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtFastraxII::~Cls_GrDvrPtzPtFastraxII()
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::LcSendPkt( __u8 A_Cmd, __u8 A_Data1, __u8 A_Data2 )
{
	// local -------------------
		__u8	Tv_PktBuf[GrDvrPtzPtFastraxIIPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xA5;
		Tv_PktBuf[1]		= m_Id;
		Tv_PktBuf[2]		=	A_Cmd;
		Tv_PktBuf[3]		=	A_Data1;
		Tv_PktBuf[4]		=	A_Data2;
		Tv_PktBuf[5]		=	0x55;		
		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[0] + (__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4] + (__u32)Tv_PktBuf[5];
		Tv_PktBuf[GrDvrPtzPtFastraxIIChksumIdx]	=	(__u8)( 0xFF & Tv_Chksum );
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxII::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtFastraxIIPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxII::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data2;
	// code --------------------
		Tv_Data2 = 0;
		//pan
		if ( A_IsLeft )
		{
			Tv_Data2	=	Tv_Data2 | 0x0F;
			Tv_Data2	=	Tv_Data2 | LcCvtPanTiltSpeed(A_SpdPan);
			Tv_Data2	=	Tv_Data2 & 0x0F;
		}
		if ( A_IsRight )
		{
			Tv_Data2	=	Tv_Data2 | 0x8F;
			Tv_Data2	=	Tv_Data2 | LcCvtPanTiltSpeed(A_SpdPan);
			Tv_Data2	=	Tv_Data2 & 0x8F;
		}
		if ( A_IsUp )
		{
			Tv_Data2	=	Tv_Data2 | 0x00;
			Tv_Data2	=	Tv_Data2 | LcCvtPanTiltSpeed(A_SpdTilt);
		}
		if ( A_IsDown )
		{
			Tv_Data2	=	Tv_Data2 | 0x08;
			Tv_Data2	=	Tv_Data2 | LcCvtPanTiltSpeed(A_SpdTilt);
			Tv_Data2	=	Tv_Data2 & 0x08;
		}
		LcSendPkt( 0x10, 0x00, Tv_Data2 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxII::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data1;
	// code --------------------
		Tv_Data1 = 0;
		if( A_IsWide )
		{
			if ( 2 < A_Speed )
			{
				Tv_Data1 = Tv_Data1 | 0x0B;
			}
			Tv_Data1 = Tv_Data1 | 0x0C;
		}
		else
		{
			if ( 2 < A_Speed )
			{
				Tv_Data1 = Tv_Data1 | 0x0F;
			}
			Tv_Data1 = Tv_Data1 | 0x0E;
		}
		LcSendPkt( 0x10, Tv_Data1, 0x88 );
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxII::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data1;
	// code --------------------
		Tv_Data1 = 0;
		if( A_IsFar )
		{
			Tv_Data1 = 0x04;
		}
		else
		{
			Tv_Data1 = 0x03;
		}
		LcSendPkt( 0x10, Tv_Data1, 0x88 );
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtFastraxII::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Data1;
	// code --------------------
		Tv_Data1 = 0;
		if( A_IsOpen )
		{
			Tv_Data1 = Tv_Data1 | 0x06;
		}
		else
		{
			Tv_Data1 = Tv_Data1 | 0x05;
		}
		LcSendPkt( 0x10, Tv_Data1, 0x88 );
		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoStop( void )
{
	// local -------------------
	__u8	Tv_PktBuf[GrDvrPtzPtFastraxIIPktSize];
	// code --------------------
	//build id
	Tv_PktBuf[0]		=	0xA5;
	Tv_PktBuf[1]		= m_Id;
	Tv_PktBuf[2]		=	0x10;
	Tv_PktBuf[3]		=	0x3d;
	Tv_PktBuf[4]		=	0x88;
	Tv_PktBuf[5]		=	0x55;		
	Tv_PktBuf[6]		=	0xA0;		
	//send
	GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoAutoFocus( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoMenuCall( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoMenuExit( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoPresetSet( __u8 A_Id )
{
	// local -------------------
		__u8	Tv_Data1;
		__u8	Tv_Data2;
	// code --------------------
		Tv_Data1 = 0x80;
		Tv_Data2 = 0x80;

		if( 0x0F < A_Id )
		{
			A_Id = A_Id >> 4;
			Tv_Data1 = Tv_Data1 | A_Id;
		}
		else
		{
			Tv_Data2 = Tv_Data2 | A_Id;
		}
		LcSendPkt( 0x27, Tv_Data1, Tv_Data2 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
		__u8	Tv_Data1;
		__u8	Tv_Data2;
	// code --------------------
		Tv_Data1 = 0x80;
		Tv_Data2 = 0x80;

		if( 0x0F < A_Id )
		{
			A_Id = A_Id >> 4;
			Tv_Data1 = Tv_Data1 | A_Id;
		}
		else
		{
			Tv_Data2 = Tv_Data2 | A_Id;
		}
		LcSendPkt( 0x11, Tv_Data1, Tv_Data2 );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtFastraxII::IsMenuAble( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoMenuOk( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoMenuCancel( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::ExIdView( BOOL8 A_IsOn )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::ExIdSet( __u8 A_IdNew )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::TourRun( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::TourStop( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoMenuLeft( void )
{
		//DoPanTilt( TRUE, FALSE, FALSE, FALSE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoMenuRight( void )
{
		//DoPanTilt( FALSE, TRUE, FALSE, FALSE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoMenuUp( void )
{
		//DoPanTilt( FALSE, FALSE, TRUE, FALSE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoMenuDown( void )
{
		//DoPanTilt( FALSE, FALSE, FALSE, TRUE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtFastraxII::DoTimer( void )
{
		//void
}
//--------------------------------------------------------------------


