/*
 dvr io RTC BOX

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Io/GrDvrIoCliRTC.h>
#include	<GrUart.h>

#include	<GrStrTool.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<DvrRtl.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8	V_GrDvrIoCliRTCEBCDDecTbl[70] =	{ 
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
		0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
		0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
		0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
};
//====================================================================
//--------------------------------------------------------------------
Cls_GrDvrIoCliRTC::Cls_GrDvrIoCliRTC( __u8 A_UartCh, __u8 A_Id ):
Cls_GrDvrIoCliBase( A_UartCh, A_Id )
{
		m_RecvIn		=	0;
		m_SyncTimer	=	0;
		m_checktimeb4 = 0;
		m_checktime	= 0;
		//m_IsReqSync	=	TRUE;
		m_UartCh		= A_UartCh;
		m_IsCheckRtcBox = FALSE;
}
//--------------------------------------------------------------------
Cls_GrDvrIoCliRTC::~Cls_GrDvrIoCliRTC()
{
		m_IsCheckRtcBox = FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrIoCliRTC::RtlProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ReqSize;
		__u8	Tv_Recved;
		__u8*	Tv_PtrRecv;
		BOOL8	Tv_IsTmSm;
		Def_GrTime	Tv_TimeGap;
		Def_GrTime	Tv_RtcTime;
		__u32	Tv_TmMili;		
		Def_GrTime	Tv_DvrTime;
		Def_GrTime	Tv_BaseTime;//!!new111
		Def_GrTime	Tv_LimitTime;//!!new111
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_IsTmSm		=	FALSE;
		Tv_TimeGap	= 0;
		Tv_Recved		= 0;
		// receive
		if ( 0 == m_RecvIn )
		{
			Tv_ReqSize	=	1;
		}
		else
		{
			Tv_ReqSize	=	E_GrDvrIoCliRTCRcvBufSize - m_RecvIn;
		}

		Tv_PtrRecv	=	&m_RecvBuf[m_RecvIn];
		Tv_Recved		=	(__u8)GrUartRecv( m_UartCh, (void*)Tv_PtrRecv, (__u32)Tv_ReqSize );
		if ( 0 != Tv_Recved )
		{
			m_RecvIn	=	m_RecvIn + Tv_Recved;
			// check protocol
			if ( 1 == m_RecvIn )
			{
				// check header
				if ( 0xF0 != m_RecvBuf[0] )
				{
					// reset
					m_RecvIn	=	0;
				}
			}
			else if( E_GrDvrIoCliRTCRcvBufSize == m_RecvIn )
			{
				m_checktime++;
				m_SyncTimer = 600;
				Tv_RtcTime = LcTimeGet();	
				Tv_DvrTime = GrTimeSysTimeGet( &Tv_TmMili );
				//!!new111
				Tv_BaseTime		=	GrTimeEnc( 2008, 1, 1, 0, 0, 0 );
				Tv_LimitTime	=	GrTimeEnc( 2037, 1, 1, 0, 0, 0 );				

				if ( (Tv_BaseTime <= Tv_RtcTime) && (Tv_LimitTime > Tv_RtcTime) )				
				{
					if( Tv_RtcTime > Tv_DvrTime )
					{
						Tv_TimeGap	= Tv_RtcTime - Tv_DvrTime;
					}
					else if( Tv_DvrTime > Tv_RtcTime )
					{
						Tv_TimeGap	= Tv_DvrTime - Tv_RtcTime;
					}


					if( 30 < Tv_TimeGap )
					{
						if ( 2 <= DvrRtlHwVerGet() )
						{
							DbgMsgPrint("Cls_GrDvrIoCliRTC::RtlProc - RTC-Box ignored , cause now Board-RTC use \n");
						}						
						else
						{
							LcSyncTime();
						}						
					}
				}
				//!!new111
//#endif
				// finish
				m_RecvIn	=	0;
				Tv_Result	=	TRUE;			
			}
		}	

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliRTC::RtlTimer( void )
{
	// local -------------------
	// code --------------------
		m_SyncTimer++;
		if ( E_GrDvrIoCliRTCUpdtPriod < m_SyncTimer )
		{
			if ( m_checktime !=  m_checktimeb4 )  
			{
				m_IsCheckRtcBox = TRUE;
			}
			else
			{
				m_IsCheckRtcBox = FALSE;
			}
			m_checktime = 0;
			m_checktimeb4 = 0;			
			m_SyncTimer = 0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliRTC::LcSyncTime( void )
{
	// local -------------------		
	// code --------------------		
#ifdef E_DvrRtlTestVersion_TimeSystemUpdt
		GrTimeSysTimeSet( LcTimeGet() );
#else
		GrTimeSet( LcTimeGet() );
#endif
		DvrRtlErrClear( E_DvrRtlErrBattery );//new-121023
		DbgMsgPrint( "Cls_GrDvrIoCliRTC::LcSyncTime() RTC Sync Message Send.\n" );
}//!!new111
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliRTC::TimeSet( Def_GrTime A_Time )
{
	// local -------------------
		__u8	Tv_Buf[8];
		__u16	Tv_Year;
		__u8	Tv_Month;
		__u8	Tv_Day;
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;
	// code --------------------
		GrTimeDec( A_Time, &Tv_Year, &Tv_Month, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec );
		Tv_Buf[0] =	0xF0;
		Tv_Buf[1] =	V_GrDvrIoCliRTCEBCDDecTbl[Tv_Sec];
		Tv_Buf[2] =	V_GrDvrIoCliRTCEBCDDecTbl[Tv_Min];
		Tv_Buf[3] =	V_GrDvrIoCliRTCEBCDDecTbl[Tv_Hour];
#ifdef E_DvrRtlTestVersion_TimeSystemUpdt
		Tv_Buf[4] =	GrTimeGetWeekByDate( Tv_Year, Tv_Month, Tv_Day );
#else
		Tv_Buf[4] =	0;
#endif		
		Tv_Buf[5] =	V_GrDvrIoCliRTCEBCDDecTbl[Tv_Day];
		Tv_Buf[6] =	V_GrDvrIoCliRTCEBCDDecTbl[Tv_Month];
		Tv_Buf[7] =	V_GrDvrIoCliRTCEBCDDecTbl[Tv_Year - 2000];
		GrUartSend(m_UartCh, Tv_Buf, sizeof(Tv_Buf), TRUE );
		DbgMsgPrint( "Cls_GrDvrIoCliRTC::TimeSet() RTC Setting.\n" );
}
//--------------------------------------------------------------------
__u32 Cls_GrDvrIoCliRTC::LcTimeGet( void )
{
	// local -------------------
		__u16	Tv_Year;
		__u8	Tv_Month;
		__u8	Tv_Day;
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;
		Def_GrTime	Tv_Time;
	// code --------------------
		Tv_Sec		=	EBCDIC( m_RecvBuf[1] );
		Tv_Min		=	EBCDIC( m_RecvBuf[2] );
		Tv_Hour		=	EBCDIC( m_RecvBuf[3] );
		Tv_Day		=	EBCDIC( m_RecvBuf[5] );
		Tv_Month	=	EBCDIC( m_RecvBuf[6] );
		Tv_Year		=	EBCDIC( m_RecvBuf[7] ) + 2000;
		Tv_Time		=	GrTimeEnc( Tv_Year, Tv_Month, Tv_Day, Tv_Hour, Tv_Min, Tv_Sec );
		return	Tv_Time;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrIoCliRTC::EBCDIC( __u8 A_Code )
{
	// local -------------------
		__u8	Tv_ForPart;
		__u8	Tv_LastPart;
		__u8	Tv_Buf;
		__u32	Tv_WkIdx;
		__u8	Tv_Result;
	// code --------------------
		Tv_ForPart = 0;
		Tv_LastPart = 0;

		Tv_ForPart = ( A_Code & 0xF0 ) >> 4;
		Tv_Buf = Tv_ForPart;
		Tv_LastPart = A_Code & 0x0F;
		for( Tv_WkIdx = 0; Tv_WkIdx < 9; Tv_WkIdx++)
		{
			Tv_ForPart = Tv_ForPart + Tv_Buf;
		}
		Tv_Result = Tv_ForPart + Tv_LastPart;


		//Tv_ForPart = A_Code / 16;
		//Tv_LastPart = A_Code % 16;
		//Tv_Result = (Tv_ForPart*10) + Tv_LastPart;
		return Tv_Result;
}
//-------------------------------------------------------------------
void	Cls_GrDvrIoCliRTC::CheckRtcBox( void )
{
	// local -------------------
		__u8	Tv_Buf[8];
	// code --------------------
		Tv_Buf[0] =	0xF1;
		Tv_Buf[1] =	0x01;
		Tv_Buf[2] =	0x01;
		Tv_Buf[3] =	0x01;
		Tv_Buf[4] =	0x01;
		Tv_Buf[5] =	0x01;
		Tv_Buf[6] =	0x01;
		Tv_Buf[7] =	0x01;
		GrUartSend(m_UartCh, Tv_Buf, sizeof(Tv_Buf), TRUE );
}
//-------------------------------------------------------------------
BOOL8	Cls_GrDvrIoCliRTC::RetrnRtcBox( void )
{
		return m_IsCheckRtcBox;
}
//-------------------------------------------------------------------

