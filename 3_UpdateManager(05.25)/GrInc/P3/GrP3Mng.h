/*
	GAUSE Platform manager

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3Mng
#define	_EPC_GrP3Mng

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<GrGdibBase.h>
#include	<P3/GrP3Base.h>

#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3Font.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3DoBase.h>
#include	<P3/GrP3DoWin.h>

#include <Dvr/PtzV2/GrPtzV2Mng.h>
#include <P3/Sio/GrP3SioMng.h>

//====================================================================
//constance

#define E_GrP3MngConMaxCnt				E_GrP3ScptConMaxCnt

#define E_GrP3MngFontMaxCnt				8				/* font max count */

#define	E_GrP3MngImgScptMaxCnt		512
#define E_GrP3MngImgUserMaxCnt		16

#define	E_GrP3MngTextScptMaxCnt		1024		/* text max count */

#define E_GrP3MngHdayCtlgPageMax	64

#define E_GrP3MngAccountCnt				8		/* authority account count */

#define E_GrP3MngClrIdxBlack			0		/* console color table */
#define E_GrP3MngClrIdxWhite			1		/* console color table */
#define E_GrP3MngClrIdxGray				2		/* console color table */
#define E_GrP3MngClrIdxRed				3		/* console color table */

#define E_GrP3MngClrCount					4		

#define E_GrP3MngLangAuto					0xFF		/* language - auto */

#define E_GrP3MngVmCnt						8

#define E_GrP3MngVmIdxNone				0xFF		/* virtual machine index none */

#define	E_GrP3MngGvuMaxCnt				128

#define E_GrP3MngSdaSize					0x8000	/* setup data area size */

#define E_GrP3MngSysStatNone			0
#define E_GrP3MngSysStatInit			1
#define E_GrP3MngSysStatStart			2
#define E_GrP3MngSysStatFwUpgd		3		/* firmware upgrade - develop only */

#define E_GrP3MngGvsVarCnt				129

#define E_GrP3MngCtlgValMask			0xFFFFFF
#define E_GrP3MngCtlgIdShft				24
#define E_GrP3MngCtlgIdMask				0xFF

// PTZ control mode
#define E_GrP3MngPtzModeNone			0
#define E_GrP3MngPtzModePantilt		1
#define E_GrP3MngPtzModeZoom			2
#define E_GrP3MngPtzModeFocus			3

// registry index
#define E_GrP3MngRgstIdxEmgc			0		/* emergency status */
#define E_GrP3MngRgstIdxQuadMode	1		/* last quad mode */
#define E_GrP3MngRgstIdxQuadPage	2		/* last quad page */

// error map
#define E_GrP3MngErrMapCnt				32

#define E_GrP3MngErrHddFull				8
#define E_GrP3MngErrHddNone				9
#define E_GrP3MngErrHddUnformat		10

#define E_GrP3MngErrDns						16
#define E_GrP3MngErrSmtp					17
#define E_GrP3MngErrNtp						18
#define E_GrP3MngErrDhcp					19

#define E_GrP3MngErrBattery				24

#define E_GrP3MngDpcUpiParaCnt		6

