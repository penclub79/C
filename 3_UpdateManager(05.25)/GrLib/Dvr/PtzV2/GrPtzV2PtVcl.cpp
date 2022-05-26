/*
 DVR pan tilt protocol VCL

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <Dvr/PtzV2/GrPtzV2PtVcl.h>
#include <GrUart.h>
#include <GrStrTool.h>

//====================================================================
//local const
#define E_GrPtzV2PtVclPktSize			3	


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var
__u8	V_GrPtzV2PtVclPanTiltSpdTbl[5]	=	
{
	0x0F, 0x20, 0x40, 0x60, 0x7F
};

__u8	V_GrPtzV2PtVclTourSpdTbl[5]	=	
{
	0x0F, 0x20, 0x40, 0x60, 0x7F
};

__u8	V_GrPtzV2PtVclStopCmdTbl[E_GrPtzV2PtVclOnCnt]	=	
{
	0x6C,	0x75,	0x72,	0x6E,	0x2A,	0x2B,	0x2D,	0x2C,
	0x2E,	0x2F
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrPtzV2PtVcl::Cls_GrPtzV2PtVcl( void ):
Cls_GrPtzV2PtBase()
{
		GrStrStrToWstr( m_StrModel, "VCL" );

}
//--------------------------------------------------------------------
Cls_GrPtzV2PtVcl::~Cls_GrPtzV2PtVcl()
{

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::LcSendPkt( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Cmd, __u8 A_Para, BOOL8 A_IsPara )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrPtzV2PtVclPktSize];
		__u32	Tv_PktSize;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0x80 | (A_PtrMng->Id - 1);
		Tv_PktBuf[1]		=	A_Cmd;
		Tv_PktBuf[2]		=	A_Para;

		Tv_PktSize			=	2;
		if ( A_IsPara )
		{
			Tv_PktSize ++;
		}
		//send
		GrUartSetup( A_PtrMng->UartCh, A_PtrMng->Baud, A_PtrMng->DataBit, A_PtrMng->Parity, A_PtrMng->StopBit );
		GrUartSend( A_PtrMng->UartCh, Tv_PktBuf, Tv_PktSize, TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::LcSendTourData( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Preset, __u8 A_Speed, __u8 A_Dwell )
{
	// local -------------------
		__u8	Tv_PktBuf[3];
	// code --------------------
		Tv_PktBuf[0]	=	A_Preset;
		Tv_PktBuf[1]	=	V_GrPtzV2PtVclTourSpdTbl[A_Speed];
		Tv_PktBuf[2]	=	A_Dwell;
		//send
		GrUartSetup( A_PtrMng->UartCh, A_PtrMng->Baud, A_PtrMng->DataBit, A_PtrMng->Parity, A_PtrMng->StopBit );
		GrUartSend( A_PtrMng->UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
__u8	Cls_GrPtzV2PtVcl::LcCvtPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_GrPtzV2PtVclPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::Init( Ptr_GrPtzV2ChMng A_PtrMng )
{
		// power on

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::LcSendStop( Ptr_GrPtzV2ChMng A_PtrMng )
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u32	Tv_Mask;
		__u8	Tv_Cmd;
	// code --------------------
		Tv_Mask	=	1;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrPtzV2PtVclOnCnt;Tv_WkIdx++ )
		{
			if ( 0 != (A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & Tv_Mask)  )
			{
				Tv_Cmd	=	V_GrPtzV2PtVclStopCmdTbl[Tv_WkIdx];
				LcSendPkt( A_PtrMng, Tv_Cmd );
				// clear
				A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx]	=	A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & (~Tv_Mask);
			}
			Tv_Mask	=	Tv_Mask << 1;
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoPanTilt( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
	__u8 A_SpdPan, __u8 A_SpdTilt )
{
	// local -------------------
		__u8	Tv_Cmd;
		__u8	Tv_Spd;
	// code --------------------
		// left
		if ( A_IsLeft )
		{
			// do
			if ( 0 == (A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & (1 << E_GrPtzV2PtVclOnLeft) ) )
			{
				Tv_Cmd	=	0x4C;
				Tv_Spd	=	LcCvtPanTiltSpeed( A_SpdPan );
				LcSendPkt( A_PtrMng, Tv_Cmd, Tv_Spd, TRUE );
				A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx]	=	A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] | (1 << E_GrPtzV2PtVclOnLeft);
			}
		}
		else
		{
			// stop
			if ( 0 != (A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & (1 << E_GrPtzV2PtVclOnLeft) ) )
			{
				Tv_Cmd	=	0x6C;
				Tv_Spd	=	LcCvtPanTiltSpeed( A_SpdPan );
				LcSendPkt( A_PtrMng, Tv_Cmd, Tv_Spd, TRUE );
				A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx]	=	A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & ~(1 << E_GrPtzV2PtVclOnLeft);
			}
		}

		// right
		if ( A_IsRight )
		{
			// do
			if ( 0 == (A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & (1 << E_GrPtzV2PtVclOnRight) ) )
			{
				Tv_Cmd	=	0x52;
				Tv_Spd	=	LcCvtPanTiltSpeed( A_SpdPan );
				LcSendPkt( A_PtrMng, Tv_Cmd, Tv_Spd, TRUE );
				A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx]	=	A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] | (1 << E_GrPtzV2PtVclOnRight);
			}
		}
		else
		{
			// stop
			if ( 0 != (A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & (1 << E_GrPtzV2PtVclOnRight) ) )
			{
				Tv_Cmd	=	0x72;
				Tv_Spd	=	LcCvtPanTiltSpeed( A_SpdPan );
				LcSendPkt( A_PtrMng, Tv_Cmd, Tv_Spd, TRUE );
				A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx]	=	A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & ~(1 << E_GrPtzV2PtVclOnRight);
			}
		}

		// up
		if ( A_IsUp )
		{
			// do
			if ( 0 == (A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & (1 << E_GrPtzV2PtVclOnUp) ) )
			{
				Tv_Cmd	=	0x55;
				Tv_Spd	=	LcCvtPanTiltSpeed( A_SpdTilt );
				LcSendPkt( A_PtrMng, Tv_Cmd, Tv_Spd, TRUE );
				A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx]	=	A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] | (1 << E_GrPtzV2PtVclOnUp);
			}
		}
		else
		{
			// stop
			if ( 0 != (A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & (1 << E_GrPtzV2PtVclOnUp) ) )
			{
				Tv_Cmd	=	0x75;
				Tv_Spd	=	LcCvtPanTiltSpeed( A_SpdTilt );
				LcSendPkt( A_PtrMng, Tv_Cmd, Tv_Spd, TRUE );
				A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx]	=	A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & ~(1 << E_GrPtzV2PtVclOnUp);
			}
		}

		// down
		if ( A_IsDown )
		{
			// do
			if ( 0 == (A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & (1 << E_GrPtzV2PtVclOnDown) ) )
			{
				Tv_Cmd	=	0x4E;
				Tv_Spd	=	LcCvtPanTiltSpeed( A_SpdTilt );
				LcSendPkt( A_PtrMng, Tv_Cmd, Tv_Spd, TRUE );
				A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx]	=	A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] | (1 << E_GrPtzV2PtVclOnDown);
			}
		}
		else
		{
			// stop
			if ( 0 != (A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & (1 << E_GrPtzV2PtVclOnDown) ) )
			{
				Tv_Cmd	=	0x6E;
				Tv_Spd	=	LcCvtPanTiltSpeed( A_SpdTilt );
				LcSendPkt( A_PtrMng, Tv_Cmd, Tv_Spd, TRUE );
				A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx]	=	A_PtrMng->UserData[E_GrPtzV2PtVclOnMapIdx] & ~(1 << E_GrPtzV2PtVclOnDown);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoZoom( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsWide, __u8 A_Speed )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		//set zoom	
		Tv_Cmd		=	0x3A;
		if ( A_IsWide )
		{
			Tv_Cmd	=	0x2A;
		}
		LcSendPkt( A_PtrMng, Tv_Cmd );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoFocus( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsFar, __u8 A_Speed )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		Tv_Cmd	=	0x3C;
		if ( A_IsFar )
		{
			Tv_Cmd	=	0x3D;
		}
		LcSendPkt( A_PtrMng, Tv_Cmd );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoIris( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsOpen, __u8 A_Speed )
{
	// local -------------------
		__u8	Tv_Cmd;
	// code --------------------
		Tv_Cmd	=	0x3F;
		if ( A_IsOpen )
		{
			Tv_Cmd	=	0x3E;
		}
		LcSendPkt( A_PtrMng, Tv_Cmd );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoStop( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendStop( A_PtrMng );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoAutoFocus( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x41 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoPresetSet( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id )
{
	// local -------------------
	// code --------------------
		if ( 0x80 > A_Id )
		{
			LcSendPkt( A_PtrMng, 0x47, A_Id, TRUE );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoPresetMove( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		if ( 0x80 > A_Id )
		{
			LcSendPkt( A_PtrMng, 0x42, A_Id, TRUE );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPtzV2PtVcl::IsMenuAble( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoMenuCall( Ptr_GrPtzV2ChMng A_PtrMng )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoMenuOk( Ptr_GrPtzV2ChMng A_PtrMng )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::DoMenuCancel( Ptr_GrPtzV2ChMng A_PtrMng )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::TourSet( Ptr_GrPtzV2ChMng A_PtrMng )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		if ( 0 != A_PtrMng->TourCnt )
		{
			// set start
			LcSendPkt( A_PtrMng, 0x50, 1, TRUE );
			for ( Tv_WkIdx=0;Tv_WkIdx < 20;Tv_WkIdx++ )
			{
				if ( A_PtrMng->TourCnt > Tv_WkIdx )
				{
					if ( 0 == A_PtrMng->TourTbl[Tv_WkIdx].Maintain )
					{
						break;
					}
					LcSendTourData( A_PtrMng, A_PtrMng->TourTbl[Tv_WkIdx].PresetId, A_PtrMng->TourTbl[Tv_WkIdx].Spd, A_PtrMng->TourTbl[Tv_WkIdx].Maintain );
				}
			}
			// finish
			LcSendPkt( A_PtrMng, 0x70 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::TourRun( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x5E, 1 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtVcl::TourStop( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x7E );
}
//--------------------------------------------------------------------
