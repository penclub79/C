/*
 dvr pantilt protocol dongyang unitech dmax

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtUTCDmax.h>
#include	<GrUart.h>
//====================================================================
//local const
#define E_GrPtzPtUTCDmaxChksumIdx			10+4

#define E_GrPtzPtUTCDmaxPktSize				11+4


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8	V_PtzPtUTCDmaxPanTiltSpdTbl[5]	=	
{
	0x95, 0xA0,	0xA9, 0xB0, 0xBF
};

__u8	V_PtzPtUTCDmaxZoomFcsSpdTbl[5]	=	
{
	0x55, 0x77,	0x99, 0xBB, 0xDD
};

__u8	V_PtzPtUTCDmaxTourSpdTbl[5]	=	
{
	0x8D, 0x99, 0xAA, 0xB5, 0xC0
};


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtUTCDmax::Cls_GrDvrPtzPtUTCDmax( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtUTCDmax::~Cls_GrDvrPtzPtUTCDmax()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::LcSendPkt( __u8 A_Och, __u8 A_Ocl, __u8 A_Db1, __u8 A_Db2, __u8 A_Db3, __u8 A_Db4 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrPtzPtUTCDmaxPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0x5A;
		Tv_PktBuf[1]		=	0x01;
		Tv_PktBuf[2]		=	m_Ch;
		Tv_PktBuf[3]		=	Tv_PktBuf[1] + Tv_PktBuf[2];

		Tv_PktBuf[4]		=	0x55;
		Tv_PktBuf[5]		=	0;
		Tv_PktBuf[6]		=	m_Id;
		Tv_PktBuf[7]		=	A_Och;
		Tv_PktBuf[8]		=	A_Ocl;
		Tv_PktBuf[9]		=	A_Db1;
		Tv_PktBuf[10]		=	A_Db2;
		Tv_PktBuf[11]		=	A_Db3;
		Tv_PktBuf[12]		=	A_Db4;
		Tv_PktBuf[13]		=	0xAA;

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[4] + (__u32)Tv_PktBuf[5] + (__u32)Tv_PktBuf[6] + (__u32)Tv_PktBuf[7] + (__u32)Tv_PktBuf[8] + 
			(__u32)Tv_PktBuf[9] + (__u32)Tv_PktBuf[10] + (__u32)Tv_PktBuf[11] + (__u32)Tv_PktBuf[12] + (__u32)Tv_PktBuf[13];
		Tv_PktBuf[E_GrPtzPtUTCDmaxChksumIdx]	=	(__u8)( 0xFF & (0x2020 - Tv_Chksum) );
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
#ifndef LINUX_APP
		//DbgMsgPrint( "Dmax- och:%2X ocl:%2X \n", A_Och, A_Ocl );
#endif
		GrPrcSleep( 60 );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCDmax::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_PtzPtUTCDmaxPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCDmax::LcCvtZoomFcsSpeed( __u8 A_Spd )
{
		return	V_PtzPtUTCDmaxZoomFcsSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCDmax::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Ocl;
	// code --------------------
		Tv_Ocl	=	0;
		//pan
		if ( A_IsLeft )
		{
			Tv_Ocl	=	Tv_Ocl | 0x04;
		}
		if ( A_IsRight )
		{
			Tv_Ocl	=	Tv_Ocl | 0x02;
		}
		if ( A_IsUp )
		{
			Tv_Ocl	=	Tv_Ocl | 0x08;
		}
		if ( A_IsDown )
		{
			Tv_Ocl	=	Tv_Ocl | 0x10;
		}
		LcSendPkt( 0x00, Tv_Ocl, LcCvtPanTiltSpeed(A_SpdPan), LcCvtPanTiltSpeed(A_SpdTilt), 0x00, 0x00 );
		
		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCDmax::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Ocl;
	// code --------------------
		//set zoom		
		Tv_Ocl		=	0x20;
		if ( A_IsWide )
		{
			Tv_Ocl	=	0x40;
		}
		LcSendPkt( 0x00, Tv_Ocl, 0x00, 0x00, LcCvtZoomFcsSpeed( A_Speed ), 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtUTCDmax::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Och;
	// code --------------------
		Tv_Och	=	0x02;
		if ( A_IsFar )
		{
			Tv_Och	=	0x01;
		}
		LcSendPkt( Tv_Och, 0x00, 0x00, 0x00, LcCvtZoomFcsSpeed( A_Speed ), 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::DoStop( void )
{
		LcSendPkt( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::DoAutoFocus( void )
{
		LcSendPkt( 0x04, 0x00, 0x00, 0x00, 0x99, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::DoPresetSet( __u8 A_Id )
{
	// local -------------------
		__u8	Tv_Ex1;
		__u8	Tv_Nn;
	// code --------------------
		if ( 0 < A_Id ) 
		{
			Tv_Ex1	=	0;
			if ( 0x40 >= A_Id )
			{
				Tv_Nn		=	(A_Id - 1) + 0x80;
			}
			else
			{
				Tv_Nn		=	(A_Id - 0x41) + 0x80;
				Tv_Ex1	=	1;
			}
			LcSendPkt( 0x00, 0x03, 0x00, Tv_Ex1, Tv_Nn, 0x00 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
		__u8	Tv_Ex1;
		__u8	Tv_Nn;
	// code --------------------
		if ( 0 < A_Id )
		{
			Tv_Ex1	=	0;
			if ( 0x40 >= A_Id )
			{
				Tv_Nn		=	(A_Id - 1) + 0x80;
			}
			else
			{
				Tv_Nn		=	(A_Id - 0x41) + 0x80;
				Tv_Ex1	=	1;
			}
			LcSendPkt( 0x00, 0x07, 0x00, Tv_Ex1, Tv_Nn, 0x00 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_Spd;	
		__u8	Tv_Id;
	// code --------------------
		Tv_WkIdx	=	0;
		TourStop();
		//clear
		//LcSendPkt( 0x00, 0x2B, 0x00, 0x00, 0x01, 0xF1 );
		//GrPrcSleep( 20 );
		//set start
		LcSendPkt( 0x00, 0x23, 0x81, 0x81, 0xA0, 0x00 );
		while ( A_Cnt > Tv_WkIdx )
		{
			if ( (0 == A_PtrTbl[Tv_WkIdx].Maintain) || (0 == A_PtrTbl[Tv_WkIdx].PresetId) )
			{
				break;
			}
			Tv_Spd	=	V_PtzPtUTCDmaxTourSpdTbl[A_PtrTbl[Tv_WkIdx].Spd];
			Tv_Id		=	A_PtrTbl[Tv_WkIdx].PresetId;
			if ( 0x40 < Tv_Id )
			{
				Tv_Id	=	0x40;
			}
			Tv_Id	=	(Tv_Id - 1) + 0x80;
			LcSendPkt( 0x00, 0x23, Tv_Spd, 0x80 + A_PtrTbl[Tv_WkIdx].Maintain -1 , 0xA0, 
				Tv_Id );
			//check 
			//next
			Tv_WkIdx ++;
		}
		LcSendPkt( 0x00, 0x23, 0x81, 0x81, 0xA0, 0xC0 );	//end mark
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::TourRun( void )
{
		LcSendPkt( 0x00, 0x21, 0x00, 0x00, 0xA0, 0xA0 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::TourStop( void )
{
		LcSendPkt( 0x00, 0x2F, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtUTCDmax::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::DoMenuCall( void )
{
		LcSendPkt( 0x00, 0xB1, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::DoMenuExit( void )
{
		LcSendPkt( 0x00, 0xB1, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::DoMenuOk( void )
{
		DoFocus( TRUE, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtUTCDmax::DoMenuCancel( void )
{
		DoFocus( FALSE, 2, FALSE );
}
//--------------------------------------------------------------------

