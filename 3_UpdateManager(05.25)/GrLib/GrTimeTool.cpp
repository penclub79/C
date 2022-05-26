/*
 time Tools

 linux - nt time query
	38year = 13879day = 1,198,368,000 sec = 0x476DA500
	bacause Nttimebase = 0x2FD8A1380ULL - 0x476DA500 = 0x2B61C6E80

 */


//====================================================================
// uses
#include	<GrLnxApp.h>

#ifdef	LINUX_APP
	#include	<sys/time.h>
	#include	<unistd.h>
	#include	<fcntl.h>
	#include	<sys/stat.h>
	#include	<sys/ioctl.h>
	#include	<time.h>
	#include	<linux/rtc.h>
	#include	<sys/times.h>
#else
	#include	<winternl.h>

#if 0
	typedef	NTSTATUS	(__stdcall *Fnc_NtQuerySystemTime)	( PLARGE_INTEGER SystemTime );
#endif

#endif

#include	<GrTypeBase.h>
#include	<GrMath.h>
#include	<GrTimeTool.h>
#include	<GrDumyTool.h>
#include <GrTaskBase.h>
#include <GrTaskSvr.h>


//====================================================================
//local const

#define	E_GrTimeModeFileTime	/* file time mode */

//#define E_GrTimeNttimeBase	0x2B61C6E80ULL
#define E_GrTimeNttimeBase	0x2B6109100ULL

#ifdef GR_TICK_MUL
#define E_GrTimeTickMul			GR_TICK_MUL
#else
#define E_GrTimeTickMul			10
#endif


//====================================================================
//local type

typedef	struct St_GrTimeSmItm
{
	__u32	Low;
	__u32	High;
}	*Ptr_GrTimeSmItm;

typedef	struct	St_GrTimeLimDh		//time limit day - hour
{
	__u8		Hour;					// hour
	__u8		Week;					// week th
	__u8		Day;					// day
	__u8		Month;				// month
}	*Ptr_GrTimeLimDh;

typedef	struct St_GrTimeMode 
{
	BOOL8	IsSmUse;
	BOOL8	IsSmDst;

	struct St_GrTimeLimDh	SmStTime;		// DST start time
	struct St_GrTimeLimDh	SmEdTime;		// DST end time

}	*Ptr_GrTimeMode;

typedef struct St_GrTimeDstRtl
{
	BOOL8	IsSm;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	TimeUpdt;
	St_GrTimeSmItm	SmTbl[E_GrTimeSmTblCnt];		// DST check table
}	*Ptr_GrTimeDstRtl;

typedef struct St_GrTimeRtl
{
	St_GrTimeDstRtl	DstRtl;
}	*Ptr_GrTimeRtl;

//====================================================================
//local function

void	GrTimeDstRtlBuild( Def_GrTime A_BaseTime, struct St_GrTimeDstRtl* A_PtrRtRtl );
Def_GrTime	GrTimeGetByOs( __u32* A_PtrRtMili );
void	GrTimeDstRtlUpdtSm( struct St_GrTimeDstRtl* A_PtrRtRtl );

//====================================================================
//var
	__u8	V_GrTimeDayOfMonTbl[12]			=	{31,28,31,30,31,30,31,31,30,31,30,31};		// day count table
	__u16	V_GrTimeDaySumOfMonTbl[12]	=	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };		// day count table

	Def_GrTime	V_GrTimeSecOfMonTbl[12]	=	
	{31*E_GrTimeSecOfDay,28*E_GrTimeSecOfDay,31*E_GrTimeSecOfDay,30*E_GrTimeSecOfDay,31*E_GrTimeSecOfDay,30*E_GrTimeSecOfDay,
	31*E_GrTimeSecOfDay,31*E_GrTimeSecOfDay,30*E_GrTimeSecOfDay,31*E_GrTimeSecOfDay,30*E_GrTimeSecOfDay,31*E_GrTimeSecOfDay};
#ifndef	LINUX_APP

#ifndef E_GrTimeModeFileTime
		BOOL8	V_GrTimeIsGetTimeInited	=	FALSE;
		//expansion function
		Fnc_NtQuerySystemTime	V_GrTimeFncNtQuerySystemTime = NULL;
		__u64	V_GrTimeTimeInited	=	E_GrTimeNttimeBase;
#endif

#ifdef LINUX_EMU
		__u32	V_GrTimeEmuGap	=	0;
#endif

#endif

#if defined(LINUX_APP) || defined(LINUX_EMU)
	struct St_GrTimeMode V_GrTimeMode	=	{ FALSE, FALSE };

	struct St_GrTimeRtl V_GrTimeRtl;
#endif

