/*
		platform manager

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrDebug.h>
#include	<P3/GrP3Mng.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<GrTimeTool.h>
#include <GrGdibBase.h>
#include <GrUart.h>

#include	<P3/GrP3DoLbl.h>
#include	<P3/GrP3DoPrgsBarLoc.h>
#include	<P3/GrP3DoPrgsBarGvs.h>
#include	<P3/GrP3Cfg.h>
#include	<P3/GrP3DoQuad.h>
#include	<P3/GrP3ApiBase.h>
#include	<P3/GrP3DoFrame.h>
#include	<P3/GrP3DoBtnPage.h>
#include	<P3/GrP3DoStatic.h>
#include	<P3/GrP3DoEdtStpStr.h>
#include	<P3/GrP3DoBtnLbl.h>
#include	<P3/GrP3Bios.h>
#include	<P3/GrP3DoSpinNumStp.h>
#include	<P3/GrP3DoSpinTxtStp.h>
#include	<P3/GrP3DoRelStc.h>
#include	<P3/GrP3DoSpinPageTxt.h>
#include	<P3/GrP3DoSpinDdgtStp.h>
#include	<P3/GrP3DoBtnStc.h>
#include	<P3/GrP3DoChkbSsb.h>
#include	<P3/GrP3DoChkbSsm.h>
#include	<P3/GrP3DoSbarVar.h>
#include	<P3/GrP3DoSbarStp.h>
#include	<P3/GrP3DoRbtnStp.h>
#include	<P3/GrP3DoChkbSgb.h>
#include	<P3/GrP3DoChkbSgm.h>
#include	<P3/GrP3DoSpinPnt.h>
#include	<P3/GrP3DoSpinNumLoc.h>
#include	<P3/GrP3DoSpinDdgtLoc.h>
#include	<P3/GrP3DoSpinTxtLoc.h>
#include	<P3/GrP3DoTimeCtrl.h>
#include	<P3/GrP3DoCalRec.h>
#include <P3/GrP3DoGridRscStp.h>
#include <P3/GrP3DoBtnRelStc.h>
#include <P3/GrP3DoGridRstat.h>
#include <P3/GrP3DoTimeLbl.h>

#include <P3/GrP3DoLbxCtlg.h>

#include	<P3/GrP3DoVkbdTxt.h>
#include	<P3/GrP3DoVkbdNum.h>

#include	<P3/GrP3DoNotify.h>
#include	<P3/GrP3DoQuery.h>

#include <P3/GrP3DoSpinQuadStp.h>
#include <P3/GrP3DoRbtnGvu.h>
#include <P3/GrP3DoRbtnSsm.h>
#include <P3/GrP3DoChkbLoc.h>
#include <P3/GrP3DoEdtBase.h>
#include <P3/GrP3DoEdtLoc.h>
#include <P3/GrP3DoSpinId.h>
#include <P3/GrP3DoGridMtnStp.h>
#include <P3/GrP3DoCalHday.h>
#include <P3/GrP3DoPnlPtz.h>

#include <P3/GrP3DoSpinBtxtStp.h>
#include <P3/GrP3DoGridRrng.h>

#include <P3/GrP3DoSpinValStp.h>

#include <P3/GrP3DoGridPushStp.h>
#include <P3/GrP3DoGridPlayBar.h>

#include <P3/GrP3UiCmd.h>


#ifdef GR_P3_EDITOR
#include	<P3/Dvl/GrP3DvlBase.h>
#include	<P3/Dvl/GrP3DvlMng.h>
#endif

//====================================================================
//local const

#define E_GrP3MngTickPeriod			100			/* 1/10 sec */
#define E_GrP3MngMsgRestTime		10

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var
Cls_GrP3Mng*	V_GrP3Mng	=	NULL;

extern	St_GrP3ApiTbl	V_GrP3ApiCodeTbl[E_GrP3ApiCnt];

__u32	V_GrP3UartSpeedTbl[E_GrP3CfgUartSpeedCnt]	=	
{	1200, 2400, 4800, 9600, 19200	};

#ifdef GR_P3_EDITOR

