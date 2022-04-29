/*
 dvr pantilt protocol pelco-d

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtUTCFixed.h>
#include	<GrUart.h>
//====================================================================
//local const
#define E_GrDvrPtzPtUTCFixedChksumIdx		6+4

#define E_GrDvrPtzPtUTCFixedPktSize			7+4


//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

__u8	V_DvrPtzPtUTCFixedPanTiltSpdTbl[5]	=	
{
	0x0F, 0x1F,	0x2F, 0x3F, 0xFF
};
__u16	V_GrDvrPtzPtUTCFixedTourSpdTbl[5]	=	
{
	0x79E, 0xE42, 0x1644, 0x1C52, 0x2454
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtUTCFixed::Cls_GrDvrPtzPtUTCFixed( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init

		//power on
		LcSendPkt( 0x88, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtUTCFixed::~Cls_GrDvrPtzPtUTCFixed()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::LcSendPkt( __u8 A_Cmd1, __u8 A_Cmd2, __u8 A_Data1, __u8 A_Data2 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtUTCFixedPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0x5A;
		Tv_PktBuf[1]		=	0x00;
		Tv_PktBuf[2]		=	m_Ch;
		Tv_PktBuf[3]		=	Tv_PktBuf[1] + Tv_PktBuf[2];

		Tv_PktBuf[4]		=	0xFF;
		Tv_PktBuf[5]		=	m_Id;
		Tv_PktBuf[6]		=	A_Cmd1;
		Tv_PktBuf[7]		=	A_Cmd2;
		Tv_PktBuf[8]		=	A_Data1;
		Tv_PktBuf[9]		=	A_Data2;
		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[5] + (__u32)Tv_PktBuf[6] + (__u32)Tv_PktBuf[7] + (__u32)Tv_PktBuf[8] + (__u32)Tv_PktBuf[9];
		Tv_PktBuf[E_GrDvrPtzPtUTCFixedChksumIdx]	=	(__u8)( 0xFF & Tv_Chksum );
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCFixed::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtUTCFixedPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCFixed::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
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
		LcSendPkt( 0x00, Tv_Cmd2, LcCvtPanTiltSpeed(A_SpdPan), LcCvtPanTiltSpeed(A_SpdTilt) );
		
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCFixed::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
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
		LcSendPkt( 0x00, Tv_Cmd2, 0x00, 0x02 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCFixed::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd1;
		__u8	Tv_Cmd2;
	// code --------------------
		Tv_Cmd1	=	0x01;
		Tv_Cmd2	=	0x00;
		if ( A_IsFar )
		{
			Tv_Cmd1	=	0x00;
			Tv_Cmd2	=	0x80;
		}
		LcSendPkt( Tv_Cmd1, Tv_Cmd2, 0x00, 0x02 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCFixed::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Cmd1;
		__u8	Tv_Cmd2;
	// code --------------------
		Tv_Cmd1	=	0x04;
		Tv_Cmd2	=	0x00;
		if ( A_IsOpen )
		{
			Tv_Cmd1	=	0x02;
		}
		LcSendPkt( Tv_Cmd1, Tv_Cmd2, 0x00, 0x02 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::DoStop( void )
{
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::DoAutoFocus( void )
{
		LcSendPkt( 0x20, 0x2B, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::DoPresetSet( __u8 A_Id )
{
	// local -------------------
	// code --------------------
		if ( 100 > A_Id )
		{
			LcSendPkt( 0x00, 0x03, 0x00, A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		if ( 100 > A_Id )
		{
			LcSendPkt( 0x00, 0x07, 0x00, A_Id );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtUTCFixed::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::DoMenuCall( void )
{
		DoPresetMove( 95, 2 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::DoMenuOk( void )
{
		DoFocus( TRUE, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::DoMenuCancel( void )
{
		DoFocus( FALSE, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::LcSendSpcPkt( __u8 A_Val3, __u8 A_Val4, __u8 A_Val5, __u8 A_Val6, __u8 A_Val7, 
	__u8 A_Val8, __u8 A_Val9, __u8 A_Val10 )
{
		// local -------------------
		__u8	Tv_PktBuf[11+4];
		__u32	Tv_Chksum;
		// code --------------------
		//build id
		Tv_PktBuf[0]		=	0x5A;
		Tv_PktBuf[1]		=	0x00;
		Tv_PktBuf[2]		=	m_Ch;
		Tv_PktBuf[3]		=	Tv_PktBuf[1] + Tv_PktBuf[2];

		Tv_PktBuf[4]		=	0xFF;
		Tv_PktBuf[5]		=	m_Id;
		Tv_PktBuf[6]		=	A_Val3;
		Tv_PktBuf[7]		=	A_Val4;
		Tv_PktBuf[8]		=	A_Val5;
		Tv_PktBuf[9]		=	A_Val6;
		Tv_PktBuf[10]		=	A_Val7;
		Tv_PktBuf[11]		=	A_Val8;
		Tv_PktBuf[12]		=	A_Val9;
		Tv_PktBuf[13]		=	A_Val10;
		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[4] + (__u32)Tv_PktBuf[5] + (__u32)Tv_PktBuf[6] + (__u32)Tv_PktBuf[7]
		+ (__u32)Tv_PktBuf[8] + (__u32)Tv_PktBuf[9] + (__u32)Tv_PktBuf[10] + (__u32)Tv_PktBuf[11]
		+ (__u32)Tv_PktBuf[12];
		Tv_PktBuf[13]	=	(__u8)( 0xFF & Tv_Chksum );
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
	// local -------------------
	__u8	Tv_WkIdx;
	__u16	Tv_SpdMsb;
	__u16	Tv_SpdLsb;
	BOOL8	Tv_IsAble;
	// code --------------------
	Tv_WkIdx	=	0;
	Tv_IsAble	=	TRUE;
	for ( Tv_WkIdx=0;Tv_WkIdx < 20;Tv_WkIdx++ )
	{
		if ( A_Cnt > Tv_WkIdx )
		{
			if ( 0 == A_PtrTbl[Tv_WkIdx].Maintain )
			{
				Tv_IsAble	=	FALSE;
			}
			if ( Tv_IsAble )
			{
				//send data
				Tv_SpdMsb	=	(__u16)((V_GrDvrPtzPtUTCFixedTourSpdTbl[A_PtrTbl[Tv_WkIdx].Spd] >> 8) & 0xFF);
				Tv_SpdLsb	=	(__u16)(V_GrDvrPtzPtUTCFixedTourSpdTbl[A_PtrTbl[Tv_WkIdx].Spd] & 0xFF);
				LcSendSpcPkt( 0, 0x65, Tv_WkIdx + 1, 1, A_PtrTbl[Tv_WkIdx].PresetId, A_PtrTbl[Tv_WkIdx].Maintain,
					Tv_SpdMsb, Tv_SpdLsb );
			}
		}
		if ( !Tv_IsAble )
		{
			LcSendSpcPkt( 0, 0x65, Tv_WkIdx + 1, 0, 0, 0, 0, 0 );
		}
		GrPrcSleep( 100 );
	}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::TourRun( void )
{
		LcSendPkt( 0x00, 0x07, 0x00, 151 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCFixed::TourStop( void )
{
		DoStop();
}
//--------------------------------------------------------------------


