/*
 dvr pantilt protocol base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtElmo.h>
#include	<GrUart.h>

//====================================================================
//local const
#define E_GrDvrPtzPtElmoPktChksumIdx	5

#define E_GrDvrPtzPtElmoPktSize				6

#define E_GrDvrPtzPtElmoMinWait				30

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8	V_DvrPtzPtElmoPanTiltSpdTbl[5]	=	
{
	1,	2,	4,	6,	7
};

__u8	V_DvrPtzPtElmoZoomFocusSpdTbl[5]	=	
{
	0,	1,	1,	1,	2
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzPtElmo::Cls_GrDvrPtzPtElmo( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch ):
Cls_GrDvrPtzPtBase( A_UartCh, A_Id, A_Ch )
{
		//init
		m_ZoomSpd			=	0xFF;
		m_DoMap				=	0;				//last doing map
		m_ReqPanSpd		=	0;				//pan speed
		m_ReqTiltSpd	=	0;				//tilt speed
		m_ReqFocusSpd	=	0;				//focus speed
		m_ReqZoomSpd	=	0;				//request zoom speed
		m_ReqDoMap		=	0;				//request do map
		m_ReqPrSet		=	0xFF;			//preset set, 0xff = not use
		m_ReqPrMove		=	0xFF;			//preset move, 0xff = not use
		m_IsReqAtFcs	=	FALSE;		//autofocus

		//power on
		LcSendPkt( 0x82, 0x01, 0x00 );
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtElmo::~Cls_GrDvrPtzPtElmo()
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtElmo::LcSendPkt( __u8 A_Cmd2, __u8 A_Cmd3, __u8 A_Cmd4 )
{
	// local -------------------
		__u8	Tv_PktBuf[E_GrDvrPtzPtElmoPktSize];
		__u8	Tv_Chksum;
	// code --------------------
		//build id
		Tv_PktBuf[0]		=	m_Id;
		Tv_PktBuf[1]		=	0;
		Tv_PktBuf[2]		=	A_Cmd2;
		Tv_PktBuf[3]		=	A_Cmd3;
		Tv_PktBuf[4]		=	A_Cmd4;
		//build checksum
		Tv_Chksum	=	Tv_PktBuf[0] ^ Tv_PktBuf[1] ^ Tv_PktBuf[2] ^ Tv_PktBuf[3] ^ Tv_PktBuf[4];
		Tv_PktBuf[E_GrDvrPtzPtElmoPktChksumIdx]	=	Tv_Chksum;
		//send
		GrUartSend( m_UartCh, Tv_PktBuf, sizeof(Tv_PktBuf), TRUE );
		//wait
		//GrPrcSleep( E_GrDvrPtzPtElmoMinWait );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtElmo::LcCvtElmoPanTiltSpeed( __u8 A_Spd )
{
	// local -------------------
	// code --------------------
		return	V_DvrPtzPtElmoPanTiltSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtElmo::LcCvtElmoZoomFocusSpeed( __u8 A_Spd )
{
		return	V_DvrPtzPtElmoZoomFocusSpdTbl[A_Spd];
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtElmo::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_ReqDo;
	// code --------------------
		Tv_ReqDo	=	0;
		//update do
		if ( A_IsLeft )
		{
			Tv_ReqDo	=	Tv_ReqDo | E_GrDvrPtzPtElmoDoLeft;
		}
		if ( A_IsRight )
		{
			Tv_ReqDo	=	Tv_ReqDo | E_GrDvrPtzPtElmoDoRight;
		}
		if ( A_IsUp )
		{
			Tv_ReqDo	=	Tv_ReqDo | E_GrDvrPtzPtElmoDoUp;
		}
		if ( A_IsDown )
		{
			Tv_ReqDo	=	Tv_ReqDo | E_GrDvrPtzPtElmoDoDown;
		}
		m_ReqDoMap	=	(m_ReqDoMap & (~E_GrDvrPtzPtElmoMaskPantilt)) | Tv_ReqDo;
		//update speed
		m_ReqPanSpd		=	LcCvtElmoPanTiltSpeed(A_SpdPan);
		m_ReqTiltSpd	=	LcCvtElmoPanTiltSpeed(A_SpdTilt);

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtElmo::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_ReqDo;
	// code --------------------
		Tv_ReqDo	=	E_GrDvrPtzPtElmoDoTele;
		if ( A_IsWide )
		{
			Tv_ReqDo	=	E_GrDvrPtzPtElmoDoWide;
		}
		m_ReqDoMap	=	(m_ReqDoMap & (~E_GrDvrPtzPtElmoMaskZoom)) | Tv_ReqDo;
		//update speed
		m_ReqZoomSpd	=	LcCvtElmoZoomFocusSpeed(A_Speed);

		return	A_AutoStop;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtElmo::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_ReqDo;
	// code --------------------
		Tv_ReqDo	=	E_GrDvrPtzPtElmoDoNear;
		if ( A_IsFar )
		{
			Tv_ReqDo	=	E_GrDvrPtzPtElmoDoFar;
		}
		m_ReqDoMap	=	(m_ReqDoMap & (~E_GrDvrPtzPtElmoMaskFocus)) | Tv_ReqDo;
		//update speed
		m_ReqFocusSpd	=	LcCvtElmoZoomFocusSpeed(A_Speed);

		return	A_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtElmo::DoStop( void )
{
		m_ReqDoMap	=	0;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtElmo::DoAutoFocus( void )
{
		m_IsReqAtFcs	=	TRUE;
		m_ReqDoMap		=	(m_ReqDoMap & (~E_GrDvrPtzPtElmoMaskFocus));
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtElmo::DoPresetSet( __u8 A_Id )
{
	// local -------------------
	// code --------------------
		m_ReqPrSet	=	(A_Id - 1) & 0x0F;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtElmo::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
	// code --------------------
		m_ReqPrMove	=	(A_Id - 1) & 0x0F;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtElmo::DoTimer( void )
{
	// local -------------------
	// code --------------------
		
		if ( m_ZoomSpd != m_ReqZoomSpd )
		{
			//change zoom speed
			LcSendPkt( 0x24, 0x03, m_ReqZoomSpd );
			m_ZoomSpd	=	m_ReqZoomSpd;
		}
		else if ( 0xFF != m_ReqPrSet )	//preset set
		{
			LcSendPkt( 0x1D, 0x00, m_ReqPrSet );
			m_ReqPrSet	=	0xFF;
		}
		else if ( 0xFF != m_ReqPrMove )
		{
			LcSendPkt( 0x11, 0x00, m_ReqPrMove );
			m_ReqPrMove	=	0xFF;
		}
		else if ( m_DoMap != m_ReqDoMap )
		{
			//change doing
			//check pan
			if ( (E_GrDvrPtzPtElmoMaskPan & m_DoMap) != (E_GrDvrPtzPtElmoMaskPan & m_ReqDoMap) )
			{
				if ( 0 == (E_GrDvrPtzPtElmoMaskPan & m_ReqDoMap) )
				{
					LcSendPkt( 0x13, 0x00, 0x00 );	//pan stop
				}
				else if ( 0 != (E_GrDvrPtzPtElmoDoLeft & m_ReqDoMap) )
				{
					LcSendPkt( 0x18, 0x01, m_ReqPanSpd );
				}
				else
				{
					LcSendPkt( 0x18, 0x00, m_ReqPanSpd );
				}
				//update pan
				m_DoMap	=	(m_DoMap & (~E_GrDvrPtzPtElmoMaskPan)) | (m_ReqDoMap & E_GrDvrPtzPtElmoMaskPan);
			}
			else if ( (E_GrDvrPtzPtElmoMaskTilt & m_DoMap) != (E_GrDvrPtzPtElmoMaskTilt & m_ReqDoMap) )		//check tilt
			{
				if ( 0 == (E_GrDvrPtzPtElmoMaskTilt & m_ReqDoMap) )
				{
					LcSendPkt( 0x14, 0x00, 0x00 );	//stop
				}
				else if ( 0 != (E_GrDvrPtzPtElmoDoUp & m_ReqDoMap) )
				{
					LcSendPkt( 0x18, 0x02, m_ReqTiltSpd );
				}
				else
				{
					LcSendPkt( 0x18, 0x03, m_ReqTiltSpd );
				}
				//update tilt
				m_DoMap	=	(m_DoMap & (~E_GrDvrPtzPtElmoMaskTilt)) | (m_ReqDoMap & E_GrDvrPtzPtElmoMaskTilt);
			}
			else if ( (E_GrDvrPtzPtElmoMaskFocus & m_DoMap) != (E_GrDvrPtzPtElmoMaskFocus & m_ReqDoMap) )		// focus
			{
				if ( 0 == (E_GrDvrPtzPtElmoMaskFocus & m_ReqDoMap) )
				{
					LcSendPkt( 0x25, 0x04, 0x00 );	//stop
				}
				else if ( 0 != (E_GrDvrPtzPtElmoDoNear & m_ReqDoMap) )
				{
					LcSendPkt( 0x25, 0x05, m_ReqFocusSpd );
				}
				else
				{
					LcSendPkt( 0x25, 0x06, m_ReqFocusSpd );
				}
				//update
				m_DoMap	=	(m_DoMap & (~E_GrDvrPtzPtElmoMaskFocus)) | (m_ReqDoMap & E_GrDvrPtzPtElmoMaskFocus);
			}
			else	//zoom
			{
				if ( 0 == (E_GrDvrPtzPtElmoMaskZoom & m_ReqDoMap) )
				{
					LcSendPkt( 0x24, 0x04, 0x00 );	//stop
				}
				else if ( 0 != (E_GrDvrPtzPtElmoDoTele & m_ReqDoMap) )
				{
					LcSendPkt( 0x24, 0x01, 0x00 );
				}
				else
				{
					LcSendPkt( 0x24, 0x00, 0x00 );
				}
				//update
				m_DoMap	=	(m_DoMap & (~E_GrDvrPtzPtElmoMaskZoom)) | (m_ReqDoMap & E_GrDvrPtzPtElmoMaskZoom);
			}
		}
		else if ( m_IsReqAtFcs )
		{
			LcSendPkt( 0x26, 0x03, 0x00 );
			m_IsReqAtFcs	=	FALSE;
		}
}
//--------------------------------------------------------------------
