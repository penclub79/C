/*
 DVR pan tilt protocol PELCO-D

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <Dvr/PtzV2/GrPtzV2PtPelcoD.h>
#include <GrUart.h>
#include <GrStrTool.h>

//====================================================================
//local const
#define E_GrPtzV2PtPelcoDChksumIdx		6

#define E_GrPtzV2PtPelcoDPktSize			7	


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8	V_GrPtzV2PtPelcoDPanTiltSpdTbl[5]	=	
{
	0x0F, 0x1F,	0x2F, 0x3F, 0xFF
};
__u16	V_GrPtzV2PtPelcoDTourSpdTbl[5]	=	
{
	0x79E, 0xE42, 0x1644, 0x1C52, 0x2454
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrPtzV2PtPelcoD::Cls_GrPtzV2PtPelcoD( void ):
Cls_GrPtzV2PtBase()
{
		GrStrStrToWstr( m_StrModel, "PELCO-D" );
}
//--------------------------------------------------------------------
Cls_GrPtzV2PtPelcoD::~Cls_GrPtzV2PtPelcoD()
{

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::LcSendPkt( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Cmd1, __u8 A_Cmd2, __u8 A_Data1, __u8 A_Data2 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrPtzV2PtPelcoDPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xFF;
		Tv_PktBuf[1]		=	A_PtrMng->Id;
		Tv_PktBuf[2]		=	A_Cmd1;
		Tv_PktBuf[3]		=	A_Cmd2;
		Tv_PktBuf[4]		=	A_Data1;
		Tv_PktBuf[5]		=	A_Data2;
		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4] + (__u32)Tv_PktBuf[5];
		Tv_PktBuf[E_GrPtzV2PtPelcoDChksumIdx]	=	(__u8)( 0xFF & Tv_Chksum );
		//send
		GrUartSetup( A_PtrMng->UartCh, A_PtrMng->Baud, A_PtrMng->DataBit, A_PtrMng->Parity, A_PtrMng->StopBit );
		GrUartSend( A_PtrMng->UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::LcSendSpcPkt( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Val3, __u8 A_Val4, __u8 A_Val5, __u8 A_Val6, 
	__u8 A_Val7, __u8 A_Val8, __u8 A_Val9, __u8 A_Val10 )
{
	// local -------------------
		__u8	Tv_PktBuf[11];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xFF;
		Tv_PktBuf[1]		=	A_PtrMng->Id;
		Tv_PktBuf[2]		=	A_Val3;
		Tv_PktBuf[3]		=	A_Val4;
		Tv_PktBuf[4]		=	A_Val5;
		Tv_PktBuf[5]		=	A_Val6;
		Tv_PktBuf[6]		=	A_Val7;
		Tv_PktBuf[7]		=	A_Val8;
		Tv_PktBuf[8]		=	A_Val9;
		Tv_PktBuf[9]		=	A_Val10;
		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4]
			+ (__u32)Tv_PktBuf[5] + (__u32)Tv_PktBuf[6] + (__u32)Tv_PktBuf[7] + (__u32)Tv_PktBuf[8]
			+ (__u32)Tv_PktBuf[9];
		Tv_PktBuf[10]	=	(__u8)( 0xFF & Tv_Chksum );
		//send
		GrUartSetup( A_PtrMng->UartCh, A_PtrMng->Baud, A_PtrMng->DataBit, A_PtrMng->Parity, A_PtrMng->StopBit );
		GrUartSend( A_PtrMng->UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrPtzV2PtPelcoD::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_GrPtzV2PtPelcoDPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::Init( Ptr_GrPtzV2ChMng A_PtrMng )
{
		// power on
		LcSendPkt( A_PtrMng, 0x88, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoPanTilt( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
	__u8 A_SpdPan, __u8 A_SpdTilt )
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
		LcSendPkt( A_PtrMng, 0x00, Tv_Cmd2, LcCvtPanTiltSpeed(A_SpdPan), LcCvtPanTiltSpeed(A_SpdTilt) );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoZoom( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsWide, __u8 A_Speed )
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
		LcSendPkt( A_PtrMng, 0x00, Tv_Cmd2, 0x00, 0x02 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoFocus( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsFar, __u8 A_Speed )
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
		LcSendPkt( A_PtrMng, Tv_Cmd1, Tv_Cmd2, 0x00, 0x02 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoIris( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsOpen, __u8 A_Speed )
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
		LcSendPkt( A_PtrMng, Tv_Cmd1, Tv_Cmd2, 0x00, 0x02 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoStop( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoAutoFocus( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x20, 0x2B, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoPresetSet( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id )
{
	// local -------------------
	// code --------------------
		if ( 100 > A_Id )
		{
			LcSendPkt( A_PtrMng, 0x00, 0x03, 0x00, A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoPresetMove( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		if ( 100 > A_Id )
		{
			LcSendPkt( A_PtrMng, 0x00, 0x07, 0x00, A_Id );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPtzV2PtPelcoD::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoMenuCall( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoPresetMove( A_PtrMng, 95, 2 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoMenuOk( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoFocus( A_PtrMng, TRUE, 2 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::DoMenuCancel( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoFocus( A_PtrMng, FALSE, 2 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::TourSet( Ptr_GrPtzV2ChMng A_PtrMng )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u16	Tv_SpdMsb;
		__u16	Tv_SpdLsb;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_WkIdx	=	0;
		Tv_IsAble	=	TRUE;
		if ( 0 != A_PtrMng->TourCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < 20;Tv_WkIdx++ )
			{
				if ( A_PtrMng->TourCnt > Tv_WkIdx )
				{
					if ( 0 == A_PtrMng->TourTbl[Tv_WkIdx].Maintain )
					{
						Tv_IsAble	=	FALSE;
					}
					if ( Tv_IsAble )
					{
						//send data
						Tv_SpdMsb	=	(__u16)((V_GrPtzV2PtPelcoDTourSpdTbl[A_PtrMng->TourTbl[Tv_WkIdx].Spd] >> 8) & 0xFF);
						Tv_SpdLsb	=	(__u16)(V_GrPtzV2PtPelcoDTourSpdTbl[A_PtrMng->TourTbl[Tv_WkIdx].Spd] & 0xFF);
						LcSendSpcPkt( A_PtrMng, 0, 0x65, Tv_WkIdx + 1, 1, A_PtrMng->TourTbl[Tv_WkIdx].PresetId, 
							A_PtrMng->TourTbl[Tv_WkIdx].Maintain, (__u8)Tv_SpdMsb, (__u8)Tv_SpdLsb );
					}
				}
				if ( !Tv_IsAble )
				{
					LcSendSpcPkt( A_PtrMng, 0, 0x65, Tv_WkIdx + 1, 0, 0, 0, 0, 0 );
				}
				GrPrcSleep( 100 );
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::TourRun( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0x07, 0x00, 151 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPelcoD::TourStop( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoStop( A_PtrMng );
}
//--------------------------------------------------------------------
