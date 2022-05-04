/*
 dvr pantilt protocol powertech

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtAp8777z.h>
#include	<GrUart.h>

//====================================================================
//local const


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtAp8777z::Cls_GrDvrPtzPtAp8777z( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtPelcoD( A_UartCh, A_Id, A_Ch )
{

}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtAp8777z::~Cls_GrDvrPtzPtAp8777z()
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtAp8777z::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtAp8777z::DoMenuCall( void )
{
		LcSendPkt( 0x00, 0x07, 0x00, 95 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtAp8777z::DoMenuExit( void )
{
		/*
		LcSendPkt( 0x00, 0x80, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( 0x00, 0x80, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( 0x20, 0x2B, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
		*/
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtAp8777z::DoMenuOk( void )
{
		LcSendPkt( 0x01, 0x00, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtAp8777z::DoMenuCancel( void )
{
		LcSendPkt( 0x00, 0x80, 0x00, 0x00 );
		GrPrcSleep( 20 );
		LcSendPkt( 0x00, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtAp8777z::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtAp8777z::TourRun( void )
{
		LcSendPkt( 0x00, 0x07, 0x00, 0x47 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtAp8777z::TourStop( void )
{
		//DoStop();
		DoAutoFocus();
}
//--------------------------------------------------------------------