//====================================================================
//--------------------------------------------------------------------
BOOL8		GrTimeIsLeapYear( __u16 A_Year )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		if ( 0 == (A_Year & 3) )
		{
			Tv_Result		=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrTimeDayCntBySysTime( Def_GrTime A_Time )
{
		return	A_Time / E_GrTimeSecOfDay;
}
//--------------------------------------------------------------------
Def_GrTime	GrTimeTimeToSysTime( Def_GrTime A_Time, BOOL8 A_IsSm )
{
	// local -------------------
		Def_GrTime	Tv_Result;
	// code --------------------
		Tv_Result	=	A_Time;
		if ( A_IsSm )
		{
			Tv_Result	=	Tv_Result - E_GrTimeSecOfHour;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	GrTimeSysTimeToTime( Def_GrTime A_SysTime, BOOL8* A_PtrRtIsSm )
{
	// local -------------------
		Def_GrTime	Tv_Result;
	// code --------------------
		Tv_Result			=	A_SysTime;
		*A_PtrRtIsSm	=	FALSE;
		//check summer time
		if ( GrTimeIsSmTime(Tv_Result) )
		{
			*A_PtrRtIsSm	=	TRUE;
			Tv_Result			=	Tv_Result + E_GrTimeSecOfHour;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrTimeIsSmTime( Def_GrTime A_SysTime )
{
	// local -------------------
		BOOL8		Tv_Result;
#if defined(LINUX_APP) || defined(LINUX_EMU)
		__u32		Tv_WkIdx;
		struct St_GrTimeDstRtl	Tv_DstRtl;
#else
		ULARGE_INTEGER	Tv_Tm64;
		_FILETIME		Tv_TmFl;
		_FILETIME		Tv_TmLfl;
		SYSTEMTIME	Tv_TmSys;
		SYSTEMTIME	Tv_TmLc;
		SYSTEMTIME	Tv_TmZlc;
#endif
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		
#if defined(LINUX_APP) || defined(LINUX_EMU)
		if ( V_GrTimeMode.IsSmUse )
		{
			// check in hour range
			if ( (A_SysTime >= V_GrTimeRtl.DstRtl.TimeUpdt) && (A_SysTime < (V_GrTimeRtl.DstRtl.TimeUpdt + E_GrTimeSecOfHour) ) )
			{
				Tv_Result	=	V_GrTimeRtl.DstRtl.IsSm;
			}
			else
			{
				// check cached summer time
				if ( (A_SysTime >= V_GrTimeRtl.DstRtl.SmTbl[0].Low) && (A_SysTime <= V_GrTimeRtl.DstRtl.SmTbl[E_GrTimeSmTblCnt-1].High) )
				{
					// check by cached
					for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTimeSmTblCnt;Tv_WkIdx++ )
					{
						if ( (V_GrTimeRtl.DstRtl.SmTbl[Tv_WkIdx].Low <= A_SysTime) && (V_GrTimeRtl.DstRtl.SmTbl[Tv_WkIdx].High > A_SysTime ) )
						{
							//in summer time
							Tv_Result	=	TRUE;
							break;
						}
					}
				}
				else
				{
					// make 
					GrTimeDstRtlBuild( A_SysTime, &Tv_DstRtl );
					Tv_Result	=	Tv_DstRtl.IsSm;
				}

			}
		}
#else
		Tv_Tm64.QuadPart	=	(((ULONGLONG)A_SysTime) + E_GrTimeNttimeBase) * 10000000;
		Tv_TmLfl.dwLowDateTime		=	Tv_Tm64.LowPart;
		Tv_TmLfl.dwHighDateTime		=	Tv_Tm64.HighPart;

		if(FileTimeToSystemTime(&Tv_TmLfl,&Tv_TmLc))
		{
			if(LocalFileTimeToFileTime(&Tv_TmLfl, &Tv_TmFl))
			{
				if(FileTimeToSystemTime(&Tv_TmFl, &Tv_TmSys))
				{
					if(SystemTimeToTzSpecificLocalTime(NULL, &Tv_TmSys, &Tv_TmZlc))
					{
						if((Tv_TmZlc.wHour != Tv_TmLc.wHour) || (Tv_TmZlc.wMinute != Tv_TmLc.wMinute))
						{
							Tv_Result	=	TRUE;
						}
					}
				}
			}
		}

#endif
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrTimeDayCntByDate( __u16 A_Year, __u8 A_Month, __u8 A_Day )
{
	// local -------------------
		__u32			Tv_Result;
		__u32			Tv_Year;
	// code --------------------
		//calculate day count
		//calculate year
		Tv_Year		=	A_Year - E_GrTimeBaseYear;
		Tv_Result	=	(Tv_Year * 365);
		if(2 < Tv_Year)
		{
			Tv_Result	=	Tv_Result + ((Tv_Year - 2) >> 2);
		}
		
		if(0 != (0x03 & A_Year))
		{
			Tv_Result ++;
		}

		//calculate month
		if ( (1 <= A_Month) && (12 >= A_Month) )
		{
			Tv_Result		=	Tv_Result + (__u32)	V_GrTimeDaySumOfMonTbl[A_Month - 1];
		}
		//check leap
		if ( (2 < A_Month ) && GrTimeIsLeapYear( A_Year ) )
		{
			Tv_Result ++;
		}

		//add day
		Tv_Result	=	Tv_Result + A_Day - 1;
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	GrTimeEnc(	__u16 A_Year, __u8 A_Month, __u8 A_Day, 
												__u8 A_Hour, __u8 A_Min, __u8 A_Sec/*, BOOL8* A_PtrRtIsSm*/ )
{
	// local -------------------
		Def_GrTime		Tv_SysTime;
#if defined(LINUX_APP) || defined(LINUX_EMU)
		__u32			Tv_Days;
#else
		SYSTEMTIME	Tv_TmSys;
		_FILETIME		Tv_TmFl;
		ULARGE_INTEGER	Tv_Tm64;
		__u64				Tv_NtMs;
#endif
	// code --------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		//calculate day count
		Tv_Days		=	GrTimeDayCntByDate( A_Year, A_Month, A_Day );
		Tv_SysTime	=	(Tv_Days * E_GrTimeSecOfDay) + (A_Hour * E_GrTimeSecOfHour ) + (A_Min * E_GrTimeSecOfMin) + A_Sec;
#else
		Tv_SysTime				=	0;

		Tv_TmSys.wYear		=	A_Year;
		Tv_TmSys.wMonth		=	A_Month;
		Tv_TmSys.wDay			=	A_Day;
		Tv_TmSys.wDayOfWeek	=	0;
		Tv_TmSys.wHour		=	A_Hour;
		Tv_TmSys.wMinute	=	A_Min;
		Tv_TmSys.wSecond	=	A_Sec;
		Tv_TmSys.wMilliseconds	=	0;

		if(SystemTimeToFileTime(&Tv_TmSys, &Tv_TmFl))
		{
			Tv_Tm64.LowPart		=	Tv_TmFl.dwLowDateTime;
			Tv_Tm64.HighPart	=	Tv_TmFl.dwHighDateTime;
			Tv_NtMs			=	(Tv_Tm64.QuadPart / 10000);
			Tv_SysTime	=	(Def_GrTime)((Tv_NtMs / 1000) - E_GrTimeNttimeBase);
		}

#endif

		return	Tv_SysTime;
}
//--------------------------------------------------------------------
void		GrTimeDec(	Def_GrTime A_Time, 
										__u16* A_PtrRtYear, __u8* A_PtrRtMonth, __u8* A_PtrRtDay,	
										__u8* A_PtrRtHour, __u8* A_PtrRtMin, __u8* A_PtrRtSec )
{
	// local -------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		Def_GrTime	Tv_WkIdx;
		__u32	Tv_Remain;
		BOOL8	Tv_IsLeap;
		__u32	Tv_Tmp;
#else
		SYSTEMTIME	Tv_TmSys;
		_FILETIME		Tv_TmFl;
		_FILETIME		Tv_TmLfl;
		SYSTEMTIME	Tv_TmLc;
		ULARGE_INTEGER	Tv_Tm64;
#endif
	// code --------------------
		//init
		*A_PtrRtYear	=	E_GrTimeBaseYear;
		*A_PtrRtMonth	=	1;
		*A_PtrRtDay		=	1;
		*A_PtrRtHour	=	0;
		*A_PtrRtMin		=	0;
		*A_PtrRtSec		=	0;

#if defined(LINUX_APP) || defined(LINUX_EMU)

		// year
		Tv_IsLeap	=	FALSE;
		if((E_GrTimeSecOfYear * 2) > A_Time)
		{
			Tv_WkIdx	=	0;
			Tv_Remain	=	A_Time;
			if(E_GrTimeSecOfYear < A_Time)
			{
				Tv_WkIdx ++;
				Tv_Remain	=	Tv_Remain - E_GrTimeSecOfYear;
			}
			*A_PtrRtYear	=	E_GrTimeBaseYear + (__u16)Tv_WkIdx;
		}
		else
		{
			Tv_IsLeap	=	TRUE;
			Tv_Remain	=	A_Time - (E_GrTimeSecOfYear * 2);
			Tv_WkIdx	=	((Tv_Remain / E_GrTimeSecOf4year) * 4) + 2;
			Tv_Remain	=	Tv_Remain % E_GrTimeSecOf4year;
			if(E_GrTimeSecOfLeafYear <= Tv_Remain)
			{
				Tv_WkIdx ++;
				Tv_Remain	=	Tv_Remain - E_GrTimeSecOfLeafYear;
				Tv_IsLeap	=	FALSE;

				if(E_GrTimeSecOfYear <= Tv_Remain)
				{
					Tv_WkIdx ++;
					Tv_Remain	=	Tv_Remain - E_GrTimeSecOfYear;
				}
				if(E_GrTimeSecOfYear <= Tv_Remain)
				{
					Tv_WkIdx ++;
					Tv_Remain	=	Tv_Remain - E_GrTimeSecOfYear;
				}
			}
			*A_PtrRtYear	=	E_GrTimeBaseYear + (__u16)Tv_WkIdx;
		}

		// month
		for(Tv_WkIdx=0; Tv_WkIdx < 12; Tv_WkIdx++)
		{
			Tv_Tmp	=	V_GrTimeSecOfMonTbl[Tv_WkIdx];
			if(1 == Tv_WkIdx)
			{
				if(Tv_IsLeap)
				{
					Tv_Tmp	=	Tv_Tmp + E_GrTimeSecOfDay;
				}
			}
			//check month over
			if(Tv_Remain < Tv_Tmp)
			{
				break;
			}
			//next
			Tv_Remain	=	Tv_Remain - Tv_Tmp;
			(*A_PtrRtMonth)++;
		}

		// day
		Tv_Tmp		=	Tv_Remain / E_GrTimeSecOfDay;
		*A_PtrRtDay = (__u8)(Tv_Tmp + 1);
		Tv_Remain	=	Tv_Remain % E_GrTimeSecOfDay;

		// hour
		Tv_Tmp				=	Tv_Remain / E_GrTimeSecOfHour;
		Tv_Remain			=	Tv_Remain %E_GrTimeSecOfHour;
		*A_PtrRtHour	=	(__u8)Tv_Tmp;

		//min
		Tv_Tmp				=	Tv_Remain / E_GrTimeSecOfMin;
		*A_PtrRtMin		=	(__u8)Tv_Tmp;
		Tv_Remain			=	Tv_Remain % E_GrTimeSecOfMin;

		//calculate sec
		*A_PtrRtSec	=	(__u8)Tv_Remain;

#else
		Tv_Tm64.QuadPart	=	(((ULONGLONG)A_Time) + E_GrTimeNttimeBase) * 10000000;
		Tv_TmLfl.dwLowDateTime	=	Tv_Tm64.LowPart;
		Tv_TmLfl.dwHighDateTime	=	Tv_Tm64.HighPart;
		if(LocalFileTimeToFileTime(&Tv_TmLfl,&Tv_TmFl))
		{
			if(FileTimeToSystemTime(&Tv_TmFl, &Tv_TmSys))
			{
				if(SystemTimeToTzSpecificLocalTime(NULL, &Tv_TmSys, &Tv_TmLc))
				{
					// update
					*A_PtrRtYear	=	Tv_TmLc.wYear;
					*A_PtrRtMonth	=	(__u8)Tv_TmLc.wMonth;
					*A_PtrRtDay		=	(__u8)Tv_TmLc.wDay;
					*A_PtrRtHour	=	(__u8)Tv_TmLc.wHour;
					*A_PtrRtMin		=	(__u8)Tv_TmLc.wMinute;
					*A_PtrRtSec		=	(__u8)Tv_TmLc.wSecond;
				}
			}
		}

#endif

}
//--------------------------------------------------------------------
__u8		GrTimeGetWeekByTime( Def_GrTime A_Time )
{
	// local -------------------
		__u32		Tv_Result;
#if defined(LINUX_APP) || defined(LINUX_EMU)
		__u32		Tv_DayCnt;
#else
		SYSTEMTIME	Tv_TmSys;
		_FILETIME		Tv_TmFl;
		_FILETIME		Tv_TmLfl;
		SYSTEMTIME	Tv_TmLc;
		ULARGE_INTEGER	Tv_Tm64;
#endif
	// code --------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		Tv_DayCnt	=	GrTimeDayCntBySysTime( A_Time );
		Tv_Result	=	(Tv_DayCnt + E_GrTimeBaseWeek) % 7;
#else
		Tv_Result	=	0;

		Tv_Tm64.QuadPart	=	(((ULONGLONG)A_Time) + E_GrTimeNttimeBase) * 10000000;
		Tv_TmLfl.dwLowDateTime		=	Tv_Tm64.LowPart;
		Tv_TmLfl.dwHighDateTime		=	Tv_Tm64.HighPart;
		if(LocalFileTimeToFileTime(&Tv_TmLfl,&Tv_TmFl))
		{
			if(FileTimeToSystemTime(&Tv_TmFl, &Tv_TmSys))
			{
				if(SystemTimeToTzSpecificLocalTime(NULL, &Tv_TmSys, &Tv_TmLc))
				{
					// update
					Tv_Result	=	(__u32)Tv_TmLc.wDayOfWeek;
				}
			}
		}

#endif
		
		return	(__u8)Tv_Result;
}
//--------------------------------------------------------------------
__u8		GrTimeGetWeekByDate( __u16 A_Year, __u8 A_Month, __u8 A_Day )
{
	// local -------------------
		__u32		Tv_Result;
#if defined(LINUX_APP) || defined(LINUX_EMU)
		__u32		Tv_DayCnt;
#else
		SYSTEMTIME	Tv_TmSys;
		//_FILETIME		Tv_TmFl;
		_FILETIME		Tv_TmLfl;
		SYSTEMTIME	Tv_TmLc;
#endif
	// code --------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		Tv_DayCnt	=	GrTimeDayCntByDate( A_Year, A_Month, A_Day );
		Tv_Result	=	(Tv_DayCnt + E_GrTimeBaseWeek) % 7;
#else
		Tv_TmLc.wYear				=	A_Year;
		Tv_TmLc.wMonth			=	A_Month;
		Tv_TmLc.wDay				=	A_Day;
		Tv_TmLc.wDayOfWeek	=	0;
		Tv_TmLc.wHour				=	0;
		Tv_TmLc.wMinute			=	0;
		Tv_TmLc.wSecond			=	0;
		Tv_TmLc.wMilliseconds	=	0;

		if(SystemTimeToFileTime(&Tv_TmLc, &Tv_TmLfl))
		{
			if(FileTimeToSystemTime(&Tv_TmLfl, &Tv_TmSys))
			{
				Tv_Result	=	Tv_TmSys.wDayOfWeek;
			}

			/*
			if(LocalFileTimeToFileTime(&Tv_TmLfl, &Tv_TmFl))
			{
				if(FileTimeToSystemTime(&Tv_TmFl, &Tv_TmSys))
				{
					if(SystemTimeToTzSpecificLocalTime(NULL,&Tv_TmSys,&Tv_TmLc))
					{
						Tv_Result	=	Tv_TmLc.wDayOfWeek;
					}
				}
			}
			*/
		}

#endif

		return	(__u8)Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	GrTimeCvtGtodToGrtime( __u32 A_Sec, BOOL8* A_PtrRtIsSm )
{
	// local -------------------
	// code --------------------
		return	GrTimeSysTimeToTime(A_Sec, A_PtrRtIsSm);
}
//--------------------------------------------------------------------
Def_GrTime	GrTimeCvtGtodToSysTime( __u32 A_Sec )
{
		return	A_Sec;
}
//--------------------------------------------------------------------
__u32		GrTimeCvtGrtimeToGtod( Def_GrTime A_Time, BOOL8 A_IsSm )
{
	// local -------------------
	// code --------------------
		return	GrTimeTimeToSysTime( A_Time, A_IsSm );
}
//--------------------------------------------------------------------
__u32		GrTimeCvtSysTimeToGtod( Def_GrTime A_Time )
{
		return	A_Time;
}
//--------------------------------------------------------------------
Def_GrTime	GrTimeGet( __u32* A_PtrRtMili, BOOL8* A_PtrRtIsSm )
{
	// local -------------------
		
#if defined(LINUX_APP) || defined(LINUX_EMU)
		Def_GrTime	Tv_Result;
#else
		Def_GrTime	Tv_Result;
		_FILETIME		Tv_TmFl;
		_FILETIME		Tv_TmLfl;
		SYSTEMTIME	Tv_TmSys;
		SYSTEMTIME	Tv_TmLc;
		SYSTEMTIME	Tv_TmZlc;
		ULARGE_INTEGER	Tv_Tm64;
		__u64				Tv_NtMs;
#endif
	// code --------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		//init
		Tv_Result	=	GrTimeSysTimeGet( A_PtrRtMili );
		Tv_Result	=	GrTimeSysTimeToTime( Tv_Result, A_PtrRtIsSm );
#else
		*A_PtrRtIsSm	=	FALSE;

		GetSystemTimeAsFileTime(&Tv_TmFl);
		FileTimeToLocalFileTime(&Tv_TmFl, &Tv_TmLfl);
		Tv_Tm64.LowPart		=	Tv_TmLfl.dwLowDateTime;
		Tv_Tm64.HighPart	=	Tv_TmLfl.dwHighDateTime;
		Tv_NtMs			=	(Tv_Tm64.QuadPart / 10000);
		Tv_Result		=	(Def_GrTime)((Tv_NtMs / 1000) - E_GrTimeNttimeBase);

		*A_PtrRtMili	=	(__u32)(Tv_NtMs % 1000);

		if(FileTimeToSystemTime(&Tv_TmLfl, &Tv_TmLc))
		{
			if(FileTimeToSystemTime(&Tv_TmFl, &Tv_TmSys))
			{
				if(SystemTimeToTzSpecificLocalTime(NULL, &Tv_TmSys, &Tv_TmZlc))
				{
					if((Tv_TmZlc.wHour != Tv_TmLc.wHour) || (Tv_TmZlc.wMinute != Tv_TmLc.wMinute))
					{
						*A_PtrRtIsSm	=	TRUE;
					}
				}
			}
		}

#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	GrTimeSysTimeGet( __u32* A_PtrRtMili )
{
	// local -------------------
		Def_GrTime	Tv_Result;
	// code --------------------
		// time get
		Tv_Result	=	GrTimeGetByOs( A_PtrRtMili );

#if defined(LINUX_APP) || defined(LINUX_EMU)
		if ( V_GrTimeMode.IsSmUse )
		{
			// check out of hour range
			if ( (Tv_Result < V_GrTimeRtl.DstRtl.TimeUpdt) || (Tv_Result >= (V_GrTimeRtl.DstRtl.TimeUpdt + E_GrTimeSecOfHour )) )
			{
				// update new hour
				V_GrTimeRtl.DstRtl.TimeUpdt	=	Tv_Result - (Tv_Result % E_GrTimeSecOfHour);
				if ( (Tv_Result >= V_GrTimeRtl.DstRtl.SmTbl[0].Low) && (Tv_Result <= V_GrTimeRtl.DstRtl.SmTbl[E_GrTimeSmTblCnt-1].High) )
				{
					GrTimeDstRtlUpdtSm( &V_GrTimeRtl.DstRtl );
				}
				else
				{
					// update table
					GrTimeDstRtlBuild( V_GrTimeRtl.DstRtl.TimeUpdt, &V_GrTimeRtl.DstRtl );
				}
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifndef	LINUX_APP

#if 0

NTSTATUS	NTAPI	NtQuerySystemTime( PLARGE_INTEGER SystemTime )
{
	// local -------------------
		NTSTATUS	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	STATUS_ACCESS_VIOLATION;
		if ( NULL != V_GrTimeFncNtQuerySystemTime )
		{
			Tv_Result		=	V_GrTimeFncNtQuerySystemTime( SystemTime );
		}

		return	Tv_Result;
}

#endif

#endif
//--------------------------------------------------------------------
__u8		GrTimeGetMonthMaxDay( __u16 A_Year, __u8 A_Month )
{
	// local -------------------
		__u8		Tv_Result;
	// code --------------------
		Tv_Result		=	31;
		//check month range
		if ( (1 <= A_Month) && (12 >= A_Month) )
		{
			Tv_Result		=	V_GrTimeDayOfMonTbl[A_Month - 1];
			if ( 2 == A_Month )
			{
				if ( GrTimeIsLeapYear( A_Year ) )
				{
					Tv_Result ++;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrTimeGetTick( void )
{
	// local -------------------
#ifdef LINUX_APP
		struct tms	Tv_Tms;
#endif
	// code --------------------
#ifdef LINUX_APP
		return	(__u32)times(&Tv_Tms);
#else
		return	GetTickCount();
#endif
}
//--------------------------------------------------------------------
__u32		GrTimeTickDifMs( __u32 A_Old, __u32 A_Near )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		#ifdef LINUX_APP
			//Tv_Result		=	((A_Near - A_Old) * 1000) / sysconf(_SC_CLK_TCK);
#if (1 == E_GrTimeTickMul)
		Tv_Result		=	(A_Near - A_Old);
#else
		Tv_Result		=	(A_Near - A_Old) * E_GrTimeTickMul;
#endif
			
		#else
			Tv_Result		=	(A_Near - A_Old);
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrTimeMsToTick( __u32 A_Ms )
{
#ifdef LINUX_APP
		//return	(A_Ms * sysconf(_SC_CLK_TCK) ) / 1000;
#if (1 == E_GrTimeTickMul)
		return	A_Ms;
#else
	return	A_Ms / E_GrTimeTickMul;
#endif

#else
		return	A_Ms;
#endif
}
//--------------------------------------------------------------------
__u32		GrTimeTickToMs( __u32 A_Tick )
{
#ifdef LINUX_APP
		//return	(A_Tick * 1000) / sysconf(_SC_CLK_TCK);
		return	A_Tick * E_GrTimeTickMul;
#else
		return	A_Tick;
#endif
}
//--------------------------------------------------------------------
void	GrTimeSetDstTime( BOOL8 A_IsAble, __u8 A_StMon, __u8 A_StWkTh, __u8 A_StWkDay, __u8 A_StHour, __u8 A_EndMon, __u8 A_EndWkTh, __u8 A_EndWkDay, __u8 A_EndHour )
{
	// local -------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		__u32	Tv_Mili;
		Def_GrTime	Tv_Time;
#endif
	// code --------------------

#if defined(LINUX_APP) || defined(LINUX_EMU)
		V_GrTimeMode.IsSmUse	=	A_IsAble;
		if ( V_GrTimeMode.IsSmUse )
		{
			Tv_Time	=	GrTimeGetByOs( &Tv_Mili );
			V_GrTimeRtl.DstRtl.TimeUpdt	=	Tv_Time - (Tv_Time % E_GrTimeSecOfHour);

			V_GrTimeMode.IsSmDst	=	TRUE;

			V_GrTimeMode.SmStTime.Month	=	A_StMon;
			V_GrTimeMode.SmStTime.Week	=	A_StWkTh;	
			V_GrTimeMode.SmStTime.Day		=	A_StWkDay;
			V_GrTimeMode.SmStTime.Hour	=	A_StHour;
			V_GrTimeMode.SmEdTime.Month	=	A_EndMon;
			V_GrTimeMode.SmEdTime.Week	=	A_EndWkTh;
			V_GrTimeMode.SmEdTime.Day		=	A_EndWkDay;
			V_GrTimeMode.SmEdTime.Hour	=	A_EndHour;

			GrTimeDstRtlBuild( V_GrTimeRtl.DstRtl.TimeUpdt, &V_GrTimeRtl.DstRtl );

		}
#endif

}
//--------------------------------------------------------------------
void	GrTimeSetSummerTime( BOOL8 A_IsAble, __u8 A_StMon, __u8 A_StDay, __u8 A_StHour, __u8 A_EndMon, __u8 A_EndDay, __u8 A_EndHour )
{
	// local -------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		__u32	Tv_Mili;
		Def_GrTime	Tv_Time;
#endif
	// code --------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		V_GrTimeMode.IsSmUse	=	(BOOL8)A_IsAble;
		if ( V_GrTimeMode.IsSmUse )
		{
			Tv_Time	=	GrTimeGetByOs( &Tv_Mili );
			V_GrTimeRtl.DstRtl.TimeUpdt	=	Tv_Time - (Tv_Time % E_GrTimeSecOfHour);

			V_GrTimeMode.IsSmDst	=	FALSE;

			V_GrTimeMode.SmStTime.Month	=	A_StMon;
			V_GrTimeMode.SmStTime.Day		=	A_StDay;
			V_GrTimeMode.SmStTime.Hour	=	A_StHour;
			V_GrTimeMode.SmEdTime.Month	=	A_EndMon;
			V_GrTimeMode.SmEdTime.Day		=	A_EndDay;
			V_GrTimeMode.SmEdTime.Hour	=	A_EndHour;

			GrTimeDstRtlBuild( V_GrTimeRtl.DstRtl.TimeUpdt, &V_GrTimeRtl.DstRtl );
		}
#endif
}
//--------------------------------------------------------------------
void		GrTimeSet( Def_GrTime A_Time, __u32 A_Fcc )
{
	// local -------------------
			Def_GrTime	Tv_SysTime;			
			BOOL8	Tv_IsSm;
	// code --------------------
			Tv_IsSm			=	GrTimeIsSmTime( A_Time );
			Tv_SysTime	=	GrTimeTimeToSysTime( A_Time, Tv_IsSm );
			GrTimeSysTimeSet( Tv_SysTime, A_Fcc );
}
//--------------------------------------------------------------------
void		GrTimeSysTimeSet( Def_GrTime A_Time, __u32 A_Fcc )
{
	// local -------------------
#ifdef LINUX_APP
		struct timeval	Tv_Gtod;	
#else

#if 1
		_FILETIME	Tv_TmFl;
		ULARGE_INTEGER	Tv_Tm64;
#else
		LARGE_INTEGER	Tv_NtTime;
		HINSTANCE	Tv_HinstNtdll;
#endif

		__u64		Tv_NtMs;
		__u32		Tv_Sec;
#endif
		St_GrTaskMsgTimeUpdt	Tv_Msg;
	// code --------------------
#ifdef LINUX_APP
		//set time
		Tv_Gtod.tv_usec		=	0;
		Tv_Gtod.tv_sec		=	A_Time;
		settimeofday( &Tv_Gtod, NULL );
#else

#ifdef E_GrTimeModeFileTime

		// check initialized
		GetSystemTimeAsFileTime(&Tv_TmFl);
		Tv_Tm64.LowPart		=	Tv_TmFl.dwLowDateTime;
		Tv_Tm64.HighPart	=	Tv_TmFl.dwHighDateTime;
		Tv_NtMs			=	(Tv_Tm64.QuadPart / 10000);
		Tv_Sec		=	(Def_GrTime)(Tv_NtMs / 1000);	
#else

		if ( !V_GrTimeIsGetTimeInited )
		{
			V_GrTimeTimeInited	=	E_GrTimeNttimeBase;
			V_GrTimeIsGetTimeInited	=	TRUE;
			//load API
			Tv_HinstNtdll		=	LoadLibraryA("Ntdll.dll"); 
			if ( NULL != Tv_HinstNtdll )
			{
				V_GrTimeFncNtQuerySystemTime	=	(Fnc_NtQuerySystemTime)GetProcAddress(Tv_HinstNtdll, "NtQuerySystemTime" ); 
			}
		}
		NtQuerySystemTime( &Tv_NtTime );
		Tv_NtMs		=	(Tv_NtTime.QuadPart / 10000);
		Tv_Sec		=	(Def_GrTime)(((Tv_NtMs / 1000) - V_GrTimeTimeInited) + 0x7E90);	// GMT korea = +9
#endif

#if defined(LINUX_APP) || defined(LINUX_EMU)
		V_GrTimeEmuGap	=	Tv_Sec - A_Time;
#endif

#endif

#if defined(LINUX_APP) || defined(LINUX_EMU)
		// update init
		if ( V_GrTimeMode.IsSmUse )
		{
			V_GrTimeRtl.DstRtl.TimeUpdt	=	A_Time - (A_Time % E_GrTimeSecOfHour);
			GrTimeDstRtlBuild( V_GrTimeRtl.DstRtl.TimeUpdt, &V_GrTimeRtl.DstRtl );
		}
#endif

		// notify
		Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgTimeUpdt;
		Tv_Msg.Fcc				=	A_Fcc;
		Tv_Msg.Time				=	A_Time;
		GrTaskSvrMsgBroadCast( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	GrTimeDstRtlBuild( Def_GrTime A_BaseTime, struct St_GrTimeDstRtl* A_PtrRtRtl )
{
	// local -------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		__u16	Tv_Yr;
		__u8	Tv_Mo;
		__u8	Tv_Dy;
		__u8	Tv_Hr;
		__u8	Tv_Mi;
		__u8	Tv_Sc;
		__u16	Tv_AddYear;
		__u8	Tv_WkIdx;
		__u16	Tv_Year;
		__u32	Tv_Days;
		__u8	Tv_LastDay;
		__u8	Tv_Week;
		__u32	Tv_StDays;
		__u32	Tv_EdDays;
#endif
	// code --------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		// last year make
		if ( V_GrTimeMode.IsSmUse )
		{
			// get base time
			GrTimeDec( A_BaseTime, &Tv_Yr, &Tv_Mo, &Tv_Dy, &Tv_Hr, &Tv_Mi, &Tv_Sc );
			if ( V_GrTimeMode.IsSmDst )
			{
				// relative DST
				// low
				// calculate date
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTimeSmTblCnt;Tv_WkIdx++ )
				{
					Tv_Year	=	Tv_WkIdx + Tv_Yr - 1;
					if ( 0 == V_GrTimeMode.SmStTime.Week )
					{
						// last week
						Tv_LastDay	=	GrTimeGetMonthMaxDay( Tv_Year, V_GrTimeMode.SmStTime.Month );
						Tv_StDays	=	GrTimeDayCntByDate( Tv_Year, V_GrTimeMode.SmStTime.Month, Tv_LastDay );
						Tv_Week	=	GrTimeGetWeekByDate( Tv_Year, V_GrTimeMode.SmStTime.Month, Tv_LastDay );
						if ( Tv_Week < V_GrTimeMode.SmStTime.Day )
						{
							Tv_StDays	=	Tv_StDays - ( (7 + (__u32)Tv_Week) - (__u32)V_GrTimeMode.SmStTime.Day );
						}
						else
						{
							Tv_StDays	=	Tv_StDays - ( (__u32)Tv_Week - (__u32)V_GrTimeMode.SmStTime.Day );
						}
					}
					else
					{
						// normal
						Tv_StDays	=	GrTimeDayCntByDate( Tv_Year, V_GrTimeMode.SmStTime.Month, 1 );
						Tv_Week	=	GrTimeGetWeekByDate( Tv_Year, V_GrTimeMode.SmStTime.Month, 1 );
						if ( Tv_Week > V_GrTimeMode.SmStTime.Day )
						{
							Tv_StDays	= Tv_StDays + 7;
						}
						Tv_StDays	=	Tv_StDays + ( 7 * ( (__u32)V_GrTimeMode.SmStTime.Week - 1 ) ) + (__u32)V_GrTimeMode.SmStTime.Day - (__u32)Tv_Week;
					}
					A_PtrRtRtl->SmTbl[Tv_WkIdx].Low	=	(Tv_StDays * E_GrTimeSecOfDay) + ((__u32)V_GrTimeMode.SmStTime.Hour * E_GrTimeSecOfHour );
				}

				// high
				Tv_Year	=	Tv_Yr - 1;
				if ( V_GrTimeMode.SmStTime.Month > V_GrTimeMode.SmEdTime.Month )
				{
					Tv_Year ++;
				}
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTimeSmTblCnt;Tv_WkIdx++ )
				{
					if ( 0 == V_GrTimeMode.SmEdTime.Week )
					{
						// last week
						Tv_LastDay	=	GrTimeGetMonthMaxDay( Tv_Year, V_GrTimeMode.SmEdTime.Month );
						Tv_EdDays	=	GrTimeDayCntByDate( Tv_Year, V_GrTimeMode.SmEdTime.Month, Tv_LastDay );
						Tv_Week	=	GrTimeGetWeekByDate( Tv_Year, V_GrTimeMode.SmEdTime.Month, Tv_LastDay );
						if ( Tv_Week < V_GrTimeMode.SmEdTime.Day )
						{
							Tv_EdDays	=	Tv_EdDays - ( (7 + (__u32)Tv_Week) - (__u32)V_GrTimeMode.SmEdTime.Day );
						}
						else
						{
							Tv_EdDays	=	Tv_EdDays - ( (__u32)Tv_Week - (__u32)V_GrTimeMode.SmEdTime.Day );
						}
					}
					else
					{
						// normal
						Tv_EdDays	=	GrTimeDayCntByDate( Tv_Year, V_GrTimeMode.SmEdTime.Month, 1 );
						Tv_Week	=	GrTimeGetWeekByDate( Tv_Year, V_GrTimeMode.SmEdTime.Month, 1 );
						if ( Tv_Week > V_GrTimeMode.SmEdTime.Day )
						{
							Tv_EdDays	= Tv_EdDays + 7;
						}
						Tv_EdDays	=	Tv_EdDays + ( 7 * ( (__u32)V_GrTimeMode.SmEdTime.Week - 1 ) ) + (__u32)V_GrTimeMode.SmEdTime.Day - (__u32)Tv_Week;
					}
					A_PtrRtRtl->SmTbl[Tv_WkIdx].High	=	(Tv_EdDays * E_GrTimeSecOfDay) + ((__u32)V_GrTimeMode.SmEdTime.Hour * E_GrTimeSecOfHour );

					// next
					Tv_Year ++;
				}
			}
			else
			{
				// absoluteness DST
				Tv_AddYear	=	0;
				if (	(V_GrTimeMode.SmStTime.Month > V_GrTimeMode.SmEdTime.Month) || 
							((V_GrTimeMode.SmStTime.Month == V_GrTimeMode.SmEdTime.Month) && (V_GrTimeMode.SmStTime.Day > V_GrTimeMode.SmEdTime.Day)) || 
							( (V_GrTimeMode.SmStTime.Month == V_GrTimeMode.SmEdTime.Month) && (V_GrTimeMode.SmStTime.Day == V_GrTimeMode.SmEdTime.Day) && (V_GrTimeMode.SmStTime.Hour > V_GrTimeMode.SmEdTime.Hour) ) )
				{
					Tv_AddYear	=	1;
				}
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTimeSmTblCnt;Tv_WkIdx++ )
				{
					Tv_Year	=	Tv_WkIdx + Tv_Yr - 1;
					Tv_Days	=	GrTimeDayCntByDate( Tv_Year, V_GrTimeMode.SmStTime.Month, V_GrTimeMode.SmStTime.Day );
					A_PtrRtRtl->SmTbl[Tv_WkIdx].Low	=	(Tv_Days * E_GrTimeSecOfDay) + ((__u32)V_GrTimeMode.SmStTime.Hour * E_GrTimeSecOfHour );
					Tv_Year	=	Tv_Year + Tv_AddYear;
					Tv_Days	=	GrTimeDayCntByDate( Tv_Year, V_GrTimeMode.SmEdTime.Month, V_GrTimeMode.SmEdTime.Day );
					
					A_PtrRtRtl->SmTbl[Tv_WkIdx].High	=	(Tv_Days * E_GrTimeSecOfDay) + ((__u32)V_GrTimeMode.SmEdTime.Hour * E_GrTimeSecOfHour ) 
						- 1;
				}
			}

			GrTimeDstRtlUpdtSm( A_PtrRtRtl );
		}
#endif
}
//--------------------------------------------------------------------
Def_GrTime	GrTimeGetByOs( __u32* A_PtrRtMili )
{
	// local -------------------
		Def_GrTime	Tv_Result;
#ifdef	LINUX_APP
		struct timeval	Tv_Gtod;
#else

#if 1
		_FILETIME	Tv_TmFl;
		_FILETIME	Tv_TmLfl;
		ULARGE_INTEGER	Tv_Tm64;
#else
		LARGE_INTEGER	Tv_NtTime;
		HINSTANCE	Tv_HinstNtdll;
#endif // 0

		__u64		Tv_NtMs;

#endif
	// code --------------------
		//init
#ifdef	LINUX_APP
		gettimeofday( &Tv_Gtod, NULL );
		Tv_Result	=	Tv_Gtod.tv_sec;
		*A_PtrRtMili	=	Tv_Gtod.tv_usec >> 10;
#else

#ifdef E_GrTimeModeFileTime

		GetSystemTimeAsFileTime(&Tv_TmFl);
		FileTimeToLocalFileTime(&Tv_TmFl,&Tv_TmLfl);
		Tv_Tm64.LowPart		=	Tv_TmLfl.dwLowDateTime;
		Tv_Tm64.HighPart	=	Tv_TmLfl.dwHighDateTime;
		Tv_NtMs			=	(Tv_Tm64.QuadPart / 10000);
		Tv_Result		=	(Def_GrTime)((Tv_NtMs / 1000) - E_GrTimeNttimeBase);
#else
		if(!V_GrTimeIsGetTimeInited)
		{
			V_GrTimeTimeInited	=	E_GrTimeNttimeBase;
			V_GrTimeIsGetTimeInited	=	TRUE;
			//load API
			Tv_HinstNtdll		=	LoadLibraryA("Ntdll.dll");
			if(NULL != Tv_HinstNtdll)
			{
				V_GrTimeFncNtQuerySystemTime	=	(Fnc_NtQuerySystemTime)GetProcAddress(Tv_HinstNtdll, "NtQuerySystemTime");
			}
		}
		NtQuerySystemTime(&Tv_NtTime);
		Tv_NtMs			=	(Tv_NtTime.QuadPart / 10000);
		Tv_Result		=	(Def_GrTime)(((Tv_NtMs / 1000) - V_GrTimeTimeInited) + 0x7E90) - V_GrTimeEmuGap;		// GMT + 9

#endif // 0

		*A_PtrRtMili	=	(__u32)(Tv_NtMs % 1000);

#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrTimeDstRtlUpdtSm( struct St_GrTimeDstRtl* A_PtrRtRtl )
{
	// local -------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		__u8	Tv_WkIdx;
#endif
	// code --------------------
#if defined(LINUX_APP) || defined(LINUX_EMU)
		// update summer time status
		A_PtrRtRtl->IsSm	=	FALSE;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTimeSmTblCnt;Tv_WkIdx++ )
		{
			if ( (A_PtrRtRtl->SmTbl[Tv_WkIdx].Low <= A_PtrRtRtl->TimeUpdt ) && (A_PtrRtRtl->SmTbl[Tv_WkIdx].High > A_PtrRtRtl->TimeUpdt ) )
			{
				//in summer time
				A_PtrRtRtl->IsSm	=	TRUE;
				break;
			}
		}
#endif
}
//--------------------------------------------------------------------
