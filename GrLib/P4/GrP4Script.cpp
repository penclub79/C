/*
	GAUSE Platfrom Script

*/

#include	<P4/GrP4Script.h>

#ifdef GR_P4_EDITOR
#include	<P4/Dvl/GrP4DvlBase.h>
#include	<P4/Dvl/GrP4DvlMng.h>
#endif

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------

#ifdef GR_P4_EDITOR

St_GrP4DvlDoPrInfoData	V_GrP4ScptMdlAtbClstTbl[E_GrP4ScptMdlAtbClstCnt]	=	
{
	{	L"AsmInit",				TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmInit,			NULL,	L"ASM code - system initialization."	},
	{	L"AsmStart",			TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmStart,		NULL,	L"ASM code - When initialize finished."	},
	{	L"AsmFormat",			TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmFormat,		NULL,	L"ASM code - When format finished. ireg0: success, ireg1: hdd index"	},
	{	L"AsmLivePlay",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmLivePlay,	NULL,	L"ASM code - When mode change live or play"	},
	{	L"AsmCfgReset",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmCfgReset,	NULL,	L"ASM code - CFG initialize, ireg1: 0=normal, 1=factory"	},
	{	L"AsmCfgPatch",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmCfgPatch,	NULL,	L"ASM code - you can modify CFG before configuration assign"	},
	{	L"AsmFwUpDone",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmFwUpDone,	NULL,	L"ASM code - When finish firmware upgrade"	},
	{	L"AsmDenEmgc",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmDenEmgc,	NULL,	L"ASM code - Emergency denial"	},
	{	L"AsmWinPriv",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmWinPriv,	NULL,	L"ASM code - window can not create,because have not privilege"	},
	{	L"AsmLanOnOff",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmLanOnOff,	NULL,	L"ASM code - LAN cable status change"	},
	{	L"AsmPlayStat",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmPlayStat,	NULL,	L"ASM code - play status change"	},
	{	L"AsmUsbChg",			TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmUsbChg,		NULL,	L"ASM code - USB memory count change"	},
	{	L"AsmCmsChg",			TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmCmsChg,		NULL,	L"ASM code - CMS client count change"	},
	{	L"AsmQuad0Chg",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmQuad0Chg,	NULL,	L"ASM code - console 0 QUAD mode change"	},
	{	L"AsmQuad1Chg",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmQuad1Chg,	NULL,	L"ASM code - console 1 QUAD mode change"	},
	{	L"AsmBkupStart",	TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmBkupStart,	NULL,	L"ASM code - backup start"	},
	{	L"AsmBkupEnd",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmBkupEnd,	NULL,	L"ASM code - backup end"	},
	{	L"AsmBkupErr",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmBkupErr,	NULL,	L"ASM code - backup error"	},
	{	L"AsmErrOcu",			TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmErrOcu,		NULL,	L"ASM code - error occur"	},
	{	L"AsmUserChg",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmUserChg,	NULL,	L"ASM code - user login status change"	},
	{	L"AsmNtpTest",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmNtpTest,	NULL,	L"ASM code - NTP test finish"	},
	{	L"AsmLogStart",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmLogStart,	NULL,	L"ASM code - log backup start"	},
	{	L"AsmLogEnd",			TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmLogEnd,		NULL,	L"ASM code - log backup end"	},
	{	L"AsmLogErr",			TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmLogErr,		NULL,	L"ASM code - log backup error"	},
	{	L"AsmNetUpgd",		TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->AsmNetUpgd,		NULL,	L"ASM code - network upgrade ready"	},

	{	L"VsstType",			TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VsstType,		NULL,	L"VSST type - 0 = none"	},
	{	L"IsIoBox",				TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->IsIoBox,			NULL,	L"set external I/O box can use"	},
	{	L"WinVkbdTxt",		TRUE,		E_GrP4DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->TxtVkbdId,		NULL,	L"text virtual keyboard window id"	},
	{	L"WinVkbdNum",		TRUE,		E_GrP4DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->NumVkbdId,		NULL,	L"number virtual keyboard window id"	},
	{	L"NotifyId",			TRUE,		E_GrP4DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->NotifyId,		NULL,	L"default notify window id"	},

	{	L"LongKeyTime",		TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->LongKeyTime,	NULL,	L"time of key long push"	},
	{	L"MaxHddCnt",			TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->MaxHddCnt,		NULL,	L"Max hard disk count"	},
	{	L"IsPassChk",			TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->IsPassChk,		NULL,	L"advance password rule check"	},
	{	L"PassRuleTitle",	TRUE,		E_GrP4DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->PassRuleTitle,	NULL,	L"text id of bad rule password notify title"	},
	{	L"PassRuleCont",	TRUE,		E_GrP4DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->PassRuleCont,	NULL,	L"text id of bad rule password notify contents"	},
	{	L"UserIdCnt",			TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->UserIdCnt,		NULL,	L"user id count, max = 8"	},
	{	L"IsPassDual",		TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->IsPassDual,	NULL,	L"dual password mode"	},
	{	L"MasterPass",		TRUE,		E_GrP4DvlParaTypeWchar,				16,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->MasterPass,	NULL,	L"Master password"	},
	{	L"IsEmgcLock",		TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->IsEmgcLock,	NULL,	L"Emergency lock usable"	},
	{	L"IsMtnSchd",			TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->IsMtnSchd,		NULL,	L"motion schedule mode"	},
	{	L"IsLogRec",			TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->IsLogRec,		NULL,	L"record relate log only"	},
	{	L"IsCvtUsr",			TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->IsCvtUsr,		NULL,	L"user covert active"	},
	{	L"IsDmaxFront",		TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->IsDmaxFront,	NULL,	L"D-MAX front use"	},

	{	L"RqR0S0Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][0][0],	NULL,	L"record quality res 0 scale 0 lowest"	},
	{	L"RqR0S0Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][0][1],	NULL,	L"record quality res 0 scale 0 low"	},
	{	L"RqR0S0Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][0][2],	NULL,	L"record quality res 0 scale 0 middle"	},
	{	L"RqR0S0High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][0][3],	NULL,	L"record quality res 0 scale 0 high"	},
	{	L"RqR0S0Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][0][4],	NULL,	L"record quality res 0 scale 0 highest"	},

	{	L"RqR0S1Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][1][0],	NULL,	L"record quality res 0 scale 1 lowest"	},
	{	L"RqR0S1Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][1][1],	NULL,	L"record quality res 0 scale 1 low"	},
	{	L"RqR0S1Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][1][2],	NULL,	L"record quality res 0 scale 1 middle"	},
	{	L"RqR0S1High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][1][3],	NULL,	L"record quality res 0 scale 1 high"	},
	{	L"RqR0S1Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][1][4],	NULL,	L"record quality res 0 scale 1 highest"	},

	{	L"RqR0S2Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][2][0],	NULL,	L"record quality res 0 scale 2 lowest"	},
	{	L"RqR0S2Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][2][1],	NULL,	L"record quality res 0 scale 2 low"	},
	{	L"RqR0S2Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][2][2],	NULL,	L"record quality res 0 scale 2 middle"	},
	{	L"RqR0S2High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][2][3],	NULL,	L"record quality res 0 scale 2 high"	},
	{	L"RqR0S2Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][2][4],	NULL,	L"record quality res 0 scale 2 highest"	},

	{	L"RqR0S3Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][3][0],	NULL,	L"record quality res 0 scale 3 lowest"	},
	{	L"RqR0S3Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][3][1],	NULL,	L"record quality res 0 scale 3 low"	},
	{	L"RqR0S3Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][3][2],	NULL,	L"record quality res 0 scale 3 middle"	},
	{	L"RqR0S3High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][3][3],	NULL,	L"record quality res 0 scale 3 high"	},
	{	L"RqR0S3Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[0][3][4],	NULL,	L"record quality res 0 scale 3 highest"	},

	{	L"RqR1S0Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][0][0],	NULL,	L"record quality res 1 scale 0 lowest"	},
	{	L"RqR1S0Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][0][1],	NULL,	L"record quality res 1 scale 0 low"	},
	{	L"RqR1S0Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][0][2],	NULL,	L"record quality res 1 scale 0 middle"	},
	{	L"RqR1S0High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][0][3],	NULL,	L"record quality res 1 scale 0 high"	},
	{	L"RqR1S0Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][0][4],	NULL,	L"record quality res 1 scale 0 highest"	},

	{	L"RqR1S1Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][1][0],	NULL,	L"record quality res 1 scale 1 lowest"	},
	{	L"RqR1S1Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][1][1],	NULL,	L"record quality res 1 scale 1 low"	},
	{	L"RqR1S1Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][1][2],	NULL,	L"record quality res 1 scale 1 middle"	},
	{	L"RqR1S1High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][1][3],	NULL,	L"record quality res 1 scale 1 high"	},
	{	L"RqR1S1Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][1][4],	NULL,	L"record quality res 1 scale 1 highest"	},

	{	L"RqR1S2Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][2][0],	NULL,	L"record quality res 1 scale 2 lowest"	},
	{	L"RqR1S2Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][2][1],	NULL,	L"record quality res 1 scale 2 low"	},
	{	L"RqR1S2Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][2][2],	NULL,	L"record quality res 1 scale 2 middle"	},
	{	L"RqR1S2High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][2][3],	NULL,	L"record quality res 1 scale 2 high"	},
	{	L"RqR1S2Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][2][4],	NULL,	L"record quality res 1 scale 2 highest"	},

	{	L"RqR1S3Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][3][0],	NULL,	L"record quality res 1 scale 3 lowest"	},
	{	L"RqR1S3Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][3][1],	NULL,	L"record quality res 1 scale 3 low"	},
	{	L"RqR1S3Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][3][2],	NULL,	L"record quality res 1 scale 3 middle"	},
	{	L"RqR1S3High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][3][3],	NULL,	L"record quality res 1 scale 3 high"	},
	{	L"RqR1S3Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[1][3][4],	NULL,	L"record quality res 1 scale 3 highest"	},

	{	L"RqR2S0Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][0][0],	NULL,	L"record quality res 2 scale 0 lowest"	},
	{	L"RqR2S0Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][0][1],	NULL,	L"record quality res 2 scale 0 low"	},
	{	L"RqR2S0Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][0][2],	NULL,	L"record quality res 2 scale 0 middle"	},
	{	L"RqR2S0High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][0][3],	NULL,	L"record quality res 2 scale 0 high"	},
	{	L"RqR2S0Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][0][4],	NULL,	L"record quality res 2 scale 0 highest"	},

	{	L"RqR2S1Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][1][0],	NULL,	L"record quality res 2 scale 1 lowest"	},
	{	L"RqR2S1Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][1][1],	NULL,	L"record quality res 2 scale 1 low"	},
	{	L"RqR2S1Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][1][2],	NULL,	L"record quality res 2 scale 1 middle"	},
	{	L"RqR2S1High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][1][3],	NULL,	L"record quality res 2 scale 1 high"	},
	{	L"RqR2S1Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][1][4],	NULL,	L"record quality res 2 scale 1 highest"	},

	{	L"RqR2S2Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][2][0],	NULL,	L"record quality res 2 scale 2 lowest"	},
	{	L"RqR2S2Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][2][1],	NULL,	L"record quality res 2 scale 2 low"	},
	{	L"RqR2S2Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][2][2],	NULL,	L"record quality res 2 scale 2 middle"	},
	{	L"RqR2S2High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][2][3],	NULL,	L"record quality res 2 scale 2 high"	},
	{	L"RqR2S2Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][2][4],	NULL,	L"record quality res 2 scale 2 highest"	},

	{	L"RqR2S3Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][3][0],	NULL,	L"record quality res 2 scale 3 lowest"	},
	{	L"RqR2S3Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][3][1],	NULL,	L"record quality res 2 scale 3 low"	},
	{	L"RqR2S3Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][3][2],	NULL,	L"record quality res 2 scale 3 middle"	},
	{	L"RqR2S3High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][3][3],	NULL,	L"record quality res 2 scale 3 high"	},
	{	L"RqR2S3Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[2][3][4],	NULL,	L"record quality res 2 scale 3 highest"	},

	{	L"RqR3S0Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][0][0],	NULL,	L"record quality res 3 scale 0 lowest"	},
	{	L"RqR3S0Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][0][1],	NULL,	L"record quality res 3 scale 0 low"	},
	{	L"RqR3S0Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][0][2],	NULL,	L"record quality res 3 scale 0 middle"	},
	{	L"RqR3S0High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][0][3],	NULL,	L"record quality res 3 scale 0 high"	},
	{	L"RqR3S0Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][0][4],	NULL,	L"record quality res 3 scale 0 highest"	},

	{	L"RqR3S1Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][1][0],	NULL,	L"record quality res 3 scale 1 lowest"	},
	{	L"RqR3S1Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][1][1],	NULL,	L"record quality res 3 scale 1 low"	},
	{	L"RqR3S1Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][1][2],	NULL,	L"record quality res 3 scale 1 middle"	},
	{	L"RqR3S1High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][1][3],	NULL,	L"record quality res 3 scale 1 high"	},
	{	L"RqR3S1Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][1][4],	NULL,	L"record quality res 3 scale 1 highest"	},

	{	L"RqR3S2Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][2][0],	NULL,	L"record quality res 3 scale 2 lowest"	},
	{	L"RqR3S2Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][2][1],	NULL,	L"record quality res 3 scale 2 low"	},
	{	L"RqR3S2Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][2][2],	NULL,	L"record quality res 3 scale 2 middle"	},
	{	L"RqR3S2High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][2][3],	NULL,	L"record quality res 3 scale 2 high"	},
	{	L"RqR3S2Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][2][4],	NULL,	L"record quality res 3 scale 2 highest"	},

	{	L"RqR3S3Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][3][0],	NULL,	L"record quality res 3 scale 3 lowest"	},
	{	L"RqR3S3Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][3][1],	NULL,	L"record quality res 3 scale 3 low"	},
	{	L"RqR3S3Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][3][2],	NULL,	L"record quality res 3 scale 3 middle"	},
	{	L"RqR3S3High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][3][3],	NULL,	L"record quality res 3 scale 3 high"	},
	{	L"RqR3S3Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VrecQt[3][3][4],	NULL,	L"record quality res 3 scale 3 highest"	},


	{	L"NqR0S0Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][0][0],	NULL,	L"network quality res 0 scale 0 lowest"	},
	{	L"NqR0S0Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][0][1],	NULL,	L"network quality res 0 scale 0 low"	},
	{	L"NqR0S0Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][0][2],	NULL,	L"network quality res 0 scale 0 middle"	},
	{	L"NqR0S0High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][0][3],	NULL,	L"network quality res 0 scale 0 high"	},
	{	L"NqR0S0Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][0][4],	NULL,	L"network quality res 0 scale 0 highest"	},

	{	L"NqR0S1Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][1][0],	NULL,	L"network quality res 0 scale 1 lowest"	},
	{	L"NqR0S1Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][1][1],	NULL,	L"network quality res 0 scale 1 low"	},
	{	L"NqR0S1Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][1][2],	NULL,	L"network quality res 0 scale 1 middle"	},
	{	L"NqR0S1High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][1][3],	NULL,	L"network quality res 0 scale 1 high"	},
	{	L"NqR0S1Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][1][4],	NULL,	L"network quality res 0 scale 1 highest"	},

	{	L"NqR0S2Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][2][0],	NULL,	L"network quality res 0 scale 2 lowest"	},
	{	L"NqR0S2Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][2][1],	NULL,	L"network quality res 0 scale 2 low"	},
	{	L"NqR0S2Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][2][2],	NULL,	L"network quality res 0 scale 2 middle"	},
	{	L"NqR0S2High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][2][3],	NULL,	L"network quality res 0 scale 2 high"	},
	{	L"NqR0S2Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][2][4],	NULL,	L"network quality res 0 scale 2 highest"	},

	{	L"NqR0S3Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][3][0],	NULL,	L"network quality res 0 scale 3 lowest"	},
	{	L"NqR0S3Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][3][1],	NULL,	L"network quality res 0 scale 3 low"	},
	{	L"NqR0S3Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][3][2],	NULL,	L"network quality res 0 scale 3 middle"	},
	{	L"NqR0S3High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][3][3],	NULL,	L"network quality res 0 scale 3 high"	},
	{	L"NqR0S3Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[0][3][4],	NULL,	L"network quality res 0 scale 3 highest"	},

	{	L"NqR1S0Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][0][0],	NULL,	L"network quality res 1 scale 0 lowest"	},
	{	L"NqR1S0Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][0][1],	NULL,	L"network quality res 1 scale 0 low"	},
	{	L"NqR1S0Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][0][2],	NULL,	L"network quality res 1 scale 0 middle"	},
	{	L"NqR1S0High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][0][3],	NULL,	L"network quality res 1 scale 0 high"	},
	{	L"NqR1S0Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][0][4],	NULL,	L"network quality res 1 scale 0 highest"	},

	{	L"NqR1S1Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][1][0],	NULL,	L"network quality res 1 scale 1 lowest"	},
	{	L"NqR1S1Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][1][1],	NULL,	L"network quality res 1 scale 1 low"	},
	{	L"NqR1S1Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][1][2],	NULL,	L"network quality res 1 scale 1 middle"	},
	{	L"NqR1S1High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][1][3],	NULL,	L"network quality res 1 scale 1 high"	},
	{	L"NqR1S1Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][1][4],	NULL,	L"network quality res 1 scale 1 highest"	},

	{	L"NqR1S2Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][2][0],	NULL,	L"network quality res 1 scale 2 lowest"	},
	{	L"NqR1S2Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][2][1],	NULL,	L"network quality res 1 scale 2 low"	},
	{	L"NqR1S2Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][2][2],	NULL,	L"network quality res 1 scale 2 middle"	},
	{	L"NqR1S2High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][2][3],	NULL,	L"network quality res 1 scale 2 high"	},
	{	L"NqR1S2Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][2][4],	NULL,	L"network quality res 1 scale 2 highest"	},

	{	L"NqR1S3Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][3][0],	NULL,	L"network quality res 1 scale 3 lowest"	},
	{	L"NqR1S3Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][3][1],	NULL,	L"network quality res 1 scale 3 low"	},
	{	L"NqR1S3Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][3][2],	NULL,	L"network quality res 1 scale 3 middle"	},
	{	L"NqR1S3High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][3][3],	NULL,	L"network quality res 1 scale 3 high"	},
	{	L"NqR1S3Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[1][3][4],	NULL,	L"network quality res 1 scale 3 highest"	},

	{	L"NqR2S0Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][0][0],	NULL,	L"network quality res 2 scale 0 lowest"	},
	{	L"NqR2S0Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][0][1],	NULL,	L"network quality res 2 scale 0 low"	},
	{	L"NqR2S0Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][0][2],	NULL,	L"network quality res 2 scale 0 middle"	},
	{	L"NqR2S0High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][0][3],	NULL,	L"network quality res 2 scale 0 high"	},
	{	L"NqR2S0Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][0][4],	NULL,	L"network quality res 2 scale 0 highest"	},

	{	L"NqR2S1Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][1][0],	NULL,	L"network quality res 2 scale 1 lowest"	},
	{	L"NqR2S1Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][1][1],	NULL,	L"network quality res 2 scale 1 low"	},
	{	L"NqR2S1Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][1][2],	NULL,	L"network quality res 2 scale 1 middle"	},
	{	L"NqR2S1High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][1][3],	NULL,	L"network quality res 2 scale 1 high"	},
	{	L"NqR2S1Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][1][4],	NULL,	L"network quality res 2 scale 1 highest"	},

	{	L"NqR2S2Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][2][0],	NULL,	L"network quality res 2 scale 2 lowest"	},
	{	L"NqR2S2Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][2][1],	NULL,	L"network quality res 2 scale 2 low"	},
	{	L"NqR2S2Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][2][2],	NULL,	L"network quality res 2 scale 2 middle"	},
	{	L"NqR2S2High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][2][3],	NULL,	L"network quality res 2 scale 2 high"	},
	{	L"NqR2S2Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][2][4],	NULL,	L"network quality res 2 scale 2 highest"	},

	{	L"NqR2S3Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][3][0],	NULL,	L"network quality res 2 scale 3 lowest"	},
	{	L"NqR2S3Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][3][1],	NULL,	L"network quality res 2 scale 3 low"	},
	{	L"NqR2S3Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][3][2],	NULL,	L"network quality res 2 scale 3 middle"	},
	{	L"NqR2S3High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][3][3],	NULL,	L"network quality res 2 scale 3 high"	},
	{	L"NqR2S3Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[2][3][4],	NULL,	L"network quality res 2 scale 3 highest"	},

	{	L"NqR3S0Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][0][0],	NULL,	L"network quality res 3 scale 0 lowest"	},
	{	L"NqR3S0Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][0][1],	NULL,	L"network quality res 3 scale 0 low"	},
	{	L"NqR3S0Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][0][2],	NULL,	L"network quality res 3 scale 0 middle"	},
	{	L"NqR3S0High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][0][3],	NULL,	L"network quality res 3 scale 0 high"	},
	{	L"NqR3S0Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][0][4],	NULL,	L"network quality res 3 scale 0 highest"	},

	{	L"NqR3S1Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][1][0],	NULL,	L"network quality res 3 scale 1 lowest"	},
	{	L"NqR3S1Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][1][1],	NULL,	L"network quality res 3 scale 1 low"	},
	{	L"NqR3S1Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][1][2],	NULL,	L"network quality res 3 scale 1 middle"	},
	{	L"NqR3S1High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][1][3],	NULL,	L"network quality res 3 scale 1 high"	},
	{	L"NqR3S1Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][1][4],	NULL,	L"network quality res 3 scale 1 highest"	},

	{	L"NqR3S2Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][2][0],	NULL,	L"network quality res 3 scale 2 lowest"	},
	{	L"NqR3S2Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][2][1],	NULL,	L"network quality res 3 scale 2 low"	},
	{	L"NqR3S2Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][2][2],	NULL,	L"network quality res 3 scale 2 middle"	},
	{	L"NqR3S2High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][2][3],	NULL,	L"network quality res 3 scale 2 high"	},
	{	L"NqR3S2Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][2][4],	NULL,	L"network quality res 3 scale 2 highest"	},

	{	L"NqR3S3Lowest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][3][0],	NULL,	L"network quality res 3 scale 3 lowest"	},
	{	L"NqR3S3Low",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][3][1],	NULL,	L"network quality res 3 scale 3 low"	},
	{	L"NqR3S3Middle",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][3][2],	NULL,	L"network quality res 3 scale 3 middle"	},
	{	L"NqR3S3High",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][3][3],	NULL,	L"network quality res 3 scale 3 high"	},
	{	L"NqR3S3Highest",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->VnetQt[3][3][4],	NULL,	L"network quality res 3 scale 3 highest"	},

	{	L"WebLogo",				TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4ScptCfgMdl)0)->WebLogo,				NULL,	L"built in web logo index"	},
};

#endif