// message command
#define E_GrP3MngCmdQuadModeSet		1		/* quad mode set : St_GrP3MngMsgQuadModeSet */
#define E_GrP3MngCmdQuadModeNext	2		/* quad mode next : St_GrP3MngMsgQuadMovePara */
#define E_GrP3MngCmdQuadModePrev	3		/* quad mode previous : St_GrP3MngMsgQuadMovePara */
#define E_GrP3MngCmdQuadPageNext	4		/* quad page next : St_GrP3MngMsgQuadMovePara */
#define E_GrP3MngCmdQuadModeGet		5		/* quad mode get : St_GrP3MngMsgQuadModeGet */
#define E_GrP3MngCmdAsmDcall			6		/* assembly DCALL : Ptr_GrP3MngMsgAsmDcall */
#define E_GrP3MngCmdDpcUpi				7		/* dispatched UPI : Ptr_GrP3MngMsgDpcUpi */
#define E_GrP3MngCmdErrSetSetOne	8		/* error set one : Ptr_GrP3MngMsgErrSetOne */
#define E_GrP3MngCmdErrSetClrMap	9		/* error clear map : Ptr_GrP3MngMsgErrClrMap */
#define E_GrP3MngCmdSetupSave			10	/* setup save : St_GrP3MngMsgBase */

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct	St_GrP3MngGvs
{
	__u32		HwVer;			// hardware version	
	__u32		ScptTime;		// script time
	__u8		VdoMaxCh;		// video max channel
	__u8		AdoMaxCh;		// audio max channel
	__u8		DiChCnt;		// sensor channel count
	__u8		DoChCnt;		// alarm channel count

	BOOL8		IsEdio;			// expand digital input output found
	__u8		PrgsInit;		// initialize progress
	BOOL8		TblIsPlay[2];	// main play mode

	__u8		LoginIdx;		// login index - 0xFF = log off
	__u8		PrgsFwUp;		// firmware upraged progress;
	__u16		VlossMap;		// video loss map

	__u16		DiStat;			// sensor status
	__u16		DoStat;			// alarm status

	__u8		HddMaxCnt;	// HDD max count
	BOOL8		IsEmgc;			// emergency status
	__u16		LoginPriv;	// login privilege

	BOOL8		IsAtSeq;		// auto sequence on
	__u8		IsLanOn;		// LAN cable connected
	__u8		PlayStat;		// main play status
	BOOL8		IsPlayBack;	// play direction

	__u16		CovertMap;
	BOOL8		IsRmtTmpAble;	// REMOCON temporary able
	__u8		RunRmtId;		// running REMOCON ID

	__u32		NetVer;			// network version

	__u8		FwUpErr;		// firmware upgrade error code
	__u8		UsbMemCnt;	// USB memory count
	__u8		CmsCliCnt;	// CMS client count
	__u8		BkupErrCode;	// backup error code

	__u32		NetModel;		// network model
	__s32			PlaySpd;		// play speed

	__u32		ErrMap;

	__u8		PtzCtrlMode;
	BOOL8		IsPtzCtrlLock;
	__u8		PtzCtrlCh;
	__u8		IsOddExist;	// ODD exist

	__u8		MenuMode;			// menu mode
	BOOL8		IsLoginMenu;	// login menu mode
	BOOL8		IsNtpOk;			// NTP test result
	__u8		LogRecDev;		// log backup record device

	__u16		LogChMap;			// selected log channel map
	__u16		LogEvtMap;		// selected log event type map

	__u32		LogTimeSt;		// log backup start time
	__u32		LogTimeEd;		// log backup end time
	__u32		LogTotal;			// log total count
	__u32		LogPrgs;			// log progress

	__u32		V4Ip;
	__u32		V4NetMask;
	__u32		V4GateWay;

	__u8		AdoLiveCh;
	__u8		AdoPlayCh;
	__u8		BkupRecDev;
	__u8		BkupMedia;

	__u8		AdoLiveVol[E_GrDvrMaxChCnt];
	__u8		AdoPlayVol[E_GrDvrMaxChCnt];

	Def_GrTime	BkupTimeSt;
	Def_GrTime	BkupTimeEd;

	__u16		BkupChMap;
	BOOL8		IsBkupCmbViewer;
	__u8		CrtRmtId;
	Def_GrTime	BkupTimePrgs;

	Def_GrTime	LastErrTime[32];

	__u8		LogErrCode;			// log backup error code
	__u8		MsgBoxMode;			// message box mode
	__u16		PtzTourMsk;			//ptz touring mode bitmask

	__u8		RunDispRes;			// now running display resolution
	__u8		_rsvd1;
	__u16		_rsvd2;

}	*Ptr_GrP3MngGvs;

typedef struct St_GrP3MngConMng
{
	__u8	LastLiveQuadMode;
	__u8	LastLiveQuadPage;
}	*Ptr_GrP3MngConMng;

typedef struct St_GrP3MngMsgBase
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrP3MngMsgBase;

