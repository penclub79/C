/*
 DVR pan tilt protocol PMA200

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <Dvr/PtzV2/GrPtzV2PtPma200.h>
#include <GrUart.h>
#include <GrStrTool.h>

//====================================================================
//local const


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u16	V_GrPtzV2PtPma200TourSpdTbl[5]	=	
{
	4, 10, 20, 30, 50
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrPtzV2PtPma200::Cls_GrPtzV2PtPma200(  ):
Cls_GrPtzV2PtPelcoD(  )
{
		GrStrStrToWstr( m_StrModel, "PMA-200HF" );
}
//--------------------------------------------------------------------
Cls_GrPtzV2PtPma200::~Cls_GrPtzV2PtPma200()
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPtzV2PtPma200::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPma200::DoMenuCall( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0x07, 0x00, 95 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPma200::DoMenuExit( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0x07, 0x00, 95 );
		/*
		GrPrcSleep( 100 );
		LcSendPkt( 0x00, 0x80, 0x00, 0x00 );
		GrPrcSleep( 100 );
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
		*/
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPma200::DoMenuOk( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x01, 0x00, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( A_PtrMng, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPma200::DoMenuCancel( Ptr_GrPtzV2ChMng A_PtrMng )
{
		LcSendPkt( A_PtrMng, 0x00, 0x80, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( A_PtrMng, 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtPma200::TourSet( Ptr_GrPtzV2ChMng A_PtrMng )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_SpdMsb;
		__u8	Tv_SpdLsb;
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
						Tv_SpdMsb	=	(__u8)((V_GrPtzV2PtPma200TourSpdTbl[A_PtrMng->TourTbl[Tv_WkIdx].Spd] >> 8) & 0xFF);
						Tv_SpdLsb	=	(__u8)(V_GrPtzV2PtPma200TourSpdTbl[A_PtrMng->TourTbl[Tv_WkIdx].Spd] & 0xFF);
						LcSendSpcPkt( A_PtrMng, 0, 0x65, Tv_WkIdx + 1, 1, A_PtrMng->TourTbl[Tv_WkIdx].PresetId, 
							A_PtrMng->TourTbl[Tv_WkIdx].Maintain, Tv_SpdMsb, Tv_SpdLsb );
						/*
						DbgMsgPrint( "Tour save ID %d seq %d prs %d dwell %d opt %d %d \n", A_PtrMng->Id, Tv_WkIdx + 1, 
							A_PtrMng->TourTbl[Tv_WkIdx].PresetId, A_PtrMng->TourTbl[Tv_WkIdx].Maintain, Tv_SpdMsb, Tv_SpdLsb );
							*/
					}
				}
				if ( !Tv_IsAble )
				{
					LcSendSpcPkt( A_PtrMng, 0, 0x65, Tv_WkIdx + 1, 0, 0, 0, 0, 0 );
				}
				GrPrcSleep( 300 );
			}
		}

}
//--------------------------------------------------------------------

