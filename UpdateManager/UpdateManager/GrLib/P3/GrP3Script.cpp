/*
	GAUSE Platfrom Script

*/

#include	<P3/GrP3Script.h>

#ifdef GR_P3_EDITOR
#include	<P3/Dvl/GrP3DvlBase.h>
#include	<P3/Dvl/GrP3DvlMng.h>
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

#ifdef GR_P3_EDITOR

St_GrP3DvlDoPrInfoData	V_GrP3ScptMdlAtbClstTbl[E_GrP3ScptMdlAtbClstCnt]	=	
{
	{	L"AsmInit",				TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmInit,			NULL,	L"ASM code - system initialization."	},
	{	L"AsmStart",			TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmStart,		NULL,	L"ASM code - When initialize finished."	},
	{	L"AsmFormat",			TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmFormat,		NULL,	L"ASM code - When format finished. ireg0: success, ireg1: hdd index"	},
	{	L"AsmLivePlay",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmLivePlay,	NULL,	L"ASM code - When mode change live or play"	},
	{	L"AsmCfgReset",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmCfgReset,	NULL,	L"ASM code - CFG initialize, ireg1: 0=normal, 1=factory"	},
	{	L"AsmCfgPatch",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmCfgPatch,	NULL,	L"ASM code - you can modify CFG before configuration assign"	},
	{	L"AsmFwUpDone",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmFwUpDone,	NULL,	L"ASM code - When finish firmware upgrade"	},
	{	L"AsmDenEmgc",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmDenEmgc,	NULL,	L"ASM code - Emergency denial"	},
	{	L"AsmWinPriv",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmWinPriv,	NULL,	L"ASM code - window can not create,because have not privilege"	},
	{	L"AsmLanOnOff",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmLanOnOff,	NULL,	L"ASM code - LAN cable status change"	},
	{	L"AsmPlayStat",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmPlayStat,	NULL,	L"ASM code - play status change"	},
	{	L"AsmUsbChg",			TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmUsbChg,		NULL,	L"ASM code - USB memory count change"	},
	{	L"AsmCmsChg",			TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmCmsChg,		NULL,	L"ASM code - CMS client count change"	},
	{	L"AsmQuad0Chg",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmQuad0Chg,	NULL,	L"ASM code - console 0 QUAD mode change"	},
	{	L"AsmQuad1Chg",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmQuad1Chg,	NULL,	L"ASM code - console 1 QUAD mode change"	},
	{	L"AsmBkupStart",	TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmBkupStart,	NULL,	L"ASM code - backup start"	},
	{	L"AsmBkupEnd",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmBkupEnd,	NULL,	L"ASM code - backup end"	},
	{	L"AsmBkupErr",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmBkupErr,	NULL,	L"ASM code - backup error"	},
	{	L"AsmErrOcu",			TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmErrOcu,		NULL,	L"ASM code - error occur"	},
	{	L"AsmUserChg",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmUserChg,	NULL,	L"ASM code - user login status change"	},
	{	L"AsmNtpTest",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmNtpTest,	NULL,	L"ASM code - NTP test finish"	},
	{	L"AsmLogStart",		TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmLogStart,	NULL,	L"ASM code - log backup start"	},
	{	L"AsmLogEnd",			TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmLogEnd,		NULL,	L"ASM code - log backup end"	},
	{	L"AsmLogErr",			TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmLogErr,		NULL,	L"ASM code - log backup error"	},
	{	L"AsmFwUpStart",	TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->AsmFwUpStart,	NULL,	L"ASM code - net upgrade"	},

	{	L"VsstType",			TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VsstType,		NULL,	L"VSST type - 0 = none"	},
	{	L"IsIoBox",				TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->IsIoBox,			NULL,	L"set external I/O box can use"	},
	{	L"WinVkbdTxt",		TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->TxtVkbdId,		NULL,	L"text virtual keyboard window id"	},
	{	L"WinVkbdNum",		TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->NumVkbdId,		NULL,	L"number virtual keyboard window id"	},
	{	L"Notify0Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->Notify0Id,		NULL,	L"notify window 0 id"	},
	{	L"Notify1Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->Notify1Id,		NULL,	L"notify window 1 id"	},
	{	L"Notify2Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->Notify2Id,		NULL,	L"notify window 2 id"	},
	{	L"Notify3Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->Notify3Id,		NULL,	L"notify window 3 id"	},
	{	L"QryYn0Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->QryYn0Id,		NULL,	L"query yes , no window 0 id"	},
	{	L"QryYn1Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->QryYn1Id,		NULL,	L"query yes , no window 1 id"	},
	{	L"QryYn2Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->QryYn2Id,		NULL,	L"query yes , no window 2 id"	},
	{	L"QryYn3Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->QryYn3Id,		NULL,	L"query yes , no window 3 id"	},
	{	L"QryYnc0Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->QryYnc0Id,		NULL,	L"query yes , no , cancel window 0 id"	},
	{	L"QryYnc1Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->QryYnc1Id,		NULL,	L"query yes , no , cancel window 1 id"	},
	{	L"QryYnc2Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->QryYnc2Id,		NULL,	L"query yes , no , cancel window 2 id"	},
	{	L"QryYnc3Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->QryYnc3Id,		NULL,	L"query yes , no , cancel window 3 id"	},

	{	L"LongKeyTime",		TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->LongKeyTime,	NULL,	L"time of key long push"	},
	{	L"MaxHddCnt",			TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->MaxHddCnt,		NULL,	L"Max hard disk count"	},
	{	L"IsPassChk",			TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->IsPassChk,		NULL,	L"advance password rule check"	},
	{	L"PassRuleTitle",	TRUE,		E_GrP3DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->PassRuleTitle,	NULL,	L"text id of bad rule password notify title"	},
	{	L"PassRuleCont",	TRUE,		E_GrP3DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->PassRuleCont,	NULL,	L"text id of bad rule password notify contents"	},
	{	L"UserIdCnt",			TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->UserIdCnt,		NULL,	L"user id count, max = 8"	},
	{	L"IsPassDual",		TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->IsPassDual,	NULL,	L"dual password mode"	},
	{	L"MasterPass",		TRUE,		E_GrP3DvlParaTypeWhcar,				16,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->MasterPass,	NULL,	L"Master password"	},
	{	L"IsEmgcLock",		TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->IsEmgcLock,	NULL,	L"Emergency lock usable"	},
	{	L"IsMtnSchd",			TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->IsMtnSchd,		NULL,	L"motion schedule mode"	},
	{	L"IsLogRec",			TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->IsLogRec,		NULL,	L"record relate log only"	},
	{	L"IsCvtUsr",			TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->IsCvtUsr,		NULL,	L"user covert active"	},
	{	L"IsDmaxFront",		TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->IsDmaxFront,	NULL,	L"D-MAX front use"	},

	{	L"RqR0S0Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][0][0],	NULL,	L"record quality res 0 scale 0 lowest"	},
	{	L"RqR0S0Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][0][1],	NULL,	L"record quality res 0 scale 0 low"	},
	{	L"RqR0S0Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][0][2],	NULL,	L"record quality res 0 scale 0 middle"	},
	{	L"RqR0S0High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][0][3],	NULL,	L"record quality res 0 scale 0 high"	},
	{	L"RqR0S0Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][0][4],	NULL,	L"record quality res 0 scale 0 highest"	},

	{	L"RqR0S1Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][1][0],	NULL,	L"record quality res 0 scale 1 lowest"	},
	{	L"RqR0S1Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][1][1],	NULL,	L"record quality res 0 scale 1 low"	},
	{	L"RqR0S1Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][1][2],	NULL,	L"record quality res 0 scale 1 middle"	},
	{	L"RqR0S1High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][1][3],	NULL,	L"record quality res 0 scale 1 high"	},
	{	L"RqR0S1Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][1][4],	NULL,	L"record quality res 0 scale 1 highest"	},

	{	L"RqR0S2Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][2][0],	NULL,	L"record quality res 0 scale 2 lowest"	},
	{	L"RqR0S2Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][2][1],	NULL,	L"record quality res 0 scale 2 low"	},
	{	L"RqR0S2Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][2][2],	NULL,	L"record quality res 0 scale 2 middle"	},
	{	L"RqR0S2High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][2][3],	NULL,	L"record quality res 0 scale 2 high"	},
	{	L"RqR0S2Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][2][4],	NULL,	L"record quality res 0 scale 2 highest"	},

	{	L"RqR0S3Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][3][0],	NULL,	L"record quality res 0 scale 3 lowest"	},
	{	L"RqR0S3Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][3][1],	NULL,	L"record quality res 0 scale 3 low"	},
	{	L"RqR0S3Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][3][2],	NULL,	L"record quality res 0 scale 3 middle"	},
	{	L"RqR0S3High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][3][3],	NULL,	L"record quality res 0 scale 3 high"	},
	{	L"RqR0S3Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[0][3][4],	NULL,	L"record quality res 0 scale 3 highest"	},

	{	L"RqR1S0Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][0][0],	NULL,	L"record quality res 1 scale 0 lowest"	},
	{	L"RqR1S0Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][0][1],	NULL,	L"record quality res 1 scale 0 low"	},
	{	L"RqR1S0Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][0][2],	NULL,	L"record quality res 1 scale 0 middle"	},
	{	L"RqR1S0High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][0][3],	NULL,	L"record quality res 1 scale 0 high"	},
	{	L"RqR1S0Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][0][4],	NULL,	L"record quality res 1 scale 0 highest"	},

	{	L"RqR1S1Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][1][0],	NULL,	L"record quality res 1 scale 1 lowest"	},
	{	L"RqR1S1Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][1][1],	NULL,	L"record quality res 1 scale 1 low"	},
	{	L"RqR1S1Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][1][2],	NULL,	L"record quality res 1 scale 1 middle"	},
	{	L"RqR1S1High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][1][3],	NULL,	L"record quality res 1 scale 1 high"	},
	{	L"RqR1S1Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][1][4],	NULL,	L"record quality res 1 scale 1 highest"	},

	{	L"RqR1S2Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][2][0],	NULL,	L"record quality res 1 scale 2 lowest"	},
	{	L"RqR1S2Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][2][1],	NULL,	L"record quality res 1 scale 2 low"	},
	{	L"RqR1S2Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][2][2],	NULL,	L"record quality res 1 scale 2 middle"	},
	{	L"RqR1S2High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][2][3],	NULL,	L"record quality res 1 scale 2 high"	},
	{	L"RqR1S2Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][2][4],	NULL,	L"record quality res 1 scale 2 highest"	},

	{	L"RqR1S3Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][3][0],	NULL,	L"record quality res 1 scale 3 lowest"	},
	{	L"RqR1S3Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][3][1],	NULL,	L"record quality res 1 scale 3 low"	},
	{	L"RqR1S3Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][3][2],	NULL,	L"record quality res 1 scale 3 middle"	},
	{	L"RqR1S3High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][3][3],	NULL,	L"record quality res 1 scale 3 high"	},
	{	L"RqR1S3Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[1][3][4],	NULL,	L"record quality res 1 scale 3 highest"	},

	{	L"RqR2S0Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][0][0],	NULL,	L"record quality res 2 scale 0 lowest"	},
	{	L"RqR2S0Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][0][1],	NULL,	L"record quality res 2 scale 0 low"	},
	{	L"RqR2S0Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][0][2],	NULL,	L"record quality res 2 scale 0 middle"	},
	{	L"RqR2S0High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][0][3],	NULL,	L"record quality res 2 scale 0 high"	},
	{	L"RqR2S0Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][0][4],	NULL,	L"record quality res 2 scale 0 highest"	},

	{	L"RqR2S1Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][1][0],	NULL,	L"record quality res 2 scale 1 lowest"	},
	{	L"RqR2S1Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][1][1],	NULL,	L"record quality res 2 scale 1 low"	},
	{	L"RqR2S1Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][1][2],	NULL,	L"record quality res 2 scale 1 middle"	},
	{	L"RqR2S1High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][1][3],	NULL,	L"record quality res 2 scale 1 high"	},
	{	L"RqR2S1Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][1][4],	NULL,	L"record quality res 2 scale 1 highest"	},

	{	L"RqR2S2Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][2][0],	NULL,	L"record quality res 2 scale 2 lowest"	},
	{	L"RqR2S2Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][2][1],	NULL,	L"record quality res 2 scale 2 low"	},
	{	L"RqR2S2Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][2][2],	NULL,	L"record quality res 2 scale 2 middle"	},
	{	L"RqR2S2High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][2][3],	NULL,	L"record quality res 2 scale 2 high"	},
	{	L"RqR2S2Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][2][4],	NULL,	L"record quality res 2 scale 2 highest"	},

	{	L"RqR2S3Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][3][0],	NULL,	L"record quality res 2 scale 3 lowest"	},
	{	L"RqR2S3Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][3][1],	NULL,	L"record quality res 2 scale 3 low"	},
	{	L"RqR2S3Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][3][2],	NULL,	L"record quality res 2 scale 3 middle"	},
	{	L"RqR2S3High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][3][3],	NULL,	L"record quality res 2 scale 3 high"	},
	{	L"RqR2S3Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[2][3][4],	NULL,	L"record quality res 2 scale 3 highest"	},

	{	L"RqR3S0Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][0][0],	NULL,	L"record quality res 3 scale 0 lowest"	},
	{	L"RqR3S0Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][0][1],	NULL,	L"record quality res 3 scale 0 low"	},
	{	L"RqR3S0Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][0][2],	NULL,	L"record quality res 3 scale 0 middle"	},
	{	L"RqR3S0High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][0][3],	NULL,	L"record quality res 3 scale 0 high"	},
	{	L"RqR3S0Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][0][4],	NULL,	L"record quality res 3 scale 0 highest"	},

	{	L"RqR3S1Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][1][0],	NULL,	L"record quality res 3 scale 1 lowest"	},
	{	L"RqR3S1Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][1][1],	NULL,	L"record quality res 3 scale 1 low"	},
	{	L"RqR3S1Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][1][2],	NULL,	L"record quality res 3 scale 1 middle"	},
	{	L"RqR3S1High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][1][3],	NULL,	L"record quality res 3 scale 1 high"	},
	{	L"RqR3S1Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][1][4],	NULL,	L"record quality res 3 scale 1 highest"	},

	{	L"RqR3S2Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][2][0],	NULL,	L"record quality res 3 scale 2 lowest"	},
	{	L"RqR3S2Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][2][1],	NULL,	L"record quality res 3 scale 2 low"	},
	{	L"RqR3S2Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][2][2],	NULL,	L"record quality res 3 scale 2 middle"	},
	{	L"RqR3S2High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][2][3],	NULL,	L"record quality res 3 scale 2 high"	},
	{	L"RqR3S2Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][2][4],	NULL,	L"record quality res 3 scale 2 highest"	},

	{	L"RqR3S3Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][3][0],	NULL,	L"record quality res 3 scale 3 lowest"	},
	{	L"RqR3S3Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][3][1],	NULL,	L"record quality res 3 scale 3 low"	},
	{	L"RqR3S3Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][3][2],	NULL,	L"record quality res 3 scale 3 middle"	},
	{	L"RqR3S3High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][3][3],	NULL,	L"record quality res 3 scale 3 high"	},
	{	L"RqR3S3Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VrecQt[3][3][4],	NULL,	L"record quality res 3 scale 3 highest"	},


	{	L"NqR0S0Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][0][0],	NULL,	L"network quality res 0 scale 0 lowest"	},
	{	L"NqR0S0Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][0][1],	NULL,	L"network quality res 0 scale 0 low"	},
	{	L"NqR0S0Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][0][2],	NULL,	L"network quality res 0 scale 0 middle"	},
	{	L"NqR0S0High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][0][3],	NULL,	L"network quality res 0 scale 0 high"	},
	{	L"NqR0S0Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][0][4],	NULL,	L"network quality res 0 scale 0 highest"	},

	{	L"NqR0S1Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][1][0],	NULL,	L"network quality res 0 scale 1 lowest"	},
	{	L"NqR0S1Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][1][1],	NULL,	L"network quality res 0 scale 1 low"	},
	{	L"NqR0S1Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][1][2],	NULL,	L"network quality res 0 scale 1 middle"	},
	{	L"NqR0S1High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][1][3],	NULL,	L"network quality res 0 scale 1 high"	},
	{	L"NqR0S1Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][1][4],	NULL,	L"network quality res 0 scale 1 highest"	},

	{	L"NqR0S2Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][2][0],	NULL,	L"network quality res 0 scale 2 lowest"	},
	{	L"NqR0S2Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][2][1],	NULL,	L"network quality res 0 scale 2 low"	},
	{	L"NqR0S2Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][2][2],	NULL,	L"network quality res 0 scale 2 middle"	},
	{	L"NqR0S2High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][2][3],	NULL,	L"network quality res 0 scale 2 high"	},
	{	L"NqR0S2Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][2][4],	NULL,	L"network quality res 0 scale 2 highest"	},

	{	L"NqR0S3Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][3][0],	NULL,	L"network quality res 0 scale 3 lowest"	},
	{	L"NqR0S3Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][3][1],	NULL,	L"network quality res 0 scale 3 low"	},
	{	L"NqR0S3Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][3][2],	NULL,	L"network quality res 0 scale 3 middle"	},
	{	L"NqR0S3High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][3][3],	NULL,	L"network quality res 0 scale 3 high"	},
	{	L"NqR0S3Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[0][3][4],	NULL,	L"network quality res 0 scale 3 highest"	},

	{	L"NqR1S0Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][0][0],	NULL,	L"network quality res 1 scale 0 lowest"	},
	{	L"NqR1S0Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][0][1],	NULL,	L"network quality res 1 scale 0 low"	},
	{	L"NqR1S0Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][0][2],	NULL,	L"network quality res 1 scale 0 middle"	},
	{	L"NqR1S0High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][0][3],	NULL,	L"network quality res 1 scale 0 high"	},
	{	L"NqR1S0Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][0][4],	NULL,	L"network quality res 1 scale 0 highest"	},

	{	L"NqR1S1Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][1][0],	NULL,	L"network quality res 1 scale 1 lowest"	},
	{	L"NqR1S1Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][1][1],	NULL,	L"network quality res 1 scale 1 low"	},
	{	L"NqR1S1Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][1][2],	NULL,	L"network quality res 1 scale 1 middle"	},
	{	L"NqR1S1High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][1][3],	NULL,	L"network quality res 1 scale 1 high"	},
	{	L"NqR1S1Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][1][4],	NULL,	L"network quality res 1 scale 1 highest"	},

	{	L"NqR1S2Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][2][0],	NULL,	L"network quality res 1 scale 2 lowest"	},
	{	L"NqR1S2Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][2][1],	NULL,	L"network quality res 1 scale 2 low"	},
	{	L"NqR1S2Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][2][2],	NULL,	L"network quality res 1 scale 2 middle"	},
	{	L"NqR1S2High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][2][3],	NULL,	L"network quality res 1 scale 2 high"	},
	{	L"NqR1S2Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][2][4],	NULL,	L"network quality res 1 scale 2 highest"	},

	{	L"NqR1S3Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][3][0],	NULL,	L"network quality res 1 scale 3 lowest"	},
	{	L"NqR1S3Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][3][1],	NULL,	L"network quality res 1 scale 3 low"	},
	{	L"NqR1S3Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][3][2],	NULL,	L"network quality res 1 scale 3 middle"	},
	{	L"NqR1S3High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][3][3],	NULL,	L"network quality res 1 scale 3 high"	},
	{	L"NqR1S3Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[1][3][4],	NULL,	L"network quality res 1 scale 3 highest"	},

	{	L"NqR2S0Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][0][0],	NULL,	L"network quality res 2 scale 0 lowest"	},
	{	L"NqR2S0Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][0][1],	NULL,	L"network quality res 2 scale 0 low"	},
	{	L"NqR2S0Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][0][2],	NULL,	L"network quality res 2 scale 0 middle"	},
	{	L"NqR2S0High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][0][3],	NULL,	L"network quality res 2 scale 0 high"	},
	{	L"NqR2S0Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][0][4],	NULL,	L"network quality res 2 scale 0 highest"	},

	{	L"NqR2S1Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][1][0],	NULL,	L"network quality res 2 scale 1 lowest"	},
	{	L"NqR2S1Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][1][1],	NULL,	L"network quality res 2 scale 1 low"	},
	{	L"NqR2S1Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][1][2],	NULL,	L"network quality res 2 scale 1 middle"	},
	{	L"NqR2S1High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][1][3],	NULL,	L"network quality res 2 scale 1 high"	},
	{	L"NqR2S1Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][1][4],	NULL,	L"network quality res 2 scale 1 highest"	},

	{	L"NqR2S2Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][2][0],	NULL,	L"network quality res 2 scale 2 lowest"	},
	{	L"NqR2S2Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][2][1],	NULL,	L"network quality res 2 scale 2 low"	},
	{	L"NqR2S2Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][2][2],	NULL,	L"network quality res 2 scale 2 middle"	},
	{	L"NqR2S2High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][2][3],	NULL,	L"network quality res 2 scale 2 high"	},
	{	L"NqR2S2Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][2][4],	NULL,	L"network quality res 2 scale 2 highest"	},

	{	L"NqR2S3Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][3][0],	NULL,	L"network quality res 2 scale 3 lowest"	},
	{	L"NqR2S3Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][3][1],	NULL,	L"network quality res 2 scale 3 low"	},
	{	L"NqR2S3Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][3][2],	NULL,	L"network quality res 2 scale 3 middle"	},
	{	L"NqR2S3High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][3][3],	NULL,	L"network quality res 2 scale 3 high"	},
	{	L"NqR2S3Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[2][3][4],	NULL,	L"network quality res 2 scale 3 highest"	},

	{	L"NqR3S0Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][0][0],	NULL,	L"network quality res 3 scale 0 lowest"	},
	{	L"NqR3S0Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][0][1],	NULL,	L"network quality res 3 scale 0 low"	},
	{	L"NqR3S0Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][0][2],	NULL,	L"network quality res 3 scale 0 middle"	},
	{	L"NqR3S0High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][0][3],	NULL,	L"network quality res 3 scale 0 high"	},
	{	L"NqR3S0Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][0][4],	NULL,	L"network quality res 3 scale 0 highest"	},

	{	L"NqR3S1Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][1][0],	NULL,	L"network quality res 3 scale 1 lowest"	},
	{	L"NqR3S1Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][1][1],	NULL,	L"network quality res 3 scale 1 low"	},
	{	L"NqR3S1Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][1][2],	NULL,	L"network quality res 3 scale 1 middle"	},
	{	L"NqR3S1High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][1][3],	NULL,	L"network quality res 3 scale 1 high"	},
	{	L"NqR3S1Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][1][4],	NULL,	L"network quality res 3 scale 1 highest"	},

	{	L"NqR3S2Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][2][0],	NULL,	L"network quality res 3 scale 2 lowest"	},
	{	L"NqR3S2Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][2][1],	NULL,	L"network quality res 3 scale 2 low"	},
	{	L"NqR3S2Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][2][2],	NULL,	L"network quality res 3 scale 2 middle"	},
	{	L"NqR3S2High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][2][3],	NULL,	L"network quality res 3 scale 2 high"	},
	{	L"NqR3S2Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][2][4],	NULL,	L"network quality res 3 scale 2 highest"	},

	{	L"NqR3S3Lowest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][3][0],	NULL,	L"network quality res 3 scale 3 lowest"	},
	{	L"NqR3S3Low",			TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][3][1],	NULL,	L"network quality res 3 scale 3 low"	},
	{	L"NqR3S3Middle",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][3][2],	NULL,	L"network quality res 3 scale 3 middle"	},
	{	L"NqR3S3High",		TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][3][3],	NULL,	L"network quality res 3 scale 3 high"	},
	{	L"NqR3S3Highest",	TRUE,		E_GrP3DvlParaTypeWord,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->VnetQt[3][3][4],	NULL,	L"network quality res 3 scale 3 highest"	},

	{	L"WebLogo",				TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3ScptCfgMdl)0)->WebLogo,				NULL,	L"built in web logo index"	},
};

#endif