typedef struct St_GrP3MngMsgQuadModeSet
{
	St_GrP3MngMsgBase	Hd;
	__u8	ConIdx;
	__u8	Mode;
	__u8	Page;
	__u8	_rsvd0;
	__u32	Fcc;
}	*Ptr_GrP3MngMsgQuadModeSet;

typedef struct St_GrP3MngMsgQuadMovePara
{
	St_GrP3MngMsgBase	Hd;
	__u8	ConIdx;
	__u8	_rsvd0;
	__u16	_rsvd1;
	__u32	Fcc;
}	*Ptr_GrP3MngMsgQuadMovePara;

typedef struct St_GrP3MngMsgQuadModeGet
{
	St_GrP3MngMsgBase	Hd;

	__u8	ConIdx;
	__u8	_rsvd0;
	__u16	_rsvd1;

	__u8*	PtrRtMode;
	__u8*	PtrRtPage;
}	*Ptr_GrP3MngMsgQuadModeGet;

typedef struct St_GrP3MngMsgAsmDcall
{
	St_GrP3MngMsgBase	Hd;
	__u16	CodeId;
}	*Ptr_GrP3MngMsgAsmDcall;

typedef struct St_GrP3MngMsgDpcUpi
{
	St_GrP3MngMsgBase	Hd;
	__u8	ConIdx;
	__u16	WinScptId;
	__u16	ObjScptId;
	__u16	UpiCmd;
	__s32		Para[E_GrP3MngDpcUpiParaCnt+1];
}	*Ptr_GrP3MngMsgDpcUpi;

typedef struct St_GrP3MngMsgErrSetOne 
{
	St_GrP3MngMsgBase	Hd;
	__u8	ErrCode;
	BOOL8	IsOn;
}	*Ptr_GrP3MngMsgErrSetOne;

typedef struct St_GrP3MngMsgErrClrMap 
{
	St_GrP3MngMsgBase	Hd;
	__u32	Map;
}	*Ptr_GrP3MngMsgErrClrMap;


//====================================================================
//global class
class	Cls_GrP3Mng	:	public	Cls_GrTaskCli
{
	protected:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_HndlTimer;
		__u32		m_MsgAloc;

		Cls_GrP3ConBase*	m_ConTbl[E_GrP3MngConMaxCnt];
		St_GrP3MngConMng	m_ConMng[E_GrP3MngConMaxCnt];

		// front
		BOOL8		m_IsKeyBuzRun;
		__u8		m_KeyBlock;

		// mouse
		__u16		m_MseX;						// mouse x position
		__u16		m_MseY;						// mouse y position
		__u8		m_MseBtnMap;			// mouse button map
		__u8		m_MseSenSl;				// mouse sensitivity
		__u8		m_MseSenSr;

		// script
		void*		m_PtrScptBuf;
		__u32		m_ScptSize;
		__u32		m_ScptDecPrgs;		// script decode progress
		BOOL8		m_IsScptErr;			// script error
		BOOL8		m_IsScptAble;			// script able

		Ptr_GrP3ScptCfgPrj	m_PtrCfgPrj;
		Ptr_GrP3ScptCfgRtl	m_PtrCfgRtl;
		Ptr_GrP3ScptCfgMdl	m_PtrCfgMdl;

		// window
		__u32		m_WinCnt;
		Ptr_GrP3ScptWinInfo	m_PtrWinTbl;

		// page
		__u32		m_PageCnt;
		Ptr_GrP3ScptPageInfo	m_PtrPageTbl;

		// font
		Cls_GrP3Font*	m_FontMng[E_GrP3MngFontMaxCnt];		// font manager table

		// text
		Ptr_GrP3ScptTxtItm	m_PtrTxtTbl;
		St_GrP3ScptTxtItm		m_TxtScptTbl[E_GrP3MngTextScptMaxCnt];

		// image
		Ptr_GrGdib*	m_PtrImgTbl;		// image position table
		Ptr_GrGdib	m_ImgScptTbl[E_GrP3MngImgScptMaxCnt];
		Ptr_GrGdib	m_ImgUserTbl[E_GrP3MngImgUserMaxCnt];

		// paint script
		__u32	m_PstCnt;
		Ptr_GrP3ScptPstItm	m_PtrPstTbl;	// paint script count

