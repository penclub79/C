/*
	real time clock base
*/

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<GrTimeTool.h>
#include	<Hw/Ds3231/GrHwDs3231.h>
#include	<GrI2c.h>

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
Cls_GrHwDs3231::Cls_GrHwDs3231( __u8 A_I2cCh, __u32 A_Adr, __u8 A_Slot )
{
		m_I2cCh			=	A_I2cCh;
		m_ChipAdr		=	(__u8)A_Adr;
		m_I2cSlot		=	A_Slot;

		//init time
		/*m_IsTimeReq			=	TRUE;
		LcReqRead();*/
}
//--------------------------------------------------------------------
Cls_GrHwDs3231::~Cls_GrHwDs3231()
{
}
//--------------------------------------------------------------------
BOOL8	Cls_GrHwDs3231::LcReqRead( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	GrI2cStdRead( m_I2cCh, m_ChipAdr, E_GrHwDs3231RaSec, E_GrHwDs3231TimeLen, m_I2cSlot );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrHwDs3231::LcReqWrite( Def_GrTime A_Time )
{
	// local -------------------
		__u16		Tv_Year;
		__u8		Tv_Month;
		__u8		Tv_Day;
		__u8		Tv_Hour;
		__u8		Tv_Min;
		__u8		Tv_Sec;
		__u8		Tv_RtcTm[E_GrHwDs3231TimeLen];
	// code --------------------
		//make rtc time
		GrTimeDec( A_Time, &Tv_Year, &Tv_Month, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec );
		/*
		DbgMsgPrint("Cls_GrHwDs3231::LcReqWrite - set time %d/%02d/%02d %02d:%02d:%02d\n",
			Tv_Year, Tv_Month, Tv_Day, Tv_Hour, Tv_Min, Tv_Sec);
			*/

		//make year
		Tv_Year		=	Tv_Year % 100;
		Tv_RtcTm[6]		=	(__u8)( (Tv_Year / 10) << 4 );
		Tv_RtcTm[6]		=	Tv_RtcTm[6] | (__u8)(Tv_Year % 10);
		//make month
		Tv_RtcTm[5]		=	((Tv_Month / 10) << 4) | (Tv_Month % 10);
		//make day
		Tv_RtcTm[4]		=	((Tv_Day / 10) << 4) | (Tv_Day % 10);
		//make week
		// Tv_RtcTm[3]		=	(__u8)GrTimeGetWeekByTime( A_Time ) + 1;
		//make hour
		Tv_RtcTm[2]		=	((Tv_Hour / 10) << 4) | (Tv_Hour % 10);
		//make min
		Tv_RtcTm[1]		=	((Tv_Min / 10) << 4) | (Tv_Min % 10);
		//make sec
		Tv_RtcTm[0]		=	((Tv_Sec / 10) << 4) | (Tv_Sec % 10);
		
		//write
		return	GrI2cStdWrite( m_I2cCh, m_ChipAdr, E_GrHwDs3231RaSec, Tv_RtcTm, E_GrHwDs3231TimeLen );
}
//--------------------------------------------------------------------
__u32	Cls_GrHwDs3231::TryTimeGet( void )
{
	// local -------------------
		__u8	Tv_Rval[E_GrHwDs3231TimeLen];
		__u32	Tv_Time;
	// code --------------------
		Tv_Time	=	0;
		if ( m_IsTimeReq )
		{
			if ( GrI2cSlotRead( m_I2cCh, m_I2cSlot, Tv_Rval, FALSE ) )
			{
				m_IsTimeReq		=	FALSE;
				Tv_Time				=	LcParseTime( Tv_Rval );
			}
		}

		return	Tv_Time;
}
//--------------------------------------------------------------------
__u32	Cls_GrHwDs3231::LcParseTime( void* A_PtrRecv )
{
	// local -------------------
		__u16		Tv_Year;
		__u8		Tv_Month;
		__u8		Tv_Day;
		__u8		Tv_Hour;
		__u8		Tv_Min;
		__u8		Tv_Sec;
		__u8*		Tv_PtrVal;
		__u32		Tv_Time;
	// code --------------------
		Tv_Time		=	0;
		Tv_PtrVal	=	(__u8*)A_PtrRecv;
		if ( 0xFF != Tv_PtrVal[6] )			//check bad rtc
		{
			//get year
			Tv_Year		=	(Tv_PtrVal[6] & 0x0F) + (((Tv_PtrVal[6] >> 4) & 0x0F) * 10 );
			Tv_Year		=	Tv_Year + 2000;
			//get month
			Tv_Month	=	(Tv_PtrVal[5] & 0x0F) + (((Tv_PtrVal[5] >> 4) & 0x01) * 10 );
			//get day
			Tv_Day		=	(Tv_PtrVal[4] & 0x0F) + (((Tv_PtrVal[4] >> 4) & 0x03) * 10 );
			//get hour
			Tv_Hour		=	(Tv_PtrVal[2] & 0x0F); 
			if ( 0 == (0x40 & Tv_PtrVal[2]) ) 
			{
				//24 houre type
				Tv_Hour		=	Tv_Hour + (((Tv_PtrVal[2] >> 4) & 0x03) * 10 );
			}
			else
			{
				//12 hour type
				Tv_Hour		=	Tv_Hour + (((Tv_PtrVal[2] >> 4) & 0x01) * 10 );
				if ( 0 != (0x20 & Tv_PtrVal[2]) )
				{
					Tv_Hour		=	Tv_Hour + 12;
				}
			}
			//get min
			Tv_Min		=	(Tv_PtrVal[1] & 0x0F) + (((Tv_PtrVal[1] >> 4) & 0x07) * 10 );
			//get sec
			Tv_Sec		=	(Tv_PtrVal[0] & 0x0F) + (((Tv_PtrVal[0] >> 4) & 0x07) * 10 );
			//make time
			Tv_Time	=	GrTimeEnc( Tv_Year, Tv_Month, Tv_Day, Tv_Hour, Tv_Min, Tv_Sec );
		}

		return	Tv_Time;
}
//--------------------------------------------------------------------
void	Cls_GrHwDs3231::ReqTimeRead( void )
{
		if ( !m_IsTimeReq )
		{
			if ( LcReqRead() )
			{
				m_IsTimeReq	=	TRUE;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrHwDs3231::ReqTimeWrite( Def_GrTime A_Time )
{
		LcReqWrite( A_Time );
}
//--------------------------------------------------------------------

