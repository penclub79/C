/*
 dvr pantilt protocol powertech

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtPwtech.h>
#include	<GrUart.h>

//====================================================================
//local const


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u16	V_GrDvrPtzPtPwtechTourSpdTbl[5]	=	
{
	2, 5, 10, 15, 25
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtPwtech::Cls_GrDvrPtzPtPwtech( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtPelcoD( A_UartCh, A_Id, A_Ch )
{

}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtPwtech::~Cls_GrDvrPtzPtPwtech()
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtPwtech::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPwtech::DoMenuCall( void )
{
		LcSendPkt( 0x00, 0x07, 0x00, 95 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPwtech::DoMenuExit( void )
{
		LcSendPkt( 0x00, 0x07, 0x00, 95 );
		/*
		GrPrcSleep( 100 );
		LcSendPkt( 0x00, 0x80, 0x00, 0x00 );
		GrPrcSleep( 100 );
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
		*/
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPwtech::DoMenuOk( void )
{
		LcSendPkt( 0x01, 0x00, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPwtech::DoMenuCancel( void )
{
		LcSendPkt( 0x00, 0x80, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPwtech::LcSendSpcPkt( __u8 A_Val3, __u8 A_Val4, __u8 A_Val5, __u8 A_Val6, __u8 A_Val7, 
																				 __u8 A_Val8, __u8 A_Val9, __u8 A_Val10 )
{
	// local -------------------
		__u8	Tv_PktBuf[11];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xFF;
		Tv_PktBuf[1]		=	m_Id;
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
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPwtech::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_SpdMsb;
		__u8	Tv_SpdLsb;
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
					Tv_SpdMsb	=	(__u8)((V_GrDvrPtzPtPwtechTourSpdTbl[A_PtrTbl[Tv_WkIdx].Spd] >> 8) & 0xFF);
					Tv_SpdLsb	=	(__u8)(V_GrDvrPtzPtPwtechTourSpdTbl[A_PtrTbl[Tv_WkIdx].Spd] & 0xFF);
					LcSendSpcPkt( 0, 0x65, Tv_WkIdx + 1, 1, A_PtrTbl[Tv_WkIdx].PresetId, A_PtrTbl[Tv_WkIdx].Maintain,
						Tv_SpdMsb, Tv_SpdLsb );
				}
			}
			if ( !Tv_IsAble )
			{
				LcSendSpcPkt( 0, 0x65, Tv_WkIdx + 1, 0, 0, 0, 0, 0 );
			}
			GrPrcSleep( 300 );//GrPrcSleep( 100 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPwtech::TourRun( void )
{
		LcSendPkt( 0x00, 0x07, 0x00, 151 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtPwtech::TourStop( void )
{
		DoStop();
}
//--------------------------------------------------------------------