		// assembly
		__u32	m_AsmCnt;
		Ptr_GrP3ScptAsmInfo	m_PtrAsmTbl;			// code table pointer

		// global variable
		St_GrP3MngGvs	m_Gvs;
		__s32		m_GvuTbl[E_GrP3MngGvuMaxCnt];
		__u8	m_SdaBuf[E_GrP3MngSdaSize];		// setup data area

		// virtual machine
		__u8		m_VmIdx;				// VM run index
		St_GrP3AsmVm	m_VmTbl[E_GrP3MngVmCnt];

		// VM runtime
		__u16		m_VmCsLim;			// code segment limit count	
		Ptr_GrP3AsmOpBase		m_PtrVmCs;
		Cls_GrP3DoWin*	m_VmHostWin;
		Cls_GrP3DoBase*	m_VmHostObj;

		// holiday catalog
		__u32		m_HdayCtlgPageSize;
		__u32		m_HdayCtlgPageIdx;
		__u32		m_HdayCtlgPageCnt;

		// PTZ
		Cls_GrPtzV2Mng*	m_PtzMng;

		// controller
		Cls_GrP3SioMng*	m_SioMng;

		// runtime
		__u8		m_FcsConIdx;	// focus console index
		__u8		m_LangIdx;		// language index
		__s32		m_EventLock;	// user event lock level

		__u32		m_ObjFindId;	// object find ID

		__u8		m_LiveAtbTbl[E_GrDvrMaxChCnt];		// live attribute table
		__u8		m_PlayAtbTbl[E_GrDvrMaxChCnt];		// play attribute table

		__u8		m_UiAlphaVal;

		__u16		m_VlossStat;

		// system
		__u32		m_SysStat;		// system status

		// auto key lock
		__u32		m_AtLockTimer;
		__u32		m_AtLockPeriod;
		__u8		m_IsAtLock;

		// edit

		// local function
		virtual	void	LcGvsReset( void );
		
		void	LcMseSenUpdt( void );

		void	LcVlossUpdt( void );
		void	LcCovertUpdt( void );

