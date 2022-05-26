/*
	time tool 
	uses
		GrTypeBase.h

		predefine
			GR_TICK_MUL - mili tick multiplier - default = 10  ( jiffies * 10 = mili tick )
			
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrTimeTool
#define	_EPC_GrTimeTool

//====================================================================
// uses

#include	<GrTypeBase.h>
#ifdef LINUX_APP
	#ifndef LINUX_DRV
		#include	<time.h>
	#endif
#endif
	
//====================================================================
//constance
#define		E_GrTimeHourOfDay		24
#define		E_GrTimeMinOfDay		(24*60)
#define		E_GrTimeMinOfHour		60
#define		E_GrTimeSecOfDay		(24*60*60) //86400
#define		E_GrTimeSecOfHour		(60*60)
#define		E_GrTimeSecOfMin		60
#define		E_GrTimeSecOfLeafYear	(366 * E_GrTimeSecOfDay)	//31622400
#define		E_GrTimeSecOfYear		(365 * E_GrTimeSecOfDay)	//31536000
#define		E_GrTimeSecOf4year	( (366 + (365*3)) * E_GrTimeSecOfDay )	//126230400
#define		E_GrTimeDayOf4year	( 366 + (365*3) )

#define		E_GrTimeMiliOfSec		1000
#define		E_GrTimeMiliOfMin		(E_GrTimeMiliOfSec * 60)
#define		E_GrTimeMiliOfHour	(E_GrTimeMiliOfMin * 60)

#define		E_GrTimeDayOfWeek		7
#define		E_GrTimeMonthOfYear 12

//week
#define		E_GrTimeWeekSun			0
#define		E_GrTimeWeekMon			1
#define		E_GrTimeWeekTue			2
#define		E_GrTimeWeekWed			3
#define		E_GrTimeWeekThu			4
#define		E_GrTimeWeekFri			5
#define		E_GrTimeWeekSat			6

#define		E_GrTimeFindRngIn				(1 << 0)		/* not use */
#define		E_GrTimeFindRngGreate		(1 << 1)		/* not use */
#define		E_GrTimeFindRngSmall		(1 << 2)		/* not use */

#define		E_GrTimeBaseYear				1970		/* linux base time */
#define		E_GrTimeMaxYear					2038

#define	E_GrTimeLnxGap				0				/* LINUX time gap */
#define	E_GrTimeWinGap				0				/* window time gap */

#define	E_GrTimeSmTblCnt			3

#define	E_GrTimeBaseWeek			E_GrTimeWeekThu

#define E_GrTimeFcc						Mac_GrMakeFcc('G','R','T','M')		/* FCC code for message notify  */

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

//====================================================================
//global function
BOOL8		GrTimeIsLeapYear( __u16 A_Year );		//check leap year validate 2001 ~ 2099

BOOL8		GrTimeIsSmTime( Def_GrTime A_SysTime );
Def_GrTime	GrTimeEnc(	__u16 A_Year, __u8 A_Month, __u8 A_Day, 
												__u8 A_Hour, __u8 A_Min, __u8 A_Sec /*, BOOL8* A_PtrRtIsSm*/ );

void		GrTimeDec(	Def_GrTime A_Time, 
										__u16* A_PtrRtYear, __u8* A_PtrRtMonth, __u8* A_PtrRtDay,	
										__u8* A_PtrRtHour, __u8* A_PtrRtMin, __u8* A_PtrRtSec  );

__u8		GrTimeGetWeekByTime( Def_GrTime A_Time );
__u8		GrTimeGetWeekByDate( __u16 A_Year, __u8 A_Month, __u8 A_Day );

Def_GrTime	GrTimeCvtGtodToGrtime( __u32 A_Sec, BOOL8* A_PtrRtIsSm );
__u32		GrTimeCvtGrtimeToGtod( Def_GrTime A_Time, BOOL8 A_IsSm );
Def_GrTime	GrTimeCvtGtodToSysTime( __u32 A_Sec );
__u32		GrTimeCvtSysTimeToGtod( Def_GrTime A_Time );
Def_GrTime	GrTimeSysTimeGet( __u32* A_PtrRtMili );

Def_GrTime	GrTimeTimeToSysTime( Def_GrTime A_Time, BOOL8 A_IsSm );
Def_GrTime	GrTimeSysTimeToTime( Def_GrTime A_SysTime, BOOL8* A_PtrRtIsSm );
Def_GrTime	GrTimeGet( __u32* A_PtrRtMili, BOOL8* A_PtrRtIsSm );
void				GrTimeSetSummerTime( BOOL8 A_IsAble, __u8 A_StMon, __u8 A_StDay, __u8 A_StHour, __u8 A_EndMon, __u8 A_EndDay, __u8 A_EndHour );
void				GrTimeSetDstTime( BOOL8 A_IsAble, __u8 A_StMon, __u8 A_StWkTh, __u8 A_StWkDay, __u8 A_StHour, __u8 A_EndMon, __u8 A_EndWkTh, __u8 A_EndWkDay, __u8 A_EndHour );

__u8		GrTimeGetMonthMaxDay( __u16 A_Year, __u8 A_Month );

__u32		GrTimeGetTick( void );
__u32		GrTimeTickDifMs( __u32 A_Old, __u32 A_Near );
__u32		GrTimeMsToTick( __u32 A_Ms );
__u32		GrTimeTickToMs( __u32 A_Tick );

#ifdef __cplusplus
void		GrTimeSet( Def_GrTime A_Time, __u32 A_Fcc = E_GrTimeFcc );
void		GrTimeSysTimeSet( Def_GrTime A_Time, __u32 A_Fcc = E_GrTimeFcc );
#else
void		GrTimeSet( Def_GrTime A_Time, __u32 A_Fcc );
void		GrTimeSysTimeSet( Def_GrTime A_Time, __u32 A_Fcc );
#endif

__u32		GrTimeDayCntBySysTime( Def_GrTime A_Time );
__u32		GrTimeDayCntByDate( __u16 A_Year, __u8 A_Month, __u8 A_Day );

//====================================================================
//out Environment conditional
#endif
