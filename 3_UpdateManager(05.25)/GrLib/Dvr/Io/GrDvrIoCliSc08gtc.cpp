/*
 dvr io client WTX1200A

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Io/GrDvrIoCliSc08gtc.h>
#include	<GrUart.h>
#include	<DvrRtl.h>//new-121023
#include	<GrStrTool.h>
#include	<GrDumyTool.h>

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
Cls_GrDvrIoCliSc08gtc::Cls_GrDvrIoCliSc08gtc( __u8 A_UartCh, __u8 A_Id ):
Cls_GrDvrIoCliBase( A_UartCh, A_Id )
{
		m_RecvIn			=	0;
		m_SyncTimer		=	0;
		m_checktime		= 0;
		m_checktimeb4	= 0;
		m_IsCheckGPS	= FALSE;
		//m_IsReqSync	=	TRUE;
}
//--------------------------------------------------------------------
Cls_GrDvrIoCliSc08gtc::~Cls_GrDvrIoCliSc08gtc()
{
		m_IsCheckGPS	= FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrIoCliSc08gtc::RtlProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ReqSize;
		__u8	Tv_Recved;
		__u8*	Tv_PtrRecv;
		__u8	Tv_WkIdx;
		__u8	Tv_SubIdx;
		Def_GrTime	Tv_BaseTime;
		Def_GrTime	Tv_LimitTime;
		Def_GrTime	Tv_TimeGap;
		Def_GrTime	Tv_GpsTime;
		Def_GrTime	Tv_Time;
		__u32	Tv_TmMili;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_TimeGap = 0;
		// recive
		if ( 0 == m_RecvIn )
		{
			Tv_ReqSize	=	1;
		}
		else
		{
			Tv_ReqSize	=	E_GrDvrIoCliSc08gtcRcvBufSize - m_RecvIn;
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
				if ( 0x5B != m_RecvBuf[0] )
				{
					// reset
					m_RecvIn	=	0;
				}
			}
			else if ( E_GrDvrIoCliSc08gtcRcvBufSize == m_RecvIn )
			{
				// check protocol rule
				if ( (0x74 == m_RecvBuf[1]) && (0x5D == m_RecvBuf[E_GrDvrIoCliSc08gtcRcvBufSize - 1]) )
				{
					m_checktime++;
					m_SyncTimer = 600;
					// match protocol
					// parsing protocol
					Tv_GpsTime		=	LcTimeGet();
					Tv_Time			=	GrTimeSysTimeGet( &Tv_TmMili );
					Tv_BaseTime		=	GrTimeEnc( 2008, 1, 1, 0, 0, 0 );
					Tv_LimitTime	=	GrTimeEnc( 2037, 1, 1, 0, 0, 0 );			
					if ( (Tv_BaseTime <= Tv_GpsTime) && (Tv_LimitTime > Tv_GpsTime) )
					{
						if( Tv_Time > Tv_GpsTime )
						{
							Tv_TimeGap	= Tv_Time - Tv_GpsTime;
						}
						else if( Tv_GpsTime > Tv_Time )
						{
							Tv_TimeGap	= Tv_GpsTime - Tv_Time;
						}

						if( 30 < Tv_TimeGap )
						{
							LcSyncTime();
						}
						//if ( m_IsReqSync )
						//{						
						//	LcSyncTime();
						//	m_IsReqSync	=	FALSE;
						//}
					}										
					// finish
					m_RecvIn	=	0;
					Tv_Result	=	TRUE;
				}
				else
				{
					// failed protocol
					// find stx
					for ( Tv_WkIdx=1;Tv_WkIdx < E_GrDvrIoCliSc08gtcRcvBufSize;Tv_WkIdx++ )
					{
						if ( 0x74 == m_RecvBuf[Tv_WkIdx])
						{
							// finded
							// move
							Tv_SubIdx	=	0;
							while ( E_GrDvrIoCliSc08gtcRcvBufSize > (Tv_SubIdx + Tv_WkIdx) )
							{
								m_RecvBuf[Tv_SubIdx]	=	m_RecvBuf[Tv_SubIdx + Tv_WkIdx];
								Tv_SubIdx ++;
							}
							// set size
							m_RecvIn	=	E_GrDvrIoCliSc08gtcRcvBufSize - Tv_WkIdx;
							// finish
							break;
						}
					}
					// not matched head reset
					if ( E_GrDvrIoCliSc08gtcRcvBufSize == m_RecvIn )
					{
						m_RecvIn	=	0;
					}
				}
			}
		}		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliSc08gtc::RtlTimer( void )
{
	// local -------------------
	// code --------------------
		//if ( m_IsReqSync )
		//{
		//	m_SyncTimer	=	0;
		//}
		//else
		//{
		//	m_SyncTimer ++;
		//	if ( E_GrDvrIoCliSc08gtcUpdtPriod < m_SyncTimer )
		//	{
		//		m_IsReqSync	=	TRUE;
		//		m_SyncTimer	=	0;
		//	}
		//}
		m_SyncTimer++;
		if ( E_GrDvrIoCliSc08gtcUpdtPriod < m_SyncTimer )
		{
			if ( m_checktime !=  m_checktimeb4 )  
			{
				m_IsCheckGPS = TRUE;
			}
			else
			{
				m_IsCheckGPS = FALSE;
			}			
			m_checktime = 0;
			m_checktimeb4 = 0;			
			m_SyncTimer = 0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliSc08gtc::UserAction( __u32 A_ActId )
{
	// local -------------------
	// code --------------------
		//if ( 0 == A_ActId )
		//{
		//	m_IsReqSync	=	TRUE;
		//}
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoCliSc08gtc::LcSyncTime( void )
{
		// local -------------------
		// code --------------------
		GrTimeSysTimeSet( LcTimeGet() );
		DvrRtlErrClear( E_DvrRtlErrBattery );//new-121023
		DbgMsgPrint( "Cls_GrDvrIoCliSc08gtc::LcSyncTime() GPS Sync Message Send.\n" );
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrIoCliSc08gtc::LcTimeGet( void )
{
	// local -------------------
		__u16	Tv_Year;
		__u8	Tv_Mon;
		__u8	Tv_Day;
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;
		Def_GrTime	Tv_Time;
	// code --------------------
		// parsing
		// year
		Tv_Year	=	2000 + (__u16)( 10 * ( m_RecvBuf[2] - 0x30 ) ) + (__u16)( m_RecvBuf[3] - 0x30 );
		// month
		Tv_Mon	=	( 10 * ( m_RecvBuf[4] - 0x30 ) ) + ( m_RecvBuf[5] - 0x30 );
		// day
		Tv_Day	=	( 10 * ( m_RecvBuf[6] - 0x30 ) ) + ( m_RecvBuf[7] - 0x30 );
		// hour
		Tv_Hour	=	( 10 * ( m_RecvBuf[8] - 0x30 ) ) + ( m_RecvBuf[9] - 0x30 );
		// minute
		Tv_Min	=	( 10 * ( m_RecvBuf[10] - 0x30 ) ) + ( m_RecvBuf[11] - 0x30 );
		// second
		Tv_Sec	=	( 10 * ( m_RecvBuf[12] - 0x30 ) ) + ( m_RecvBuf[13] - 0x30 );
		// encode time
		Tv_Time				=	GrTimeEnc( Tv_Year, Tv_Mon, Tv_Day, Tv_Hour, Tv_Min, Tv_Sec );
		return Tv_Time;
}
//--------------------------------------------------------------------
BOOL8 Cls_GrDvrIoCliSc08gtc::RetrnGPS( void )
{
	return m_IsCheckGPS;
}
//--------------------------------------------------------------------