St_GrP3DvlDoPrInfoData	V_GrP3MngGvsVarInfo[E_GrP3MngGvsVarCnt]	=	
{
	{	L"HwVer(__u32)",			TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->HwVer,							L"hardware version"	},
	{	L"ScptTime(__u32)",		TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->ScptTime,					L"Script making time"	},
	{	L"NetVer(__u32)",			TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->NetVer,						L"network version"	},
	{	L"VdoMaxCh(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->VdoMaxCh,					L"video max channel"	},
	{	L"AdoMaxCh(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoMaxCh,					L"audio max channel"	},
	{	L"DiChCnt(__u8)",			TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->DiChCnt,						L"sensor channel count"	},
	{	L"DoChCnt(__u8)",			TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->DoChCnt,						L"alarm channel count"	},
	{	L"IsEdio(bool8)",			TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->IsEdio,						L"extension i/o box found"	},
	{	L"PrgsInit(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->PrgsInit,					L"initialize progress (0 ~ 100)"	},
	{	L"PrgsFwUp(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->PrgsFwUp,					L"firmware upgrade progress (0~100)"	},
	{	L"IsPlayCon0(BOOL8)",	TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->TblIsPlay[0],			L"status is console 0 play mode, now"	},
	{	L"IsPlayCon1(BOOL8)",	TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->TblIsPlay[0],			L"status is console 1 play mode, now"	},
	{	L"LoginIdx(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LoginIdx,					L"login index - 0 ~ 7, user id index, other - log off"	},
	{	L"LoginPriv(__u16)",		TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LoginPriv,					L"Login privilege map"	},
	{	L"VlossMap(__u16)",		TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3MngGvs)0)->VlossMap,					L"Video loss channel map"	},
	{	L"CovertMap(__u16)",		TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3MngGvs)0)->CovertMap,					L"covert map"	},
	{	L"DiStat(__u16)",			TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3MngGvs)0)->DiStat,						L"sensor status map"	},
	{	L"DoStat(__u16)",			TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3MngGvs)0)->DoStat,						L"alarm status map"	},
	{	L"HddMaxCnt(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->HddMaxCnt,					L"HDD max count"	},
	{	L"IsEmgc(BOOL8)",			TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->IsEmgc,						L"Now emergency status"	},
	{	L"FwUpErr(__u8)",			TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->FwUpErr,						L"firmware upgrade error code"	},
	{	L"IsAtSeq(BOOL8)",		TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->IsAtSeq,						L"auto sequence"	},
	{	L"IsLanOn(BOOL8)",		TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->IsLanOn,						L"LAN cable on/off status"	},
	{	L"NetModel(__u32)",		TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->NetModel,					L"CMS network model number"	},
	{	L"PlayStat(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->PlayStat,					L"play status"	},
	{	L"PlaySpd(__s32)",			TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3MngGvs)0)->PlaySpd,						L"play speed"	},
	{	L"IsPlayBack(BOOL8)",	TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->IsPlayBack,				L"play direction"	},
	{	L"UsbMemCnt(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->UsbMemCnt,					L"USB memory count"	},
	{	L"CmsCliCnt(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->CmsCliCnt,					L"CMS client count"	},
	{	L"V4Ip(__u32)",				TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->V4Ip,							L"running V4 IP"	},
	{	L"V4NetMask(__u32)",	TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->V4NetMask,					L"running V4 NETMASK"	},
	{	L"V4GateWay(__u32)",	TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->V4GateWay,					L"running V4 GATEWAY"	},
	{	L"AdoLiveCh(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveCh,					L"audio live channel"	},
	{	L"AdoPlayCh(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayCh,					L"audio play channel"	},
	{	L"AdoLiveVol0(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[0],			L"audio live volume ch 0"	},
	{	L"AdoLiveVol1(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[1],			L"audio live volume ch 1"	},
	{	L"AdoLiveVol2(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[2],			L"audio live volume ch 2"	},
	{	L"AdoLiveVol3(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[3],			L"audio live volume ch 3"	},
	{	L"AdoLiveVol4(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[4],			L"audio live volume ch 4"	},
	{	L"AdoLiveVol5(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[5],			L"audio live volume ch 5"	},
	{	L"AdoLiveVol6(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[6],			L"audio live volume ch 6"	},
	{	L"AdoLiveVol7(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[7],			L"audio live volume ch 7"	},
	{	L"AdoLiveVol8(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[8],			L"audio live volume ch 8"	},
	{	L"AdoLiveVol9(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[9],			L"audio live volume ch 9"	},
	{	L"AdoLiveVol10(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[10],		L"audio live volume ch 10"	},
	{	L"AdoLiveVol11(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[11],		L"audio live volume ch 11"	},
	{	L"AdoLiveVol12(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[12],		L"audio live volume ch 12"	},
	{	L"AdoLiveVol13(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[13],		L"audio live volume ch 13"	},
	{	L"AdoLiveVol14(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[14],		L"audio live volume ch 14"	},
	{	L"AdoLiveVol15(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoLiveVol[15],		L"audio live volume ch 15"	},

	{	L"AdoPlayVol0(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[0],			L"audio play volume ch 0"	},
	{	L"AdoPlayVol1(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[1],			L"audio play volume ch 1"	},
	{	L"AdoPlayVol2(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[2],			L"audio play volume ch 2"	},
	{	L"AdoPlayVol3(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[3],			L"audio play volume ch 3"	},
	{	L"AdoPlayVol4(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[4],			L"audio play volume ch 4"	},
	{	L"AdoPlayVol5(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[5],			L"audio play volume ch 5"	},
	{	L"AdoPlayVol6(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[6],			L"audio play volume ch 6"	},
	{	L"AdoPlayVol7(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[7],			L"audio play volume ch 7"	},
	{	L"AdoPlayVol8(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[8],			L"audio play volume ch 8"	},
	{	L"AdoPlayVol9(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[9],			L"audio play volume ch 9"	},
	{	L"AdoPlayVol10(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[10],		L"audio play volume ch 10"	},
	{	L"AdoPlayVol11(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[11],		L"audio play volume ch 11"	},
	{	L"AdoPlayVol12(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[12],		L"audio play volume ch 12"	},
	{	L"AdoPlayVol13(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[13],		L"audio play volume ch 13"	},
	{	L"AdoPlayVol14(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[14],		L"audio play volume ch 14"	},
	{	L"AdoPlayVol15(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3MngGvs)0)->AdoPlayVol[15],		L"audio play volume ch 15"	},

	{	L"IsRmtTmpAble(BOOL8)",	TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->IsRmtTmpAble,		L"REMOCON temporary input able"	},
	{	L"RunRmtId(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,						1,	(void*)&((Ptr_GrP3MngGvs)0)->RunRmtId,					L"REMOCON running ID"	},
	{	L"CrtRmtId(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->CrtRmtId,					L"REMOCON current ID"	},

	{	L"BkupMedia(__u8)",		TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->BkupMedia,					L"Backup target device"	},
	{	L"BkupRecDev(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->BkupRecDev,				L"Backup record device"	},
	{	L"BkupChMap(__u16)",		TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3MngGvs)0)->BkupChMap,					L"Backup channel map"	},
	{	L"BkupTimeSt(__u32)",	TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->BkupTimeSt,				L"Backup start time"	},
	{	L"BkupTimeEd(__u32)",	TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->BkupTimeEd,				L"Backup end time"	},
	{	L"BkupTimeEd(__u32)",	TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->BkupTimeEd,				L"Backup end time"	},
	{	L"IsBkupCmbViewer(BOOL8)",	TRUE,		E_GrP3DvlParaTypeBool8,		1,	(void*)&((Ptr_GrP3MngGvs)0)->IsBkupCmbViewer,		L"Backup viewer combine data"	},

	{	L"BkupErrCode(__u8)",	TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->BkupErrCode,				L"Backup error code"	},
	{	L"BkupTimePrgs(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->BkupTimePrgs,			L"Backup progress time"	},

	{	L"ErrMap(__u32)",				TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->ErrMap,						L"system error map"	},
	{	L"PtzCtrlMode(__u8)",		TRUE,	E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->PtzCtrlMode,				L"PTZ control mode"	},
	{	L"IsPtzCtrlLock(BOOL8)",	TRUE,	E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3MngGvs)0)->IsPtzCtrlLock,			L"PTZ control lock"	},
	{	L"PtzCtrlCh(__u8)",			TRUE,	E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->PtzCtrlCh,					L"PTZ control channel"	},

	{	L"IsOddExist(BOOL8)",		TRUE,	E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->IsOddExist,				L"ODD exist"	},
	{	L"MenuMode(__u8)",			TRUE,	E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->MenuMode,					L"UI menu mode"	},
	{	L"IsLoginMenu(BOOL8)",	TRUE,	E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->IsLoginMenu,				L"UI login mode"	},
	{	L"MsgBoxMode(__u8)",		TRUE,	E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->MsgBoxMode,				L"Message box mode"	},

	{	L"LastErrTime0(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[0],		L"Last error time 0"	},
	{	L"LastErrTime1(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[1],		L"Last error time 1"	},
	{	L"LastErrTime2(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[2],		L"Last error time 2"	},
	{	L"LastErrTime3(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[3],		L"Last error time 3"	},
	{	L"LastErrTime4(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[4],		L"Last error time 4"	},
	{	L"LastErrTime5(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[5],		L"Last error time 5"	},
	{	L"LastErrTime6(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[6],		L"Last error time 6"	},
	{	L"LastErrTime7(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[7],		L"Last error time 7"	},
	{	L"LastErrTime8(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[8],		L"Last error time 8"	},
	{	L"LastErrTime9(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[9],		L"Last error time 9"	},
	{	L"LastErrTime10(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[10],		L"Last error time 10"	},
	{	L"LastErrTime11(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[11],		L"Last error time 11"	},
	{	L"LastErrTime12(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[12],		L"Last error time 12"	},
	{	L"LastErrTime13(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[13],		L"Last error time 13"	},
	{	L"LastErrTime14(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[14],		L"Last error time 14"	},
	{	L"LastErrTime15(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[15],		L"Last error time 15"	},
	{	L"LastErrTime16(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[16],		L"Last error time 16"	},
	{	L"LastErrTime17(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[17],		L"Last error time 17"	},
	{	L"LastErrTime18(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[18],		L"Last error time 18"	},
	{	L"LastErrTime19(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[19],		L"Last error time 19"	},
	{	L"LastErrTime20(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[20],		L"Last error time 20"	},
	{	L"LastErrTime21(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[21],		L"Last error time 21"	},
	{	L"LastErrTime22(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[22],		L"Last error time 22"	},
	{	L"LastErrTime23(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[23],		L"Last error time 23"	},
	{	L"LastErrTime24(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[24],		L"Last error time 24"	},
	{	L"LastErrTime25(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[25],		L"Last error time 25"	},
	{	L"LastErrTime26(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[26],		L"Last error time 26"	},
	{	L"LastErrTime27(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[27],		L"Last error time 27"	},
	{	L"LastErrTime28(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[28],		L"Last error time 28"	},
	{	L"LastErrTime29(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[29],		L"Last error time 29"	},
	{	L"LastErrTime30(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[30],		L"Last error time 30"	},
	{	L"LastErrTime31(__u32)",	TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LastErrTime[31],		L"Last error time 31"	},

	{	L"IsNtpOk(BOOL8)",				TRUE,	E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3MngGvs)0)->IsNtpOk,						L"NTP process result"	},

	{	L"LogRecDev(__u8)",				TRUE,	E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LogRecDev,					L"log backup record device"	},
	{	L"LogChMap(__u16)",				TRUE,	E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LogChMap,					L"log backup channel map"	},
	{	L"LogEvtMap(__u16)",				TRUE,	E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LogEvtMap,					L"log backup event type map"	},
	{	L"LogTimeSt(__u32)",			TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LogTimeSt,					L"log backup start time"	},
	{	L"LogTimeEd(__u32)",			TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LogTimeEd,					L"log backup end time"	},
	{	L"LogErrCode(__u8)",			TRUE,	E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LogErrCode,				L"log backup error code"	},
	{	L"LogTotal(__u32)",				TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LogTotal,					L"log backup item total count"	},
	{	L"LogPrgs(__u32)",				TRUE,	E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3MngGvs)0)->LogPrgs,						L"log backup progress"	},
	{	L"PtzTourMsk(__u16)",			TRUE,	E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3MngGvs)0)->PtzTourMsk,				L"Ptz tour mask"	},

	{	L"RunDispRes(__u8)",			TRUE,	E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3MngGvs)0)->RunDispRes,				L"running display resolution index"	},

};
#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP3Mng::Cls_GrP3Mng( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		//init
		m_TaskSvr				=	A_TaskSvr;
		m_MsgAloc				=	GrTaskSvrMsgCmdAlloc();

		m_PtrScptBuf		=	NULL;
		m_ScptSize			=	0;
		m_ScptDecPrgs		=	0;
		m_IsScptAble		=	FALSE;
		m_IsScptErr			=	FALSE;

		m_IsKeyBuzRun	=	FALSE;

		m_MseX			=	0;
		m_MseY			=	0;
		m_MseBtnMap	=	0;
		m_MseSenSl	=	0;
		m_MseSenSr	=	0;

		m_VmIdx			=	0;
		m_FcsConIdx	=	0;
		m_EventLock	=	0;

		m_PtrWinTbl		=	NULL;
		m_WinCnt		=	0;

		m_PtrPageTbl		=	NULL;
		m_PageCnt		=	0;

		m_PtrImgTbl		=	m_ImgScptTbl;
		m_PtrTxtTbl		=	m_TxtScptTbl;

		m_PtrAsmTbl		=	NULL;
		m_AsmCnt			=	0;

		m_PtrPstTbl		=	NULL;
		m_PstCnt			=	0;

		m_LangIdx			=	0;

		m_UiAlphaVal	=	0xFF;

		m_HdayCtlgPageSize	=	0;
		m_HdayCtlgPageIdx		=	0;
		m_HdayCtlgPageCnt		=	0;

		m_PtzMng			=	NULL;

		m_KeyBlock		=	0;

		m_VlossStat		=	0;

		m_ObjFindId		=	0;

		m_IsAtLock		=	FALSE;
		m_AtLockTimer	=	0;
		m_AtLockPeriod	=	0;

		// init
		m_SysStat			=	E_GrP3MngSysStatNone;

		GrDumyZeroMem( m_ConTbl, sizeof(m_ConTbl) );
		GrDumyZeroMem( m_ConMng, sizeof(m_ConMng) );
		GrDumyZeroMem( m_VmTbl, sizeof(m_VmTbl) );
		GrDumyZeroMem( m_FontMng, sizeof(m_FontMng) );
		GrDumyZeroMem( m_ImgUserTbl, sizeof(m_ImgUserTbl) );

		GrDumyZeroMem( &m_GvuTbl, sizeof(m_GvuTbl) );
		GrDumyZeroMem( &m_Gvs, sizeof(m_Gvs) );
		GrDumyZeroMem( m_SdaBuf, sizeof(m_SdaBuf) );

		GrDumyZeroMem( m_LiveAtbTbl, sizeof(m_LiveAtbTbl) );

		m_PtrCfgPrj		=	NULL;
		m_PtrCfgRtl		=	NULL;
		m_PtrCfgMdl		=	NULL;
		GrDumyZeroMem( m_TxtScptTbl, sizeof(m_TxtScptTbl) );
		GrDumyZeroMem( m_ImgScptTbl, sizeof(m_ImgScptTbl) );

		// reset GVS table
		LcGvsReset();

		//register server
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrP3MngTickPeriod, &m_HndlTimer );
}
//--------------------------------------------------------------------
Cls_GrP3Mng::~Cls_GrP3Mng( void )
{
		//unregister server
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

		GrTaskSvrMsgCmdFree( m_MsgAloc );

		LcFontReset();		// release font

}
//--------------------------------------------------------------------
BOOL8	GrP3MngInit( Cls_GrTaskSvr* A_TaskSvr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL == V_GrP3Mng )
		{
			V_GrP3Mng	=	(Cls_GrP3Mng*)new Cls_GrP3Mng( A_TaskSvr );
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP3MngFinish( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != V_GrP3Mng )
		{
			delete	V_GrP3Mng;
			V_GrP3Mng	=	NULL;
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP3ConBase*	Cls_GrP3Mng::ConsoleGet( __u8 A_ConIdx )
{
	// local -------------------
		Cls_GrP3ConBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrP3ConIdxFocus == A_ConIdx )
		{
			Tv_Result	=	m_ConTbl[m_FcsConIdx];
		}
		else if ( E_GrP3MngConMaxCnt > A_ConIdx )
		{
			Tv_Result	=	m_ConTbl[m_FcsConIdx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::UiMsgPost( Ptr_GrP3MsgHd A_PtrMsg, __u32 A_Size )
{
		return	m_TaskSvr->MsgExPost( E_GrTaskMsgUiMsg, (void*)A_PtrMsg, A_Size );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::UiMsgPump( void )
{
		return	m_TaskSvr->DoStep();
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::UiMsgRest( void )
{
		GrPrcSleep( E_GrP3MngMsgRestTime );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcFontReset( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngFontMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_FontMng[Tv_WkIdx] )
			{
				delete	m_FontMng[Tv_WkIdx];
				m_FontMng[Tv_WkIdx]	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecHead( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP3ScptFcc == ((Ptr_GrP3ScptHdInfo)m_PtrScptBuf)->Fcc )
		{
			if ( E_GrP3ScptVer == ((Ptr_GrP3ScptHdInfo)m_PtrScptBuf)->Ver )
			{
				m_Gvs.ScptTime	=	((Ptr_GrP3ScptHdInfo)m_PtrScptBuf)->Time;		// update time
				m_ScptDecPrgs		=	m_ScptDecPrgs	+ sizeof(St_GrP3ScptHdInfo);
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcScptHeadChk - script version ( %d -> %d ) mismatch!\n", E_GrP3ScptVer, ((Ptr_GrP3ScptHdInfo)m_PtrScptBuf)->Ver );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptHeadChk - bad FCC code ( %08X )!\n", ((Ptr_GrP3ScptHdInfo)m_PtrScptBuf)->Fcc );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecCfgPrj( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
	// code --------------------

		if ( sizeof(St_GrP3ScptCfgPrj) != A_PtrDtHd->Size )
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecCfgPrj - (warnning) different size ( %d -> %d ).\n", sizeof(St_GrP3ScptCfgPrj), A_PtrDtHd->Size );
		}
		m_PtrCfgPrj	=	(Ptr_GrP3ScptCfgPrj)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecCfgRtl( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
	// code --------------------

		if ( sizeof(St_GrP3ScptCfgRtl) != A_PtrDtHd->Size )
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecCfgRtl - (warnning) different size ( %d -> %d ).\n", sizeof(St_GrP3ScptCfgRtl), A_PtrDtHd->Size );
		}
		m_PtrCfgRtl	=	(Ptr_GrP3ScptCfgRtl)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecMdlAtb( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
	// code --------------------

		if ( sizeof(St_GrP3ScptCfgMdl) != A_PtrDtHd->Size )
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecMdlAtb - (warnning) different size ( %d -> %d ).\n", sizeof(St_GrP3ScptCfgMdl), A_PtrDtHd->Size );
		}
		m_PtrCfgMdl	=	(Ptr_GrP3ScptCfgMdl)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecPstTbl( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_PstCnt		=	A_PtrDtHd->Size / sizeof(St_GrP3ScptPstItm);
		m_PtrPstTbl	=	(Ptr_GrP3ScptPstItm)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer
		DbgMsgPrint( "[P3] PST table count = %d.\n", m_PstCnt );
		// reset pointer
		for ( Tv_WkIdx=0;Tv_WkIdx < m_PstCnt;Tv_WkIdx++ )
		{
			m_PtrPstTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecPstScpt( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrPstTbl )
		{
			// check index
			if ( A_PtrDtHd->Id < (__u16)m_PstCnt )
			{
				// check count
				Tv_Size	=	sizeof(St_GrP3ScptPstData) * ((__u32)m_PtrPstTbl[A_PtrDtHd->Id].Cnt);
				if ( A_PtrDtHd->Size == Tv_Size )
				{
					// check alreay allocated
					if ( NULL == m_PtrPstTbl[A_PtrDtHd->Id].Pos )
					{
						m_PtrPstTbl[A_PtrDtHd->Id].Pos	=	(void*)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// set pointer
						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcScptDecPstScpt - bad PST ( %d ) already loaded.\n", A_PtrDtHd->Id );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcScptDecPstScpt - bad PST ( %d ) command count ( %d -> %d ).\n", A_PtrDtHd->Id, Tv_Size , A_PtrDtHd->Size );
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcScptDecPstScpt - bad PST index ( %d ).\n", A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecPstScpt - PST table not loaded.\n" );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecWinTbl( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_WinCnt		=	A_PtrDtHd->Size / sizeof(St_GrP3ScptWinInfo);
		m_PtrWinTbl	=	(Ptr_GrP3ScptWinInfo)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer
		DbgMsgPrint( "[P3] Window table count = %d.\n", m_WinCnt );
		// reset pointer
		for ( Tv_WkIdx=0;Tv_WkIdx < m_WinCnt;Tv_WkIdx++ )
		{
			m_PtrWinTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecWinScpt( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrWinTbl )
		{
			// check index
			if ( A_PtrDtHd->Id < (__u16)m_WinCnt )
			{
				// check alreay allocated
				if ( NULL == m_PtrWinTbl[A_PtrDtHd->Id].Pos )
				{
					m_PtrWinTbl[A_PtrDtHd->Id].Pos	=	(void*)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// set pointer
					Tv_Result	=	TRUE;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcScptDecWinScpt - bad Window ( %d ) already loaded.\n", A_PtrDtHd->Id );
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcScptDecWinScpt - bad Window index ( %d ).\n", A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecWinScpt - Window table not loaded.\n" );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecPageTbl( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_PageCnt		=	A_PtrDtHd->Size / sizeof(St_GrP3ScptPageInfo);
		m_PtrPageTbl	=	(Ptr_GrP3ScptPageInfo)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer
		DbgMsgPrint( "[P3] Page table count = %d.\n", m_PageCnt );
		// reset pointer
		for ( Tv_WkIdx=0;Tv_WkIdx < m_PageCnt;Tv_WkIdx++ )
		{
			m_PtrPageTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecPageScpt( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrPageTbl )
		{
			// check index
			if ( A_PtrDtHd->Id < (__u16)m_PageCnt )
			{
				// check alreay allocated
				if ( NULL == m_PtrPageTbl[A_PtrDtHd->Id].Pos )
				{
					m_PtrPageTbl[A_PtrDtHd->Id].Pos	=	(void*)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// set pointer
					Tv_Result	=	TRUE;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcScptDecPageScpt - bad Page ( %d ) already loaded.\n", A_PtrDtHd->Id );
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcScptDecPageScpt - bad Page index ( %d ).\n", A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecPageScpt - Page table not loaded.\n" );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecAsmTbl( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_AsmCnt		=	A_PtrDtHd->Size / sizeof(St_GrP3ScptAsmInfo);
		m_PtrAsmTbl	=	(Ptr_GrP3ScptAsmInfo)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer
		DbgMsgPrint( "[P3] ASM table count = %d.\n", m_AsmCnt );
		// reset pointer
		for ( Tv_WkIdx=0;Tv_WkIdx < m_AsmCnt;Tv_WkIdx++ )
		{
			m_PtrAsmTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecAsmCode( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrAsmTbl )
		{
			// check index
			if ( A_PtrDtHd->Id < (__u16)m_AsmCnt )
			{
				// check count
				Tv_Size	=	E_GrP3AsmOpUnitSize * ((__u32)m_PtrAsmTbl[A_PtrDtHd->Id].Cnt);
				if ( A_PtrDtHd->Size == Tv_Size )
				{
					// check alreay allocated
					if ( NULL == m_PtrAsmTbl[A_PtrDtHd->Id].Pos )
					{
						m_PtrAsmTbl[A_PtrDtHd->Id].Pos	=	(void*)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// set pointer
						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcScptDecAsmCode - bad ASM ( %d ) already loaded.\n", A_PtrDtHd->Id );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcScptDecAsmCode - bad ASM ( %d ) code count ( %d -> %d ).\n", A_PtrDtHd->Id, Tv_Size , A_PtrDtHd->Size );
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcScptDecAsmCode - bad ASM index ( %d ).\n", A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecAsmCode - ASM table not loaded.\n" );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecImgData( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3MngImgScptMaxCnt > A_PtrDtHd->Id )
		{
			if ( NULL == m_ImgScptTbl[A_PtrDtHd->Id] )
			{
				m_ImgScptTbl[A_PtrDtHd->Id]	=	(Ptr_GrGdib)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcScptDecImgData - Image ( %d ) already allocated.\n" , A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecImgData - bad index ( %d ).\n" , A_PtrDtHd->Id );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecTextData( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (E_GrP3MngTextScptMaxCnt > A_PtrDtHd->Id) && ( E_GrP3ScptLangMaxCnt > A_PtrDtHd->SubIdx ) )
		{
			if ( NULL == m_TxtScptTbl[A_PtrDtHd->Id].LngTbl[A_PtrDtHd->SubIdx] )
			{
				m_TxtScptTbl[A_PtrDtHd->Id].LngTbl[A_PtrDtHd->SubIdx]	=	(WCHAR*)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcScptDecTextData - Text ( %d : %d ) already allocated.\n" , A_PtrDtHd->Id , A_PtrDtHd->SubIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecTextData - bad index ( %d : %d ).\n" , A_PtrDtHd->Id, A_PtrDtHd->SubIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcScptDecFontData( Ptr_GrP3ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrP3FontHead	Tv_PtrFont;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3MngFontMaxCnt > A_PtrDtHd->Id )
		{
			if ( NULL == m_FontMng[A_PtrDtHd->Id] )
			{
				Tv_PtrFont	=	(Ptr_GrP3FontHead)( (__u32)A_PtrDtHd + sizeof(St_GrP3ScptDataHd) );	// update pointer
				m_FontMng[A_PtrDtHd->Id]	=	(Cls_GrP3Font*)new Cls_GrP3Font( Tv_PtrFont );
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcScptDecFontData - Font ( %d ) already allocated.\n" , A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcScptDecFontData - bad index ( %d ).\n" , A_PtrDtHd->Id );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::ScriptSet( void* A_PtrScpt, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrP3ScptDataHd	Tv_PtrDtHd;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result			=	FALSE;

		m_PtrScptBuf	=	A_PtrScpt;
		m_ScptSize		=	A_Size;
		m_ScptDecPrgs	=	0;
		m_IsScptErr		=	FALSE;

		m_PtrCfgPrj		=	NULL;
		m_PtrCfgRtl		=	NULL;
		m_PtrCfgMdl		=	NULL;
		m_PtrPstTbl		=	NULL;
		m_PstCnt			=	0;
		m_PtrWinTbl		=	NULL;
		m_WinCnt			=	0;
		m_PtrPageTbl	=	NULL;
		m_PageCnt			=	0;
		m_PtrAsmTbl		=	NULL;
		m_AsmCnt			=	0;
		GrDumyZeroMem( m_TxtScptTbl, sizeof(m_TxtScptTbl) );
		GrDumyZeroMem( m_ImgScptTbl, sizeof(m_ImgScptTbl) );

		// reset all object
		LcFontReset();
		// check exist script
		if ( NULL != m_PtrScptBuf )
		{
			// check script header
			if ( LcScptDecHead() )
			{
				Tv_Result	=	TRUE;
				while ( (m_ScptDecPrgs < m_ScptSize) && Tv_Result )
				{
					// get data head
					Tv_PtrDtHd	=	(Ptr_GrP3ScptDataHd)( (__u32)m_PtrScptBuf + m_ScptDecPrgs );
					switch ( Tv_PtrDtHd->Type )
					{
						case E_GrP3ScptDataCfgPrj:
							Tv_Result	=	LcScptDecCfgPrj( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataCfgRtl:
							Tv_Result	=	LcScptDecCfgRtl( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataMdlAtb:
							Tv_Result	=	LcScptDecMdlAtb( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataPstTbl:
							Tv_Result	=	LcScptDecPstTbl( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataPstScptData:
							Tv_Result	=	LcScptDecPstScpt( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataWinScptTbl:
							Tv_Result	=	LcScptDecWinTbl( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataWinScptData:
							Tv_Result	=	LcScptDecWinScpt( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataPageScptTbl:
							Tv_Result	=	LcScptDecPageTbl( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataPageScptData:
							Tv_Result	=	LcScptDecPageScpt( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataAsmTbl:
							Tv_Result	=	LcScptDecAsmTbl( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataAsmData:
							Tv_Result	=	LcScptDecAsmCode( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataImageData:
							Tv_Result	=	LcScptDecImgData( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataTextData:
							Tv_Result	=	LcScptDecTextData( Tv_PtrDtHd );
							break;
						case E_GrP3ScptDataFontData:
							Tv_Result	=	LcScptDecFontData( Tv_PtrDtHd );
							break;

						default:
							DbgMsgPrint( "Cls_GrP3Mng::ScriptSet - unknown data type ( %d ) at %08X .\n", Tv_PtrDtHd->Type, m_ScptDecPrgs );
							Tv_Result	=	FALSE;
							m_IsScptErr	=	TRUE;
							break;
					}	//	switch ( Tv_PtrDtHd->Type )
					// next
					m_ScptDecPrgs	=	m_ScptDecPrgs + Tv_PtrDtHd->Size + sizeof(St_GrP3ScptDataHd);
				}	//	while ( (m_ScptDecPrgs < m_ScptSize) && Tv_Result )

			}

			// check able
			if ( Tv_Result )
			{
				if ( (NULL != m_PtrCfgPrj) && (NULL != m_PtrCfgRtl) && (NULL != m_PtrCfgMdl) )
				{
					m_IsScptAble	=	TRUE;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::ScriptSet - CFGRTL, CFGPRJ, CFGMDL not exist!\n" );
				}
			}

			// change notify to console
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngConMaxCnt;Tv_WkIdx++ )
			{
				if ( NULL != m_ConTbl[Tv_WkIdx] )
				{
					m_ConTbl[Tv_WkIdx]->RtlScptUpdt();
				}
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrGdib	Cls_GrP3Mng::ImgGet( __u16 A_Id )
{
	// local -------------------
		Ptr_GrGdib	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrP3ImgUserIdStart > A_Id )
		{
			// find normal image
			if ( E_GrP3ImgIdBase <= A_Id )
			{
				Tv_Idx	=	A_Id - E_GrP3ImgIdBase;
				if ( E_GrP3MngImgScptMaxCnt > Tv_Idx )
				{
					Tv_Result	=	m_PtrImgTbl[Tv_Idx];
				}
			}
		}
		else
		{
			// user image
			Tv_Idx	=	A_Id - E_GrP3ImgUserIdStart;
			if ( E_GrP3MngImgUserMaxCnt > Tv_Idx )
			{
				Tv_Result	=	m_ImgUserTbl[Tv_Idx];
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP3MngGetWinIdInfo( __u16 A_WinId, __u8* A_PtrRtLayer, __u8* A_PtrRtSlot )
{
	// local -------------------
	// code --------------------
		*A_PtrRtLayer	=	(__u8)( (A_WinId >> 12) & 0x0F );
		*A_PtrRtSlot	=	(__u8)( (A_WinId >> 8) & 0x0F );
}
//--------------------------------------------------------------------
__u16	GrP3MngMakeWinId( __u8 A_Layer, __u8 A_Slot, __u8 A_Seq )
{
		return	(__u16)A_Seq | ( ((__u16)A_Layer) << 12 ) | ( ((__u16)A_Slot) << 8 );
}
//--------------------------------------------------------------------
__u32	GrP3MngMakeCtlgHndl( __u8 A_Id, __u32 A_ItemIdx )
{
		return	((__u32)A_Id << E_GrP3MngCtlgIdShft) | A_ItemIdx;
}
//--------------------------------------------------------------------
__u32	GrP3MngCtlgHndlToIdx( __u32 A_CtlgHndl, __u32* A_PtrRtId )
{
	// local -------------------
	// code --------------------
		*A_PtrRtId	=	(A_CtlgHndl >> E_GrP3MngCtlgIdShft) & E_GrP3MngCtlgIdMask;
		return	A_CtlgHndl & E_GrP3MngCtlgValMask;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::KeyInput( __u8 A_Key, BOOL8 A_IsPush, __u8 A_DevType )
{
	// local -------------------
		St_GrP3MsgKey	Tv_Msg;
		__s32		Tv_Rint[2];
		BOOL8	Tv_IsAble;
	// code --------------------
		// check able
		if ( m_IsScptAble )
		{
			// check able key
			if ( 0 != A_Key )
			{
				// check change focus key
				if ( E_GrP3KeyDisplay == A_Key)
				{
					if ( A_IsPush && m_PtrCfgRtl->IsChgConAble )
					{
						// change focus
						// not implement
					}
				}
				else
				{
					if ( A_Key != m_KeyBlock )
					{
						// check REMOCON able
						Tv_IsAble	=	TRUE;
						if ( (E_GrP3KeyId <= A_Key) && (E_GrP3KeySelId >= A_Key) )
						{
							if ( E_GrP3KeySelId != A_Key )
							{
								m_Gvs.RunRmtId	=	A_Key - E_GrP3KeyId;
							}
						}
						else 
						{
							if ( (E_GrP3InDevRemocon == A_DevType) && (!m_Gvs.IsRmtTmpAble) )
							{
								if ( m_Gvs.RunRmtId != m_Gvs.CrtRmtId )
								{
									Tv_IsAble	=	FALSE;
								}
							}
						}

						if ( Tv_IsAble )
						{
							// post key message
							Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdConInpKey;
							Tv_Msg.Hd.ConIdx	=	m_FcsConIdx;
							Tv_Msg.Hd.WinId		=	E_GrP3WinIdNone;
							Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdNone;
							Tv_Msg.Dev				=	A_DevType;
							Tv_Msg.Key				=	A_Key;
							Tv_Msg.IsPush			=	A_IsPush;
							UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
						}
					}
					else
					{
						if ( !A_IsPush )
						{
							m_KeyBlock	=	0;
						}
					}
				}

				if ( m_IsKeyBuzRun && A_IsPush )
				{
					Tv_Rint[1]	=	1;
					GrP3BiosBuzRun( Tv_Rint, NULL );
				}

				// auto lock time reset
				m_AtLockTimer	=	m_AtLockPeriod;
			}
		}	//	if ( m_IsScptAble )
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgBrdcst( Ptr_GrP3MsgHd A_PtrMsg )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP3MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				A_PtrMsg->ConIdx	=	Tv_ConIdx;
				m_ConTbl[Tv_ConIdx]->RtlUiMsgProc( A_PtrMsg );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgUiMsg( Ptr_GrP3MsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
		// check broadcast message
		if ( E_GrP3ConIdxAll == A_PtrMsg->ConIdx )
		{
			LcProcMsgBrdcst( A_PtrMsg );
		}
		else
		{
			if ( E_GrP3MngConMaxCnt > A_PtrMsg->ConIdx )
			{
				if ( NULL != m_ConTbl[A_PtrMsg->ConIdx] )
				{
					m_ConTbl[A_PtrMsg->ConIdx]->RtlUiMsgProc( A_PtrMsg );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcProcMsgUiMsg - bad console index %d\n", A_PtrMsg->ConIdx );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		__u32	Tv_Tick;
	// code --------------------
		//get tick
		Tv_Tick	=	GrTimeGetTick();
		//broadcast tick
		LcTickMsgBrdcst( Tv_Tick );
		// check VM resume
		if ( m_VmTbl[m_VmIdx].IsRun && m_VmTbl[m_VmIdx].IsOpWait )
		{
			RtlVmRun();
		}
		// update video loss
		LcVlossUpdt();
		// auto lock
		RtlAtLockProc();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		void*		Tv_PtrPara;
		Ptr_GrP3MngMsgBase	Tv_PtrHd;
		__u8	Tv_ConIdx;
	// code --------------------
		// init
		Tv_Result	=	TRUE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_PtrHd	=	(Ptr_GrP3MngMsgBase)A_PtrMsg;
			// message command
			switch ( Tv_PtrHd->Cmd )
			{
				case E_GrP3MngCmdQuadModeSet:
					Tv_ConIdx	=	((Ptr_GrP3MngMsgQuadModeSet)Tv_PtrHd)->ConIdx;
					if ( E_GrP3MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadModeSet( ((Ptr_GrP3MngMsgQuadModeSet)Tv_PtrHd)->Mode, ((Ptr_GrP3MngMsgQuadModeSet)Tv_PtrHd)->Page, ((Ptr_GrP3MngMsgQuadModeSet)Tv_PtrHd)->Fcc );
						}
					}
					break;
				case E_GrP3MngCmdQuadModeNext:
					Tv_ConIdx	=	((Ptr_GrP3MngMsgQuadMovePara)Tv_PtrHd)->ConIdx;
					if ( E_GrP3MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadModeNext( ((Ptr_GrP3MngMsgQuadMovePara)Tv_PtrHd)->Fcc );
						}
					}
					break;
				case E_GrP3MngCmdQuadModePrev:
					Tv_ConIdx	=	((Ptr_GrP3MngMsgQuadMovePara)Tv_PtrHd)->ConIdx;
					if ( E_GrP3MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadModePrev( ((Ptr_GrP3MngMsgQuadMovePara)Tv_PtrHd)->Fcc );
						}
					}
					break;
				case E_GrP3MngCmdQuadPageNext:
					Tv_ConIdx	=	((Ptr_GrP3MngMsgQuadMovePara)Tv_PtrHd)->ConIdx;
					if ( E_GrP3MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadPageNext( ((Ptr_GrP3MngMsgQuadMovePara)Tv_PtrHd)->Fcc );
						}
					}
					break;
				case E_GrP3MngCmdQuadModeGet:
					Tv_ConIdx	=	((Ptr_GrP3MngMsgQuadModeGet)Tv_PtrHd)->ConIdx;
					if ( E_GrP3MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadModeGet( ((Ptr_GrP3MngMsgQuadModeGet)Tv_PtrHd)->PtrRtMode, ((Ptr_GrP3MngMsgQuadModeGet)Tv_PtrHd)->PtrRtPage );
						}
					}
					break;
				case E_GrP3MngCmdAsmDcall:
					VmRunCode( ((Ptr_GrP3MngMsgAsmDcall)A_PtrMsg)->CodeId, NULL,NULL );
					break;
				case E_GrP3MngCmdDpcUpi:
					LcProcDpcUpi( (Ptr_GrP3MngMsgDpcUpi)A_PtrMsg );
					break;
				case E_GrP3MngCmdErrSetSetOne:
					LcProcMsgErrSetOne( (Ptr_GrP3MngMsgErrSetOne)A_PtrMsg );
					break;
				case E_GrP3MngCmdErrSetClrMap:
					LcProcMsgErrClrMap( (Ptr_GrP3MngMsgErrClrMap)A_PtrMsg );
					break;
				case E_GrP3MngCmdSetupSave:
					LcProcMsgSetupSave();
					break;
			}
		}
		else
		{
			Tv_PtrPara	=	(void*)( (__u32)A_PtrMsg + sizeof(St_GrTaskMsgHd) );
			switch( A_PtrMsg->Cmd )
			{
				case	E_GrTaskMsgUiMsg:
					LcProcMsgUiMsg( (Ptr_GrP3MsgHd)Tv_PtrPara );
					break;
				case	E_GrTaskMsgQuadChgReq:
					LcProcMsgQuadChgReq( (Ptr_GrTaskMsgQuadChgReq)A_PtrMsg );
					break;
				case E_GrTaskMsgDiskFormat:
					// notify HDD format
					LcProcMsgFormat( (Ptr_GrTaskMsgDiskFormat)A_PtrMsg );
					break;
				case E_GrTaskMsgUiEvt:
					LcProcMsgEvtCall( (Ptr_GrP3EvtCallMsg)Tv_PtrPara );
					break;
				case E_GrTaskMsgFwUpgdFinish:
					LcProcMsgFwUpgdDone( (Ptr_GrTaskMsgFwUpgdEnd)A_PtrMsg );
					break;
				case E_GrTaskMsgNetOff:
					LcProcMsgNetOnOff( FALSE );
					break;
				case E_GrTaskMsgNetOn:
					LcProcMsgNetOnOff( TRUE );
					break;
				case E_GrTaskMsgPlayStatChg:
					LcProcMsgPlayStatChg();
					break;
				case E_GrTaskMsgUsbChg:
					LcProcMsgUsbChg();
					break;
				case E_GrTaskMsgCmsCliChg:
					LcProcMsgCmsCliChg();
					break;
				case E_GrTaskMsgBkupStart:
					LcProcMsgBkupStart();
					break;
				case E_GrTaskMsgBkupEnd:
					LcProcMsgBkupEnd();
					break;
				case E_GrTaskMsgBkupError:
					LcProcMsgBkupErr();
					break;
				case E_GrTaskMsgLogBkupStart:
					LcProcMsgLogBkupStart();
					break;
				case E_GrTaskMsgLogBkupEnd:
					LcProcMsgLogBkupEnd();
					break;
				case E_GrTaskMsgLogBkupError:
					LcProcMsgLogBkupError();
					break;

				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::MousePosInput( __s32 A_X, __s32 A_Y, BOOL8 A_IsAbs )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		St_GrP3MsgMouse	Tv_Msg;
	// code --------------------
		//check able
		if ( NULL != m_ConTbl[m_FcsConIdx] )
		{
			Tv_X	=	A_X;
			Tv_Y	=	A_Y;
			if ( !A_IsAbs )
			{
				if ( 0 != m_MseSenSl )
				{
					Tv_X	=	Tv_X << m_MseSenSl;
					Tv_Y	=	Tv_Y << m_MseSenSl;
				}
				else if ( 0 != m_MseSenSr )
				{
					Tv_X	=	Tv_X >> m_MseSenSr;
					Tv_Y	=	Tv_Y >> m_MseSenSr;
				}
				Tv_X	=	Tv_X + m_MseX;
				Tv_Y	=	Tv_Y + m_MseY;
			}
			//patch position x
			if ( Tv_X < 0 )
			{
				Tv_X	=	0;
			}
			else if ( Tv_X >= (__s32)m_ConTbl[m_FcsConIdx]->GetResX() )
			{
				Tv_X	=	(__s32)m_ConTbl[m_FcsConIdx]->GetResX() - 1;
			}
			//patch position y
			if ( Tv_Y < 0 )
			{
				Tv_Y		=	0;
			}
			else if ( Tv_Y >= (__s32)m_ConTbl[m_FcsConIdx]->GetResY()  )
			{
				Tv_Y	=	(__s32)m_ConTbl[m_FcsConIdx]->GetResY() - 1;
			}
			//check change position
			if ( (m_MseX != (__u16)Tv_X) || (m_MseY != (__u16)Tv_Y ) )
			{
				//update cursor position
				m_MseX		=	(__u16)Tv_X;
				m_MseY		=	(__u16)Tv_Y;
				//make message
				Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdConInpMouse;
				Tv_Msg.Hd.ConIdx	=	m_FcsConIdx;
				Tv_Msg.Hd.WinId		=	E_GrP3WinIdNone;		//console message
				Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdNone;
				Tv_Msg.BtnMap			=	m_MseBtnMap;
				Tv_Msg.X					=	m_MseX;
				Tv_Msg.Y					=	m_MseY;
				Tv_Msg.Atb				=	E_GrP3MouseAtbPosChg;

				UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			}
		}
		// auto lock time reset
		m_AtLockTimer	=	m_AtLockPeriod;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::MouseBtnInput( __u8 A_BtnIdx, BOOL8 A_IsPush )
{
	// local -------------------
		St_GrP3MsgMouse		Tv_Msg;
	// code --------------------
		if ( NULL != m_ConTbl[m_FcsConIdx] )
		{
			//update button status
			if ( A_IsPush )
			{
				m_MseBtnMap	=	m_MseBtnMap | (1 << A_BtnIdx);
			}
			else
			{
				m_MseBtnMap	=	m_MseBtnMap & ~(1 << A_BtnIdx);
			}

			//make message
			Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdConInpMouse;
			Tv_Msg.Hd.ConIdx	=	m_FcsConIdx;
			Tv_Msg.Hd.WinId		=	E_GrP3WinIdNone;		//console message
			Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdNone;
			Tv_Msg.BtnMap			=	m_MseBtnMap;
			Tv_Msg.X					=	m_MseX;
			Tv_Msg.Y					=	m_MseY;
			Tv_Msg.Atb				=	1 << A_BtnIdx;

			UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		// auto lock time reset
		m_AtLockTimer	=	m_AtLockPeriod;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::MouseWheelInput( BOOL8 A_IsDown )
{
	// local -------------------
		St_GrP3MsgWheel		Tv_Msg;
	// code --------------------
		if ( NULL != m_ConTbl[m_FcsConIdx] )
		{
			Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdConInpWheel;
			Tv_Msg.Hd.ConIdx	=	m_FcsConIdx;
			Tv_Msg.Hd.WinId		=	E_GrP3WinIdNone;
			Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdNone;
			Tv_Msg.IsDown			=	A_IsDown;
			UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		// auto lock time reset
		m_AtLockTimer	=	m_AtLockPeriod;
}
//--------------------------------------------------------------------
Ptr_GrP3ScptPstData	Cls_GrP3Mng::PstDataGet( __u16 A_Id, __u8* A_PtrRtCnt )
{
	// local -------------------
		Ptr_GrP3ScptPstData	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCnt	=	0;

		if ( (E_GrP3ScptPstIdBase <= A_Id) && ( (E_GrP3ScptPstIdBase + m_PstCnt) > A_Id ) )
		{
			// check exist
			Tv_Idx	=	A_Id - E_GrP3ScptPstIdBase;
			if ( 0 != m_PtrPstTbl[Tv_Idx].Cnt )
			{
				*A_PtrRtCnt	=	m_PtrPstTbl[Tv_Idx].Cnt;
				Tv_Result	=	(Ptr_GrP3ScptPstData)m_PtrPstTbl[Tv_Idx].Pos;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3Mng::TextPtrGet( __u16 A_Id, __u8 A_Lang )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16		Tv_TxtIdx;
		__u8		Tv_LangIdx;
	// code --------------------
		Tv_Result	=	NULL;

		if ( (E_GrP3ScptTxtIdBase <= A_Id) && ((E_GrP3ScptTxtIdBase + E_GrP3MngTextScptMaxCnt) > A_Id) )
		{
			Tv_TxtIdx		=	A_Id - E_GrP3ScptTxtIdBase;
			Tv_LangIdx	=	m_LangIdx;
			if ( E_GrP3ScptLangMaxCnt > A_Lang )
			{
				Tv_LangIdx	=	A_Lang;
			}
			Tv_Result	=	m_PtrTxtTbl[Tv_TxtIdx].LngTbl[Tv_LangIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::MousePosGet( __u16* A_PtrRtX, __u16* A_PtrRtY )
{
		*A_PtrRtX	=	m_MseX;
		*A_PtrRtY	=	m_MseY;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::DvlConResX( __u8 A_ConIdx )
{
		return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::DvlConResY( __u8 A_ConIdx )
{
		return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::PixelFormatGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgPrj )
		{
			Tv_Result	=	m_PtrCfgPrj->FmtFcc;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::ConsoleRegist( Cls_GrP3ConBase* A_ObjCon, __u8 A_Idx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check index
		if ( E_GrP3MngConMaxCnt > A_Idx )
		{
			m_ConTbl[A_Idx]	=	A_ObjCon;
			m_ConTbl[A_Idx]->IndexSet( A_Idx );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::ConsoleUnregist( Cls_GrP3ConBase* A_ObjCon )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngConMaxCnt;Tv_WkIdx++ )
		{
			if ( A_ObjCon == m_ConTbl[Tv_WkIdx] )
			{
				// remove
				m_ConTbl[Tv_WkIdx]	=	NULL;
				Tv_Result	=	TRUE;
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3Mng::LcUiObjCreate( __u8 A_Class, __u16 A_ScptId, Ptr_GrRect A_PtrRc, Cls_GrP3DoBase* A_ObjPrnt )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		switch( A_Class )
		{
			case E_GrP3DoClassFrame:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoFrame( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassLabel:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoLbl( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassPrgsBarLoc:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoPrgsBarLoc( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassQuad:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoQuad( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassBtnPage:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoBtnPage( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassStatic:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoStatic( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassEdtStpStr:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoEdtStpStr( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassBtnLbl:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoBtnLbl( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinNumStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinNumStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinTxtStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinTxtStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassRelStc:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoRelStc( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinPageTxt:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinPageTxt( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinDdgtStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinDdgtStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassBtnStc:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoBtnStc( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassChkbSsb:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoChkbSsb( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassChkbSsm:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoChkbSsm( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSbarVar:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSbarVar( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSbarStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSbarStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassRbtnStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoRbtnStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassChkbSgb:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoChkbSgb( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassChkbSgm:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoChkbSgm( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinPnt:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinPnt( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinNumLoc:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinNumLoc( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinDdgtLoc:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinDdgtLoc( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinTxtLoc:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinTxtLoc( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassTimeCtrl:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoTimeCtrl( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassCalRec:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoCalRec( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassPrgsBarGvs:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoPrgsBarGvs( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassGridRscStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoGridRscStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassBtnRelStc:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoBtnRelStc( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassGridRstat:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoGridRstat( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassTimeLbl:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoTimeLbl( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassLbxCtlg:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoLbxCtlg( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinQuadStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinQuadStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassRbtnGvu:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoRbtnGvu( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassRbtnSsm:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoRbtnSsm( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassChkbLoc:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoChkbLoc( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassEdtLoc:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoEdtLoc( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinId:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinId( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassGridMtnStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoGridMtnStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassCalHday:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoCalHday( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassPnlPtz:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoPnlPtz( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinBtxtStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinBtxtStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassGridRrng:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoGridRrng( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassSpinValStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoSpinValStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassGridPushStp:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoGridPushStp( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;
			case E_GrP3DoClassGridPlayBar:
				Tv_Result	=	(Cls_GrP3DoBase*)new Cls_GrP3DoGridPlayBar( A_ScptId, A_PtrRc, A_ObjPrnt );
				break;

			default:
				DbgMsgPrint( "Cls_GrP3Mng::LcUiObjCreate - unknown class id ( %d )\n", A_Class );
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::LcUiDecSlave( Cls_GrP3DoBase* A_ObjPrnt, void* A_PtrScpt, BOOL8 A_IsWithPrnt )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	TV_Deced;			// local decode
		__u8*	Tv_PtrScpt;
		Ptr_GrP3ScptObjHd		TV_PtrStx;
		Ptr_GrP3ScptObjEnd	Tv_PtrEdx;
		Cls_GrP3DoBase*	Tv_Obj;
		St_GrRect	Tv_RcObj;
#ifdef GR_P3_EDITOR
		BOOL8	Tv_IsParaUpdt;
		__u16	Tv_ScptId;
#endif
	// code --------------------
		Tv_Result	=	0;
		TV_PtrStx	=	(Ptr_GrP3ScptObjHd)A_PtrScpt;
#ifdef GR_P3_EDITOR
		Tv_IsParaUpdt	=	TRUE;
#endif
		// check stx
		if ( E_GrP3ScptObjStx == TV_PtrStx->Stx )
		{
			if ( A_IsWithPrnt )
			{
				// parent parametter update mode
				Tv_Obj		=	A_ObjPrnt;
#ifdef GR_P3_EDITOR
				Tv_ScptId	=	A_ObjPrnt->ScptIdGet();
				if ( (E_GrP3ScptIdPageBase <= Tv_ScptId) && ((E_GrP3ScptIdPageBase + m_PageCnt) > Tv_ScptId) )
				{
					Tv_IsParaUpdt	=	FALSE;
				}
#endif
			}
			else
			{
				// create object
				Tv_Obj		=	NULL;
				Tv_RcObj.left		=	TV_PtrStx->Rect.left;
				Tv_RcObj.top		=	TV_PtrStx->Rect.top;
				Tv_RcObj.right	=	TV_PtrStx->Rect.right;
				Tv_RcObj.bottom	=	TV_PtrStx->Rect.bottom;

				Tv_Obj	=	LcUiObjCreate( TV_PtrStx->ClassId, TV_PtrStx->ScptId, &Tv_RcObj, A_ObjPrnt );
			}
			if ( NULL != Tv_Obj )
			{
				// update para
#ifdef GR_P3_EDITOR
				if ( Tv_IsParaUpdt )
				{
					Tv_Obj->RtlScptParaUpdt( TV_PtrStx );
				}
#else
				Tv_Obj->RtlScptParaUpdt( TV_PtrStx );
#endif
				// go next
				Tv_Result		=	sizeof(St_GrP3ScptObjHd) + (__u32)TV_PtrStx->ParaSize; 
				Tv_PtrScpt	=	(__u8*)( (__u32)TV_PtrStx + sizeof(St_GrP3ScptObjHd) + (__u32)TV_PtrStx->ParaSize ) ;
				while ( E_GrP3ScptObjStx == (*Tv_PtrScpt) )
				{
					// slave decode
					TV_Deced	=	LcUiDecSlave( Tv_Obj, Tv_PtrScpt, FALSE );
					if ( 0 == TV_Deced )
					{
						// error quit
						break;
					}
					// go next
					Tv_Result		=	Tv_Result + TV_Deced;
					Tv_PtrScpt	=	(__u8*)( (__u32)Tv_PtrScpt + TV_Deced );
				}
				// check edx
				Tv_PtrEdx	=	(Ptr_GrP3ScptObjEnd)Tv_PtrScpt;
				if ( E_GrP3ScptObjEdx != Tv_PtrEdx->Edx )
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcUiDecSlave - script ( %d ) edx error!  \n", A_ObjPrnt->ScptIdGet() );
				}
				// edx add
				Tv_Result	=	Tv_Result + sizeof(St_GrP3ScptObjEnd);
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcUiDecSlave - script ( %d ) stx error!\n" , A_ObjPrnt->ScptIdGet() );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::LcUiDecWindow( __u16 A_ScptId, __u8 A_ConIdx )
{
	// local -------------------
		__u32	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP3DoWin*	Tv_Win;
		Ptr_GrP3ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		__u8	Tv_ConIdx;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrP3ScptIdWinBase <= A_ScptId )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
			if ( m_WinCnt > Tv_SlotIdx )
			{
				// get script
				if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
				{
					// check privilege
					Tv_IsAble	=	TRUE;
					if ( 0 != m_PtrWinTbl[Tv_SlotIdx].Privilege )
					{
						Tv_IsAble	=	FALSE;
						// check privilege
						if ( 0 != (m_Gvs.LoginPriv & m_PtrWinTbl[Tv_SlotIdx].Privilege) )
						{
							Tv_IsAble	=	TRUE;
						}
#ifdef GR_P3_EDITOR
						if ( !((Cls_GrP3DvlMng*)this)->DvlIsDbgRun() )
						{
							Tv_IsAble	=	TRUE;
						}
#endif
					}
					if ( Tv_IsAble )
					{
						// script decode
						Tv_PtrScpt	=	(Ptr_GrP3ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
						// check script head
						if ( E_GrP3ScptObjStx == Tv_PtrScpt->Stx )
						{
							// check first window class type
							if ( E_GrP3DoClassWin == Tv_PtrScpt->ClassId )
							{
								// check script id
								if ( A_ScptId == Tv_PtrScpt->ScptId )
								{
									// get console
									Tv_ConIdx	=	A_ConIdx;
									if ( E_GrP3ConIdxFocus ==  Tv_ConIdx )
									{
										Tv_ConIdx	=	m_FcsConIdx;
									}
									// check console index
									if ( E_GrP3MngConMaxCnt > Tv_ConIdx )
									{
										// check exist console
										if ( NULL != m_ConTbl[Tv_ConIdx] )
										{
											// check already exist
											if ( NULL == m_ConTbl[Tv_ConIdx]->WinFindByScptId( A_ScptId ) )
											{
												// create window
												Tv_RcObj.left		=	Tv_PtrScpt->Rect.left;
												Tv_RcObj.top		=	Tv_PtrScpt->Rect.top;
												Tv_RcObj.right	=	Tv_PtrScpt->Rect.right;
												Tv_RcObj.bottom	=	Tv_PtrScpt->Rect.bottom;
												Tv_Win	=	(Cls_GrP3DoWin*)new Cls_GrP3DoWin( A_ScptId, &Tv_RcObj, m_PtrWinTbl[Tv_SlotIdx].Layer,
													m_ConTbl[Tv_ConIdx] );
												// decode other option
												Tv_Result	=	LcUiDecSlave( (Cls_GrP3DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
												// call init
												Tv_Win->RtlInit();
												// request focus change
												m_ConTbl[Tv_ConIdx]->FocusReset();
												m_ConTbl[Tv_ConIdx]->FocusCorrect();
	#ifdef GR_P3_EDITOR
												DbgMsgPrint("Window ( %d ) decoded! size = %d byte.\n" , A_ScptId, Tv_Result );
	#endif
											}
											else
											{
												DbgMsgPrint( "Cls_GrP3Mng::LcUiDecWindow - window ( %d ) already exist!\n", A_ScptId );
											}
										}
										else
										{
											DbgMsgPrint( "Cls_GrP3Mng::LcUiDecWindow - console ( %d ) not exist!\n", Tv_ConIdx );
										}
									}
									else
									{
										DbgMsgPrint( "Cls_GrP3Mng::LcUiDecWindow - bad console ( %d ) index!\n", Tv_ConIdx );
									}
								}
								else
								{
									DbgMsgPrint( "Cls_GrP3Mng::LcUiDecWindow - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , A_ScptId  );
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP3Mng::LcUiDecWindow - bad script ( %d )  - class type ( %d ) missmatch!\n" , A_ScptId, Tv_PtrScpt->ClassId );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP3Mng::LcUiDecWindow - script ( %d ) error! : bad stx.\n" , A_ScptId );
						}	//	if ( E_GrP3ScptObjStx == Tv_PtrScpt->Stx )

					}
					else
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcUiDecWindow - script ( %d ) have not privilege ( 0x%04X -> 0x%04X )!.\n" , A_ScptId, m_PtrWinTbl[Tv_SlotIdx].Privilege, m_Gvs.LoginPriv );
						// event
						if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmWinPriv )
						{
							VmReqEvtCall( m_PtrCfgMdl->AsmWinPriv, NULL, NULL );
						}
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcUiDecWindow - script ( %d ) not exist .\n" , A_ScptId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcUiDecWindow - Window Id ( %d ) is out of range.\n" , A_ScptId );
			}
		}
	
		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP3ScptAsmInfo	Cls_GrP3Mng::AsmPtrGet( __u16 A_Id )
{
	// local -------------------
		Ptr_GrP3ScptAsmInfo	Tv_Result;
		__u16	Tv_AsmIdx;
	// code --------------------
		Tv_Result	=	NULL;

		// check exist code
		if ( 0 != m_AsmCnt )
		{
			Tv_AsmIdx	=	A_Id - E_GrP3AsmIdBase;
			if ( m_AsmCnt > Tv_AsmIdx )
			{
				Tv_Result	=	&m_PtrAsmTbl[Tv_AsmIdx];
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP3Mng::LcAsmArgLoad( __u8 A_Type, __s32 A_Para )
{
	// local -------------------
		__s32		Tv_Result;
		__u8	Tv_ArgAtb;
		void*	Tv_PtrArg;
		__u8	Tv_Size;
		__s32		Tv_Para;
	// code --------------------
		Tv_Result	=	0;
		Tv_ArgAtb	=	E_GrP3AsmArgTypeAtbMask & A_Type;
		Tv_Size		=	E_GrP3AsmArgTypeSzDword;
		// check constant
		if ( E_GrP3AsmArgTypeAtbConst == Tv_ArgAtb )
		{
			Tv_Result	=	A_Para;
		}
		else
		{
			switch ( Tv_ArgAtb )
			{
				case	E_GrP3AsmArgTypeAtbReg:
					Tv_Para		=	A_Para & 0xFF;
					Tv_PtrArg	=	(void*)&m_VmTbl[m_VmIdx].RegIntTbl[Tv_Para];
#ifdef GR_P3_EDITOR
					//check para
					if ( (0 > Tv_Para) || (E_GrP3AsmMaxIntRegCnt <= Tv_Para) )
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgLoad - [ %4X : %4X ] register index over!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						Tv_PtrArg	=	NULL;
					}
#endif				
					break;
				case E_GrP3AsmArgTypeAtbGvs:
					Tv_Para	=	A_Para & 0xFFFF;
					Tv_PtrArg	=	(void*)( (__u32)&m_Gvs + (__u32)Tv_Para );
					Tv_Size		=	E_GrP3AsmArgTypeSzMask & A_Type;
#ifdef GR_P3_EDITOR
					if ( (0 > Tv_Para) || ( sizeof(m_Gvs) <= (__u32)Tv_Para) )
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgLoad - [ %4X : %4X ] global variable system offset over!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						Tv_PtrArg	=	NULL;
					}
#endif
					break;
				case E_GrP3AsmArgTypeAtbGvu:
					Tv_Para	=	A_Para & 0xFFFF;
					Tv_PtrArg	=	(void*)( &m_GvuTbl[Tv_Para] );
#ifdef GR_P3_EDITOR
					if ( (0 > Tv_Para) || ( E_GrP3MngGvuMaxCnt <= (__u32)Tv_Para) )
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgLoad - [ %4X : %4X ] global variable user offset over!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						Tv_PtrArg	=	NULL;
					}
#endif
					break;
				case E_GrP3AsmArgTypeAtbSetup:
					Tv_Para	=	A_Para & 0xFFFF;
					Tv_PtrArg	=	(void*)( (__u32)m_SdaBuf + (__u32)Tv_Para );
					Tv_Size		=	E_GrP3AsmArgTypeSzMask & A_Type;
#ifdef GR_P3_EDITOR
					if ( (0 > Tv_Para) || ( E_GrP3MngSdaSize <= (__u32)Tv_Para) )
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgLoad - [ %4X : %4X ] setup offset over!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						Tv_PtrArg	=	NULL;
					}
#endif
					break;
				default:
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgLoad - [ %4X : %4X ] bad attribute!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
					break;
			}
			if ( NULL != Tv_PtrArg )
			{
				//calculate size
				switch ( Tv_Size )
				{
					case E_GrP3AsmArgTypeSzByte:
						Tv_Result	=	(__s32)( *((__u8*)Tv_PtrArg) );
						break;
					case E_GrP3AsmArgTypeSzWord:
						Tv_Result	=	(__s32)( *((__s16*)Tv_PtrArg) );
						break;
					case E_GrP3AsmArgTypeSzDword:
						Tv_Result	=	*((__s32*)Tv_PtrArg);
						break;
					default:
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgLoad - [ %4X : %4X ] bad size!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						Tv_Result	=	0;
						break;
				}
			}
			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmArgStore( __u8 A_Type, __s32 A_Para, __s32 A_Val )
{
	// local -------------------
		__u8	Tv_ArgAtb;
		void*	Tv_PtrArg;
		__u8	Tv_Size;
		__u32	Tv_Para;
	// code --------------------
		Tv_ArgAtb	=	E_GrP3AsmArgTypeAtbMask & A_Type;
		Tv_Size		=	E_GrP3AsmArgTypeSzDword;
		// check constant
		if ( E_GrP3AsmArgTypeAtbConst == Tv_ArgAtb )
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgStore - [ %4X : %4X ] target can not be constant!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
		}
		else
		{
			switch ( Tv_ArgAtb )
			{
				case	E_GrP3AsmArgTypeAtbReg:
					Tv_Para	=	0xFF & (__u32)A_Para;			// get register value index
					Tv_PtrArg	=	(void*)&m_VmTbl[m_VmIdx].RegIntTbl[Tv_Para];
#ifdef GR_P3_EDITOR
					//check para
					if ( (0 > Tv_Para) || (E_GrP3AsmMaxIntRegCnt <= Tv_Para) )
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgStore - [ %4X : %4X ] register index over!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						Tv_PtrArg	=	NULL;
					}
#endif					
					break;
				case E_GrP3AsmArgTypeAtbGvs:
					Tv_PtrArg	=	NULL;
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgStore - [ %4X : %4X ] GVS can not write!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
					/*
					Tv_Para	=	0xFFFF & (__u32)A_Para;			// get position
					Tv_PtrArg	=	(void*)( (__u32)&m_Gvs + A_Para );
					Tv_Size		=	E_GrP3AsmArgTypeSzMask & A_Type;
#ifdef GR_P3_EDITOR
					if ( (0 > Tv_Para) || ( sizeof(m_Gvs) <= Tv_Para) )
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgStore - [ %4X : %4X ] global variable system offset over!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						Tv_PtrArg	=	NULL;
					}
#endif
					*/
					break;
				case E_GrP3AsmArgTypeAtbGvu:
					Tv_Para	=	0xFFFF & (__u32)A_Para;			// get position
					Tv_PtrArg	=	(void*)( &m_GvuTbl[Tv_Para] );
#ifdef GR_P3_EDITOR
					if ( (0 > Tv_Para) || ( E_GrP3MngGvuMaxCnt <= Tv_Para) )
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgStore - [ %4X : %4X ] global variable system offset over!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						Tv_PtrArg	=	NULL;
					}
#endif
					break;
				case E_GrP3AsmArgTypeAtbSetup:
					Tv_Para	=	0xFFFF & (__u32)A_Para;			// get position
					Tv_PtrArg	=	(void*)( (__u32)m_SdaBuf + Tv_Para );
					Tv_Size		=	E_GrP3AsmArgTypeSzMask & A_Type;
#ifdef GR_P3_EDITOR
					if ( (0 > Tv_Para) || ( E_GrP3MngSdaSize <= Tv_Para) )
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgStore - [ %4X : %4X ] setup offset over!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						Tv_PtrArg	=	NULL;
					}
#endif
					break;
				default:
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgStore - [ %4X : %4X ] bad attribute!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
					break;
			}
			//calculate size
			if ( NULL != Tv_PtrArg )
			{
				switch ( Tv_Size )
				{
					case E_GrP3AsmArgTypeSzByte:
						*((__s8*)Tv_PtrArg)		=	(__s8)A_Val;
						break;
					case E_GrP3AsmArgTypeSzWord:
						*((__s16*)Tv_PtrArg)		=	(__s16)A_Val;
						break;
					case E_GrP3AsmArgTypeSzDword:
						*((__s32*)Tv_PtrArg)	=	A_Val;
						break;
					default:
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmArgStore - [ %4X : %4X ] bad size!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						break;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpCond( Ptr_GrP3AsmOpCond A_PtrCode )
{
	// local -------------------
		__s32		Tv_ArgLeft;
		__s32		Tv_ArgRight;
		__u8	Tv_Flag;
	// code --------------------
		Tv_ArgLeft	=	LcAsmArgLoad( A_PtrCode->ArgTypeLeft, A_PtrCode->ArgLeftVal );
		Tv_ArgRight	=	LcAsmArgLoad( A_PtrCode->ArgTypeRight, A_PtrCode->ArgRightVal );

		Tv_Flag	=	0;
		switch ( A_PtrCode->CondCode )
		{
			case E_GrP3AsmCondTypeEq:
				if ( Tv_ArgLeft == Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP3AsmCondTypeNe:
				if ( Tv_ArgLeft != Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP3AsmCondTypeGt:
				if ( Tv_ArgLeft > Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP3AsmCondTypeLt:
				if ( Tv_ArgLeft < Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP3AsmCondTypeGe:
				if ( Tv_ArgLeft >= Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP3AsmCondTypeLe:
				if ( Tv_ArgLeft <= Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP3AsmCondTypeAnd:
				if ( (0 != Tv_ArgLeft) && (0 != Tv_ArgRight) )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP3AsmCondTypeOr:
				if ( (0 != Tv_ArgLeft) || (0 != Tv_ArgRight) )
				{
					Tv_Flag		=	1;
				}
				break;
			default:
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpCond - [ %4X : %4X ] unknown condition code!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
				break;
		}
#ifdef GR_P3_EDITOR
		if ( E_GrP3AsmFlagCnt <= A_PtrCode->FlagIdxRslt )
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpCond - [ %4X : %4X ] bad flag index!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
		}
#endif
		Tv_Flag	=	Tv_Flag << A_PtrCode->FlagIdxRslt;
		switch ( A_PtrCode->FlagUpdtType )
		{
			case E_GrP3AsmUpdtFgDirect:
				m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag & ( ~(1 << A_PtrCode->FlagIdxRslt) );
				m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag | Tv_Flag;
				break;
			case E_GrP3AsmUpdtFgOr:
				m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag | Tv_Flag;
				break;
			case E_GrP3AsmUpdtFgAnd:
				m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag & (~Tv_Flag);
				break;
			default:
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpCond - [ %4X : %4X ] bad flag update type!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpMove( Ptr_GrP3AsmOpMove A_PtrCode )
{
	// local -------------------
		__s32		Tv_ArgSr;
	// code --------------------
		Tv_ArgSr	=	LcAsmArgLoad( A_PtrCode->SrArgType, A_PtrCode->SrArgVal );
		LcAsmArgStore( A_PtrCode->TgArgType, A_PtrCode->TgArgVal, Tv_ArgSr );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpFlag( Ptr_GrP3AsmOpFlag A_PtrCode )
{
	// local -------------------
		__s32		Tv_FlagIdx;
	// code --------------------
		Tv_FlagIdx	=	LcAsmArgLoad( A_PtrCode->ArgType, A_PtrCode->ArgVal );
#ifdef GR_P3_EDITOR
		if ( (0 > Tv_FlagIdx) || (E_GrP3AsmFlagCnt <= Tv_FlagIdx)  )
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpFlag - [ %4X : %4X ] bad flag index!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
		}
#endif
		switch ( A_PtrCode->SetType )
		{
			case E_GrP3AsmSetFgClear:
				m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag & (~(1 << (__u8)Tv_FlagIdx));
				break;
			case E_GrP3AsmSetFgSet:
				m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag | (1 << (__u8)Tv_FlagIdx);
				break;
			case E_GrP3AsmSetFgReverse:
				m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag ^ (1 << (__u8)Tv_FlagIdx);
				break;
			default:
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpFlag - [ %4X : %4X ] bad flag setting type!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpCalc( Ptr_GrP3AsmOpCalc A_PtrCode )
{
	// local -------------------
		__s32		Tv_ArgSr;
		__s32		Tv_ArgTg;
	// code --------------------
		Tv_ArgSr	=	LcAsmArgLoad( A_PtrCode->SrArgType, A_PtrCode->SrArgVal );
		Tv_ArgTg	=	LcAsmArgLoad( A_PtrCode->TgArgType, A_PtrCode->TgArgVal );

		switch ( A_PtrCode->CalcCode )
		{
			case E_GrP3AsmCalcAdd:
				Tv_ArgTg	=	Tv_ArgTg + Tv_ArgSr;
				break;
			case E_GrP3AsmCalcSub:
				Tv_ArgTg	=	Tv_ArgTg - Tv_ArgSr;
				break;
			case E_GrP3AsmCalcMul:
				Tv_ArgTg	=	Tv_ArgTg * Tv_ArgSr;
				break;
			case E_GrP3AsmCalcDiv:
				if ( 0 != Tv_ArgSr )
				{
					Tv_ArgTg	=	Tv_ArgTg / Tv_ArgSr;
				}
				else
				{
					Tv_ArgTg	=	0;
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpCalc - divide by zero!\n" );
				}
				break;
			case E_GrP3AsmCalcRem:
				Tv_ArgTg	=	Tv_ArgTg % Tv_ArgSr;
				break;
			case E_GrP3AsmCalcAnd:
				Tv_ArgTg	=	Tv_ArgTg & Tv_ArgSr;
				break;
			case E_GrP3AsmCalcOr:
				Tv_ArgTg	=	Tv_ArgTg | Tv_ArgSr;
				break;
			case E_GrP3AsmCalcXor:
				Tv_ArgTg	=	Tv_ArgTg ^ Tv_ArgSr;
				break;
			case E_GrP3AsmCalcShl:
				Tv_ArgTg	=	Tv_ArgTg << Tv_ArgSr;
				break;
			case E_GrP3AsmCalcShr:
				Tv_ArgTg	=	Tv_ArgTg >> Tv_ArgSr;
				break;
			default:
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpCalc - [ %4X : %4X ] bad flag setting type!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
				break;
		}
		LcAsmArgStore( A_PtrCode->TgArgType, A_PtrCode->TgArgVal, Tv_ArgTg );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpNot( Ptr_GrP3AsmOpNot A_PtrCode )
{
	// local -------------------
		__s32		Tv_ArgSr;
	// code --------------------
		Tv_ArgSr	=	LcAsmArgLoad( A_PtrCode->SrArgType, A_PtrCode->SrArgVal );
		if ( A_PtrCode->IsLogical )
		{
			if ( 0 == Tv_ArgSr )
			{
				Tv_ArgSr	=	1;
			}
			else
			{
				Tv_ArgSr	=	0;
			}
		}
		else
		{
			Tv_ArgSr	=	~Tv_ArgSr;
		}
		LcAsmArgStore( A_PtrCode->TgArgType, A_PtrCode->TgArgVal, Tv_ArgSr );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpDirJump( Ptr_GrP3AsmOpDirJump A_PtrCode )
{
	// local -------------------
		Ptr_GrP3ScptAsmInfo	Tv_CodeInfo;
	// code --------------------
		Tv_CodeInfo	=	AsmPtrGet( A_PtrCode->FuncId );
		if(A_PtrCode->FuncId == 363)
		{
			__s32 i = 0;
		}
		if ( NULL != Tv_CodeInfo )
		{
			if ( A_PtrCode->IsCall )
			{
				// check stack full
				if ( E_GrP3AsmCallMaxCnt > m_VmTbl[m_VmIdx].Sp )
				{
					// insert stack
					m_VmTbl[m_VmIdx].CallStack[m_VmTbl[m_VmIdx].Sp].Cs	=	m_VmTbl[m_VmIdx].Cs;
					m_VmTbl[m_VmIdx].CallStack[m_VmTbl[m_VmIdx].Sp].Ip	=	m_VmTbl[m_VmIdx].Ip + 1;
					m_VmTbl[m_VmIdx].Sp ++;

					// change pointer
					m_PtrVmCs				=	(Ptr_GrP3AsmOpBase)Tv_CodeInfo->Pos;
					m_VmCsLim				=	Tv_CodeInfo->Cnt;
					m_VmTbl[m_VmIdx].Cs			=	A_PtrCode->FuncId;
					m_VmTbl[m_VmIdx].Ip			=	0;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpDirJump - [ %4X : %4X ] stack overflow.\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
					m_VmTbl[m_VmIdx].IsRun	=	FALSE;
				}
			}
			else
			{
				m_PtrVmCs		=	(Ptr_GrP3AsmOpBase)Tv_CodeInfo->Pos;
				m_VmCsLim		=	Tv_CodeInfo->Cnt;
				m_VmTbl[m_VmIdx].Cs			=	A_PtrCode->FuncId;
				m_VmTbl[m_VmIdx].Ip			=	0;
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpDirJump - [ %4X : %4X ] function %d not exist.\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->FuncId );
			m_VmTbl[m_VmIdx].IsRun	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpRefJump( Ptr_GrP3AsmOpRefJump A_PtrCode )
{
	// local -------------------
		Ptr_GrP3ScptAsmInfo	Tv_CodeInfo;
		__u16	Tv_FuncId;
	// code --------------------
		Tv_FuncId	=	(__u16)LcAsmArgLoad( A_PtrCode->ArgType, A_PtrCode->ArgVal );
		if ( 0x10000 > Tv_FuncId )
		{
			Tv_CodeInfo	=	AsmPtrGet( (__u16)Tv_FuncId );
			if ( NULL != Tv_CodeInfo )
			{
				if ( A_PtrCode->IsCall )
				{
					if ( E_GrP3AsmCallMaxCnt > m_VmTbl[m_VmIdx].Sp )
					{
						// insert stack
						m_VmTbl[m_VmIdx].CallStack[m_VmTbl[m_VmIdx].Sp].Cs	=	m_VmTbl[m_VmIdx].Cs;
						m_VmTbl[m_VmIdx].CallStack[m_VmTbl[m_VmIdx].Sp].Ip	=	m_VmTbl[m_VmIdx].Ip + 1;
						m_VmTbl[m_VmIdx].Sp ++;

						// change pointer
						m_PtrVmCs	=	(Ptr_GrP3AsmOpBase)Tv_CodeInfo->Pos;
						m_VmCsLim	=	Tv_CodeInfo->Cnt;
						m_VmTbl[m_VmIdx].Cs			=	Tv_FuncId;
						m_VmTbl[m_VmIdx].Ip			=	0;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpRefJump - [ %4X : %4X ] stack overflow.\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						m_VmTbl[m_VmIdx].IsRun	=	FALSE;
					}
				}
				else
				{
					m_PtrVmCs	=	(Ptr_GrP3AsmOpBase)Tv_CodeInfo->Pos;
					m_VmCsLim	=	Tv_CodeInfo->Cnt;
					m_VmTbl[m_VmIdx].Cs			=	Tv_FuncId;
					m_VmTbl[m_VmIdx].Ip			=	0;
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpRefJump - [ %4X : %4X ] function %d not exist.\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, Tv_FuncId );
				m_VmTbl[m_VmIdx].IsRun	=	FALSE;
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpRefJump - [ %4X : %4X ] function id %d is bad.\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, Tv_FuncId );
			m_VmTbl[m_VmIdx].IsRun	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpRelJump( Ptr_GrP3AsmOpRelJump A_PtrCode )
{
	// local -------------------
	// code --------------------
		if ( 0 == A_PtrCode->Count )
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpRelJump - [ %4X : %4X ] jump count zero!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
			m_VmTbl[m_VmIdx].IsRun	=	FALSE;
		}
		else
		{
			// check over range
			if ( A_PtrCode->IsRvs )
			{
				if ( A_PtrCode->Count > m_VmTbl[m_VmIdx].Ip )
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpRelJump - [ %4X : %4X ] jump count is over (reverse - %d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->Count );
					m_VmTbl[m_VmIdx].IsRun	=	FALSE;
				}
				else
				{
					m_VmTbl[m_VmIdx].Ip	=	m_VmTbl[m_VmIdx].Ip - A_PtrCode->Count;
				}
			}
			else
			{
				if ( (m_VmTbl[m_VmIdx].Ip + A_PtrCode->Count) >= m_VmCsLim )
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpRelJump - [ %4X : %4X ] jump count is over(normal - %d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->Count );
					m_VmTbl[m_VmIdx].IsRun	=	FALSE;
				}
				else
				{
					m_VmTbl[m_VmIdx].Ip	=	m_VmTbl[m_VmIdx].Ip + A_PtrCode->Count;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpOfsJump( Ptr_GrP3AsmOpOfsJump A_PtrCode )
{
	// local -------------------
	// code --------------------
		// check equal my position
		if ( A_PtrCode->Pos == m_VmTbl[m_VmIdx].Ip )
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpOfsJump - [ %4X : %4X ] jump to self!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
			m_VmTbl[m_VmIdx].IsRun	=	FALSE;
		}
		else
		{
			if ( A_PtrCode->Pos >= m_VmCsLim )
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpOfsJump - [ %4X : %4X ] jump position is over(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->Pos );
				m_VmTbl[m_VmIdx].IsRun	=	FALSE;
			}
			else
			{
				m_VmTbl[m_VmIdx].Ip	=	A_PtrCode->Pos;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtClr( Ptr_GrP3AsmOpTxtClear A_PtrCode )
{
	// local -------------------
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			GrStrClear( m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->RegIdx] ) ;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtClr - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtId( Ptr_GrP3AsmOpTxtIdGet A_PtrCode )
{
	// local -------------------
		__u16	Tv_TxtId;
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_TxtId	=	(__u16)LcAsmArgLoad( A_PtrCode->ArgType, A_PtrCode->ArgVal );
			GrStrWcat( m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], TextPtrGet( Tv_TxtId ), 255 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtId - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtDec( Ptr_GrP3AsmOpTxtDec A_PtrCode )
{
	// local -------------------
		__s32		Tv_Val;
		Def_WstrNum	Tv_StrNum;
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_Val	=	LcAsmArgLoad( A_PtrCode->ArgType, A_PtrCode->ArgVal );
			GrStrIntToWstr( Tv_StrNum, Tv_Val, (__u32)A_PtrCode->Digit, (BOOL8)A_PtrCode->IsFix, (BOOL8)A_PtrCode->IsSign );
			GrStrWcat( m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], Tv_StrNum, E_GrP3AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtDec - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtTime( Ptr_GrP3AsmOpTxtTime A_PtrCode )
{
	// local -------------------
		__s32		Tv_Val;
		Def_WstrTag	Tv_StrTime;
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_Val	=	LcAsmArgLoad( A_PtrCode->ArgType, A_PtrCode->ArgVal );
			GrStrTimeToWstr( Tv_StrTime, (Def_GrTime)Tv_Val,
				(BOOL8)A_PtrCode->IsNoTime, (BOOL8)A_PtrCode->IsNoDate, (BOOL8)A_PtrCode->IsNoSec, 
				A_PtrCode->DayGap, A_PtrCode->TimeGap, A_PtrCode->DivGap, !A_PtrCode->IsNoDiv );
			GrStrWcat( m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], Tv_StrTime, E_GrP3AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtTime - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtCmp( Ptr_GrP3AsmOpTxtCmp A_PtrCode )
{
	// local -------------------
		__u8	Tv_Treg1;
		__u8	Tv_Treg2;
		__u8	Tv_Flag;
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->Treg1Idx )
		{
			if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->Treg2Idx )
			{
				Tv_Treg1	=	A_PtrCode->Treg1Idx;
				Tv_Treg2	=	A_PtrCode->Treg2Idx;
				Tv_Flag		=	0;

				if ( 0 == GrStrWcmp( m_VmTbl[m_VmIdx].RegTxtTbl[Tv_Treg1], m_VmTbl[m_VmIdx].RegTxtTbl[Tv_Treg2], (BOOL8)A_PtrCode->IsCaseSense ) )
				{
					Tv_Flag		=	1;
				}
				
#ifdef GR_P3_EDITOR
				if ( E_GrP3AsmFlagCnt <= A_PtrCode->FlagIdxRslt )
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtCmp - [ %4X : %4X ] bad flag index!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
				}
#endif
				Tv_Flag	=	Tv_Flag << A_PtrCode->FlagIdxRslt;
				switch ( A_PtrCode->FlagUpdtType )
				{
					case E_GrP3AsmUpdtFgDirect:
						m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag & ( ~(1 << A_PtrCode->FlagIdxRslt) );
						m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag | Tv_Flag;
						break;
					case E_GrP3AsmUpdtFgOr:
						m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag | Tv_Flag;
						break;
					case E_GrP3AsmUpdtFgAnd:
						m_VmTbl[m_VmIdx].Flag	=	m_VmTbl[m_VmIdx].Flag & (~Tv_Flag);
						break;
					default:
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtCmp - [ %4X : %4X ] bad flag update type!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
						break;
				}
				
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtCmp - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->Treg2Idx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtCmp - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->Treg1Idx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtLen( Ptr_GrP3AsmOpTxtLen A_PtrCode )
{
	// local -------------------
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->TregIdx )
		{
			if ( E_GrP3AsmMaxIntRegCnt > A_PtrCode->RegIdx )
			{
				m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->RegIdx]	=	GrStrWlen( m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->TregIdx] );
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtLen - [ %4X : %4X ] invalid integer register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->RegIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtLen - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->TregIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtHex( Ptr_GrP3AsmOpTxtHex A_PtrCode )
{
	// local -------------------
		__u32	Tv_Val;
		Def_WstrNum	Tv_StrNum;
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_Val	=	(__u32)LcAsmArgLoad( A_PtrCode->ArgType, A_PtrCode->ArgVal );
			GrStrDwordToHexWstr( Tv_StrNum, Tv_Val, (__u32)A_PtrCode->Digit, (BOOL8)A_PtrCode->IsFix );
			GrStrWcat( m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], Tv_StrNum, E_GrP3AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtHex - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtCharAdd( Ptr_GrP3AsmOpTxtChar A_PtrCode )
{
	// local -------------------
		WCHAR	Tv_StrTxt[2];
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_StrTxt[0]	=	A_PtrCode->Char;
			Tv_StrTxt[1]	=	0;
			GrStrWcat( m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], Tv_StrTxt, E_GrP3AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtCharAdd - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtCopy( Ptr_GrP3AsmOpTxtCopy A_PtrCode )
{
	// local -------------------
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->SrRegIdx )
		{
			if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->TgRegIdx )
			{
				GrStrWcopy( m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->TgRegIdx], m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->SrRegIdx], E_GrP3AsmTxtCharCnt - 1 );
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtCopy - [ %4X : %4X ] invalid target text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->TgRegIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtCopy - [ %4X : %4X ] invalid source text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->SrRegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtSetSetup( Ptr_GrP3AsmOpTxtSetStp A_PtrCode )
{
	// local -------------------
		__u16	Tv_StpAdr;
		WCHAR*	Tv_StrStp;
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->TregIdx )
		{
			Tv_StpAdr	=	A_PtrCode->AdrSetup;
			if ( A_PtrCode->IsAdd )
			{
				if ( E_GrP3AsmMaxIntRegCnt > A_PtrCode->AddRegIdx )
				{
					Tv_StpAdr	=	(__u16)((__s32)Tv_StpAdr + m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->AddRegIdx]);
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtSetSetup - [ %4X : %4X ] invalid address add register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->TregIdx );
				}
			}
			Tv_StrStp	=	(WCHAR*)SetupDataPtrGet( Tv_StpAdr );
			GrStrWcopy(	Tv_StrStp,m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->TregIdx] );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtSetSetup - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->TregIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpTxtGetSetup( Ptr_GrP3AsmOpTxtGetStp A_PtrCode )
{
	// local -------------------
		__u16	Tv_StpAdr;
		WCHAR*	Tv_StrStp;
	// code --------------------
		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->TregIdx )
		{
			Tv_StpAdr	=	A_PtrCode->AdrSetup;
			if ( A_PtrCode->IsAdd )
			{
				if ( E_GrP3AsmMaxIntRegCnt > A_PtrCode->AddRegIdx )
				{
					Tv_StpAdr	=	(__u16)((__s32)Tv_StpAdr + m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->AddRegIdx]);
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtGetSetup - [ %4X : %4X ] invalid address add register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->TregIdx );
				}
			}
			Tv_StrStp	=	(WCHAR*)SetupDataPtrGet( Tv_StpAdr );
			GrStrWcat(	m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->TregIdx], Tv_StrStp, E_GrP3AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpTxtSetSetup - [ %4X : %4X ] invalid text register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->TregIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpSetupCopy( Ptr_GrP3AsmOpSetupCopy A_PtrCode )
{
	// local -------------------
		__u16	Tv_AdrSr;
		__u16	Tv_AdrTg;
		void*	Tv_PtrSr;
		void*	Tv_PtrTg;
	// code --------------------
		Tv_AdrTg	=	A_PtrCode->AdrTg;
		Tv_AdrSr	=	A_PtrCode->AdrSr;

		if ( A_PtrCode->IsTgAdd )
		{
			if ( E_GrP3AsmMaxIntRegCnt > A_PtrCode->TgRegIdx )
			{
				Tv_AdrTg	=	(__u16)((__s32)Tv_AdrTg + m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->TgRegIdx]);
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpSetupCopy - [ %4X : %4X ] invalid target register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->TgRegIdx );
			}
		}

		if ( A_PtrCode->IsSrAdd )
		{
			if ( E_GrP3AsmMaxIntRegCnt > A_PtrCode->SrRegIdx )
			{
				Tv_AdrSr	=	(__u16)((__s32)Tv_AdrSr + m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->SrRegIdx]);
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpSetupCopy - [ %4X : %4X ] invalid source register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->SrRegIdx );
			}
		}
		
		if ( (0 <= A_PtrCode->Size) && ( (Tv_AdrSr + A_PtrCode->Size) <= E_GrP3MngSdaSize) && ( (Tv_AdrTg + A_PtrCode->Size) <= E_GrP3MngSdaSize) )
		{
			Tv_PtrTg	=	SetupDataPtrGet( Tv_AdrTg );
			Tv_PtrSr	=	SetupDataPtrGet( Tv_AdrSr );
			GrDumyCopyMem( Tv_PtrTg, Tv_PtrSr, (__u32)A_PtrCode->Size );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpSetupFill( Ptr_GrP3AsmOpSetupFill A_PtrCode )
{
	// local -------------------
		__u16	Tv_AdrTg;
		void*	Tv_PtrTg;
	// code --------------------
		Tv_AdrTg	=	A_PtrCode->AdrSetup;
		if ( A_PtrCode->IsAdd )
		{
			if ( E_GrP3AsmMaxIntRegCnt > A_PtrCode->AddRegIdx )
			{
				Tv_AdrTg	=	(__u16)((__s32)Tv_AdrTg + m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->AddRegIdx]);
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpSetupFill - [ %4X : %4X ] invalid adder register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->AddRegIdx );
			}
		}
		if ( E_GrP3AsmMaxIntRegCnt > A_PtrCode->FillRegIdx )
		{
			if ( (0 != A_PtrCode->Size) && ((Tv_AdrTg + A_PtrCode->Size) <= E_GrP3MngSdaSize ) )
			{
				Tv_PtrTg	=	SetupDataPtrGet( Tv_AdrTg );
				GrDumyFillB( Tv_PtrTg, (__u32)A_PtrCode->Size, (__u8)m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->FillRegIdx] );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpSetupFill - [ %4X : %4X ] invalid fill value register(%d)!\n", m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip, A_PtrCode->FillRegIdx );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpWindow( Ptr_GrP3AsmOpWindow A_PtrCode )
{
	// local -------------------
		__u16	Tv_ScptId;
		__u8	Tv_ConIdx;
	// code --------------------
		Tv_ScptId	=	A_PtrCode->ScptId;
		// get console index
		Tv_ConIdx	=	A_PtrCode->ConIdx;
		if ( E_GrP3ConIdxFocus == Tv_ConIdx )
		{
			Tv_ConIdx	=	m_FcsConIdx;
		}
		// check console index over
		if ( E_GrP3MngConMaxCnt > Tv_ConIdx )
		{
			// check exist console
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				LcUiDecWindow( Tv_ScptId, Tv_ConIdx );
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpWindow - console (%d) not exist.\n", Tv_ConIdx );
				RtlVmFault();
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpWindow - bad console index(%d).\n", Tv_ConIdx );
			RtlVmFault();
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpUpi( Ptr_GrP3AsmOpUpi A_PtrCode )
{
	// local -------------------
		__u16	Tv_ScptId;
		__u8	Tv_ConIdx;
		BOOL8	Tv_IsNxIp;
		BOOL8	Tv_IsAble;
		Cls_GrP3DoWin*	Tv_Win;
		Cls_GrP3DoBase*	Tv_Obj;
		WCHAR*	Tv_TregTbl[E_GrP3AsmMaxTxtRegCnt];
	// code --------------------
		Tv_IsNxIp	=	TRUE;
		// get console index
		Tv_ConIdx	=	A_PtrCode->ConIdx;
		// check console index over
		if ( E_GrP3MngConMaxCnt > Tv_ConIdx )
		{
			// check exist console
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				// get window
				Tv_ScptId	=	A_PtrCode->WinScptId;
				if ( E_GrP3ScptIdCrnt == Tv_ScptId )
				{
					Tv_Win	=	m_VmHostWin;
				}
				else
				{
					Tv_Win	=	(Cls_GrP3DoWin*)m_ConTbl[Tv_ConIdx]->WinFindByScptId( Tv_ScptId );
				}
				if ( NULL != Tv_Win )
				{
					// get object
					Tv_ScptId	=	A_PtrCode->ObjScptId;
					if ( E_GrP3ScptIdNone == Tv_ScptId )
					{
						Tv_Obj	=	(Cls_GrP3DoBase*)Tv_Win;
					}
					else if ( E_GrP3ScptIdCrnt == Tv_ScptId )
					{
						Tv_Obj	=	m_VmHostObj;
					}
					else
					{
						Tv_Obj	=	Tv_Win->FindSlaveObjByScptId( Tv_ScptId );
					}
					if ( NULL != Tv_Obj )
					{
						// move next ip
						m_VmTbl[m_VmIdx].Ip ++;
						Tv_IsNxIp	=	FALSE;

						// check check UPI able
						Tv_IsAble	=	TRUE;
						if ( (E_GrP3UpiFrmPageSet ==  A_PtrCode->UpiCmd) || (E_GrP3UpiFrmPageBack ==  A_PtrCode->UpiCmd) )
						{
							if ( NULL != m_VmHostObj )
							{
								if ( NULL != m_VmHostObj->FindPrntObjByScptId( Tv_Obj->ScptIdGet() ) )
								{
									Tv_IsAble	=	FALSE;
									DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpUpi - ( %d : %d ) try self destruction!\n", m_VmTbl[m_VmIdx].Cs , m_VmTbl[m_VmIdx].Ip - 1 );
								}
							}
						}
						
						// run command
						if ( Tv_IsAble )
						{
							Tv_TregTbl[0]	=	m_VmTbl[m_VmIdx].RegTxtTbl[0];
							Tv_TregTbl[1]	=	m_VmTbl[m_VmIdx].RegTxtTbl[1];
							Tv_TregTbl[2]	=	m_VmTbl[m_VmIdx].RegTxtTbl[2];
							Tv_TregTbl[3]	=	m_VmTbl[m_VmIdx].RegTxtTbl[3];
							Tv_Obj->RtlCommand( A_PtrCode->UpiCmd, m_VmTbl[m_VmIdx].RegIntTbl, Tv_TregTbl );
						}
						
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpUpi - host object (%d) not found.\n", Tv_ScptId );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpUpi - window (%d) not found.\n", Tv_ScptId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpUpi - console (%d) not exist.\n", Tv_ConIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpUpi - bad console index(%d).\n", Tv_ConIdx );
		}

		if ( Tv_IsNxIp )
		{
			m_VmTbl[m_VmIdx].Ip ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpNotify( Ptr_GrP3AsmOpNotify A_PtrCode )
{
	// local -------------------
		WCHAR*	Tv_StrCont;
		WCHAR*	Tv_StrTitle;
	// code --------------------
		// init
		m_VmTbl[m_VmIdx].RegIntTbl[0]	=	0;

		if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->ConTxtReg )
		{
			Tv_StrCont	=	m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->ConTxtReg];
			Tv_StrTitle	=	TextPtrGet( A_PtrCode->TitleTxtId );
			if ( NotifyBox( A_PtrCode->ConIdx, Tv_StrTitle, Tv_StrCont, A_PtrCode->Time ) )
			{
				// success
				m_VmTbl[m_VmIdx].RegIntTbl[0]	=	1;
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpNotify - invailid contents text register ( %d )!\n", A_PtrCode->ConTxtReg );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpApi( Ptr_GrP3AsmOpApi A_PtrCode )
{
	// local -------------------
		BOOL8	Tv_IsRun;
		__u16	TV_WkIdx;
		WCHAR*	Tv_RstrTbl[E_GrP3AsmMaxTxtRegCnt];
	// code --------------------
		Tv_IsRun	=	FALSE;

		for ( TV_WkIdx=0;TV_WkIdx < E_GrP3ApiCnt;TV_WkIdx++ )
		{
			if ( A_PtrCode->ApiCmd == (__u16)V_GrP3ApiCodeTbl[TV_WkIdx].Code )
			{
				Tv_RstrTbl[0]	=	m_VmTbl[m_VmIdx].RegTxtTbl[0];
				Tv_RstrTbl[1]	=	m_VmTbl[m_VmIdx].RegTxtTbl[1];
				Tv_RstrTbl[2]	=	m_VmTbl[m_VmIdx].RegTxtTbl[2];
				Tv_RstrTbl[3]	=	m_VmTbl[m_VmIdx].RegTxtTbl[3];
				V_GrP3ApiCodeTbl[TV_WkIdx].Fnc( m_VmTbl[m_VmIdx].RegIntTbl, Tv_RstrTbl );
				Tv_IsRun	=	TRUE;
				break;
			}
		}
		
		if ( !Tv_IsRun )
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpApi - unknown api code ( %d ) .\n" , A_PtrCode->ApiCmd );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcVmDoOneCmd( void )
{
	// local -------------------
		Ptr_GrP3AsmOpBase	Tv_PtrOpBase;
		BOOL8	Tv_IsAble;
		__u8	Tv_Code;
		__u8	Tv_Sp;
		__u8	Tv_VmIdx;
		Ptr_GrP3ScptAsmInfo	Tv_CodeInfo;

	// code --------------------
		// check run able
		if ( m_VmTbl[m_VmIdx].IsRun )
		{
			// check over IP
			if ( m_VmCsLim > m_VmTbl[m_VmIdx].Ip )
			{
				// prepare parse
				Tv_PtrOpBase	=	&m_PtrVmCs[m_VmTbl[m_VmIdx].Ip];
				// check flag
				Tv_IsAble	=	TRUE;
				if ( 0 != Tv_PtrOpBase->FlagAble )
				{
					if ( 0 == (m_VmTbl[m_VmIdx].Flag & Tv_PtrOpBase->FlagAble ) )
					{
						Tv_IsAble	=	FALSE;
					}
				}
				if ( 0 != (m_VmTbl[m_VmIdx].Flag & Tv_PtrOpBase->FlagBan) )
				{
					Tv_IsAble	=	FALSE;
				}
				// check dead command
				if ( 0 != (E_GrP3AsmOpDeactMask & Tv_PtrOpBase->Code) )
				{
					Tv_IsAble	=	FALSE;
				}
				// run
				Tv_VmIdx	=	m_VmIdx;
				if ( Tv_IsAble )
				{
					// parse command
					Tv_Code	=	Tv_PtrOpBase->Code & E_GrP3AsmOpCodeMask;
					switch ( Tv_Code )
					{
						case E_GrP3AsmOpNop:
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case	E_GrP3AsmOpEnd:
							m_VmTbl[Tv_VmIdx].IsRun	=	FALSE;
							// finish
							break;
						case E_GrP3AsmOpCond:
							LcAsmOpCond( (Ptr_GrP3AsmOpCond)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpMove:
							LcAsmOpMove( (Ptr_GrP3AsmOpMove)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpFlag:
							LcAsmOpFlag( (Ptr_GrP3AsmOpFlag)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpCalc:
							LcAsmOpCalc( (Ptr_GrP3AsmOpCalc)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpNot:
							LcAsmOpNot( (Ptr_GrP3AsmOpNot)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpDirJump:
							LcAsmOpDirJump( (Ptr_GrP3AsmOpDirJump)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpRefJump:
							LcAsmOpRefJump( (Ptr_GrP3AsmOpRefJump)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpRelJump:
							LcAsmOpRelJump( (Ptr_GrP3AsmOpRelJump)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpOfsJump:
							LcAsmOpOfsJump( (Ptr_GrP3AsmOpOfsJump)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpTxtClear:
							LcAsmOpTxtClr( (Ptr_GrP3AsmOpTxtClear)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtIdGet:
							LcAsmOpTxtId( (Ptr_GrP3AsmOpTxtIdGet)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtDec:
							LcAsmOpTxtDec( (Ptr_GrP3AsmOpTxtDec)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtHex:
							LcAsmOpTxtHex( (Ptr_GrP3AsmOpTxtHex)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtChar:
							LcAsmOpTxtCharAdd( (Ptr_GrP3AsmOpTxtChar)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtCopy:
							LcAsmOpTxtCopy( (Ptr_GrP3AsmOpTxtCopy)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtSetStp:
							LcAsmOpTxtSetSetup( (Ptr_GrP3AsmOpTxtSetStp)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtGetStp:
							LcAsmOpTxtGetSetup( (Ptr_GrP3AsmOpTxtGetStp)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpSetupCopy:
							LcAsmOpSetupCopy( (Ptr_GrP3AsmOpSetupCopy)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpSetupFill:
							LcAsmOpSetupFill( (Ptr_GrP3AsmOpSetupFill)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpWindow:
							LcAsmOpWindow( (Ptr_GrP3AsmOpWindow)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpUpi:
							LcAsmOpUpi( (Ptr_GrP3AsmOpUpi)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpApi:
							m_VmTbl[Tv_VmIdx].Ip	++;
							LcAsmOpApi( (Ptr_GrP3AsmOpApi)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpNotify:
							LcAsmOpNotify( (Ptr_GrP3AsmOpNotify)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpQuery:
							LcAsmOpQuery( (Ptr_GrP3AsmOpQuery)Tv_PtrOpBase );
							//m_VmTbl[m_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtTime:
							LcAsmOpTxtTime( (Ptr_GrP3AsmOpTxtTime)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtCmp:
							LcAsmOpTxtCmp( (Ptr_GrP3AsmOpTxtCmp)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpTxtLen:
							LcAsmOpTxtLen( (Ptr_GrP3AsmOpTxtLen)Tv_PtrOpBase );
							m_VmTbl[Tv_VmIdx].Ip	++;
							break;
						case E_GrP3AsmOpWait:
							LcAsmOpWait( (Ptr_GrP3AsmOpWait)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpLabel:
							LcAsmOpLabel( (Ptr_GrP3AsmOpLabel)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpLblJump:
							LcAsmOpLblJump( (Ptr_GrP3AsmOpLblJump)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpReturn:
							LcAsmOpReturn( Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpDcall:
							LcAsmOpDcall( (Ptr_GrP3AsmOpDcall)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpStpGet:
							LcAsmOpStpGet( (Ptr_GrP3AsmOpStpGet)Tv_PtrOpBase );
							break;
						case E_GrP3AsmOpStpPut:
							LcAsmOpStpPut( (Ptr_GrP3AsmOpStpPut)Tv_PtrOpBase );
							break;

						default:
							// bad opcode
							DbgMsgPrint( "Cls_GrP3Mng::LcVmDoOneCmd - bad opcode (%d) at cs: %d , ip: %d \n" , Tv_Code, m_VmTbl[m_VmIdx].Cs, m_VmTbl[m_VmIdx].Ip );
							m_VmTbl[m_VmIdx].IsRun	=	FALSE;
							break;
					}
				}
				else
				{
					// no execute
					m_VmTbl[m_VmIdx].Ip	++;
				}
			}

			// check finish
			if ( m_VmTbl[m_VmIdx].Ip >= m_VmCsLim )
			{
				// check stack
				if ( 0 != m_VmTbl[m_VmIdx].Sp )
				{
					// stack pop
					m_VmTbl[m_VmIdx].Sp --;
					// change pointer
					Tv_Sp	=	m_VmTbl[m_VmIdx].Sp;
					Tv_CodeInfo			=	AsmPtrGet( m_VmTbl[m_VmIdx].CallStack[Tv_Sp].Cs );
					m_PtrVmCs				=	(Ptr_GrP3AsmOpBase)Tv_CodeInfo->Pos;
					m_VmCsLim				=	Tv_CodeInfo->Cnt;
					m_VmTbl[m_VmIdx].Cs			=	m_VmTbl[m_VmIdx].CallStack[Tv_Sp].Cs;
					m_VmTbl[m_VmIdx].Ip			=	m_VmTbl[m_VmIdx].CallStack[Tv_Sp].Ip;
				}
				else
				{
					// finish
					m_VmTbl[m_VmIdx].IsRun	=	FALSE;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlVmRun( void )
{
	// local -------------------
	// code --------------------
		do 
		{
			LcVmDoOneCmd();
			// check opcode wait
			if ( m_VmTbl[m_VmIdx].IsOpWait )
			{
				break;
			}
			// check modal wait
			if ( m_VmTbl[m_VmIdx].IsModalWait )
			{
				break;
			}
			// check finish code
			if ( (!m_VmTbl[m_VmIdx].IsRun) && (0 != m_VmIdx) )
			{
				m_VmIdx --;
				LcVmPrepare( NULL );
				if ( m_VmTbl[m_VmIdx].IsModalWait )
				{
					break;
				}
			}
		} while ( m_VmTbl[m_VmIdx].IsRun );
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::VmRunCode( __u16 A_CodeId, Cls_GrP3DoBase* A_ObjHost, Ptr_GrP3MngVmRpara A_PtrRpara )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsAble;
		BOOL8	Tv_IsVmCreate;
		Cls_GrP3DoWin*	Tv_Win;
		__u8	Tv_VmOld;
	// code --------------------
		Tv_Result	=	FALSE;

		// check able page
		if ( E_GrP3AsmIdNone != A_CodeId )
		{
			Tv_IsVmCreate	=	FALSE;
			Tv_IsAble	=	TRUE;
			// check already run
			if ( m_VmTbl[m_VmIdx].IsRun )
			{
				Tv_IsAble	=	FALSE;
				if ( E_GrP3MngVmCnt > (m_VmIdx + 1) )
				{
					m_VmIdx ++;
					Tv_IsAble			=	TRUE;
					Tv_IsVmCreate	=	TRUE;
#ifdef GR_P3_EDITOR				
					DbgMsgPrint("Cls_GrP3Mng::VmRunCode - VM new created( %d )!\n", m_VmIdx );
#endif
				}
				else
				{
					DbgMsgPrint("Cls_GrP3Mng::VmRunCode - VM is full!\n");
				}
			}

			if ( Tv_IsAble )
			{
				// init
				Tv_Win		=	NULL;
				m_VmTbl[m_VmIdx].HostWinId	=	E_GrP3WinIdNone;
				m_VmTbl[m_VmIdx].HostScptId	=	E_GrP3ScptIdNone;
				if ( NULL != A_ObjHost )
				{
					// get window id
					Tv_Win	=	(Cls_GrP3DoWin*)A_ObjHost->WindowGet();
					m_VmTbl[m_VmIdx].HostWinId	=	Tv_Win->WinIdGet();
					if ( !A_ObjHost->IsWindow() )
					{
						m_VmTbl[m_VmIdx].HostScptId	=	A_ObjHost->ScptIdGet();
					}
				}
				m_VmTbl[m_VmIdx].Cs		=	A_CodeId;
				m_VmTbl[m_VmIdx].Ip		=	0;

				if ( LcVmPrepare( A_PtrRpara ) )
				{
					Tv_VmOld	=	m_VmIdx;
					RtlVmRun();
					if ( Tv_VmOld != m_VmIdx )
					{
						// in modal mode
						Tv_Result	=	TRUE;
					}
				}
				else
				{
					// create failure
					if ( Tv_IsVmCreate )
					{
						m_VmIdx --;
					}
				}
			}

		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrP3Mng::VmRegIntGet( __u8 A_RegIdx )
{
		return	m_VmTbl[m_VmIdx].RegIntTbl[A_RegIdx];
}
//--------------------------------------------------------------------
__u8	Cls_GrP3Mng::VmFlagGet( void )
{
		return	m_VmTbl[m_VmIdx].Flag;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::VmRegTxtGet( __u8 A_RegIdx, WCHAR* A_Str )
{
		if ( E_GrP3AsmMaxTxtRegCnt > A_RegIdx )
		{
			GrStrWcopy( A_Str, m_VmTbl[m_VmIdx].RegTxtTbl[A_RegIdx] );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::VmModalPop( __s32 A_ModalRslt )
{
	// local -------------------
	// code --------------------
		// unmark 
		m_VmTbl[m_VmIdx].IsModalWait	=	FALSE;		// unlock modal
		// m_VmTbl[m_VmIdx].RegIntTbl[0]	=	A_ModalRslt;	// release 
		// resume
		RtlVmRun();
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcTickMsgBrdcst( __u32 A_Tick )
{
	// local -------------------
		__u8	Tv_ConIdx;
		St_GrP3MsgTimeTick	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd		=	E_GrP3MsgCmdTickPeriod;
		Tv_Msg.Hd.WinId	=	E_GrP3WinIdAll;
		Tv_Msg.Hd.ObjId	=	E_GrP3ScptIdAll;
		Tv_Msg.Tick			=	A_Tick;
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP3MngConMaxCnt;Tv_ConIdx++ )
		{
			Tv_Msg.Hd.ConIdx	=	Tv_ConIdx;
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->RtlUiMsgProc( (Ptr_GrP3MsgHd)&Tv_Msg );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcConRedrawAll( void )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP3MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->PaintAllReq();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcWinAllFree( void )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP3MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->WindowAllFree();
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrP3Mng::LangSelGet( void )
{
		return	m_LangIdx;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LangSelSet( __u8 A_Idx )
{
		if ( E_GrP3ScptLangMaxCnt > A_Idx )
		{
			m_LangIdx	=	A_Idx;
			// redraw
			LcConRedrawAll();
		}
		else
		{
			DbgMsgPrint("Cls_GrP3Mng::LangSelSet - language ( %d ) range over!\n", (__s32)A_Idx );
		}
}
//--------------------------------------------------------------------
Cls_GrP3Font*	Cls_GrP3Mng::FontGet( __u8 A_Id )
{
	// local -------------------
		Cls_GrP3Font*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( E_GrP3MngFontMaxCnt > A_Id )
		{
			Tv_Result	=	m_FontMng[A_Id];
		}
		else
		{
			DbgMsgPrint("Cls_GrP3Mng::FontGet - font id ( %d ) range over!\n", (__s32)A_Id );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::CursorColorGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgRtl )
		{
			Tv_Result	=	m_PtrCfgRtl->Skin.ClrCus;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcVmReset( void )
{
	// local -------------------
	// code --------------------
		GrDumyZeroMem( m_VmTbl, sizeof(m_VmTbl) );

		m_VmIdx		=	0;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::LcVmPrepare( Ptr_GrP3MngVmRpara A_PtrRpara )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u8	Tv_IregIdx;
		Ptr_GrP3ScptAsmInfo	Tv_PtrInfo;
	// code --------------------
		Tv_Result	=	FALSE;
		m_PtrVmCs	=	NULL;
		m_VmCsLim	=	0;
		m_VmHostObj	=	NULL;
		m_VmHostWin	=	NULL;
		//m_VmTbl[m_VmIdx].IsRun	=	FALSE;
		Tv_PtrInfo	=	AsmPtrGet( m_VmTbl[m_VmIdx].Cs );
		if ( NULL != Tv_PtrInfo )
		{
			m_PtrVmCs	=	(Ptr_GrP3AsmOpBase)Tv_PtrInfo->Pos;
			if ( NULL != m_PtrVmCs )
			{
				m_VmCsLim	=	Tv_PtrInfo->Cnt;
				if ( E_GrP3WinIdNone != m_VmTbl[m_VmIdx].HostWinId )
				{
					if ( NULL != m_ConTbl[m_VmTbl[m_VmIdx].ConIdx] )
					{
						m_VmHostWin	=	m_ConTbl[m_VmTbl[m_VmIdx].ConIdx]->WinFindByWinId( m_VmTbl[m_VmIdx].HostWinId );
						if ( NULL != m_VmHostWin )
						{
							if ( E_GrP3ScptIdNone == m_VmTbl[m_VmIdx].HostScptId )
							{
								m_VmHostObj	=	(Cls_GrP3DoBase*)m_VmHostWin;
							}
							else
							{
								m_VmHostObj	=	m_VmHostWin->FindSlaveObjByScptId( m_VmTbl[m_VmIdx].HostScptId );
							}
						}
					}
				}

				// register parameter update
				if ( NULL != A_PtrRpara )
				{
					if ( E_GrP3MngVmRparaMax >= A_PtrRpara->Cnt )
					{
						for ( Tv_WkIdx=0;Tv_WkIdx < (__u8)A_PtrRpara->Cnt;Tv_WkIdx++ )
						{
							Tv_IregIdx	=	A_PtrRpara->Ridx[Tv_WkIdx];
							if ( E_GrP3AsmMaxIntRegCnt > Tv_IregIdx )
							{
								m_VmTbl[m_VmIdx].RegIntTbl[ Tv_IregIdx ]	=	A_PtrRpara->Rval[Tv_WkIdx];	// update value
							}
						}
					}
				}

				if ( !m_VmTbl[m_VmIdx].IsRun )
				{
					m_VmTbl[m_VmIdx].IsModalWait	=	FALSE;
					m_VmTbl[m_VmIdx].IsRun				=	TRUE;
				}
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint("Cls_GrP3Mng::LcVmPrepare - ASM function ( %d ) not exist.\n", m_VmTbl[m_VmIdx].Cs );
			}
		}
		else
		{
			if ( 0 != m_VmTbl[m_VmIdx].Cs )
			{
				DbgMsgPrint("Cls_GrP3Mng::LcVmPrepare - ASM function ( %d ) not found.\n", m_VmTbl[m_VmIdx].Cs );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::SysInit( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Rint[2];
		__u8	Tv_WkIdx;
		__u8	Tv_Val;
		__u32	Tv_Speed;
		__u8	Tv_DataBit;
		__u8	Tv_StopBit;
		__u8	Tv_Parity;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsScptAble )
		{
			// GVS init
			m_Gvs.HddMaxCnt		=	m_PtrCfgMdl->MaxHddCnt;

			m_Gvs.RunDispRes	=	m_SdaBuf[E_GrP3CfgOfsDispRes];

			GrP3BiosIsLanOn( Tv_Rint, NULL );
			m_Gvs.IsLanOn		=	(BOOL8)Tv_Rint[0];	// LAN cable status initialize

			// audio
			m_Gvs.AdoLiveCh	=	0xFF;
			m_Gvs.AdoPlayCh	=	0xFF;
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
			{
				m_Gvs.AdoLiveVol[Tv_WkIdx]	=	50;
				m_Gvs.AdoPlayVol[Tv_WkIdx]	=	50;
			}

			// PTZ
			LcPtzSetup();

			// UART init
			if ( 0 != m_PtrCfgPrj->UartCnt )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < m_PtrCfgPrj->UartCnt;Tv_WkIdx++ )
				{
					if ( E_GrP3CfgUartMax <= Tv_WkIdx )
					{
						break;
					}
					Tv_Speed	=	9600;
					Tv_Val		=	m_SdaBuf[E_GrP3CfgOfsUartSpeed + Tv_WkIdx];
					if ( E_GrP3CfgUartSpeedCnt > Tv_Val )
					{
						Tv_Speed	=	V_GrP3UartSpeedTbl[Tv_Val];
					}
					Tv_DataBit	=	8;
					if ( 0 != m_SdaBuf[E_GrP3CfgOfsUartDataBit + Tv_WkIdx] )
					{
						Tv_DataBit	=	7;
					}
					Tv_StopBit	=	m_SdaBuf[E_GrP3CfgOfsUartStopBit + Tv_WkIdx];
					Tv_Parity		=	m_SdaBuf[E_GrP3CfgOfsUartParity + Tv_WkIdx];
#ifndef	GR_P3_EDITOR
					GrUartSetup( Tv_WkIdx, Tv_Speed, Tv_DataBit, Tv_Parity, Tv_StopBit );
#endif
				}
			}

			// setup assign
			GrP3ApiCfgAssign( Tv_Rint, NULL );

			// check exist script
			if ( m_IsScptAble )
			{
				// close all window
				LcWinAllFree();
				// check exist init function
				m_Gvs.PrgsInit	=	0;		// init progress
				// VM reset
				LcVmReset();
				m_VmTbl[m_VmIdx].Cs		=	m_PtrCfgMdl->AsmInit;
				m_VmTbl[m_VmIdx].Ip		=	0;
				if ( LcVmPrepare( NULL ) )
				{
					// run code
					RtlVmRun();
					Tv_Result	=	TRUE;
					m_SysStat	=	E_GrP3MngSysStatInit;
#ifdef GR_P3_EDITOR
					DbgMsgPrint("Cls_GrP3Mng::ScriptRun - Init function running...\n" );
#endif
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::ScriptRun - no script exist!\n" );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::SysStart( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_QuadMode;
		__u8	Tv_QuadPage;
		__s32		Tv_Rint[4];
	// code --------------------
		Tv_Result	=	FALSE;

		// check exist script
		if ( m_IsScptAble )
		{
			// check exist init function
			m_VmTbl[m_VmIdx].HostWinId	=	E_GrP3WinIdNone;
			m_VmTbl[m_VmIdx].HostScptId	=	E_GrP3ScptIdNone;
			m_VmTbl[m_VmIdx].Cs		=	m_PtrCfgMdl->AsmStart;
			m_VmTbl[m_VmIdx].Ip		=	0;
			if ( LcVmPrepare( NULL ) )
			{
				// run code
				RtlVmRun();
				Tv_Result	=	TRUE;
#ifdef GR_P3_EDITOR
				DbgMsgPrint("Cls_GrP3Mng::SysStart - start function running...\n" );
#endif
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::SysStart - no script exist!\n" );
		}

		// last emergency
		Tv_Rint[1]	=	E_GrP3MngRgstIdxEmgc;
		GrP3BiosRgstRead( Tv_Rint, NULL );
		if ( 0 != Tv_Rint[0] )
		{
			// run emergency
			RtlTryRecEmgc( TRUE );
		}

		// last quad restore
		Tv_Rint[1]	=	E_GrP3MngRgstIdxQuadMode;
		GrP3BiosRgstRead( Tv_Rint, NULL );
		Tv_QuadMode	=	(__u8)Tv_Rint[0];
		Tv_Rint[1]	=	E_GrP3MngRgstIdxQuadPage;
		GrP3BiosRgstRead( Tv_Rint, NULL );
		Tv_QuadPage	=	(__u8)Tv_Rint[0];
		SafeQuadModeSet( 0, Tv_QuadMode, Tv_QuadPage, 0 );

		// change mode
		m_SysStat	=	E_GrP3MngSysStatStart;
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP3Mng::VmValueGet( __u8 A_Type, __u32 A_Para )
{
	// local -------------------
	// code --------------------
		return	LcAsmArgLoad( A_Type, (__s32)A_Para );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::VmValueSet( __u8 A_Type, __u32 A_Para, __s32 A_Val )
{
	// local -------------------
	// code --------------------
		LcAsmArgStore( A_Type, A_Para, A_Val );
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3Mng::VmHostGet( void )
{
	// local -------------------
	// code --------------------
		return	m_VmHostObj;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LiveAtbSet( __u8 A_Ch, __u8 A_Atb )
{
		m_LiveAtbTbl[A_Ch]	=	A_Atb;
}
//--------------------------------------------------------------------
__u8*	Cls_GrP3Mng::LiveAtbTblPtrGet( void )
{
		return	m_LiveAtbTbl;
}
//--------------------------------------------------------------------
__u8*	Cls_GrP3Mng::PlayAtbTblPtrGet( void )
{
		return	m_PlayAtbTbl;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::IsPlayMode( __u8 A_ConIdx )
{
		return	m_Gvs.TblIsPlay[A_ConIdx];
}
//--------------------------------------------------------------------
__u8	Cls_GrP3Mng::VdoMaxChCntGet( void )
{
		return	m_Gvs.VdoMaxCh;
}
//--------------------------------------------------------------------
__u8	Cls_GrP3Mng::AdoMaxChCntGet( void )
{
		return	m_Gvs.AdoMaxCh;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::PageDec( __u16 A_PageId, Cls_GrP3DoBase* A_Prnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_PgIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check page id range
		if ( (E_GrP3ScptIdPageBase <= A_PageId) && ((E_GrP3ScptIdPageBase + m_PageCnt) > A_PageId) )
		{
			Tv_PgIdx	=	A_PageId - E_GrP3ScptIdPageBase;
			// check exist script
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				if ( 0 != LcUiDecSlave( A_Prnt, m_PtrPageTbl[Tv_PgIdx].Pos, TRUE ) )
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3Mng::PageAsmOpenGet( __u16 A_PageId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_PgIdx;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		// check page id range
		if ( (E_GrP3ScptIdPageBase <= A_PageId) && ((E_GrP3ScptIdPageBase + m_PageCnt) > A_PageId) )
		{
			Tv_PgIdx	=	A_PageId - E_GrP3ScptIdPageBase;
			// check exist script
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				Tv_Result	=	m_PtrPageTbl[Tv_PgIdx].AsmOpen;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3Mng::PageAsmCloseGet( __u16 A_PageId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_PgIdx;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		// check page id range
		if ( (E_GrP3ScptIdPageBase <= A_PageId) && ((E_GrP3ScptIdPageBase + m_PageCnt) > A_PageId) )
		{
			Tv_PgIdx	=	A_PageId - E_GrP3ScptIdPageBase;
			// check exist script
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				Tv_Result	=	m_PtrPageTbl[Tv_PgIdx].AsmClose;
			}
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
__u16	Cls_GrP3Mng::PageEscKeyFcsIdGet( __u16 A_PageId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_PgIdx;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		// check page id range
		if ( (E_GrP3ScptIdPageBase <= A_PageId) && ((E_GrP3ScptIdPageBase + m_PageCnt) > A_PageId) )
		{
			Tv_PgIdx	=	A_PageId - E_GrP3ScptIdPageBase;
			// check exist script
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				Tv_Result	=	m_PtrPageTbl[Tv_PgIdx].EscKeyFcsId;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP3Mng::VkbdTxtPageCntGet( __u8 A_Lang )
{
	// local -------------------
		__u8	Tv_LngIdx;
	// code --------------------
		Tv_LngIdx	=	m_LangIdx;
		if ( E_GrP3ScptLangMaxCnt > A_Lang )
		{
			Tv_LngIdx	=	A_Lang;
		}
		return	m_PtrCfgPrj->VkbdTbl[Tv_LngIdx].PageCnt;
}
//--------------------------------------------------------------------
WCHAR	Cls_GrP3Mng::VkbdTxtKeyCodeGet( __u8 A_Page, __u8 A_Key, __u8 A_Lang )
{
	// local -------------------
		__u8	Tv_LngIdx;
	// code --------------------
		Tv_LngIdx	=	m_LangIdx;
		if ( E_GrP3ScptLangMaxCnt > A_Lang )
		{
			Tv_LngIdx	=	A_Lang;
		}
		return	m_PtrCfgPrj->VkbdTbl[Tv_LngIdx].KeyTbl[A_Page][A_Key];
}
//--------------------------------------------------------------------
void*	Cls_GrP3Mng::SetupDataPtrGet( __u16 A_Ofs )
{
	// local -------------------
	// code --------------------
		return	(void*)( (__u32)m_SdaBuf + (__u32)A_Ofs );
}
//--------------------------------------------------------------------
void*	Cls_GrP3Mng::GvsPtrGet( void )
{
	// local -------------------
	// code --------------------
		return	(void*)&m_Gvs;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::VkbdTxtRun( Cls_GrP3DoBase* A_Host, WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd, BOOL8 A_IsAdvPasswd )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP3DoVkbdTxt*	Tv_Win;
		Ptr_GrP3ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		Cls_GrP3ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3ScptIdWinBase <= m_PtrCfgMdl->TxtVkbdId )
		{
			Tv_SlotIdx	=	m_PtrCfgMdl->TxtVkbdId - E_GrP3ScptIdWinBase;
			if ( m_WinCnt > Tv_SlotIdx )
			{
				// get script
				if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
				{
					// script decode
					Tv_PtrScpt	=	(Ptr_GrP3ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
					// check script head
					if ( E_GrP3ScptObjStx == Tv_PtrScpt->Stx )
					{
						// check first window class type
						if ( E_GrP3DoClassWin == Tv_PtrScpt->ClassId )
						{
							// check script id
							if ( m_PtrCfgMdl->TxtVkbdId == Tv_PtrScpt->ScptId )
							{
								// get console
								Tv_ObjCon	=	(Cls_GrP3ConBase*)A_Host->ConsoleGet();
								// check already exist
								if ( NULL == Tv_ObjCon->WinFindByScptId( m_PtrCfgMdl->TxtVkbdId ) )
								{
									// create window
									Tv_RcObj.left		=	Tv_PtrScpt->Rect.left;
									Tv_RcObj.top		=	Tv_PtrScpt->Rect.top;
									Tv_RcObj.right	=	Tv_PtrScpt->Rect.right;
									Tv_RcObj.bottom	=	Tv_PtrScpt->Rect.bottom;
									Tv_Win	=	(Cls_GrP3DoVkbdTxt*)new Cls_GrP3DoVkbdTxt( m_PtrCfgMdl->TxtVkbdId, 
										&Tv_RcObj, m_PtrWinTbl[Tv_SlotIdx].Layer,
										A_Host, A_PtrVal, A_MaxLen, A_IsPasswd, A_IsAdvPasswd );
									// decode other option
									LcUiDecSlave( (Cls_GrP3DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
									// call init
									Tv_Win->RtlInit();
									// request focus change
									Tv_ObjCon->FocusReset();
									Tv_ObjCon->FocusCorrect();
									// success
									Tv_Result	=	TRUE;
								}
								else
								{
									DbgMsgPrint( "Cls_GrP3Mng::VkbdTxtRun - window ( %d ) already exist!\n", m_PtrCfgMdl->TxtVkbdId );
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP3Mng::VkbdTxtRun - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , m_PtrCfgMdl->TxtVkbdId  );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP3Mng::VkbdTxtRun - bad script ( %d )  - class type ( %d ) missmatch!\n" , m_PtrCfgMdl->TxtVkbdId, Tv_PtrScpt->ClassId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3Mng::VkbdTxtRun - script ( %d ) error! : bad stx.\n" , m_PtrCfgMdl->TxtVkbdId );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::VkbdTxtRun - script ( %d ) not exist .\n" , m_PtrCfgMdl->TxtVkbdId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::VkbdTxtRun - Window Id ( %d ) is out of range.\n" , m_PtrCfgMdl->TxtVkbdId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::VkbdNumRun( Cls_GrP3DoBase* A_Host,  WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP3DoVkbdNum*	Tv_Win;
		Ptr_GrP3ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		Cls_GrP3ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3ScptIdWinBase <= m_PtrCfgMdl->NumVkbdId )
		{
			Tv_SlotIdx	=	m_PtrCfgMdl->NumVkbdId - E_GrP3ScptIdWinBase;
			if ( m_WinCnt > Tv_SlotIdx )
			{
				// get script
				if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
				{
					// script decode
					Tv_PtrScpt	=	(Ptr_GrP3ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
					// check script head
					if ( E_GrP3ScptObjStx == Tv_PtrScpt->Stx )
					{
						// check first window class type
						if ( E_GrP3DoClassWin == Tv_PtrScpt->ClassId )
						{
							// check script id
							if ( m_PtrCfgMdl->NumVkbdId == Tv_PtrScpt->ScptId )
							{
								// get console
								Tv_ObjCon	=	(Cls_GrP3ConBase*)A_Host->ConsoleGet();

								// check already exist
								if ( NULL == Tv_ObjCon->WinFindByScptId( m_PtrCfgMdl->NumVkbdId ) )
								{
									// create window
									Tv_RcObj.left		=	Tv_PtrScpt->Rect.left;
									Tv_RcObj.top		=	Tv_PtrScpt->Rect.top;
									Tv_RcObj.right	=	Tv_PtrScpt->Rect.right;
									Tv_RcObj.bottom	=	Tv_PtrScpt->Rect.bottom;
									Tv_Win	=	(Cls_GrP3DoVkbdNum*)new Cls_GrP3DoVkbdNum( m_PtrCfgMdl->NumVkbdId, 
										&Tv_RcObj, m_PtrWinTbl[Tv_SlotIdx].Layer,
										A_Host, A_PtrVal, A_MaxLen, A_IsPasswd );
									// decode other option
									LcUiDecSlave( (Cls_GrP3DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
									// call init
									Tv_Win->RtlInit();
									// requist focus change
									Tv_ObjCon->FocusReset();
									Tv_ObjCon->FocusCorrect();
									// success
									Tv_Result	=	TRUE;
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP3Mng::VkbdNumRun - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , m_PtrCfgMdl->NumVkbdId  );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP3Mng::VkbdNumRun - bad script ( %d )  - class type ( %d ) missmatch!\n" , m_PtrCfgMdl->NumVkbdId, Tv_PtrScpt->ClassId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3Mng::VkbdNumRun - script ( %d ) error! : bad stx.\n" , m_PtrCfgMdl->NumVkbdId );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::VkbdNumRun - script ( %d ) not exist .\n" , m_PtrCfgMdl->NumVkbdId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::VkbdNumRun - Window Id ( %d ) is out of range.\n" , m_PtrCfgMdl->NumVkbdId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::VkbdRun( Cls_GrP3DoBase* A_Host, WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd, 
	BOOL8 A_IsNumType, BOOL8 A_IsAdvPasswd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		if ( A_IsNumType )
		{
			Tv_Result	=	VkbdNumRun( A_Host, A_PtrVal, A_MaxLen, A_IsPasswd );
		}
		else
		{
			Tv_Result	=	VkbdTxtRun( A_Host, A_PtrVal, A_MaxLen, A_IsPasswd, A_IsAdvPasswd );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SysPrgsInitSet( __u8 A_Prgs )
{
	// local -------------------
	// code --------------------
		m_Gvs.PrgsInit	=	A_Prgs;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::IsUiEventAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 == m_EventLock )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::UiEventLock( void )
{
		m_EventLock ++;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::UiEventUnlock( void )
{
		m_EventLock --;
}
//--------------------------------------------------------------------
Ptr_GrP3ScptObjHd	Cls_GrP3Mng::LcGetWinScpt( __u8* A_PtrConIdx, __u16 A_ScptId, Ptr_GrRect A_PtrRtRc, __u8* A_PtrRtLayer )
{
	// local -------------------
		Ptr_GrP3ScptObjHd	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP3ConBase*	Tv_ObjCon;
		Ptr_GrP3ScptObjHd	Tv_PtrScpt;
	// code --------------------
		Tv_Result	=	NULL;

		// get console index
		if ( E_GrP3ConIdxFocus == *A_PtrConIdx )
		{
			*A_PtrConIdx	=	m_FcsConIdx;
		}

		// check console index over
		if ( E_GrP3MngConMaxCnt > *A_PtrConIdx )
		{
			// check exist console
			if ( NULL != m_ConTbl[*A_PtrConIdx] )
			{
				// init
				Tv_ObjCon	=	m_ConTbl[*A_PtrConIdx];

				if ( E_GrP3ScptIdWinBase <= A_ScptId )
				{
					Tv_SlotIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
					if ( m_WinCnt > Tv_SlotIdx )
					{
						// get script
						if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
						{
							// script decode
							Tv_PtrScpt	=	(Ptr_GrP3ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
							// check script head
							if ( E_GrP3ScptObjStx == Tv_PtrScpt->Stx )
							{
								// check first window class type
								if ( E_GrP3DoClassWin == Tv_PtrScpt->ClassId )
								{
									// check script id
									if ( A_ScptId == Tv_PtrScpt->ScptId )
									{
										// check already exist
										if ( NULL == Tv_ObjCon->WinFindByScptId( A_ScptId ) )
										{
											A_PtrRtRc->left		=	Tv_PtrScpt->Rect.left;
											A_PtrRtRc->top		=	Tv_PtrScpt->Rect.top;
											A_PtrRtRc->right	=	Tv_PtrScpt->Rect.right;
											A_PtrRtRc->bottom	=	Tv_PtrScpt->Rect.bottom;

											*A_PtrRtLayer		=	m_PtrWinTbl[Tv_SlotIdx].Layer;

											Tv_Result	=	Tv_PtrScpt;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpQuery( Ptr_GrP3AsmOpQuery A_PtrCode )
{
	// local -------------------
		__u8	Tv_ConIdx;
		__u8	Tv_Layer;
		__u16	Tv_ScptId;
		Cls_GrP3DoQuery*	Tv_Win;
		Ptr_GrP3ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		WCHAR*	Tv_StrCont;
		WCHAR*	Tv_StrTitle;
	// code --------------------
		// init
		m_VmTbl[m_VmIdx].RegIntTbl[0]	=	0;
		// check VM slot full
		if ( 0 == m_VmIdx )
		{
			// get console index
			Tv_ConIdx	=	A_PtrCode->ConIdx;

			if ( 0 == A_PtrCode->Type )
			{
				Tv_ScptId	=	m_PtrCfgMdl->QryYn0Id;

				if ( 1 == m_Gvs.MsgBoxMode )
				{
					Tv_ScptId	=	m_PtrCfgMdl->QryYn1Id;
				}
				else if ( 2 == m_Gvs.MsgBoxMode )
				{
					Tv_ScptId	=	m_PtrCfgMdl->QryYn2Id;
				}
				else if ( 3 == m_Gvs.MsgBoxMode )
				{
					Tv_ScptId	=	m_PtrCfgMdl->QryYn3Id;
				}
				//script error check
				if(0 == Tv_ScptId)
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpQuery - script id error ( %d )\n",  Tv_ScptId);
					Tv_ScptId	=	m_PtrCfgMdl->QryYn0Id;
				}
			}
			else
			{
				Tv_ScptId	=	m_PtrCfgMdl->QryYnc0Id;

				if ( 1 == m_Gvs.MsgBoxMode )
				{
					Tv_ScptId	=	m_PtrCfgMdl->QryYnc1Id;
				}
				else if ( 2 == m_Gvs.MsgBoxMode )
				{
					Tv_ScptId	=	m_PtrCfgMdl->QryYnc2Id;
				}
				else if ( 3 == m_Gvs.MsgBoxMode )
				{
					Tv_ScptId	=	m_PtrCfgMdl->QryYnc3Id;
				}
				//script error check
				if(0 == Tv_ScptId)
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpQuery - script id error ( %d )\n",  Tv_ScptId);
					Tv_ScptId	=	m_PtrCfgMdl->QryYnc0Id;
				}
			}

			Tv_PtrScpt	=	LcGetWinScpt( &Tv_ConIdx, Tv_ScptId, &Tv_RcObj, &Tv_Layer );
			if ( NULL != Tv_PtrScpt )
			{
				if ( E_GrP3AsmMaxTxtRegCnt > A_PtrCode->ConTxtReg )
				{
					// create window
					Tv_StrCont	=	m_VmTbl[m_VmIdx].RegTxtTbl[A_PtrCode->ConTxtReg];
					Tv_StrTitle	=	TextPtrGet( A_PtrCode->TitleTxtId );

					Tv_Win	=	(Cls_GrP3DoQuery*)new Cls_GrP3DoQuery( Tv_ScptId, 
						&Tv_RcObj, Tv_Layer, (void*)m_ConTbl[Tv_ConIdx],
						Tv_StrCont, Tv_StrTitle );
					// decode other option
					LcUiDecSlave( (Cls_GrP3DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
					// call init
					Tv_Win->RtlModalDoingSet( TRUE, &m_VmTbl[m_VmIdx].RegIntTbl[0] );
					Tv_Win->RtlInit();
					// request focus change
					m_ConTbl[Tv_ConIdx]->FocusReset();
					m_ConTbl[Tv_ConIdx]->FocusCorrect();
					// success
					m_VmTbl[m_VmIdx].IsModalWait	=	TRUE;		// mark modal wait
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpQuery - invailid contents text register ( %d )!\n", A_PtrCode->ConTxtReg );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpQuery - script ( %d ) not exist!\n", Tv_ScptId );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpQuery - Already modal mode in!\n" );
		}
		// increase ip
		m_VmTbl[m_VmIdx].Ip ++;

}
//--------------------------------------------------------------------
__u16	Cls_GrP3Mng::SoftCusImgIdGet( void )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP3ImgIdNone;
		if ( NULL != m_PtrCfgPrj )
		{
			Tv_Result	=	m_PtrCfgPrj->CusImgId;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::HwTypeGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgPrj )
		{
			Tv_Result	=	m_PtrCfgPrj->HwType;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrP3Mng::GvuTblGet( void )
{
		return	(void*)m_GvuTbl;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SysQuit( void )
{
	// local -------------------
	// code --------------------
		
		m_VmTbl[m_VmIdx].IsRun	=	FALSE;		// finish
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgFormat( Ptr_GrTaskMsgDiskFormat A_PtrMsg )
{
	// local -------------------
		St_GrP3MngVmRpara	Tv_Rpara;
	// code --------------------
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmFormat )
		{
			Tv_Rpara.Cnt	=	2;
			Tv_Rpara.Ridx[0]	=	0;
			Tv_Rpara.Rval[0]	=	(__s32)A_PtrMsg->IsSuccess;
			Tv_Rpara.Ridx[1]	=	1;
			Tv_Rpara.Rval[1]	=	(__s32)A_PtrMsg->DiskIdx;
			VmRunCode( m_PtrCfgMdl->AsmFormat, NULL, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlVmFault( void )
{
	// local -------------------
	// code --------------------
		// not implemented
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlCfgUpdate( void )
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
		__s32		Tv_Rint[2];
		__u16*	Tv_PtrPriv;
		__u32	Tv_Ofs;
		__u8	Tv_WkCh;
	// code --------------------
		if (E_GrP3MngSysStatNone != m_SysStat )
		{
			// patch configuration
			VmRunCode( m_PtrCfgMdl->AsmCfgPatch, NULL );

			// authority update
			if ( m_PtrCfgMdl->UserIdCnt > m_Gvs.LoginIdx )
			{
				Tv_Ofs					=	E_GrP3CfgOfsUserAuth + ( (__u32)m_Gvs.LoginIdx << 1 );
				Tv_PtrPriv			=	(__u16*)( (__u32)m_SdaBuf + Tv_Ofs );
				m_Gvs.LoginPriv	=	*Tv_PtrPriv;
			}
			else
			{
				m_Gvs.LoginPriv	=	0;
			}

			// check change language
			if ( m_LangIdx != m_SdaBuf[E_GrP3CfgOfsLang] )
			{
				// update language
				LangSelSet( m_SdaBuf[E_GrP3CfgOfsLang] );
			}

			// update motion
			for ( Tv_WkCh=0;Tv_WkCh < m_Gvs.VdoMaxCh;Tv_WkCh++ )
			{
				Tv_Rint[1]	=	(__s32)Tv_WkCh;
				GrP3BiosCfgMtnMapUpdt( Tv_Rint, NULL );
			}

			// update key buzzer
			m_IsKeyBuzRun	=	(BOOL8)m_SdaBuf[E_GrP3CfgOfsBtnBeep];

			// update alpha
			Tv_Rint[1]	=	(__s32)m_SdaBuf[E_GrP3CfgOfsOsdAlpha];
			GrP3ApiUiAlphaUpdt( Tv_Rint, NULL );

			// update mouse sensitivity
			LcMseSenUpdt();

			// update date format
			RtlDateDispFmtUpdt();

			// summer time update
			GrTimeSetDstTime( (BOOL8)m_SdaBuf[E_GrP3CfgOfsIsDst], 
				m_SdaBuf[E_GrP3CfgOfsDstStMon],m_SdaBuf[E_GrP3CfgOfsDstStWkTh],m_SdaBuf[E_GrP3CfgOfsDstStWkDay],m_SdaBuf[E_GrP3CfgOfsDstStHour],
				m_SdaBuf[E_GrP3CfgOfsDstEdMon],m_SdaBuf[E_GrP3CfgOfsDstEdWkTh],m_SdaBuf[E_GrP3CfgOfsDstEdWkDay],m_SdaBuf[E_GrP3CfgOfsDstEdHour] );

			// update covert
			LcCovertUpdt();

			// update PTZ
			LcPtzSetup();

			// controller setup
			LcSioSetup();

			// auto key lock
			LcAtLockSetup();

			// update REMOCON id
			m_Gvs.CrtRmtId	=	m_SdaBuf[E_GrP3CfgOfsRmtId];

			// notify setup change
			Tv_Msg.Cmd	=	E_GrTaskMsgSetupChg;
			GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlPlayModeSet( BOOL8 A_IsPlayMode, __u8 A_ConIdx )
{
	// local -------------------
		St_GrP3MsgSysStat	Tv_Msg;
		__u8	Tv_QuadMode;
	// code --------------------
		// check console number
		if ( E_GrP3MngConMaxCnt > A_ConIdx )
		{
			// check change
			if ( m_Gvs.TblIsPlay[A_ConIdx]	!=	A_IsPlayMode )
			{
				// update play mode
				m_Gvs.TblIsPlay[A_ConIdx]	=	A_IsPlayMode;
				if ( NULL != m_ConTbl[A_ConIdx] )
				{
					if ( A_IsPlayMode )
					{
						// update last live quad 
						m_ConTbl[A_ConIdx]->QuadModeGet( &m_ConMng[A_ConIdx].LastLiveQuadMode, &m_ConMng[A_ConIdx].LastLiveQuadPage );
						// update play quad
						Tv_QuadMode	=	E_GrDvrQuadMode4;
						if ( 16 <= m_Gvs.VdoMaxCh )
						{
							Tv_QuadMode	=	E_GrDvrQuadMode16;
						}
						else if ( 8 <= m_Gvs.VdoMaxCh )
						{
							Tv_QuadMode	=	E_GrDvrQuadMode9;
						}
						m_ConTbl[A_ConIdx]->QuadModeSet( Tv_QuadMode, 0 );
					}
					else
					{
						// restore live quad
						m_ConTbl[A_ConIdx]->QuadModeSet( m_ConMng[A_ConIdx].LastLiveQuadMode, m_ConMng[A_ConIdx].LastLiveQuadPage );
					}


					// update play mode
					// notify to console
					Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdSysStat;
					Tv_Msg.Hd.ConIdx	=	0;
					Tv_Msg.Hd.WinId		=	E_GrP3WinIdNone;
					Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdAll;
					Tv_Msg.Type				=	E_GrP3SysStatLivePlay;
					m_ConTbl[0]->RtlUiMsgProc( (Ptr_GrP3MsgHd)&Tv_Msg );

					// notify to window
					Tv_Msg.Hd.WinId		=	E_GrP3WinIdAll;
					//m_ConTbl[0]->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
					m_ConTbl[0]->RtlUiMsgProc( (Ptr_GrP3MsgHd)&Tv_Msg );
				}

				// model event
				if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmLivePlay )
				{
					VmRunCode( m_PtrCfgMdl->AsmLivePlay, NULL );
				}
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::RtlPlayModeSet - bad console number ( %d ) input.\n", A_ConIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgQuadChgReq( Ptr_GrTaskMsgQuadChgReq A_PtrMsg )
{
	// local -------------------
		St_GrP3MsgSysStat	Tv_Msg;
	// code --------------------

		Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdSysStat;
		Tv_Msg.Hd.ConIdx	=	A_PtrMsg->ConIdx;
		Tv_Msg.Hd.WinId		=	E_GrP3WinIdAll;
		Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdAll;
		Tv_Msg.Type				=	E_GrP3SysStatQuadMode;

		if ( E_GrP3MngConMaxCnt > A_PtrMsg->ConIdx )
		{
			if ( NULL != m_ConTbl[A_PtrMsg->ConIdx] )
			{
				// notify slave windows
				m_ConTbl[A_PtrMsg->ConIdx]->RtlUiMsgProc( (Ptr_GrP3MsgHd)&Tv_Msg );
				// notify quad message to console
				Tv_Msg.Hd.WinId		=	E_GrP3WinIdNone;
				Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdNone;
				m_ConTbl[A_PtrMsg->ConIdx]->RtlUiMsgProc( (Ptr_GrP3MsgHd)&Tv_Msg );
			}
		}

		
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcGvsReset( void )
{
	// local -------------------
	// code --------------------
		m_Gvs.VdoMaxCh	=	E_GrDvrMaxChCnt;
		m_Gvs.AdoMaxCh	=	E_GrDvrMaxChCnt;
		m_Gvs.DiChCnt		=	E_GrDvrMaxChCnt;
		m_Gvs.DoChCnt		=	E_GrDvrMaxChCnt;
		m_Gvs.LoginIdx	=	0;

}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SysBuildEnv( void )
{
	// local -------------------
		__s32	Tv_Rint[2];
	// code --------------------
		GrP3BiosCfgLoadFromRom( Tv_Rint, NULL );
}
//--------------------------------------------------------------------
__u16	Cls_GrP3Mng::VdoRecQtyGet( __u8 A_ResType, __u8 A_ResLvl, __u8 A_QtyLvl )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			if ( (E_GrP3VencResTypeCnt > A_ResType) && (E_GrP3VencResSubCnt > A_ResLvl) && (E_GrP3VencQtyCnt > A_QtyLvl) )
			{
				Tv_Result	=	m_PtrCfgMdl->VrecQt[A_ResType][A_ResLvl][A_QtyLvl];
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::VdoRecQtyGet - bad input value ( %d , %d , %d ).\n", A_ResType, A_ResLvl, A_QtyLvl );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3Mng::VdoNetQtyGet( __u8 A_ResType, __u8 A_ResLvl, __u8 A_QtyLvl )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			if ( (E_GrP3VencResTypeCnt > A_ResType) && (E_GrP3VencResSubCnt > A_ResLvl) && (E_GrP3VencQtyCnt > A_QtyLvl) )
			{
				Tv_Result	=	m_PtrCfgMdl->VnetQt[A_ResType][A_ResLvl][A_QtyLvl];
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::VdoNetQtyGet - bad input value ( %d , %d , %d ).\n", A_ResType, A_ResLvl, A_QtyLvl );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::CtlgLoad( __u8 A_Id, __s32* A_PtrParaTbl, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt )
{
	// local -------------------
		__s32	Tv_Rint[E_GrP3CtlgParaMax + 1];
		__u8	Tv_WkIdx;
	// code --------------------

		Tv_Rint[0]	=	NULL;
		// copy parameter value
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3CtlgParaMax;Tv_WkIdx++ )
		{
			Tv_Rint[Tv_WkIdx + 1]	=	A_PtrParaTbl[Tv_WkIdx];
		}

		switch ( A_Id )
		{
			case E_GrP3CtlgIdFwUpgd:
				GrP3BiosCtlgFwUpgdLoad( Tv_Rint, NULL );
				break;
			case E_GrP3CtlgIdCfg:
				GrP3BiosCtlgCfgLoad( Tv_Rint, NULL );
				break;
			case E_GrP3CtlgIdLog:
				GrP3BiosCtlgLogLoad( Tv_Rint, NULL );
				break;
		}

		if ( FALSE == Tv_Rint[0] )
		{
			*A_PtrRtNowCnt		=	0;
			*A_PtrRtTotalCnt	=	0;
			*A_PtrRtPageCnt		=	0;
		}
		else
		{
			*A_PtrRtNowCnt		=	Tv_Rint[1];
			*A_PtrRtTotalCnt	=	Tv_Rint[2];
			*A_PtrRtPageCnt		=	Tv_Rint[3];
		}
		
		return	(BOOL8)Tv_Rint[0];		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::CtlgPageNext( __u8 A_Id, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt, __u32* A_PtrRtPagePos )
{
	// local -------------------
		__s32	Tv_Rint[E_GrP3CtlgParaMax + 1];
	// code --------------------
		Tv_Rint[0]	=	FALSE;

		switch ( A_Id )
		{
			case E_GrP3CtlgIdFwUpgd:
				GrP3BiosCtlgFwUpgdNext( Tv_Rint, NULL );
				break;
			case E_GrP3CtlgIdCfg:
				GrP3BiosCtlgCfgNext( Tv_Rint, NULL );
				break;
			case E_GrP3CtlgIdLog:
				GrP3BiosCtlgLogNext( Tv_Rint, NULL );
				break;
		}
		if ( FALSE != Tv_Rint[0] )
		{
			*A_PtrRtNowCnt		=	Tv_Rint[1];
			*A_PtrRtTotalCnt	=	Tv_Rint[2];
			*A_PtrRtPageCnt		=	Tv_Rint[3];
			*A_PtrRtPagePos		=	Tv_Rint[4];
		}

		return	(BOOL8)Tv_Rint[0];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::CtlgPagePrev( __u8 A_Id, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt, __u32* A_PtrRtPagePos )
{
	// local -------------------
		__s32	Tv_Rint[E_GrP3CtlgParaMax + 1];
	// code --------------------
		Tv_Rint[0]	=	FALSE;

		switch ( A_Id )
		{
			case E_GrP3CtlgIdFwUpgd:
				GrP3BiosCtlgFwUpgdPrev( Tv_Rint, NULL );
				break;
			case E_GrP3CtlgIdCfg:
				GrP3BiosCtlgCfgPrev( Tv_Rint, NULL );
				break;
			case E_GrP3CtlgIdLog:
				GrP3BiosCtlgLogPrev( Tv_Rint, NULL );
				break;
		}
		if ( FALSE != Tv_Rint[0] )
		{
			*A_PtrRtNowCnt		=	Tv_Rint[1];
			*A_PtrRtTotalCnt	=	Tv_Rint[2];
			*A_PtrRtPageCnt		=	Tv_Rint[3];
			*A_PtrRtPagePos		=	Tv_Rint[4];
		}

		return	(BOOL8)Tv_Rint[0];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::CtlgItemValueGet( __u32 A_Hndl, __u8 A_ElemCode, __s32 A_AddVal, __s32* A_PtrRtVal )
{
	// local -------------------
		__s32	Tv_Rint[4];
		__u32	Tv_Id;
		__u32	Tv_ItmIdx;
	// code --------------------
		Tv_Rint[0]	=	FALSE;

		Tv_Id			=	(A_Hndl >> E_GrP3MngCtlgIdShft) & E_GrP3MngCtlgIdMask;
		Tv_ItmIdx	=	A_Hndl & E_GrP3MngCtlgValMask;

		Tv_Rint[1]	=	(__s32)Tv_ItmIdx;
		Tv_Rint[2]	=	(__s32)A_ElemCode;
		Tv_Rint[3]	=	A_AddVal;

		switch ( Tv_Id )
		{
			case E_GrP3CtlgIdFwUpgd:
				GrP3BiosCtlgFwUpgdItemValGet( Tv_Rint, NULL );
				break;
			case E_GrP3CtlgIdCfg:
				GrP3BiosCtlgCfgItemValGet( Tv_Rint, NULL );
				break;
			case E_GrP3CtlgIdLog:
				GrP3BiosCtlgLogItemValGet( Tv_Rint, NULL );
				break;
		}

		if ( FALSE != Tv_Rint[0] )
		{
			*A_PtrRtVal	=	Tv_Rint[1];
		}

		return	(BOOL8)Tv_Rint[0];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::CtlgItemStrGet( __u32 A_Hndl, __u8 A_ElemCode, __s32 A_AddVal, WCHAR* A_StrRt )
{
	// local -------------------
		__s32	Tv_Rint[4];
		__u32	Tv_Id;
		__u32	Tv_ItmIdx;
		WCHAR*	Tv_Rtxt[2];
	// code --------------------
		Tv_Rint[0]	=	FALSE;

		Tv_Id			=	(A_Hndl >> 24) & 0xFF;
		Tv_ItmIdx	=	A_Hndl & 0xFFFFFF;

		Tv_Rint[1]	=	(__s32)Tv_ItmIdx;
		Tv_Rint[2]	=	(__s32)A_ElemCode;
		Tv_Rint[3]	=	(__s32)A_AddVal;

		Tv_Rtxt[0]	=	A_StrRt;

		switch ( Tv_Id )
		{
			case E_GrP3CtlgIdFwUpgd:
				GrP3BiosCtlgFwUpgdItemStrGet( Tv_Rint, Tv_Rtxt );
				break;
			case E_GrP3CtlgIdCfg:
				GrP3BiosCtlgCfgItemStrGet( Tv_Rint, Tv_Rtxt );
				break;
			case E_GrP3CtlgIdLog:
				GrP3BiosCtlgLogItemStrGet( Tv_Rint, Tv_Rtxt );
				break;
		}

		return	(BOOL8)Tv_Rint[0];
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::UiAlpahSet( __u8 A_Value )
{
	// local -------------------
		__u8	Tv_AlpVal;
		__u32	Tv_Tmp;
	// code --------------------
		if ( A_Value != m_UiAlphaVal )
		{
			// check exist script
			if ( NULL != m_PtrCfgPrj )
			{
				// check able color format
				if (E_GrFccRGB32 == m_PtrCfgPrj->FmtFcc)
				{
					// make alpha value
					Tv_Tmp	=	((__u32)A_Value * 254) / 100;
					if ( 0 == Tv_Tmp )
					{
						Tv_Tmp	=	1;
					}
					else if ( 254 <= Tv_Tmp )
					{
						Tv_Tmp	=	254;
					}
					Tv_AlpVal	=	(__u8)Tv_Tmp;

					// update image
					LcImgAlphaChgRgb32( Tv_AlpVal );

					// update paint code
					LcPstAlphaChgRgb32( Tv_AlpVal );

				}
			}

			// update
			m_UiAlphaVal	=	A_Value;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcImgAlphaChgRgb32( __u8 A_AlphaVal )
{
	// local -------------------
		__u32	Tv_WkIdx;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		// normal image
		for(Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngImgScptMaxCnt;Tv_WkIdx++ )
		{
			Tv_PtrGdib	=	m_PtrImgTbl[Tv_WkIdx];
			if ( NULL != Tv_PtrGdib )
			{
				GrGdibAlphaChgRbg32( Tv_PtrGdib, A_AlphaVal );
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcPstAlphaChgRgb32( __u8 A_AlphaVal )
{
	// local -------------------
		__u16	Tv_PstIdx;
		Ptr_GrP3ScptPstData	Tv_PtrPst;
		__u8	Tv_CodeCnt;
		__u8	Tv_CodeIdx;
		__u8	Tv_ConIdx;
	// code --------------------
		if ( 0 != m_PstCnt )
		{
			for ( Tv_PstIdx=0;Tv_PstIdx < m_PstCnt;Tv_PstIdx++ )
			{
				Tv_PtrPst	=	PstDataGet( E_GrP3ScptPstIdBase + Tv_PstIdx, &Tv_CodeCnt );
				if ( (NULL != Tv_PtrPst) && (0 != Tv_CodeCnt) )
				{
					for ( Tv_CodeIdx=0;Tv_CodeIdx < Tv_CodeCnt;Tv_CodeIdx++ )
					{
						if (	(E_GrP3ScptPstTypeClrFill == Tv_PtrPst->Type) || (E_GrP3ScptPstTypeLine == Tv_PtrPst->Type) ||
									(E_GrP3ScptPstTypeText == Tv_PtrPst->Type) || (E_GrP3ScptPstTypeFont == Tv_PtrPst->Type))
						{
							if ( 0xFF != ((Ptr_GrImgFmtRGB32)(&Tv_PtrPst->Value))->A )
							{
								((Ptr_GrImgFmtRGB32)(&Tv_PtrPst->Value))->A	=	A_AlphaVal;
							}
						}
						
						// next
						Tv_PtrPst ++;
					}
				}
			}
		}

		// console redraw
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP3MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->PaintAllReq();
			}
		}
		
		
}
//--------------------------------------------------------------------
__u8	Cls_GrP3Mng::LongKeyTimeGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->LongKeyTime;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::IsAdvPassUse( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->IsPassChk;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP3Mng::HddMaxCntGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->MaxHddCnt;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcMseSenUpdt( void )
{
	// local -------------------
	// code --------------------
		switch ( m_SdaBuf[E_GrP3CfgOfsMseRes] )
		{
			case 0:
				m_MseSenSl	=	0;
				m_MseSenSr	=	2;
				break;
			case 1:
				m_MseSenSl	=	0;
				m_MseSenSr	=	1;
				break;
			case 2:
				m_MseSenSl	=	0;
				m_MseSenSr	=	0;
				break;
			case 3:
				m_MseSenSl	=	1;
				m_MseSenSr	=	0;
				break;
			case 4:
				m_MseSenSl	=	2;
				m_MseSenSr	=	0;
				break;
			default:
				m_MseSenSl	=	0;
				m_MseSenSr	=	0;
				break;
		}
		
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::FirmwareVerGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgPrj )
		{
			Tv_Result	=	m_PtrCfgPrj->FwVer;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::ModelNumGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->ModelNum;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlDateDispFmtUpdt( void )
{
	// local -------------------
		__u8	Tv_Type;
		St_GrP3MsgSysStat	Tv_Msg;
	// code --------------------
		Tv_Type	=	m_SdaBuf[E_GrP3CfgOfsDateFmt];

		if ( GrStrDateDispTypeGet() != Tv_Type )
		{
			// change date display type
			GrStrDateDispTypeSet( Tv_Type );
			// notify UI message
			Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdSysStat;
			Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxAll;
			Tv_Msg.Hd.WinId		=	E_GrP3WinIdAll;
			Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdAll;
			Tv_Msg.Type				=	E_GrP3SysStatTimeFmtChg;
			UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::NotifyBox( __u8 A_ConIdx, WCHAR* A_StrTitle, WCHAR* A_StrCont, __u8 A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ConIdx;
		__u16	Tv_SlotIdx;
		__u16	Tv_ScptId;
		Cls_GrP3DoNotify*	Tv_Win;
		Ptr_GrP3ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		Cls_GrP3ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_ConIdx	=	A_ConIdx;
		if ( E_GrP3ConIdxFocus == Tv_ConIdx )
		{
			Tv_ConIdx	=	m_FcsConIdx;
		}
		// check console index over
		if ( E_GrP3MngConMaxCnt > Tv_ConIdx )
		{
			// check exist console
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				// init
				Tv_ObjCon	=	m_ConTbl[Tv_ConIdx];
				Tv_ScptId	=	m_PtrCfgMdl->Notify0Id;
				if ( 1 == m_Gvs.MsgBoxMode )
				{
					Tv_ScptId	=	m_PtrCfgMdl->Notify1Id;
				}
				else if ( 2 == m_Gvs.MsgBoxMode )
				{
					Tv_ScptId	=	m_PtrCfgMdl->Notify2Id;
				}
				else if ( 3 == m_Gvs.MsgBoxMode )
				{
					Tv_ScptId	=	m_PtrCfgMdl->Notify3Id;
				}
				
				if ( E_GrP3ScptIdWinBase <= Tv_ScptId )
				{
					Tv_SlotIdx	=	Tv_ScptId - E_GrP3ScptIdWinBase;
					if ( m_WinCnt > Tv_SlotIdx )
					{
						// get script
						if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
						{
							// script decode
							Tv_PtrScpt	=	(Ptr_GrP3ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
							// check script head
							if ( E_GrP3ScptObjStx == Tv_PtrScpt->Stx )
							{
								// check first window class type
								if ( E_GrP3DoClassWin == Tv_PtrScpt->ClassId )
								{
									// check script id
									if ( Tv_ScptId == Tv_PtrScpt->ScptId )
									{
										// check already exist
										if ( NULL == Tv_ObjCon->WinFindByScptId( Tv_ScptId ) )
										{
											// create window
											Tv_RcObj.left		=	Tv_PtrScpt->Rect.left;
											Tv_RcObj.top		=	Tv_PtrScpt->Rect.top;
											Tv_RcObj.right	=	Tv_PtrScpt->Rect.right;
											Tv_RcObj.bottom	=	Tv_PtrScpt->Rect.bottom;

											Tv_Win	=	(Cls_GrP3DoNotify*)new Cls_GrP3DoNotify( Tv_ScptId, 
												&Tv_RcObj, m_PtrWinTbl[Tv_SlotIdx].Layer, (void*)Tv_ObjCon,
												A_StrCont, A_StrTitle, A_Time );
											// decode other option
											LcUiDecSlave( (Cls_GrP3DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
											// call init
											Tv_Win->RtlInit();
											// request focus change
											Tv_ObjCon->FocusReset();
											Tv_ObjCon->FocusCorrect();

											Tv_Result	=	TRUE;

										}
										else
										{
											DbgMsgPrint( "Cls_GrP3Mng::NotifyBox - window ( %d ) already exist!\n", Tv_ScptId );
										}
									}
									else
									{
										DbgMsgPrint( "Cls_GrP3Mng::NotifyBox - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , Tv_ScptId );
									}
								}
								else
								{
									DbgMsgPrint( "Cls_GrP3Mng::NotifyBox - bad script ( %d )  - class type ( %d ) missmatch!\n" , Tv_ScptId, Tv_PtrScpt->ClassId );
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP3Mng::NotifyBox - script ( %d ) error! : bad stx.\n" , Tv_ScptId );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP3Mng::NotifyBox - script ( %d ) not exist .\n" , Tv_ScptId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3Mng::NotifyBox - Window Id ( %d ) is out of range.\n" , Tv_ScptId );
					}
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::NotifyBox - console (%d) not exist.\n", Tv_ConIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::NotifyBox - bad console index(%d).\n", Tv_ConIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::NotifyBox( __u8 A_ConIdx, __u16 A_TxtIdTitle, __u16 A_TxtIdCont, __u8 A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_StrCont;
		WCHAR*	Tv_StrTitle;
	// code --------------------
		Tv_Result	=	FALSE;
		
		Tv_StrTitle	=	TextPtrGet( A_TxtIdTitle );
		Tv_StrCont	=	TextPtrGet( A_TxtIdCont );

		if ( NULL != Tv_StrCont )
		{
			Tv_Result	=	NotifyBox( A_ConIdx, Tv_StrTitle, Tv_StrCont, A_Time );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::NotifyBox - bad contents text.\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::RtlLogIn( __u8 A_Id, WCHAR* A_StrPass, WCHAR* A_StrDual )
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_StrPass;
		__u32	Tv_Ofs;
		__u16*	Tv_PtrPriv;
	// code --------------------
		// init
		Tv_Result		=	FALSE;
		if ( m_IsScptAble )
		{
			// check password count
			if ( m_PtrCfgMdl->UserIdCnt > A_Id )
			{
				// check master password
				if ( 0 == GrStrWcmp( A_StrPass, m_PtrCfgMdl->MasterPass, TRUE ) )
				{
					Tv_Result		=	TRUE;
				}
				else
				{
					Tv_Ofs			=	E_GrP3CfgOfsUserPass + ( (__u32)A_Id << 5 );
					Tv_StrPass	=	(WCHAR*)( (__u32)m_SdaBuf + Tv_Ofs );
					if ( 0 == GrStrWcmp( A_StrPass, Tv_StrPass, TRUE ) )
					{
						Tv_Result	=	TRUE;
						// check dual password mode
						if ( m_PtrCfgMdl->IsPassDual )
						{
							Tv_Result	=	FALSE;
							// check second password
							Tv_StrPass	=	(WCHAR*)( (__u32)Tv_StrPass + 256 );
							if ( 0 == GrStrWcmp( A_StrPass, Tv_StrPass, TRUE ) )
							{
								Tv_Result	=	TRUE;
							}
						}
					}
				}
			}
		}

		if ( Tv_Result )
		{
			// update login info
			m_Gvs.LoginIdx	=	A_Id;
			Tv_Ofs					=	E_GrP3CfgOfsUserAuth + ( (__u32)A_Id << 1 );
			Tv_PtrPriv			=	(__u16*)( (__u32)m_SdaBuf + Tv_Ofs );
			m_Gvs.LoginPriv	=	*Tv_PtrPriv;

			// call event
			LcDpcCodeRun( m_PtrCfgMdl->AsmUserChg );
		}

		// update covert
		LcCovertUpdt();
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlLogOut( void )
{
	// local -------------------
		//__s32		Tv_Val;
	// code --------------------
		// update login info
		m_Gvs.LoginIdx	=	0xFF;
		m_Gvs.LoginPriv	=	0;

		// debug
		//Tv_Val	=	LcAsmArgLoad( E_GrP3AsmArgTypeAtbGvs,  (__s32)(&((Ptr_GrP3MngGvs)0)->LoginIdx) );
		//DbgMsgPrint( "DAMGI - login index value = %d \n", Tv_Val );

		// call event
		LcDpcCodeRun( m_PtrCfgMdl->AsmUserChg );

		// update covert
		LcCovertUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::RtlPasswdRuleChk( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_StrPass;
		__u32	Tv_Ofs;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsScptAble )
		{
			if ( m_PtrCfgMdl->UserIdCnt > m_Gvs.LoginIdx )
			{
				Tv_Ofs			=	E_GrP3CfgOfsUserPass + ( (__u32)m_Gvs.LoginIdx << 5 );
				Tv_StrPass	=	(WCHAR*)( (__u32)m_SdaBuf + Tv_Ofs );
				if ( E_GrStrPasswdGood == GrStrRulePasswdWstr( Tv_StrPass ) )
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::IsEmgcLockAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->IsEmgcLock;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::IsRecEmgc( void )
{
		return	m_Gvs.IsEmgc;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlTryRecEmgc( BOOL8 A_IsOn )
{
	// local -------------------
		BOOL8	Tv_IsAble;
		St_GrP3MsgSysStat	Tv_Msg;
		__s32		Tv_Rint[5];
	// code --------------------
		Tv_IsAble	=	TRUE;
		if ( m_PtrCfgMdl->IsEmgcLock )
		{
			if ( (0 != m_SdaBuf[E_GrP3CfgOfsBtnEmgcLock]) && (A_IsOn) )
			{
				Tv_IsAble	=	FALSE;
			}
		}
		// check able
		if ( Tv_IsAble )
		{
			if ( A_IsOn != m_Gvs.IsEmgc )
			{
				m_Gvs.IsEmgc	=	A_IsOn;
				// notify change emergency
				Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdSysStat;
				Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxAll;
				Tv_Msg.Hd.WinId		=	E_GrP3WinIdAll;
				Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdNone;
				Tv_Msg.Type				=	E_GrP3SysStatRecEmgcChg;
				UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				// save registry
				Tv_Rint[1]	=	E_GrP3MngRgstIdxEmgc;
				Tv_Rint[2]	=	(__s32)A_IsOn;
				GrP3BiosRgstWrite( Tv_Rint, NULL );
				GrP3BiosRgstSave( Tv_Rint, NULL );
				// log
				Tv_Rint[1]	=	E_GrDvrLogTypeEmgcEd;
				if ( A_IsOn )
				{
					Tv_Rint[1]	=	E_GrDvrLogTypeEmgcSt;
				}
				Tv_Rint[2]	=	0xFFFF;
				Tv_Rint[3]	=	0;
				Tv_Rint[4]	=	0;
				GrP3BiosEvtLogAdd( Tv_Rint, NULL );
			}
		}
		else
		{
			// event emergency denial
			if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmDenEmgc )
			{
				VmRunCode( m_PtrCfgMdl->AsmDenEmgc, NULL, NULL );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::VmReqEvtCall( __u16 A_CodeId, Cls_GrP3DoBase* A_ObjHost, Ptr_GrP3MngVmRpara A_PtrRpara )
{
	// local -------------------
		St_GrP3EvtCallMsg	Tv_Msg;
		Cls_GrP3DoWin*	Tv_Win;
		Cls_GrP3ConBase*	Tv_Con;
	// code --------------------
		Tv_Msg.CodeId		=	A_CodeId;

		// object
		Tv_Msg.ConIdx		=	0;
		Tv_Msg.WinId		=	E_GrP3WinIdNone;
		Tv_Msg.ScptId		=	E_GrP3ScptIdNone;
		if ( NULL != A_ObjHost )
		{
			Tv_Con	=	(Cls_GrP3ConBase*)A_ObjHost->ConsoleGet();
			if ( NULL != Tv_Con )
			{
				Tv_Msg.ConIdx	=	Tv_Con->IndexGet();
				// get window id
				Tv_Win	=	(Cls_GrP3DoWin*)A_ObjHost->WindowGet();
				Tv_Msg.WinId	=	Tv_Win->WinIdGet();
				if ( !A_ObjHost->IsWindow() )
				{
					Tv_Msg.ScptId	=	A_ObjHost->ScptIdGet();
				}
			}
		}

		// parameter
		if ( NULL == A_PtrRpara )
		{
			GrDumyZeroMem( &Tv_Msg.Para, sizeof(St_GrP3MngVmRpara) );
		}
		else
		{
			GrDumyCopyMem( &Tv_Msg.Para, A_PtrRpara, sizeof(St_GrP3MngVmRpara) );
		}

		return	(BOOL8)m_TaskSvr->MsgExPost( E_GrTaskMsgUiEvt, &Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgEvtCall( Ptr_GrP3EvtCallMsg A_PtrMsg )
{
	// local -------------------
		Cls_GrP3DoWin*	Tv_Win;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		// find window
		Tv_Win		=	NULL;
		Tv_Obj		=	NULL;
		if ( E_GrP3WinIdNone != A_PtrMsg->WinId )
		{
			// find window
			Tv_Win	=	m_ConTbl[A_PtrMsg->ConIdx]->WinFindByWinId( A_PtrMsg->WinId );
			if ( E_GrP3ScptIdNone != A_PtrMsg->ScptId )
			{
				Tv_Obj	=	Tv_Win->FindObjByScptId( A_PtrMsg->ScptId );
			}
			else
			{
				Tv_Obj	=	(Cls_GrP3DoBase*)Tv_Win;
			}
		}
		VmRunCode( A_PtrMsg->CodeId, Tv_Obj, &A_PtrMsg->Para );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgFwUpgdDone( Ptr_GrTaskMsgFwUpgdEnd A_PtrMsg )
{
	// local -------------------
		
	// code --------------------
		// update error code
		m_Gvs.FwUpErr	=	A_PtrMsg->ErrCode;
		// do finish code
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmFwUpDone )
		{
			VmRunCode( m_PtrCfgMdl->AsmFwUpDone, NULL, NULL );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpWait( Ptr_GrP3AsmOpWait A_PtrCode )
{
	// local -------------------
		__u32	Tv_TickNow;
		__u32	Tv_Mili;
	// code --------------------
		// check first wait
		if ( 0 == m_VmTbl[m_VmIdx].WaitRamain )
		{
			// init
			m_VmTbl[m_VmIdx].WaitTick		=	GrTimeGetTick();
			m_VmTbl[m_VmIdx].WaitRamain	=	A_PtrCode->Time * E_GrTimeMiliOfSec;
			if ( 0 == m_VmTbl[m_VmIdx].WaitRamain )
			{
				// go next
				m_VmTbl[m_VmIdx].Ip ++;
			}
			else
			{
				m_VmTbl[m_VmIdx].IsOpWait	=	TRUE;
			}
		}
		else
		{
			Tv_TickNow	=	GrTimeGetTick();
			Tv_Mili	=	GrTimeTickDifMs( m_VmTbl[m_VmIdx].WaitTick , Tv_TickNow );
			if ( Tv_Mili >= m_VmTbl[m_VmIdx].WaitRamain )
			{
				m_VmTbl[m_VmIdx].WaitRamain	=	0;
				m_VmTbl[m_VmIdx].Ip ++;
				m_VmTbl[m_VmIdx].IsOpWait	=	FALSE;
			}
			else
			{
				m_VmTbl[m_VmIdx].WaitRamain	=	m_VmTbl[m_VmIdx].WaitRamain - Tv_Mili;
				m_VmTbl[m_VmIdx].WaitTick		=	Tv_TickNow;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SafeQuadModeSet( __u8 A_ConIdx, __u8 A_Mode, __u8 A_Page, __u32 A_Fcc )
{
	// local -------------------	
		St_GrP3MngMsgQuadModeSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdQuadModeSet;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.Mode					=	A_Mode;
		Tv_Msg.Page					=	A_Page;
		Tv_Msg.Fcc					=	A_Fcc;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SafeQuadModeNext(  __u8 A_ConIdx, __u32 A_Fcc )
{
	// local -------------------
		St_GrP3MngMsgQuadMovePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdQuadModeNext;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.Fcc					=	A_Fcc;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SafeQuadModePrev(  __u8 A_ConIdx, __u32 A_Fcc )
{
	// local -------------------
		St_GrP3MngMsgQuadMovePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdQuadModePrev;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.Fcc					=	A_Fcc;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SafeQuadPageNext( __u8 A_ConIdx, __u32 A_Fcc )
{
	// local -------------------
		St_GrP3MngMsgQuadMovePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdQuadPageNext;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.Fcc					=	A_Fcc;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::IsDispAtSeq( void )
{
		return	m_Gvs.IsAtSeq;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlDispAutoSeqSet( BOOL8 A_IsAtSeq )
{
		m_Gvs.IsAtSeq	=	A_IsAtSeq;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SafeQuadModeGet( __u8 A_ConIdx, __u8* A_PtrRtMode, __u8* A_PtrRtPage )
{
	// local -------------------
		St_GrP3MngMsgQuadModeGet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdQuadModeGet;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.PtrRtMode		=	A_PtrRtMode;
		Tv_Msg.PtrRtPage		=	A_PtrRtPage;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgNetOnOff( BOOL8 A_IsOn )
{
	// local -------------------
	// code --------------------
		m_Gvs.IsLanOn	=	A_IsOn;

		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmLanOnOff )
		{
			VmRunCode( m_PtrCfgMdl->AsmLanOnOff, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpLabel( Ptr_GrP3AsmOpLabel A_PtrCode )
{
		// go next
		m_VmTbl[m_VmIdx].Ip ++;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpLblJump( Ptr_GrP3AsmOpLblJump A_PtrCode )
{
	// local -------------------
		__u16	Tv_CdIdx;
		BOOL8	Tv_IsFound;
		Ptr_GrP3AsmOpLabel	Tv_PtrOpLbl;
	// code --------------------
		// find label
		if ( 0 != m_VmCsLim )
		{
			Tv_IsFound	=	FALSE;
			for ( Tv_CdIdx=0;Tv_CdIdx < m_VmCsLim;Tv_CdIdx++ )
			{
				// check label
				if ( E_GrP3AsmOpLabel == m_PtrVmCs[Tv_CdIdx].Code )
				{
					Tv_PtrOpLbl	=	(Ptr_GrP3AsmOpLabel)&m_PtrVmCs[Tv_CdIdx];
					if ( ( A_PtrCode->Name0 == Tv_PtrOpLbl->Name0 ) && ( A_PtrCode->Name1 == Tv_PtrOpLbl->Name1 ) )
					{
						// found
						m_VmTbl[m_VmIdx].Ip	=	Tv_CdIdx;
						Tv_IsFound	=	TRUE;
						break;
					}
				}
			}

			// check not found
			if ( !Tv_IsFound )
			{
				m_VmTbl[m_VmIdx].Ip ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpReturn( Ptr_GrP3AsmOpBase A_PtrCode )
{
	// local -------------------
	// code --------------------
		// go last line
		m_VmTbl[m_VmIdx].Ip	=	m_VmCsLim;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::KeyBlockSet( __u8 A_BlocKey )
{
		m_KeyBlock	=	A_BlocKey;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgPlayStatChg( void )
{
	// local -------------------
	// code --------------------
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmPlayStat )
		{
			VmRunCode( m_PtrCfgMdl->AsmPlayStat, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgUsbChg( void )
{
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmUsbChg )
		{
			VmRunCode( m_PtrCfgMdl->AsmUsbChg, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgCmsCliChg( void )
{
	// local -------------------
	// code --------------------
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmCmsChg )
		{
			VmRunCode( m_PtrCfgMdl->AsmCmsChg, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgBkupStart( void )
{
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmBkupStart )
		{
			VmRunCode( m_PtrCfgMdl->AsmBkupStart, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgBkupEnd( void )
{
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmBkupEnd )
		{
			VmRunCode( m_PtrCfgMdl->AsmBkupEnd, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgBkupErr( void )
{
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmBkupErr )
		{
			VmRunCode( m_PtrCfgMdl->AsmBkupErr, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgLogBkupStart( void )
{
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmLogStart )
		{
			VmRunCode( m_PtrCfgMdl->AsmLogStart, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgLogBkupEnd( void )
{
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmLogEnd )
		{
			VmRunCode( m_PtrCfgMdl->AsmLogEnd, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgLogBkupError( void )
{
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmLogErr )
		{
			VmRunCode( m_PtrCfgMdl->AsmLogErr, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcVlossUpdt( void )
{
	// local -------------------
		St_GrP3MsgSysStat	Tv_Msg;
	// code --------------------
		if ( m_VlossStat != m_Gvs.VlossMap )
		{
			// notify video loss
			Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdSysStat;
			Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxAll;
			Tv_Msg.Hd.WinId		=	E_GrP3WinIdAll;
			Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdAll;
			Tv_Msg.Type				=	E_GrP3SysStatVlossChg;
			UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			// update
			m_VlossStat	=	m_Gvs.VlossMap;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcCovertUpdt( void )
{
	// local -------------------
		__u16	Tv_CvtMap;
		__u16*	Tv_PtrWd;
	// code --------------------
		Tv_PtrWd	=	(__u16*)( (__u32)m_SdaBuf + E_GrP3CfgOfsCamCvtMap );
		Tv_CvtMap	=	*Tv_PtrWd;
		if ( m_PtrCfgMdl->IsCvtUsr )
		{
			if ( m_PtrCfgMdl->UserIdCnt > m_Gvs.LoginIdx )
			{
				Tv_PtrWd	=	(__u16*)( (__u32)m_SdaBuf + E_GrP3CfgOfsUserCovert );
				Tv_CvtMap	=	Tv_CvtMap | Tv_PtrWd[m_Gvs.LoginIdx];
			}
			else
			{
				Tv_CvtMap	=	0xFFFF;				 
			}
		}
		// update
		m_Gvs.CovertMap	=	Tv_CvtMap;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::PtzMngSet( Cls_GrPtzV2Mng* A_ObjMng )
{
		m_PtzMng	=	A_ObjMng;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcPtzSetup( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u32	Tv_Baud;
		__u8	Tv_DataBit;
		BOOL8	Tv_IsPanRvs;
		BOOL8	Tv_IsTiltRvs;
		__u32	Tv_TurIdx;
	// code --------------------
		if ( (NULL != m_PtzMng) && (0 != m_Gvs.VdoMaxCh) )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_Gvs.VdoMaxCh;Tv_WkIdx++ )
			{
				// get configuration
				Tv_Baud	=	9600;
				if ( E_GrP3CfgUartSpeedCnt > m_SdaBuf[E_GrP3CfgOfsPtzBaud + Tv_WkIdx] )
				{
					Tv_Baud	=	V_GrP3UartSpeedTbl[m_SdaBuf[E_GrP3CfgOfsPtzBaud + Tv_WkIdx]];
				}
				Tv_DataBit	=	8;
				if ( 0 != m_SdaBuf[E_GrP3CfgOfsPtzDataBit + Tv_WkIdx] )
				{
					Tv_DataBit	=	7;
				}
				Tv_IsPanRvs	=	FALSE;
				if ( 0 != (1 & m_SdaBuf[E_GrP3CfgOfsPtzAtb + Tv_WkIdx]) )
				{
					Tv_IsPanRvs	=	TRUE;
				}
				Tv_IsTiltRvs	=	FALSE;
				if ( 0 != (2 & m_SdaBuf[E_GrP3CfgOfsPtzAtb + Tv_WkIdx]) )
				{
					Tv_IsTiltRvs	=	TRUE;
				}
				m_PtzMng->SafeSetup( Tv_WkIdx, m_SdaBuf[E_GrP3CfgOfsPtzType + Tv_WkIdx],
					m_SdaBuf[E_GrP3CfgOfsPtzId + Tv_WkIdx], m_SdaBuf[E_GrP3CfgOfsPtzPort + Tv_WkIdx],
					Tv_Baud, Tv_DataBit, m_SdaBuf[E_GrP3CfgOfsPtzParity + Tv_WkIdx], 
					m_SdaBuf[E_GrP3CfgOfsPtzStopBit + Tv_WkIdx], 
					m_SdaBuf[E_GrP3CfgOfsPtzSpdPan + Tv_WkIdx], m_SdaBuf[E_GrP3CfgOfsPtzSpdTilt + Tv_WkIdx],
					m_SdaBuf[E_GrP3CfgOfsPtzSpdZoom + Tv_WkIdx], m_SdaBuf[E_GrP3CfgOfsPtzSpdFocus + Tv_WkIdx],
					Tv_IsPanRvs, Tv_IsTiltRvs, m_SdaBuf[E_GrP3CfgOfsPtzTourType + Tv_WkIdx]
					);
				// check DVR mode
				if ( 0 == m_SdaBuf[E_GrP3CfgOfsPtzTourType + Tv_WkIdx] )
				{
					// DVR tour mode
					Tv_TurIdx	=	E_GrP3CfgOfsPtzTourCfg + ((__u32)Tv_WkIdx * (12 * 4) );
					m_PtzMng->SafeTourSave( Tv_WkIdx, (Ptr_GrPtzV2TourItm)&m_SdaBuf[Tv_TurIdx], 12 );
				}
			}
		}
}
//--------------------------------------------------------------------
Cls_GrPtzV2Mng*	Cls_GrP3Mng::PtzMngGet( void )
{
		return	m_PtzMng;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3Mng::LcChMapToWstr( WCHAR* A_StrTg, __u16 A_ChMap )
{
	// local -------------------
		WCHAR*	Tv_Result;
		BOOL8	Tv_IsAddCom;
		__u16	Tv_WkIdx;
		__u16	Tv_Mask;
	// code --------------------
		Tv_Result		=	A_StrTg;
		Tv_IsAddCom	=	FALSE;

		Tv_Mask	=	1;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
		{
			if ( 0 != (Tv_Mask & A_ChMap) )
			{
				if ( Tv_IsAddCom )
				{
					Tv_Result[0]	=	0x2C;
					Tv_Result[1]	=	0;
					Tv_Result ++;
				}
				// digit 10
				if ( 10 <= (Tv_WkIdx + 1) )
				{
					Tv_Result[0]	=	0x31;
					Tv_Result[1]	=	(WCHAR)(0x30 + (Tv_WkIdx + 1 - 10));
					Tv_Result[2]	=	0;
					Tv_Result ++;
					Tv_Result ++;
				}
				else
				{
					Tv_Result[0]	=	(WCHAR)(0x30 + (Tv_WkIdx + 1));
					Tv_Result[1]	=	0;
					Tv_Result ++;
				}
				// next
				Tv_IsAddCom	=	TRUE;
			}
			// next
			Tv_Mask	=	Tv_Mask << 1;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3Mng::LcUserToWstr( WCHAR* A_StrTg, __u8 A_UserIdx )
{
	// local -------------------
		WCHAR*	Tv_Result;
		WCHAR*	Tv_StrSr;
	// code --------------------
		Tv_Result		=	A_StrTg;
		if ( m_PtrCfgMdl->UserIdCnt > A_UserIdx )
		{
			Tv_StrSr	=	(WCHAR*)( (__u32)m_SdaBuf + E_GrP3CfgOfsUserId + ( ((__u32)A_UserIdx) << 5 ) );
			Tv_Result	=	GrStrWcopy( Tv_Result, Tv_StrSr, 15 );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3Mng::LcIp4ToWstr( WCHAR* A_StrTg, __u32 A_Ip4 )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	A_StrTg;

		GrStrIntToWstr( Tv_Result, (__s32)( A_Ip4 & 0xFF ) );
		Tv_Result		=	GrStrAddWchar( Tv_Result, 0x2E );
		GrStrIntToWstr( Tv_Result, (__s32)( (A_Ip4 >> 8) & 0xFF ) );
		Tv_Result		=	GrStrAddWchar( Tv_Result, 0x2E );
		GrStrIntToWstr( Tv_Result, (__s32)( (A_Ip4 >> 16) & 0xFF ) );
		Tv_Result		=	GrStrAddWchar( Tv_Result, 0x2E );
		GrStrIntToWstr( Tv_Result, (__s32)( (A_Ip4 >> 24) & 0xFF ) );
		Tv_Result		=	GrStrAddWchar( Tv_Result, 0x20 );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::EvtLogToWstr( Ptr_GrDvrLogV2Item A_PtrLog, WCHAR* A_PtrRtStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_StrWk;
	// code --------------------
		Tv_Result	=	FALSE;

		GrStrClear( A_PtrRtStr );
		Tv_StrWk	=	A_PtrRtStr;

		// check exist script
		if ( m_IsScptAble )
		{
			Tv_Result	=	TRUE;
			switch ( A_PtrLog->Type )
			{
				case E_GrDvrLogTypeMotion:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdMotion ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
					break;
				case E_GrDvrLogTypeSensor:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdSensor ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
					break;
				case E_GrDvrLogTypeVdoLoss:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdVdoLoss ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
					break;
				case E_GrDvrLogTypeVdoRcvy:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdVdoRcvy ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
					break;
				case E_GrDvrLogTypePowerOn:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdPowerOn ) );
					break;
				case E_GrDvrLogTypePowerOff:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdPowerOff ) );
					break;
				case E_GrDvrLogTypeLogin:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdLogin ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para[0] );
					break;
				case E_GrDvrLogTypeLogout:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdLogout ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para[0] );
					break;
				case E_GrDvrLogTypeSetup:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdSetupChg ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					switch ( A_PtrLog->Para[0] )
					{
						case 1:
							// resolution
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdResolution ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
							break;
						case 2:
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdRecord ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
							break;
						case 3:
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdRecQuility ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
							break;
						case 4:
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdRecSpeed ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
							break;
						case 5:
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdRecPost ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
							break;
						case 6:
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdMotion ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
							break;
						case 7:
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdMotionMap ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
							break;
						case 8:
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdRecSchd ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
							break;
						case 9:
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdPassword ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para[1] );
							break;
						case 10:
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdTitle ) );
							Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
							Tv_StrWk	=	LcChMapToWstr( Tv_StrWk, A_PtrLog->ChMap );
							break;
					}
					break;
				case E_GrDvrLogTypeBackup:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdBackup ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdStart ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para[0] );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrTimeToWstr( Tv_StrWk, (Def_GrTime)A_PtrLog->Para[1] );
					break;
				case E_GrDvrLogTypeHddErr:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdHddErr ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x42 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Para[0] );
					break;
				case E_GrDvrLogTypeInSetup:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdSetupIn ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para[0] );
					break;
				case E_GrDvrLogTypeOutSetup:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdSetupOut ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para[0] );
					break;
				case E_GrDvrLogTypeBkupCh:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdBackup ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdEnd ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrTimeToWstr( Tv_StrWk, (Def_GrTime)A_PtrLog->Para[1] );
					break;
				case E_GrDvrLogTypeHddFormat:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdHddFormat ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Para[0] );
					break;
				case E_GrDvrLogTypeRamup:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdRamup ) );
					break;
				case E_GrDvrLogTypeRebtErr:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdHddErr ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x54 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Para[0] );
					break;
				case E_GrDvrLogTypeFsUpgrade:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdFileSystem ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdUpgrade ) );
					break;
				case E_GrDvrLogTypeFsDefect:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdFileSystem ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdChanged ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x46 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Para[0] );
					break;
				case E_GrDvrLogTypeFwUpgd:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdFirmWare ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdUpgrade ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Para[0] );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Para[1] );
					break;
				case E_GrDvrLogTypeNetDiscon:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdNetwork ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdDisconnect ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcIp4ToWstr( Tv_StrWk, A_PtrLog->Para[1] );
					break;
				case E_GrDvrLogTypeNetLogin:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdNetwork ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdConnect ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcIp4ToWstr( Tv_StrWk, A_PtrLog->Para[1] );
					break;
				case E_GrDvrLogTypeNetSearch:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdNetwork ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdSearch ) );
					break;
				case E_GrDvrLogTypeNetSetup:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdNetwork ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdSetupChg ) );
					break;
				case E_GrDvrLogTypeEmgcSt:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdEmergency ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdStart ) );
					break;
				case E_GrDvrLogTypeEmgcEd:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdEmergency ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdEnd ) );
					break;
				case E_GrDvrLogTypeBatErr:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdBatteryErr ) );
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlProcObjRelease( Cls_GrP3DoBase* A_Obj )
{
	// local -------------------
	// code --------------------
		if ( A_Obj == m_VmHostWin )
		{
			m_VmHostWin	=	NULL;
			m_VmHostObj	=	NULL;
		}
		
		if ( A_Obj == m_VmHostObj )
		{
			m_VmHostObj	=	NULL;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpDcall( Ptr_GrP3AsmOpDcall A_PtrCode )
{
	// local -------------------
	// code --------------------
		LcDpcCodeRun( A_PtrCode->CodeId );
		m_VmTbl[m_VmIdx].Ip ++;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpStpGet( Ptr_GrP3AsmOpStpGet A_PtrCode )
{
	// local -------------------
		__u32	Tv_Size;
		__u32	Tv_StpAdr;
		__u8	Tv_ValBuf[4];
		__u32	Tv_WkIdx;
	// code --------------------
		// check register index
		if ( (E_GrP3AsmMaxIntRegCnt > A_PtrCode->AdrReg) && (E_GrP3AsmMaxIntRegCnt > A_PtrCode->ValReg) )
		{
			// setup address value
			if ( E_GrP3AsmArgTypeSzDword >= A_PtrCode->Type )
			{
				Tv_Size			=	1 << A_PtrCode->Type;
				Tv_StpAdr		=	(__u32)m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->AdrReg];
				if ( E_GrP3MngSdaSize >= (Tv_StpAdr + Tv_Size) )
				{
					*((__u32*)Tv_ValBuf)	=	0;
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_Size;Tv_WkIdx++ )
					{
						Tv_ValBuf[Tv_WkIdx]	=	m_SdaBuf[Tv_StpAdr + Tv_WkIdx];
					}
					m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->ValReg]	=	*((__s32*)Tv_ValBuf);
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpStpGet - ( %d :%d ) bad setup address.\n", m_VmTbl[m_VmIdx].Cs , m_VmTbl[m_VmIdx].Ip );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpStpGet - ( %d :%d ) bad type value.\n", m_VmTbl[m_VmIdx].Cs , m_VmTbl[m_VmIdx].Ip );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpStpGet - ( %d :%d ) bad register index.\n", m_VmTbl[m_VmIdx].Cs , m_VmTbl[m_VmIdx].Ip );
		}
		m_VmTbl[m_VmIdx].Ip ++;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAsmOpStpPut( Ptr_GrP3AsmOpStpPut A_PtrCode )
{
	// local -------------------
		__u32	Tv_Size;
		__u32	Tv_StpAdr;
		__u8	Tv_ValBuf[4];
		__u32	Tv_WkIdx;
	// code --------------------
		// check register index
		if ( (E_GrP3AsmMaxIntRegCnt > A_PtrCode->AdrReg) && (E_GrP3AsmMaxIntRegCnt > A_PtrCode->ValReg) )
		{
			// setup address value
			if ( E_GrP3AsmArgTypeSzDword >= A_PtrCode->Type )
			{
				Tv_Size			=	1 << A_PtrCode->Type;
				Tv_StpAdr		=	(__u32)m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->AdrReg];
				if ( E_GrP3MngSdaSize >= (Tv_StpAdr + Tv_Size) )
				{
					*((__s32*)Tv_ValBuf)	=	m_VmTbl[m_VmIdx].RegIntTbl[A_PtrCode->ValReg];
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_Size;Tv_WkIdx++ )
					{
						m_SdaBuf[Tv_StpAdr + Tv_WkIdx]	=	Tv_ValBuf[Tv_WkIdx];
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpStpPut - ( %d :%d ) bad setup address.\n", m_VmTbl[m_VmIdx].Cs , m_VmTbl[m_VmIdx].Ip );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpStpPut - ( %d :%d ) bad type value.\n", m_VmTbl[m_VmIdx].Cs , m_VmTbl[m_VmIdx].Ip );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::LcAsmOpStpPut - ( %d :%d ) bad register index.\n", m_VmTbl[m_VmIdx].Cs , m_VmTbl[m_VmIdx].Ip );
		}
		m_VmTbl[m_VmIdx].Ip ++;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcDpcCodeRun( __u16 A_CodeId )
{
	// local -------------------
		St_GrP3MngMsgAsmDcall	Tv_Msg;
	// code --------------------
		if ( E_GrP3AsmIdNone != A_CodeId )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdAsmDcall;
			Tv_Msg.CodeId				=	A_CodeId;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3Mng::DpcUpiCall( __u8 A_ConIdx, __u16 A_WinScptId, __u16 A_ObjScptId, __u16 A_UpiCmd, 
	__s32 A_Para1, __s32 A_Para2, __s32 A_Para3, __s32 A_Para4, __s32 A_Para5, __s32 A_Para6 )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3MngMsgDpcUpi	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3MngConMaxCnt > A_ConIdx )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdDpcUpi;
			Tv_Msg.ConIdx				=	A_ConIdx;
			Tv_Msg.WinScptId		=	A_WinScptId;
			Tv_Msg.ObjScptId		=	A_ObjScptId;
			Tv_Msg.UpiCmd				=	A_UpiCmd;
			Tv_Msg.Para[1]			=	A_Para1;
			Tv_Msg.Para[2]			=	A_Para2;
			Tv_Msg.Para[3]			=	A_Para3;
			Tv_Msg.Para[4]			=	A_Para4;
			Tv_Msg.Para[5]			=	A_Para5;
			Tv_Msg.Para[6]			=	A_Para6;
			Tv_Result	=	(BOOL8)m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcDpcUpi( Ptr_GrP3MngMsgDpcUpi A_PtrMsg )
{
	// local -------------------
		Cls_GrP3DoWin*	Tv_Win;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		// check console
		if ( NULL != m_ConTbl[A_PtrMsg->ConIdx] )
		{
			// find window
			Tv_Win	=	m_ConTbl[A_PtrMsg->ConIdx]->WinFindByScptId( A_PtrMsg->WinScptId );
			if ( NULL != Tv_Win )
			{
				// find object
				Tv_Obj	=	Tv_Win->FindObjByScptId( A_PtrMsg->ObjScptId );
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->RtlCommand( A_PtrMsg->UpiCmd, A_PtrMsg->Para, NULL );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::EmailEvtToWstr( Ptr_GrDvrEmailEvt A_PtrEevt, WCHAR* A_PtrRtStr )
{
	// local -------------------
		WCHAR*	Tv_StrWk;
		__u32		Tv_ValDw;
		__s32			Tv_Rint[3];
		WCHAR*	Tv_Rtxt[2];
		__s32			Tv_Ch;
		WCHAR*	Tv_StrTmp;
		BOOL8		Tv_IsShow;
		WCHAR*	Tv_StrId;
	// code --------------------

		GrStrClear( A_PtrRtStr );
		Tv_StrWk	=	A_PtrRtStr;

		// IP
		Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdIp ) );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
		GrStrIntToWstr( Tv_StrWk, (__s32)((m_Gvs.V4Ip >> 0) & 0xFF) );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x2E );
		GrStrIntToWstr( Tv_StrWk, (__s32)((m_Gvs.V4Ip >> 8) & 0xFF) );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x2E );
		GrStrIntToWstr( Tv_StrWk, (__s32)((m_Gvs.V4Ip >> 16) & 0xFF) );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x2E );
		GrStrIntToWstr( Tv_StrWk, (__s32)((m_Gvs.V4Ip >> 24) & 0xFF) );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

		// PORT
		Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdPort ) );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
		Tv_ValDw	=	(__u32)m_SdaBuf[E_GrP3CfgOfsCmsPort] | (((__u32)(m_SdaBuf[E_GrP3CfgOfsCmsPort+1])) << 8);
		GrStrIntToWstr( Tv_StrWk, (__s32)Tv_ValDw );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

		// WEBCODE
		Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdWebcode ) );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
		Tv_Rint[1]		=	0;
		Tv_Rtxt[0]		=	Tv_StrWk;
		GrP3BiosWebCodeGet( Tv_Rint, Tv_Rtxt );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

		// separator
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
		Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

		// power on
		if ( 0 != A_PtrEevt->TimePwrOn )
		{
			Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdPowerOn ) );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
			GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->TimePwrOn );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}

		// emergency on
		if ( 0 != A_PtrEevt->TimeEmgcSt )
		{
			Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdEmergency ) );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
			Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdOn ) );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
			GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->TimeEmgcSt );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}

		// emergency off
		if ( 0 != A_PtrEevt->TimeEmgcEd )
		{
			Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdEmergency ) );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
			Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdOff ) );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
			GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->TimeEmgcEd );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}

		// motion
		Tv_IsShow	=	TRUE;
		for ( Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++ )
		{
			if ( 0 != A_PtrEevt->Mtn[Tv_Ch].Cnt )
			{
				// show head
				if ( Tv_IsShow )
				{
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdMotion ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

					Tv_IsShow	=	FALSE;
				}
				Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdChannel ) );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrIntToWstr( Tv_StrWk, Tv_Ch + 1 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x28 );
				GrStrIntToWstr( Tv_StrWk, A_PtrEevt->Mtn[Tv_Ch].Cnt );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x29 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->Mtn[Tv_Ch].LastTime );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			}
		}
		if ( !Tv_IsShow )
		{
			// separator
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}
		
		// sensor
		Tv_IsShow	=	TRUE;
		for ( Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++ )
		{
			if ( 0 != A_PtrEevt->Sen[Tv_Ch].Cnt )
			{
				// show head
				if ( Tv_IsShow )
				{
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdSensor ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

					Tv_IsShow	=	FALSE;
				}
				Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdChannel ) );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrIntToWstr( Tv_StrWk, Tv_Ch + 1 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x28 );
				GrStrIntToWstr( Tv_StrWk, A_PtrEevt->Sen[Tv_Ch].Cnt );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x29 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->Sen[Tv_Ch].LastTime );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			}
		}
		if ( !Tv_IsShow )
		{
			// separator
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}

		// video loss
		Tv_IsShow	=	TRUE;
		for ( Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++ )
		{
			if ( 0 != A_PtrEevt->Nsg[Tv_Ch].Cnt )
			{
				// show head
				if ( Tv_IsShow )
				{
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdVdoLoss ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

					Tv_IsShow	=	FALSE;
				}
				Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdChannel ) );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrIntToWstr( Tv_StrWk, Tv_Ch + 1 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x28 );
				GrStrIntToWstr( Tv_StrWk, A_PtrEevt->Nsg[Tv_Ch].Cnt );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x29 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->Nsg[Tv_Ch].LastTime );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			}
		}
		if ( !Tv_IsShow )
		{
			// separator
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}

		// password change
		Tv_IsShow	=	TRUE;
		for ( Tv_Ch=0;Tv_Ch < E_GrP3UserIdMax;Tv_Ch++ )
		{
			if ( 0 != A_PtrEevt->Pass[Tv_Ch].Cnt )
			{
				// show head
				if ( Tv_IsShow )
				{
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdPassword ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

					Tv_IsShow	=	FALSE;
				}
				Tv_StrTmp	=	(WCHAR*)( (__u32)m_SdaBuf + E_GrP3CfgOfsUserId + (__u32)(Tv_Ch * 32) );
				Tv_StrWk	=	GrStrWcopy( Tv_StrWk, Tv_StrTmp, 15 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x28 );
				GrStrIntToWstr( Tv_StrWk, A_PtrEevt->Pass[Tv_Ch].Cnt );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x29 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->Pass[Tv_Ch].LastTime );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			}
		}
		if ( !Tv_IsShow )
		{
			// separator
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}

		// HDD error
		Tv_IsShow	=	TRUE;
		for ( Tv_Ch=0;Tv_Ch < E_GrP3HddMaxCnt;Tv_Ch++ )
		{
			if ( 0 != A_PtrEevt->Herr[Tv_Ch].Cnt )
			{
				// show head
				if ( Tv_IsShow )
				{
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdHddErr ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

					Tv_IsShow	=	FALSE;
				}
				GrStrIntToWstr( Tv_StrWk, Tv_Ch );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x28 );
				GrStrIntToWstr( Tv_StrWk, A_PtrEevt->Herr[Tv_Ch].Cnt );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x29 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->Herr[Tv_Ch].LastTime );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			}
		}
		if ( !Tv_IsShow )
		{
			// separator
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}

		// network login
		Tv_IsShow	=	TRUE;
		Tv_StrId	=	(WCHAR*)&m_SdaBuf[E_GrP3CfgOfsUserId];
		for ( Tv_Ch=0;Tv_Ch < 8;Tv_Ch++ )
		{
			if ( 0 != A_PtrEevt->NetLogin[Tv_Ch].Cnt )
			{
				// show head
				if ( Tv_IsShow )
				{
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdConnect ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

					Tv_IsShow	=	FALSE;
				}

				GrStrWcopy( Tv_StrWk, Tv_StrId );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x28 );
				GrStrIntToWstr( Tv_StrWk, A_PtrEevt->NetLogin[Tv_Ch].Cnt );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x29 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->NetLogin[Tv_Ch].LastTime );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			}
			// next
			Tv_StrId	=	(WCHAR*)( (__u32)Tv_StrId + 32 );
		}
		if ( !Tv_IsShow )
		{
			// separator
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}

		// network disconnect
		Tv_IsShow	=	TRUE;
		Tv_StrId	=	(WCHAR*)&m_SdaBuf[E_GrP3CfgOfsUserId];
		for ( Tv_Ch=0;Tv_Ch < 8;Tv_Ch++ )
		{
			if ( 0 != A_PtrEevt->NetDiscon[Tv_Ch].Cnt )
			{
				// show head
				if ( Tv_IsShow )
				{
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP3ScptTxtIdDisconnect ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );

					Tv_IsShow	=	FALSE;
				}

				GrStrWcopy( Tv_StrWk, Tv_StrId );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x3A );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x28 );
				GrStrIntToWstr( Tv_StrWk, A_PtrEevt->NetDiscon[Tv_Ch].Cnt );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x29 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
				GrStrTimeToWstr( Tv_StrWk, A_PtrEevt->NetDiscon[Tv_Ch].LastTime );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
				Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
			}
			// next
			Tv_StrId	=	(WCHAR*)( (__u32)Tv_StrId + 32 );
		}
		if ( !Tv_IsShow )
		{
			// separator
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0D );
			Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x0A );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcAtLockSetup( void )
{
	// local -------------------
	// code --------------------
		switch ( m_SdaBuf[E_GrP3CfgOfsBtnAtLock] )
		{
			case 1:
				m_IsAtLock			=	TRUE;
				m_AtLockPeriod	=	100;
				m_AtLockTimer		=	100;
				break;
			case 2:
				m_IsAtLock			=	TRUE;
				m_AtLockPeriod	=	300;
				m_AtLockTimer		=	300;
				break;
			case 3:
				m_IsAtLock			=	TRUE;
				m_AtLockPeriod	=	600;
				m_AtLockTimer		=	600;
				break;
			default:
				m_IsAtLock			=	FALSE;
				m_AtLockPeriod	=	0;
				m_AtLockTimer		=	0;
				break;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::RtlAtLockProc( void )
{
	// local -------------------
	// code --------------------
		if ( m_IsAtLock )
		{
			if ( E_GrP3UserIdMax > m_Gvs.LoginIdx )
			{
				if ( 0 != m_AtLockTimer )
				{
					m_AtLockTimer --;
					if ( 0 == m_AtLockTimer )
					{
						// logout
						RtlLogOut();
						// reset timer
						m_AtLockTimer	=	m_AtLockPeriod;
					}
				}
			}
			else
			{
				m_AtLockTimer	=	m_AtLockPeriod;
			}
		}
				
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::ErrorSetOne( __u8 A_ErrCode, BOOL8 A_IsOn )
{
	// local -------------------
		St_GrP3MngMsgErrSetOne	Tv_Msg;
	// code --------------------
		if ( 32 > A_ErrCode )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdErrSetSetOne;
			Tv_Msg.ErrCode			=	A_ErrCode;
			Tv_Msg.IsOn					=	A_IsOn;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3Mng::ErrorSetOne - error code %d out of range \n", A_ErrCode );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::ErrorClearMap( __u32 A_Map )
{
	// local -------------------
		St_GrP3MngMsgErrClrMap	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdErrSetClrMap;
		Tv_Msg.Map					=	A_Map;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgErrSetOne( Ptr_GrP3MngMsgErrSetOne A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_IsEvt;
		__u32	Tv_Mask;
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		Tv_IsEvt	=	FALSE;
		Tv_Mask	=	1 << A_PtrMsg->ErrCode;
		if ( A_PtrMsg->IsOn )
		{
			if ( 0 == (Tv_Mask & m_Gvs.ErrMap) )
			{
				Tv_IsEvt	=	TRUE;
				m_Gvs.ErrMap	=	m_Gvs.ErrMap | Tv_Mask;
				// update last error time
				m_Gvs.LastErrTime[A_PtrMsg->ErrCode]	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
			}
		}
		else
		{
			if ( 0 != (Tv_Mask & m_Gvs.ErrMap) )
			{
				Tv_IsEvt	=	TRUE;
				m_Gvs.ErrMap	=	m_Gvs.ErrMap & ~Tv_Mask;
			}
		}
		// notify error occur
		if ( Tv_IsEvt )
		{
			if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmErrOcu )
			{
				VmRunCode( m_PtrCfgMdl->AsmErrOcu, NULL );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgErrClrMap( Ptr_GrP3MngMsgErrClrMap A_PtrMsg )
{
	// local -------------------
	// code --------------------
		m_Gvs.ErrMap	=	m_Gvs.ErrMap & ~A_PtrMsg->Map;
		// notify error occur
		if ( E_GrP3AsmIdNone != m_PtrCfgMdl->AsmErrOcu )
		{
			VmRunCode( m_PtrCfgMdl->AsmErrOcu, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SioMngSet( Cls_GrP3SioMng* A_ObjMng )
{
		m_SioMng	=	A_ObjMng;
}
//--------------------------------------------------------------------
Cls_GrP3SioMng*	Cls_GrP3Mng::SioMngGet( void )
{
		return	m_SioMng;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcSioSetup( void )
{
	// local -------------------
	// code --------------------
		if ( NULL != m_SioMng )
		{
			m_SioMng->SafeSetup( m_SdaBuf[E_GrP3CfgOfsCtrlModel], m_SdaBuf[E_GrP3CfgOfsCtrlUart], m_SdaBuf[E_GrP3CfgOfsCtrlId] );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SetupSaveReq( void )
{
	// local -------------------
		St_GrP3MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrP3MngCmdSetupSave;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::LcProcMsgSetupSave( void )
{
	// local -------------------
		__s32		Tv_Rint[2];
	// code --------------------
		GrP3BiosCfgSaveToRom( Tv_Rint, NULL );
		RtlCfgUpdate();
}
//--------------------------------------------------------------------
__u32	Cls_GrP3Mng::UiFindIdAlloc( void )
{
		m_ObjFindId ++;
		if ( 0 == m_ObjFindId )
		{
			m_ObjFindId	=	1;
		}
		return	m_ObjFindId;
}
//--------------------------------------------------------------------
void	Cls_GrP3Mng::SysFwUpStart(__u16 A_CodeId)
{
	// local -------------------
		St_GrP3MngMsgAsmDcall	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3MngCmdAsmDcall;
		Tv_Msg.CodeId				=	A_CodeId;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
Ptr_GrP3ScptCfgPrj	Cls_GrP3Mng::ConfigPrjPtrGet( void )
{
		return	m_PtrCfgPrj;
}
//--------------------------------------------------------------------
Ptr_GrP3ScptCfgRtl	Cls_GrP3Mng::ConfigRtlPtrGet( void )
{
		return	m_PtrCfgRtl;
}
//--------------------------------------------------------------------
Ptr_GrP3ScptCfgMdl	Cls_GrP3Mng::ConfigMdlAtbGet( void )
{
		return	m_PtrCfgMdl;
}
//--------------------------------------------------------------------

