/*
 dvr pantilt protocol lvc c100hm

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtC100hm.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtC100hmChksumIdx			5

#define E_GrDvrPtzPtC100hmPktSize				6	


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtC100hm::Cls_GrDvrPtzPtC100hm( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
		//LcSendPkt( 0xAA, 0x96, 0x00 );		
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtC100hm::~Cls_GrDvrPtzPtC100hm()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::LcSendPkt( __u8 A_Code1, __u8 A_Code2, __u8 A_Code3 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtC100hmPktSize];
		__u32	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	0xC5;
		Tv_PktBuf[1]		=	A_Code1;
		Tv_PktBuf[2]		=	A_Code2;
		Tv_PktBuf[3]		=	A_Code3;
		Tv_PktBuf[4]		=	m_Id;

		//build checksum
		Tv_Chksum	=	(__u32)Tv_PktBuf[0] + (__u32)Tv_PktBuf[1] + (__u32)Tv_PktBuf[2] + (__u32)Tv_PktBuf[3] + (__u32)Tv_PktBuf[4];
		Tv_PktBuf[E_GrDvrPtzPtC100hmChksumIdx]	=	(__u8)(0xFF & Tv_Chksum);
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::DoMenuLeft( void )
{
		LcSendPkt( 0x5F, 9, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::DoMenuRight( void )
{
		LcSendPkt( 0x5F, 8, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::DoMenuUp( void )
{
		LcSendPkt( 0x5F, 1, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::DoMenuDown( void )
{
		LcSendPkt( 0x5F, 3, 0x00 );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtC100hm::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Code2;
	// code --------------------
		//set zoom		
		Tv_Code2		=	0x01;
		if ( A_IsWide )
		{
			Tv_Code2	=	0x03;
		}
		if ( 2 < A_Speed )
		{
			Tv_Code2 ++;
		}
		LcSendPkt( 0x5F, Tv_Code2, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtC100hm::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Code2;
	// code --------------------
		Tv_Code2	=	0x09;
		if ( A_IsFar )
		{
			Tv_Code2	--;
		}
		if ( 3 > A_Speed )
		{
			Tv_Code2	=	Tv_Code2 | 0x30;
		}
		LcSendPkt( 0x5F, Tv_Code2, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtC100hm::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Code2;
	// code --------------------
		Tv_Code2	=	0x43;
		if ( A_IsOpen )
		{
			Tv_Code2	--;
		}
		LcSendPkt( 0x5F, Tv_Code2, 0x00 );

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::DoStop( void )
{
		LcSendPkt( 0x5F, 0x0C, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::DoAutoFocus( void )
{
		LcSendPkt( 0xAA, 0x73, 0x01 );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtC100hm::IsMenuAble( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::DoMenuCall( void )
{
		LcSendPkt( 0xAA, 0x63, 0x01 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::DoMenuExit( void )
{
		LcSendPkt( 0xAA, 0x63, 0x00 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::ExIdView( BOOL8 A_IsOn )
{
	// local -------------------
		__u8	Tv_Code3;
	// code --------------------
		Tv_Code3	=	0;
		if ( A_IsOn )
		{
			Tv_Code3	=	1;
		}
		LcSendPkt( 0xAA, 0x96, Tv_Code3 );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtC100hm::ExIdSet( __u8 A_IdNew )
{
		LcSendPkt( 0x78, A_IdNew, 0x00 );
		m_Id	=	A_IdNew;
}
//--------------------------------------------------------------------
