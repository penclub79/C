/*
Platform 5 text

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5Text.h>

#include <P5/GrP5Base.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>

#ifdef GR_P5_EDITOR

#include <P5/Dvl/GrP5DvlBase.h>

#endif

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

typedef struct St_GrP5TextFixInfo
{
	__u16		Id;
	WCHAR		StrName[31];
}	*Ptr_GrP5TextFixInfo;

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

#ifdef GR_P5_EDITOR

St_GrP5TextFixInfo	V_GrP5TextFixInfo[E_GrP5TxtDedicatedCnt]	=	
{
	{ E_GrP5TxtIdLang0 ,	L"Language 0"},
	{ E_GrP5TxtIdLang1, L"Language 1" },
	{ E_GrP5TxtIdLang2, L"Language 2" },
	{ E_GrP5TxtIdLang3, L"Language 3" },
	{ E_GrP5TxtIdLang4, L"Language 4" },
	{ E_GrP5TxtIdLang5, L"Language 5" },
	{ E_GrP5TxtIdLang6, L"Language 6" },
	{ E_GrP5TxtIdLang7, L"Language 7" },
	{ E_GrP5TxtIdLang8, L"Language 8" },
	{ E_GrP5TxtIdLang9, L"Language 9" },
	{ E_GrP5TxtIdLang10, L"Language 10" },
	{ E_GrP5TxtIdLang11, L"Language 11" },
	{ E_GrP5TxtIdLang12, L"Language 12" },
	{ E_GrP5TxtIdLang13, L"Language 13" },
	{ E_GrP5TxtIdLang14, L"Language 14" },
	{ E_GrP5TxtIdLang15, L"Language 15" },
	{ E_GrP5TxtIdLang16, L"Language 16" },
	{ E_GrP5TxtIdLang17, L"Language 17" },
	{ E_GrP5TxtIdLang18, L"Language 18" },
	{ E_GrP5TxtIdLang19, L"Language 19" },
	{ E_GrP5TxtIdLang20, L"Language 20" },
	{ E_GrP5TxtIdLang21, L"Language 21" },
	{ E_GrP5TxtIdLang22, L"Language 22" },
	{ E_GrP5TxtIdLang23, L"Language 23" },
	{ E_GrP5TxtIdLang24, L"Language 24" },
	{ E_GrP5TxtIdLang25, L"Language 25" },
	{ E_GrP5TxtIdLang26, L"Language 26" },
	{ E_GrP5TxtIdLang27, L"Language 27" },
	{ E_GrP5TxtIdLang28, L"Language 28" },
	{ E_GrP5TxtIdLang29, L"Language 29" },
	{ E_GrP5TxtIdLang30, L"Language 30" },
	{ E_GrP5TxtIdLang31, L"Language 31" },

	{ E_GrP5TxtIdUnitYear, L"Year" },
	{ E_GrP5TxtIdUnitMonth, L"Month" },
	{ E_GrP5TxtIdUnitDay, L"Day" },
	{ E_GrP5TxtIdUnitHour, L"Hour" },
	{ E_GrP5TxtIdUnitMin, L"Minute" },
	{ E_GrP5TxtIdUnitSec, L"Second" },

	{ E_GrP5TxtIdTimeAm, L"AM" },
	{ E_GrP5TxtIdTimePm, L"PM" },

	{ E_GrP5TxtIdMonth1, L"January" },
	{ E_GrP5TxtIdMonth2, L"February" },
	{ E_GrP5TxtIdMonth3, L"March" },
	{ E_GrP5TxtIdMonth4, L"April" },
	{ E_GrP5TxtIdMonth5, L"May" },
	{ E_GrP5TxtIdMonth6, L"June" },
	{ E_GrP5TxtIdMonth7, L"July" },
	{ E_GrP5TxtIdMonth8, L"August" },
	{ E_GrP5TxtIdMonth9, L"September" },
	{ E_GrP5TxtIdMonth10, L"October" },
	{ E_GrP5TxtIdMonth11, L"November" },
	{ E_GrP5TxtIdMonth12, L"December" },

	{ E_GrP5TxtIdWeekSun, L"Sunday" },
	{ E_GrP5TxtIdWeekMon, L"Monday" },
	{ E_GrP5TxtIdWeekTue, L"Tuesday" },
	{ E_GrP5TxtIdWeekWed, L"Wednesday" },
	{ E_GrP5TxtIdWeekThu, L"Thursday" },
	{ E_GrP5TxtIdWeekFri, L"Friday" },
	{ E_GrP5TxtIdWeekSat, L"Saturday" },

	{ E_GrP5TxtIdChannel, L"Channel" },
	{ E_GrP5TxtIdBegining, L"Begining" },
	{ E_GrP5TxtIdEndining, L"Ending" },
	{ E_GrP5TxtIdResolution, L"Resolution" },
	{E_GrP5TxtIdLgtMotion, L"LOG-Motion"},
	{ E_GrP5TxtIdVideoQuility, L"Video quality" },
	{ E_GrP5TxtIdRecSpeed, L"Record speed" },
	{E_GrP5TxtIdLgtSensor, L"LOG-Sensor"},
	{E_GrP5TxtIdLgtVdoloss, L"LOG-VdoLoss"},
	{ E_GrP5TxtIdMotion, L"Motion" },
	{ E_GrP5TxtIdSensor, L"Sensor" },
	{ E_GrP5TxtIdAlarm, L"Alarm" },
	{E_GrP5TxtIdLgtVdoRcvy, L"LOG-VdoRcvy"},
	{E_GrP5TxtIdLgtPowerOn, L"LOG-PowerOn"},
	{ E_GrP5TxtIdAudio, L"Audio" },
	{E_GrP5TxtIdLgtPowerOff, L"LOG-PowerOff"},
	{ E_GrP5TxtIdPassword, L"Password" },
	{ E_GrP5TxtIdCamera, L"Camera" },
	{E_GrP5TxtIdLgtLogin, L"LOG-Login"},
	{ E_GrP5TxtIdNetwork, L"Network" },
	{ E_GrP5TxtIdLive, L"Live" },
	{ E_GrP5TxtIdSearch, L"Search" },
	{ E_GrP5TxtIdBackup, L"Backup" },
	{ E_GrP5TxtIdSetup, L"Setup" },
	{E_GrP5TxtIdLgtLogout, L"LOG-Logout"},
	{E_GrP5TxtIdLgtSetupIn, L"LOG-SetupIn"},
	{E_GrP5TxtIdLgtSetupOut, L"LOG-SetupOut"},
	{E_GrP5TxtIdLgtBkupSt, L"LOG-BkupSt"},
	{ E_GrP5TxtIdLogIn, L"Login" },
	{ E_GrP5TxtIdLogOut, L"Logout" },
	{E_GrP5TxtIdLgtBkupEd, L"LOG-BkupEd"},
	{ E_GrP5TxtIdUsb, L"USB" },
	{ E_GrP5TxtIdOdd, L"CD/DVD" },
	{ E_GrP5TxtIdEvent, L"Event" },
	{E_GrP5TxtIdLgtHddErr, L"LOG-HddErr"},
	{ E_GrP5TxtIdMacaddress, L"MAC address" },
	{ E_GrP5TxtIdIp, L"IP" },
	{ E_GrP5TxtIdPort, L"Port" },
	{ E_GrP5TxtIdSensitivity, L"Sensitivity" },
	{E_GrP5TxtIdLgtHddFull, L"LOG-HddFull"},
	{E_GrP5TxtIdLgtHddFormat, L"LOG-HddFormat"},
	{E_GrP5TxtIdLgtFsFix, L"LOG-FsysFix"},
	{ E_GrP5TxtIdSystem, L"System" },
	{E_GrP5TxtIdLgtFwUpgd, L"LOG-FwUpgrade"},
	{ E_GrP5TxtIdDate, L"Date" },
	{ E_GrP5TxtIdTime, L"Time" },
	{E_GrP5TxtIdLgtRtcErr, L"LOG-RtcErr"},
	{E_GrP5TxtIdLgtNetLogin, L"LOG-NetLogin"},
	{ E_GrP5TxtIdId, L"ID" },
	{E_GrP5TxtIdLgtNetDiscon, L"LOG-NetDiscon"},
	{ E_GrP5TxtIdEmail, L"E-Mail" },
	{ E_GrP5TxtIdDdns, L"DDNS" },
	{E_GrP5TxtIdLgtNetSetup, L"LOG-NetSetup"},
	{E_GrP5TxtIdLgtEmgcSt, L"LOG-EmgcStart"},
	{E_GrP5TxtIdLgtEmgyEd, L"LOG-EmgyEnd"},
	{E_GrP5TxtIdLgtCfgRecPara, L"LOG-CfgRecPara"},
	{E_GrP5TxtIdLgtCfgMotion, L"LOG-CfgMotion"},
	{E_GrP5TxtIdLgtCfgIdPass, L"LOG-CfgAccount"},
	{E_GrP5TxtIdLgtCfgRecSchd, L"LOG-CfgRecSchedule"},
	{E_GrP5TxtIdLgtCfgHdd, L"LOG-CfgHdd"},
	{E_GrP5TxtIdMailTime, L"MAIL-Time"},
	{E_GrP5TxtIdMailEvtTitle, L"MAIL-EvtTitle"},
	{E_GrP5TxtIdMailMotion, L"MAIL-Motion"},
	{ E_GrP5TxtIdFormat, L"Format" },
	{ E_GrP5TxtIdEnter, L"Enter" },
	{E_GrP5TxtIdMailSensor, L"MAIL-Sensor"},
	{E_GrP5TxtIdMailVloss, L"MAIL-VdoLoss"},
	{E_GrP5TxtIdMailLogin, L"MAIL-Login"},
	{ E_GrP5TxtIdUpgrade, L"Upgrade" },
	{E_GrP5TxtIdMailRecErr, L"MAIL-RecError"},
	{E_GrP5TxtIdMailDskFull, L"MAIL-DiskFull"},
	{ E_GrP5TxtIdStop, L"Stop" },
	{ E_GrP5TxtIdPlay, L"Play" },
	{E_GrP5TxtIdMailDvrErr, L"MAIL-DvrError"},
	{E_GrP5TxtIdMailMacAdr, L"MAIL-MacAddr"},
	{ E_GrP5TxtIdFastForward, L"Fast forward" },
	{ E_GrP5TxtIdFastRewind, L"Fast rewind" },
	{E_GrP5TxtIdLgtCfgChg, L"LOG-CfgChange"},
	{E_GrP5TxtIdLgtReboot, L"LOG-Reboot"},
	{E_GrP5TxtIdLgtChgTime, L"LOG-ChgTime"},

	{E_GrP5TxtIdHighest, L"Highest"},
	{E_GrP5TxtIdHigh, L"High"},
	{E_GrP5TxtIdNormal, L"Normal"},
	{E_GrP5TxtIdLow, L"Low"},

	{E_GrP5TxtIdCh1, L"Ch 1"},
	{E_GrP5TxtIdCh2, L"Ch 2"},
	{E_GrP5TxtIdCh3, L"Ch 3"},
	{E_GrP5TxtIdCh4, L"Ch 4"},
	{E_GrP5TxtIdCh5, L"Ch 5"},
	{E_GrP5TxtIdCh6, L"Ch 6"},
	{E_GrP5TxtIdCh7, L"Ch 7"},
	{E_GrP5TxtIdCh8, L"Ch 8"},
	{E_GrP5TxtIdCh9, L"Ch 9"},
	{E_GrP5TxtIdCh10, L"Ch 10"},
	{E_GrP5TxtIdCh11, L"Ch 11"},
	{E_GrP5TxtIdCh12, L"Ch 12"},
	{E_GrP5TxtIdCh13, L"Ch 13"},
	{E_GrP5TxtIdCh14, L"Ch 14"},
	{E_GrP5TxtIdCh15, L"Ch 15"},
	{E_GrP5TxtIdCh16, L"Ch 16"},
	{E_GrP5TxtIdCh17, L"Ch 17"},
	{E_GrP5TxtIdCh18, L"Ch 18"},
	{E_GrP5TxtIdCh19, L"Ch 19"},
	{E_GrP5TxtIdCh20, L"Ch 20"},
	{E_GrP5TxtIdCh21, L"Ch 21"},
	{E_GrP5TxtIdCh22, L"Ch 22"},
	{E_GrP5TxtIdCh23, L"Ch 23"},
	{E_GrP5TxtIdCh24, L"Ch 24"},
	{E_GrP5TxtIdCh25, L"Ch 25"},
	{E_GrP5TxtIdCh26, L"Ch 26"},
	{E_GrP5TxtIdCh27, L"Ch 27"},
	{E_GrP5TxtIdCh28, L"Ch 28"},
	{E_GrP5TxtIdCh29, L"Ch 29"},
	{E_GrP5TxtIdCh30, L"Ch 30"},
	{E_GrP5TxtIdCh31, L"Ch 31"},
	{E_GrP5TxtIdCh32, L"Ch 32"},

	{E_GrP5TxtIdCfgInit, L"LOG-CfgReset"},
	{E_GrP5TxtIdCfgPtz, L"LOG-CfgPtz"},
	{E_GrP5TxtIdCfgNet, L"LOG-CfgNet"},
	{E_GrP5TxtIdCfgCamSeq, L"LOG-CfgCamSeq"},
	{E_GrP5TxtIdCfgSpot, L"LOG-CfgSpot"},
	{E_GrP5TxtIdCfgLoad, L"LOG-CfgLoad"},
	{E_GrP5TxtIdCfgSave, L"LOG-CfgSave"},
	{E_GrP5TxtIdCfgNetFrm, L"LOG-CfgNetFrm"},
	{E_GrP5TxtIdCfgDispRes, L"LOG-CfgDispRes"},
	{E_GrP5TxtIdMailSendErr, L"LOG-MailErr"},
	{E_GrP5TxtIdWdogVdo, L"LOG-WdogVdo"},
	{E_GrP5TxtIdWdogAdo, L"LOG-WdogAdo"},
	{E_GrP5TxtILogSpcSen, L"LOG-SpcSen"},
	{E_GrP5TxtILogSoftErr, L"LOG-SoftErr"},
	{E_GrP5TxtILogFsUpgrade, L"LOG-FilesysUp"},
	{E_GrP5TxtIdHddTemp, L"LOG-HddTemp"},
};
#endif

//====================================================================
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
WCHAR*	GrP5TextFixNameGet(__u16 A_TxtId)
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16		Tv_TxtIdx;
	// code --------------------
		Tv_Result	=	NULL;

		if ((E_GrP5TxtIdFixBase <= A_TxtId) && ((E_GrP5TxtIdFixBase + E_GrP5TxtDedicatedCnt) > A_TxtId))
		{
			for ( Tv_TxtIdx = 0; Tv_TxtIdx < E_GrP5TxtDedicatedCnt; Tv_TxtIdx++)
			{
				if (A_TxtId == V_GrP5TextFixInfo[Tv_TxtIdx].Id)
				{
					Tv_Result	=	V_GrP5TextFixInfo[Tv_TxtIdx].StrName;
					break;
				}
			}
		}

		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------



