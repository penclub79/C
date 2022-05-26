/*
 dvr pantilt protocol lvc c210n

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtC201n.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtC201hmPktMaxSize		16	


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtC201n::Cls_GrDvrPtzPtC201n( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
		m_IsAtFcs	=	FALSE;
		// zoom osd on
		LcSendPkt( 2, 0x41, 0x21, 0x00, 0x00 );
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtC201n::~Cls_GrDvrPtzPtC201n()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::LcSendPkt( __u8 A_CodeCnt, __u8 A_Code1, __u8 A_Code2, __u8 A_Code3, __u8 A_Code4 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtC201hmPktMaxSize];
		__u32	Tv_Chksum;
		__u8	Tv_WkIdx;
		__u8	Tv_CsIdx;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xE5;
		Tv_PktBuf[1]		=	0x80 + A_CodeCnt + 4;
		Tv_PktBuf[2]		=	m_Id;
		Tv_PktBuf[3]		=	A_Code1;
		Tv_PktBuf[4]		=	A_Code2;
		Tv_PktBuf[5]		=	A_Code3;
		Tv_PktBuf[6]		=	A_Code4;
		//build checksum
		Tv_Chksum	=	0;
		Tv_CsIdx	=	A_CodeCnt + 3;
		for ( Tv_WkIdx=0;Tv_WkIdx < Tv_CsIdx;Tv_WkIdx++ )
		{
			Tv_Chksum	=	Tv_Chksum + (__u32)Tv_PktBuf[Tv_WkIdx];
		}
		Tv_Chksum	=	Tv_Chksum & 0xFF;
		Tv_PktBuf[Tv_CsIdx]	=	(__u8)Tv_Chksum;
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, Tv_CsIdx + 1, TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::DoMenuLeft( void )
{
		LcSendPkt( 3, 0x60, 0x00, 0x01, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::DoMenuRight( void )
{
		LcSendPkt( 3, 0x60, 0x00, 0x02, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::DoMenuUp( void )
{
		LcSendPkt( 3, 0x60, 0x00, 0x08, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::DoMenuDown( void )
{
		LcSendPkt( 3, 0x60, 0x00, 0x04, 0x00 );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtC201n::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Code2;
		__u8	Tv_Code3;
	// code --------------------
		Tv_Code3		=	A_Speed;
		//set zoom		
		Tv_Code2		=	0x80;
		if ( A_IsWide )
		{
			Tv_Code2	=	0x90;
		}
		LcSendPkt( 3, 0x11, Tv_Code2, Tv_Code3, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtC201n::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Code2;
		__u8	Tv_Code3;
	// code --------------------
		Tv_Code3	=	A_Speed;
		Tv_Code2	=	0x90;
		if ( A_IsFar )
		{
			Tv_Code2	=	0x80;
		}
		LcSendPkt( 3, 0x12, Tv_Code2, Tv_Code3, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::DoStop( void )
{
		LcSendPkt( 2, 0x7F, 0xFF, 0x00, 0x00 );
		LcSendPkt( 2, 0x7F, 0xFF, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::DoAutoFocus( void )
{
	// local -------------------
		__u8	Tv_Code2;
	// code --------------------
		m_IsAtFcs	=	!m_IsAtFcs;
		Tv_Code2	=	0x39;
		if ( m_IsAtFcs )
		{
			Tv_Code2	=	0x38;
		}
		LcSendPkt( 2, 0x13, Tv_Code2, 0x00, 0x00 );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtC201n::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::DoMenuCall( void )
{
		LcSendPkt( 2, 0x41, 0x01, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::DoMenuExit( void )
{
		LcSendPkt( 2, 0x41, 0x00, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::ExIdView( BOOL8 A_IsOn )
{
	// local -------------------
		__u8	Tv_Code2;
	// code --------------------
		// osd on / off
		Tv_Code2	=	0x10;
		if ( A_IsOn )
		{
			Tv_Code2	=	0x11;
		}
		LcSendPkt( 2, 0x41, Tv_Code2, 0x00, 0x00 );
		// camera id
		Tv_Code2	=	0x40;
		if ( A_IsOn )
		{
			Tv_Code2	=	0x41;
		}
		LcSendPkt( 2, 0x41, Tv_Code2, 0x00, 0x00 );
		// zoom osd on
		LcSendPkt( 2, 0x41, 0x21, 0x00, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::ExIdSet( __u8 A_IdNew )
{
		LcSendPkt( 3, 0x01, 0x01, A_IdNew, 0x00 );
		m_Id	=	A_IdNew;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC201n::DoMenuOk( void )
{
		LcSendPkt( 3, 0x60, 0x00, 0x14, 0x00 );
}
//--------------------------------------------------------------------
