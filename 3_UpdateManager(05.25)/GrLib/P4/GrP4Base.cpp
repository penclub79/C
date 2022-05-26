/*
	GAUSE Platform Base

*/

#include <P4/GrP4Base.h>

#ifdef GR_P4_EDITOR
#include <P4/Dvl/GrP4DvlBase.h>
#endif

#include <GrStrTool.h>

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

#ifdef GR_P4_EDITOR

St_GrP4DvlDoPrInfoData	V_GrP4GvsVarInfo[E_GrP4GvsVarCnt]	=	
{
	{	L"HwVer(__u32)",			TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->HwVer,							L"hardware version"	},
	{	L"ScptTime(__u32)",		TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->ScptTime,					L"Script making time"	},
	{	L"NetVer(__u32)",			TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->NetVer,						L"network version"	},
	{	L"VdoMaxCh(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->VdoMaxCh,					L"video max channel"	},
	{	L"AdoMaxCh(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoMaxCh,					L"audio max channel"	},
	{	L"DiChCnt(__u8)",			TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->DiChCnt,						L"sensor channel count"	},
	{	L"DoChCnt(__u8)",			TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->DoChCnt,						L"alarm channel count"	},
	{	L"IsEdio(bool8)",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsEdio,						L"extension i/o box found"	},
	{	L"PrgsInit(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->PrgsInit,					L"initialize progress (0 ~ 100)"	},
	{	L"PrgsFwUp(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->PrgsFwUp,					L"firmware upgrade progress (0~100)"	},
	{	L"IsPlayMode(BOOL8)",	TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsPlayMode,				L"status play mode"	},
	{	L"LoginIdx(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->LoginIdx,					L"login index - 0 ~ 7, user id index, other - log off"	},
	{	L"LoginPriv(__u16)",		TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4Gvs)0)->LoginPriv,					L"Login privilege map"	},
	{	L"VlossMap(__u16)",		TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4Gvs)0)->VlossMap,					L"Video loss channel map"	},
	{	L"CovertMap(__u16)",		TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4Gvs)0)->CovertMap,					L"covert map"	},
	{	L"DiStat(__u16)",			TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4Gvs)0)->DiStat,						L"sensor status map"	},
	{	L"DoStat(__u16)",			TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4Gvs)0)->DoStat,						L"alarm status map"	},
	{	L"HddMaxCnt(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->HddMaxCnt,					L"HDD max count"	},
	{	L"IsEmgc(BOOL8)",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsEmgc,						L"Now emergency status"	},
	{	L"FwUpErr(__u8)",			TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->FwUpErr,						L"firmware upgrade error code"	},
	{	L"IsAtSeq(BOOL8)",		TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsAtSeq,						L"auto sequence"	},
	{	L"IsLanOn(BOOL8)",		TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsLanOn,						L"LAN cable on/off status"	},
	{	L"NetModel(__u32)",		TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->NetModel,					L"CMS network model number"	},
	{	L"PlayStat(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->PlayStat,					L"play status"	},
	{	L"PlaySpd(__s32)",			TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4Gvs)0)->PlaySpd,						L"play speed"	},
	{	L"IsPlayBack(BOOL8)",	TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsPlayBack,				L"play direction"	},
	{	L"UsbMemCnt(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->UsbMemCnt,					L"USB memory count"	},
	{	L"CmsCliCnt(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->CmsCliCnt,					L"CMS client count"	},
	{	L"V4Ip(__u32)",				TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->V4Ip,							L"running V4 IP"	},
	{	L"V4NetMask(__u32)",	TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->V4NetMask,					L"running V4 NETMASK"	},
	{	L"V4GateWay(__u32)",	TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->V4GateWay,					L"running V4 GATEWAY"	},
	{	L"AdoLiveCh(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveCh,					L"audio live channel"	},
	{	L"AdoPlayCh(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayCh,					L"audio play channel"	},
	{	L"AdoLiveVol0(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[0],			L"audio live volume ch 0"	},
	{	L"AdoLiveVol1(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[1],			L"audio live volume ch 1"	},
	{	L"AdoLiveVol2(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[2],			L"audio live volume ch 2"	},
	{	L"AdoLiveVol3(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[3],			L"audio live volume ch 3"	},
	{	L"AdoLiveVol4(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[4],			L"audio live volume ch 4"	},
	{	L"AdoLiveVol5(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[5],			L"audio live volume ch 5"	},
	{	L"AdoLiveVol6(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[6],			L"audio live volume ch 6"	},
	{	L"AdoLiveVol7(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[7],			L"audio live volume ch 7"	},
	{	L"AdoLiveVol8(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[8],			L"audio live volume ch 8"	},
	{	L"AdoLiveVol9(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[9],			L"audio live volume ch 9"	},
	{	L"AdoLiveVol10(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[10],		L"audio live volume ch 10"	},
	{	L"AdoLiveVol11(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[11],		L"audio live volume ch 11"	},
	{	L"AdoLiveVol12(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[12],		L"audio live volume ch 12"	},
	{	L"AdoLiveVol13(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[13],		L"audio live volume ch 13"	},
	{	L"AdoLiveVol14(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[14],		L"audio live volume ch 14"	},
	{	L"AdoLiveVol15(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoLiveVol[15],		L"audio live volume ch 15"	},

	{	L"AdoPlayVol0(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[0],			L"audio play volume ch 0"	},
	{	L"AdoPlayVol1(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[1],			L"audio play volume ch 1"	},
	{	L"AdoPlayVol2(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[2],			L"audio play volume ch 2"	},
	{	L"AdoPlayVol3(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[3],			L"audio play volume ch 3"	},
	{	L"AdoPlayVol4(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[4],			L"audio play volume ch 4"	},
	{	L"AdoPlayVol5(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[5],			L"audio play volume ch 5"	},
	{	L"AdoPlayVol6(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[6],			L"audio play volume ch 6"	},
	{	L"AdoPlayVol7(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[7],			L"audio play volume ch 7"	},
	{	L"AdoPlayVol8(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[8],			L"audio play volume ch 8"	},
	{	L"AdoPlayVol9(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[9],			L"audio play volume ch 9"	},
	{	L"AdoPlayVol10(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[10],		L"audio play volume ch 10"	},
	{	L"AdoPlayVol11(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[11],		L"audio play volume ch 11"	},
	{	L"AdoPlayVol12(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[12],		L"audio play volume ch 12"	},
	{	L"AdoPlayVol13(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[13],		L"audio play volume ch 13"	},
	{	L"AdoPlayVol14(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[14],		L"audio play volume ch 14"	},
	{	L"AdoPlayVol15(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4Gvs)0)->AdoPlayVol[15],		L"audio play volume ch 15"	},

	{	L"IsRmtTmpAble(BOOL8)",	TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsRmtTmpAble,		L"REMOCON temporary input able"	},
	{	L"RunRmtId(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,						1,	(void*)&((Ptr_GrP4Gvs)0)->RunRmtId,					L"REMOCON running ID"	},
	{	L"CrtRmtId(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->CrtRmtId,					L"REMOCON current ID"	},

	{	L"BkupMedia(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->BkupMedia,					L"Backup target device"	},
	{	L"BkupRecDev(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->BkupRecDev,				L"Backup record device"	},
	{	L"BkupChMap(__u16)",		TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4Gvs)0)->BkupChMap,					L"Backup channel map"	},
	{	L"BkupTimeSt(__u32)",	TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->BkupTimeSt,				L"Backup start time"	},
	{	L"BkupTimeEd(__u32)",	TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->BkupTimeEd,				L"Backup end time"	},
	{	L"BkupTimeEd(__u32)",	TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->BkupTimeEd,				L"Backup end time"	},
	{	L"IsBkupCmbViewer(BOOL8)",	TRUE,		E_GrP4DvlParaTypeBool8,		1,	(void*)&((Ptr_GrP4Gvs)0)->IsBkupCmbViewer,		L"Backup viewer combine data"	},

	{	L"BkupErrCode(__u8)",	TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->BkupErrCode,				L"Backup error code"	},
	{	L"BkupTimePrgs(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->BkupTimePrgs,			L"Backup progress time"	},

	{	L"ErrMap(__u32)",				TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->ErrMap,						L"system error map"	},
	{	L"PtzCtrlMode(__u8)",		TRUE,	E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->PtzCtrlMode,				L"PTZ control mode"	},
	{	L"IsPtzCtrlLock(BOOL8)",	TRUE,	E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4Gvs)0)->IsPtzCtrlLock,			L"PTZ control lock"	},
	{	L"PtzCtrlCh(__u8)",			TRUE,	E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->PtzCtrlCh,					L"PTZ control channel"	},

	{	L"IsOddExist(BOOL8)",		TRUE,	E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsOddExist,				L"ODD exist"	},
	{	L"MenuMode(__u8)",			TRUE,	E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->MenuMode,					L"UI menu mode"	},
	{	L"IsLoginMenu(BOOL8)",	TRUE,	E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsLoginMenu,				L"UI login mode"	},
	{	L"MsgBoxMode(__u8)",		TRUE,	E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->MsgBoxMode,				L"Message box mode"	},

	{	L"LastErrTime0(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[0],		L"Last error time 0"	},
	{	L"LastErrTime1(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[1],		L"Last error time 1"	},
	{	L"LastErrTime2(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[2],		L"Last error time 2"	},
	{	L"LastErrTime3(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[3],		L"Last error time 3"	},
	{	L"LastErrTime4(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[4],		L"Last error time 4"	},
	{	L"LastErrTime5(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[5],		L"Last error time 5"	},
	{	L"LastErrTime6(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[6],		L"Last error time 6"	},
	{	L"LastErrTime7(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[7],		L"Last error time 7"	},
	{	L"LastErrTime8(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[8],		L"Last error time 8"	},
	{	L"LastErrTime9(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[9],		L"Last error time 9"	},
	{	L"LastErrTime10(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[10],		L"Last error time 10"	},
	{	L"LastErrTime11(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[11],		L"Last error time 11"	},
	{	L"LastErrTime12(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[12],		L"Last error time 12"	},
	{	L"LastErrTime13(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[13],		L"Last error time 13"	},
	{	L"LastErrTime14(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[14],		L"Last error time 14"	},
	{	L"LastErrTime15(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[15],		L"Last error time 15"	},
	{	L"LastErrTime16(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[16],		L"Last error time 16"	},
	{	L"LastErrTime17(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[17],		L"Last error time 17"	},
	{	L"LastErrTime18(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[18],		L"Last error time 18"	},
	{	L"LastErrTime19(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[19],		L"Last error time 19"	},
	{	L"LastErrTime20(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[20],		L"Last error time 20"	},
	{	L"LastErrTime21(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[21],		L"Last error time 21"	},
	{	L"LastErrTime22(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[22],		L"Last error time 22"	},
	{	L"LastErrTime23(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[23],		L"Last error time 23"	},
	{	L"LastErrTime24(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[24],		L"Last error time 24"	},
	{	L"LastErrTime25(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[25],		L"Last error time 25"	},
	{	L"LastErrTime26(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[26],		L"Last error time 26"	},
	{	L"LastErrTime27(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[27],		L"Last error time 27"	},
	{	L"LastErrTime28(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[28],		L"Last error time 28"	},
	{	L"LastErrTime29(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[29],		L"Last error time 29"	},
	{	L"LastErrTime30(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[30],		L"Last error time 30"	},
	{	L"LastErrTime31(__u32)",	TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LastErrTime[31],		L"Last error time 31"	},

	{	L"IsNtpOk(BOOL8)",				TRUE,	E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4Gvs)0)->IsNtpOk,						L"NTP process result"	},

	{	L"LogRecDev(__u8)",				TRUE,	E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->LogRecDev,					L"log backup record device"	},
	{	L"LogChMap(__u32)",				TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LogChMap,					L"log backup channel map"	},
	{	L"LogEvtMap(__u32)",			TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LogEvtMap,					L"log backup event group map"	},
	{	L"LogTimeSt(__u32)",			TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LogTimeSt,					L"log backup start time"	},
	{	L"LogTimeEd(__u32)",			TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LogTimeEd,					L"log backup end time"	},
	{	L"LogErrCode(__u8)",			TRUE,	E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->LogErrCode,				L"log backup error code"	},
	{	L"LogTotal(__u32)",				TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LogTotal,					L"log backup item total count"	},
	{	L"LogPrgs(__u32)",				TRUE,	E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4Gvs)0)->LogPrgs,						L"log backup progress"	},
	{	L"PtzTourMsk(__u16)",			TRUE,	E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4Gvs)0)->PtzTourMsk,				L"Ptz tour mask"	},

	{	L"RunDispRes(__u8)",			TRUE,	E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4Gvs)0)->RunDispRes,				L"running display resolution index"	},

};
#endif


//====================================================================
//--------------------------------------------------------------------