		void	LcFontReset( void );
		BOOL8	LcScptDecHead( void );
		BOOL8	LcScptDecCfgPrj( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecCfgRtl( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecMdlAtb( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecPstTbl( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecPstScpt( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecWinTbl( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecWinScpt( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecPageTbl( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecPageScpt( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecAsmTbl( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecAsmCode( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecImgData( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecTextData( Ptr_GrP3ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecFontData( Ptr_GrP3ScptDataHd A_PtrDtHd );

		void	LcImgAlphaChgRgb32( __u8 A_AlphaVal );
		void	LcPstAlphaChgRgb32( __u8 A_AlphaVal );

		void	LcPtzSetup( void );
		void	LcSioSetup( void );

		void	LcAtLockSetup( void );
		virtual	void	RtlAtLockProc( void );

		WCHAR*	LcChMapToWstr( WCHAR* A_StrTg, __u16 A_ChMap );
		WCHAR*	LcUserToWstr( WCHAR* A_StrTg, __u8 A_UserIdx );
		WCHAR*	LcIp4ToWstr( WCHAR* A_StrTg, __u32 A_Ip4 );

		void	LcProcMsgUiMsg( Ptr_GrP3MsgHd A_PtrMsg );
		void	LcProcMsgBrdcst( Ptr_GrP3MsgHd A_PtrMsg );		// message console broad cast
		void	LcProcMsgFormat( Ptr_GrTaskMsgDiskFormat A_PtrMsg );	// disk format message
		void	LcProcMsgQuadChgReq( Ptr_GrTaskMsgQuadChgReq A_PtrMsg );		// quad change message
		void	LcProcMsgEvtCall( Ptr_GrP3EvtCallMsg A_PtrMsg );
		void	LcProcMsgFwUpgdDone( Ptr_GrTaskMsgFwUpgdEnd A_PtrMsg );
		void	LcProcMsgNetOnOff( BOOL8 A_IsOn );
		void	LcProcMsgPlayStatChg( void );
		void	LcProcMsgUsbChg( void );
		void	LcProcMsgCmsCliChg( void );
		void	LcProcMsgBkupStart( void );
		void	LcProcMsgBkupEnd( void );
		void	LcProcMsgBkupErr( void );
		void	LcProcMsgErrSetOne( Ptr_GrP3MngMsgErrSetOne A_PtrMsg );
		void	LcProcMsgErrClrMap( Ptr_GrP3MngMsgErrClrMap A_PtrMsg );
		void	LcProcMsgSetupSave( void );
		void	LcProcMsgLogBkupStart( void );
		void	LcProcMsgLogBkupEnd( void );
		void	LcProcMsgLogBkupError( void );

		__u32	LcUiDecWindow( __u16 A_ScptId, __u8 A_ConIdx );			// window decode
		__u32	LcUiDecSlave( Cls_GrP3DoBase* A_ObjPrnt, void* A_PtrScpt, BOOL8 A_IsWithPrnt );		// decode slave object				
		Cls_GrP3DoBase*	LcUiObjCreate( __u8 A_Class, __u16 A_ScptId, Ptr_GrRect A_PtrRc, Cls_GrP3DoBase* A_ObjPrnt );
		void	LcTickMsgBrdcst( __u32 A_Tick );
		void	LcConRedrawAll( void );
		void	LcWinAllFree( void );
		Ptr_GrP3ScptObjHd	LcGetWinScpt( __u8* A_PtrConIdx, __u16 A_ScptId, Ptr_GrRect A_PtrRtRc, __u8* A_PtrRtLayer );

		__s32		LcAsmArgLoad( __u8 A_Type, __s32 A_Para );
		void	LcAsmArgStore( __u8 A_Type, __s32 A_Para, __s32 A_Val );
		void	LcAsmOpCond( Ptr_GrP3AsmOpCond A_PtrCode );
		void	LcAsmOpMove( Ptr_GrP3AsmOpMove A_PtrCode );
		void	LcAsmOpFlag( Ptr_GrP3AsmOpFlag A_PtrCode );
		void	LcAsmOpCalc( Ptr_GrP3AsmOpCalc A_PtrCode );
		void	LcAsmOpNot( Ptr_GrP3AsmOpNot A_PtrCode );
		void	LcAsmOpDirJump( Ptr_GrP3AsmOpDirJump A_PtrCode );
		void	LcAsmOpRefJump( Ptr_GrP3AsmOpRefJump A_PtrCode );
		void	LcAsmOpRelJump( Ptr_GrP3AsmOpRelJump A_PtrCode );
		void	LcAsmOpOfsJump( Ptr_GrP3AsmOpOfsJump A_PtrCode );
		void	LcAsmOpTxtClr( Ptr_GrP3AsmOpTxtClear A_PtrCode );
		void	LcAsmOpTxtId( Ptr_GrP3AsmOpTxtIdGet A_PtrCode );
		void	LcAsmOpTxtDec( Ptr_GrP3AsmOpTxtDec A_PtrCode );
		void	LcAsmOpTxtHex( Ptr_GrP3AsmOpTxtHex A_PtrCode );
		void	LcAsmOpTxtCharAdd( Ptr_GrP3AsmOpTxtChar A_PtrCode );
		void	LcAsmOpTxtCopy( Ptr_GrP3AsmOpTxtCopy A_PtrCode );
		void	LcAsmOpTxtSetSetup( Ptr_GrP3AsmOpTxtSetStp A_PtrCode );
		void	LcAsmOpTxtGetSetup( Ptr_GrP3AsmOpTxtGetStp A_PtrCode );
		void	LcAsmOpSetupCopy( Ptr_GrP3AsmOpSetupCopy A_PtrCode );
		void	LcAsmOpSetupFill( Ptr_GrP3AsmOpSetupFill A_PtrCode );
		void	LcAsmOpWindow( Ptr_GrP3AsmOpWindow A_PtrCode );
		void	LcAsmOpUpi( Ptr_GrP3AsmOpUpi A_PtrCode );
		void	LcAsmOpApi( Ptr_GrP3AsmOpApi A_PtrCode );
		void	LcAsmOpNotify( Ptr_GrP3AsmOpNotify A_PtrCode );
		void	LcAsmOpQuery( Ptr_GrP3AsmOpQuery A_PtrCode );
		void	LcAsmOpTxtTime( Ptr_GrP3AsmOpTxtTime A_PtrCode );
		void	LcAsmOpTxtCmp( Ptr_GrP3AsmOpTxtCmp A_PtrCode );
		void	LcAsmOpTxtLen( Ptr_GrP3AsmOpTxtLen A_PtrCode );
		void	LcAsmOpWait( Ptr_GrP3AsmOpWait A_PtrCode );
		void	LcAsmOpLabel( Ptr_GrP3AsmOpLabel A_PtrCode );
		void	LcAsmOpLblJump( Ptr_GrP3AsmOpLblJump A_PtrCode );
		void	LcAsmOpReturn( Ptr_GrP3AsmOpBase A_PtrCode );
		void	LcAsmOpDcall( Ptr_GrP3AsmOpDcall A_PtrCode );
		void	LcAsmOpStpGet( Ptr_GrP3AsmOpStpGet A_PtrCode );
		void	LcAsmOpStpPut( Ptr_GrP3AsmOpStpPut A_PtrCode );

		void	LcDpcCodeRun( __u16 A_CodeId );

		void	LcVmDoOneCmd( void );

		void	LcVmReset( void );
		BOOL8	LcVmPrepare( Ptr_GrP3MngVmRpara A_PtrRpara );		// VM runtime prepare

		void	LcProcDpcUpi( Ptr_GrP3MngMsgDpcUpi A_PtrMsg );

		// VM inheritable
		virtual	void	RtlVmRun( void );
		virtual	void	RtlVmFault( void );	// VM fault

		//task inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
		Cls_GrP3Mng( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrP3Mng( void );

		// script
		BOOL8	ScriptSet( void* A_PtrScpt, __u32 A_Size );		// set script pointer

		// information
		Ptr_GrP3ScptCfgPrj	ConfigPrjPtrGet( void );
		Ptr_GrP3ScptCfgRtl	ConfigRtlPtrGet( void );
		Ptr_GrP3ScptCfgMdl	ConfigMdlAtbGet( void );

		// resource
		__u32	PixelFormatGet( void );
		Ptr_GrGdib	ImgGet( __u16 A_Id );
		Ptr_GrP3ScptPstData	PstDataGet( __u16 A_Id, __u8* A_PtrRtCnt );		// paint script get
		WCHAR*	TextPtrGet( __u16 A_Id, __u8 A_Lang = E_GrP3MngLangAuto );
		Ptr_GrP3ScptAsmInfo	AsmPtrGet( __u16 A_Id );
		Cls_GrP3Font*	FontGet( __u8 A_Id );
		__u32	CursorColorGet( void );
		void	PtzMngSet( Cls_GrPtzV2Mng* A_ObjMng );
		Cls_GrPtzV2Mng*	PtzMngGet( void );
		void	SioMngSet( Cls_GrP3SioMng* A_ObjMng );
		Cls_GrP3SioMng*	SioMngGet( void );
		
		__u8	LangSelGet( void );		// selected language get
		void	LangSelSet( __u8 A_Idx );	// select language

		BOOL8	PageDec( __u16 A_PageId, Cls_GrP3DoBase* A_Prnt );
		__u16	PageAsmOpenGet( __u16 A_PageId );
		__u16	PageAsmCloseGet( __u16 A_PageId );
		__u16	PageEscKeyFcsIdGet( __u16 A_PageId );

		void*	SetupDataPtrGet( __u16 A_Ofs );
		void*	GvuTblGet( void );
		void*	GvsPtrGet( void );

		// console
		Cls_GrP3ConBase*	ConsoleGet( __u8 A_ConIdx = E_GrP3ConIdxFocus );
		BOOL8	ConsoleRegist( Cls_GrP3ConBase* A_ObjCon, __u8 A_Idx );
		BOOL8	ConsoleUnregist( Cls_GrP3ConBase* A_ObjCon );

		// UI message
		BOOL8	UiMsgPost( Ptr_GrP3MsgHd A_PtrMsg, __u32 A_Size );
		BOOL8	UiMsgPump( void );		//message pump
		void	UiMsgRest( void );		//message pump rest

		// key
		virtual	void	KeyInput( __u8 A_Key, BOOL8 A_IsPush, __u8 A_DevType );
		void	KeyBlockSet( __u8 A_BlocKey );

		// mouse
		virtual	void	MousePosInput( __s32 A_X, __s32 A_Y, BOOL8 A_IsAbs );
		virtual	void	MouseBtnInput( __u8 A_BtnIdx, BOOL8 A_IsPush );
		virtual	void	MouseWheelInput( BOOL8 A_IsDown );
		void	MousePosGet( __u16* A_PtrRtX, __u16* A_PtrRtY );

		// virtual keyboard
		__u8	VkbdTxtPageCntGet( __u8 A_Lang = E_GrP3MngLangAuto );
		WCHAR	VkbdTxtKeyCodeGet( __u8 A_Page, __u8 A_Key, __u8 A_Lang = E_GrP3MngLangAuto );
		BOOL8	VkbdTxtRun( Cls_GrP3DoBase* A_Host, WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd, BOOL8 A_IsAdvPasswd );
		BOOL8	VkbdNumRun( Cls_GrP3DoBase* A_Host,  WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd );
		BOOL8	VkbdRun( Cls_GrP3DoBase* A_Host, WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd, BOOL8 A_IsNumType, BOOL8 A_IsAdvPasswd );

		// virtual machine
		virtual	BOOL8	VmRunCode( __u16 A_CodeId, Cls_GrP3DoBase* A_ObjHost, Ptr_GrP3MngVmRpara A_PtrRpara = NULL );
		__s32		VmRegIntGet( __u8 A_RegIdx );
		void	VmRegTxtGet( __u8 A_RegIdx, WCHAR* A_Str );
		__u8	VmFlagGet( void );
		__s32		VmValueGet( __u8 A_Type, __u32 A_Para );
		void	VmModalPop( __s32 A_ModalRslt );
		void	VmValueSet( __u8 A_Type, __u32 A_Para, __s32 A_Val );
		Cls_GrP3DoBase*	VmHostGet( void );
		BOOL8	VmReqEvtCall( __u16 A_CodeId, Cls_GrP3DoBase* A_ObjHost, Ptr_GrP3MngVmRpara A_PtrRpara = NULL );

		BOOL8	DpcUpiCall( __u8 A_ConIdx, __u16 A_WinScptId, __u16 A_ObjScptId, __u16 A_UpiCmd, 
			__s32 A_Para1 = 0, __s32 A_Para2 = 0, __s32 A_Para3 = 0, __s32 A_Para4 = 0, 
			__s32 A_Para5 = 0, __s32 A_Para6 = 0 );

		// message box
		BOOL8	NotifyBox( __u8 A_ConIdx, WCHAR* A_StrTitle, WCHAR* A_StrCont, __u8 A_Time );
		BOOL8	NotifyBox( __u8 A_ConIdx, __u16 A_TxtIdTitle, __u16 A_TxtIdCont, __u8 A_Time );

		// a/v status
		void	LiveAtbSet( __u8 A_Ch, __u8 A_Atb );
		__u8*	LiveAtbTblPtrGet( void );

		__u8*	PlayAtbTblPtrGet( void );

		BOOL8	IsPlayMode( __u8 A_ConIdx );

		__u8	VdoMaxChCntGet( void );
		__u8	AdoMaxChCntGet( void );
		__u16	VdoRecQtyGet( __u8 A_ResType, __u8 A_ResLvl, __u8 A_QtyLvl );		// video record quality value table get
		__u16	VdoNetQtyGet( __u8 A_ResType, __u8 A_ResLvl, __u8 A_QtyLvl );		// video dual stream quality value table get

		void	SafeQuadModeSet( __u8 A_ConIdx, __u8 A_Mode, __u8 A_Page, __u32 A_Fcc );
		void	SafeQuadModeNext( __u8 A_ConIdx, __u32 A_Fcc );
		void	SafeQuadModePrev( __u8 A_ConIdx, __u32 A_Fcc );
		void	SafeQuadPageNext( __u8 A_ConIdx, __u32 A_Fcc );
		void	SafeQuadModeGet( __u8 A_ConIdx, __u8* A_PtrRtMode, __u8* A_PtrRtPage );

		// info
		__u32	FirmwareVerGet( void );
		__u32	ModelNumGet( void );

		__u16	SoftCusImgIdGet( void );
		__u32	HwTypeGet( void );

		__u8	HddMaxCntGet( void );
		__u8	LongKeyTimeGet( void );
		BOOL8	IsAdvPassUse( void );
		BOOL8	IsEmgcLockAble( void );
		BOOL8	IsRecEmgc( void );
		BOOL8	IsDispAtSeq( void );		// auto sequence

		// UI
		BOOL8	IsUiEventAble( void );
		void	UiEventLock( void );
		void	UiEventUnlock( void );
		void	UiAlpahSet( __u8 A_Value );
		__u32	UiFindIdAlloc( void );

		// catalog
		BOOL8	CtlgLoad( __u8 A_Id, __s32* A_PtrParaTbl, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt );
		BOOL8	CtlgPageNext( __u8 A_Id, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt, __u32* A_PtrRtPagePos );
		BOOL8	CtlgPagePrev( __u8 A_Id, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt, __u32* A_PtrRtPagePos );
		BOOL8	CtlgItemValueGet( __u32 A_Hndl, __u8 A_ElemCode, __s32 A_AddVal, __s32* A_PtrRtVal );
		BOOL8	CtlgItemStrGet( __u32 A_Hndl, __u8 A_ElemCode, __s32 A_AddVal, WCHAR* A_StrRt );

		// make text
		BOOL8	EvtLogToWstr( Ptr_GrDvrLogV2Item A_PtrLog, WCHAR* A_PtrRtStr );
		void	EmailEvtToWstr( Ptr_GrDvrEmailEvt A_PtrEevt, WCHAR* A_PtrRtStr );

		// system
		void	SysBuildEnv( void );
		BOOL8	SysInit( void );
		void	SysPrgsInitSet( __u8 A_Prgs );
		BOOL8	SysStart( void );
		void	SysQuit( void );
		void	SysFwUpStart(__u16 A_CodeId);

		// error
		void	ErrorSetOne( __u8 A_ErrCode, BOOL8 A_IsOn );
		void	ErrorClearMap( __u32 A_Map );

		// setup
		void	SetupSaveReq( void );

		// runtime
		void	RtlCfgUpdate( void );		// configuration update
		void	RtlPlayModeSet( BOOL8 A_IsPlayMode, __u8 A_ConIdx = 0 );
		void	RtlDateDispFmtUpdt( void );
		BOOL8	RtlLogIn( __u8 A_Id, WCHAR* A_StrPass, WCHAR* A_StrDual );
		void	RtlLogOut( void );
		BOOL8	RtlPasswdRuleChk( void );
		void	RtlTryRecEmgc( BOOL8 A_IsOn );
		void	RtlDispAutoSeqSet( BOOL8 A_IsAtSeq );
		void	RtlProcObjRelease( Cls_GrP3DoBase* A_Obj );

		// editor
		virtual	__u32	DvlConResX( __u8 A_ConIdx );
		virtual	__u32	DvlConResY( __u8 A_ConIdx );


};

//====================================================================
//global function
BOOL8	GrP3MngInit( Cls_GrTaskSvr* A_TaskSvr );
BOOL8	GrP3MngFinish( void );
void	GrP3MngGetWinIdInfo( __u16 A_WinId, __u8* A_PtrRtLayer, __u8* A_PtrRtSlot );
__u16	GrP3MngMakeWinId( __u8 A_Layer, __u8 A_Slot, __u8 A_Seq );
__u32	GrP3MngMakeCtlgHndl( __u8 A_Id, __u32 A_ItemIdx );
__u32	GrP3MngCtlgHndlToIdx( __u32 A_CtlgHndl, __u32* A_PtrRtId );

//====================================================================
//out Environment conditional
#endif

