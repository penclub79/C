/*
 DVR pan tilt protocol DMAX

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <Dvr/PtzV2/GrPtzV2PtDmax.h>
#include <GrUart.h>
#include <GrStrTool.h>

//====================================================================
//local const
#define E_GrPtzV2PtDmaxChksumIdx		10

#define E_GrPtzV2PtDmaxPktSize			11	


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8	V_GrPtzV2PtDmaxPanTiltSpdTbl[5]	=	
{
	0x95, 0xA0,	0xA9, 0xB0, 0xBF
};
__u8	V_GrPtzV2PtDmaxZoomFcsSpdTbl[5]	=	
{
	0x55, 0x77,	0x99, 0xBB, 0xDD
};
__u8	V_GrPtzV2PtDmaxTourSpdTbl[5]	=	
{
	0x8D, 0x99, 0xAA, 0xB5, 0xC0
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrPtzV2PtDmax::Cls_GrPtzV2PtDmax( void ):
Cls_GrPtzV2PtBase()
{
		GrStrStrToWstr( m_StrModel, "D-MAX" );
}
//--------------------------------------------------------------------
Cls_GrPtzV2PtDmax::~Cls_GrPtzV2PtDmax()
{

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::LcSendPkt( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Och, __u8 A_Ocl, __u8 A_Db1, __u8 A_Db2, __u8 A_Db3, __u8 A_Db4 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrPtzV2PtDmaxPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0x55;
		Tv_PktBuf[1]		=	0;
		Tv_PktBuf[2]		=	A_PtrMng->Id;
		Tv_PktBuf[3]		=	A_Och;
		Tv_PktBuf[4]		=	A_Ocl;
		Tv_PktBuf[5]		=	A_Db1;
		Tv_PktBuf[6]		=	A_Db2;
		Tv_PktBuf[7]		=	A_Db3;
		Tv_PktBuf[8]		=	A_Db4;
		Tv_PktBuf[9]		=	0xAA;

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[0] + (__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4] + 
			(__u32)Tv_PktBuf[5] + (__u32)Tv_PktBuf[6] + (__u32)Tv_PktBuf[7] + (__u32)Tv_PktBuf[8] + (__u32)Tv_PktBuf[9];
		Tv_PktBuf[E_GrPtzV2PtDmaxChksumIdx]	=	(__u8)( 0xFF & (0x2020 - Tv_Chksum) );
		//send
		GrUartSetup( A_PtrMng->UartCh, A_PtrMng->Baud, A_PtrMng->DataBit, A_PtrMng->Parity, A_PtrMng->StopBit );
		GrUartSend( A_PtrMng->UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
		GrPrcSleep( 60 );

}
//--------------------------------------------------------------------
__u8	Cls_GrPtzV2PtDmax::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_GrPtzV2PtDmaxPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrPtzV2PtDmax::LcCvtZoomFcsSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_GrPtzV2PtDmaxZoomFcsSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::Init( Ptr_GrPtzV2ChMng A_PtrMng )
{
		
		
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoPanTilt( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
	__u8 A_SpdPan, __u8 A_SpdTilt )
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
		LcSendPkt( A_PtrMng, 0x00, Tv_Ocl, LcCvtPanTiltSpeed(A_SpdPan), LcCvtPanTiltSpeed(A_SpdTilt), 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoZoom( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsWide, __u8 A_Speed )
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
		LcSendPkt( A_PtrMng, 0x00, Tv_Ocl, 0x00, 0x00, LcCvtZoomFcsSpeed( A_Speed ), 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoFocus( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsFar, __u8 A_Speed )
{
	// local -------------------
		__u8	Tv_Och;
	// code --------------------
		Tv_Och	=	0x02;
		if ( A_IsFar )
		{
			Tv_Och	=	0x01;
		}
		LcSendPkt( A_PtrMng, Tv_Och, 0x00, 0x00, 0x00, LcCvtZoomFcsSpeed( A_Speed ), 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoIris( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsOpen, __u8 A_Speed )
{

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoStop( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoAutoFocus( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x04, 0x00, 0x00, 0x00, 0x99, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoPresetSet( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id )
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
			LcSendPkt( A_PtrMng, 0x00, 0x03, 0x00, Tv_Ex1, Tv_Nn, 0x00 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoPresetMove( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id, __u8 A_Speed )
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
			LcSendPkt( A_PtrMng, 0x00, 0x07, 0x00, Tv_Ex1, Tv_Nn, 0x00 );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPtzV2PtDmax::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoMenuCall( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0xB1, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoMenuOk( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoFocus( A_PtrMng, TRUE, 2 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoMenuCancel( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoFocus( A_PtrMng, FALSE, 2 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::DoMenuExit( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0xB1, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::TourSet( Ptr_GrPtzV2ChMng A_PtrMng )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_Spd;	
		__u8	Tv_Id;
	// code --------------------
		Tv_WkIdx	=	0;
		TourStop( A_PtrMng );
		//clear
		//set start
		LcSendPkt( A_PtrMng, 0x00, 0x23, 0x81, 0x81, 0xA0, 0x00 );
		while ( A_PtrMng->TourCnt > Tv_WkIdx )
		{
			if ( (0 == A_PtrMng->TourTbl[Tv_WkIdx].Maintain) || (0 == A_PtrMng->TourTbl[Tv_WkIdx].PresetId) )
			{
				break;
			}
			Tv_Spd	=	V_GrPtzV2PtDmaxTourSpdTbl[A_PtrMng->TourTbl[Tv_WkIdx].Spd];
			Tv_Id		=	A_PtrMng->TourTbl[Tv_WkIdx].PresetId;
			if ( 0x40 < Tv_Id )
			{
				Tv_Id	=	0x40;
			}
			Tv_Id	=	(Tv_Id - 1) + 0x80;
			LcSendPkt( A_PtrMng, 0x00, 0x23, Tv_Spd, 0x80 + A_PtrMng->TourTbl[Tv_WkIdx].Maintain -1 , 0xA0, 
				Tv_Id );
			//check 
			//next
			Tv_WkIdx ++;
		}
		LcSendPkt( A_PtrMng, 0x00, 0x23, 0x81, 0x81, 0xA0, 0xC0 );	//end mark

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::TourRun( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0x21, 0x00, 0x00, 0xA0, 0xA0 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtDmax::TourStop( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0x2F, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
