/*
	GAUSE Platform manager

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4Mng
#define	_EPC_GrP4Mng

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>
#include <GrGdibBase.h>
#include <P4/GrP4Base.h>

#include <P4/GrP4Script.h>
#include <P4/GrP4ConBase.h>
#include <P4/GrP4Font.h>
#include <P4/GrP4AsmBase.h>
#include <P4/GrP4DoBase.h>
#include <P4/GrP4DoWin.h>

#include <Dvr/PtzV2/GrPtzV2Mng.h>
#include <P4/Sio/GrP4SioMng.h>

//====================================================================
//constance

#define E_GrP4MngConMaxCnt				E_GrP4ScptConMaxCnt

#define E_GrP4MngFontMaxCnt				8				/* font max count */

#define	E_GrP4MngImgScptMaxCnt		512
#define E_GrP4MngImgUserMaxCnt		16

#define	E_GrP4MngTextScptMaxCnt		1024		/* text max count */

#define E_GrP4MngHdayCtlgPageMax	64

#define E_GrP4MngAccountCnt				8		/* authority account count */

#define E_GrP4MngClrIdxBlack			0		/* console color table */
#define E_GrP4MngClrIdxWhite			1		/* console color table */
#define E_GrP4MngClrIdxGray				2		/* console color table */
#define E_GrP4MngClrIdxRed				3		/* console color table */

#define E_GrP4MngClrCount					4		

#define E_GrP4MngLangAuto					0xFF		/* language - auto */

#define E_GrP4MngVmCnt						8

#define E_GrP4MngVmIdxNone				0xFF		/* virtual machine index none */

#define	E_GrP4MngGvuMaxCnt				128

#define E_GrP4MngSdaSize					0x8000	/* setup data area size */

#define	E_GrP4MngRgstSize					0x800		/* registry size */
#define	E_GrP4MngRgstUserOfs			0x400		/* user registry area offset */
#define	E_GrP4RgstSysSize					E_GrP4MngRgstUserOfs
#define	E_GrP4MngRgstUserSize			(E_GrP4MngRgstSize - E_GrP4MngRgstUserOfs)
#define	E_GrP4MngRgstUserCnt			((E_GrP4MngRgstSize - E_GrP4MngRgstUserOfs) >> 2)


#define E_GrP4MngSysStatNone			0
#define E_GrP4MngSysStatInit			1
#define E_GrP4MngSysStatStart			2
#define E_GrP4MngSysStatFwUpgd		3		/* firmware upgrade - develop only */

#define E_GrP4MngCtlgValMask			0xFFFFFF
#define E_GrP4MngCtlgIdShft				24
#define E_GrP4MngCtlgIdMask				0xFF

// PTZ control mode
#define E_GrP4MngPtzModeNone			0
#define E_GrP4MngPtzModePantilt		1
#define E_GrP4MngPtzModeZoom			2
#define E_GrP4MngPtzModeFocus			3

// error map
#define E_GrP4MngErrMapCnt				32

#define E_GrP4MngErrHddFull				8
#define E_GrP4MngErrHddNone				9
#define E_GrP4MngErrHddUnformat		10

#define E_GrP4MngErrDns						16
#define E_GrP4MngErrSmtp					17
#define E_GrP4MngErrNtp						18
#define E_GrP4MngErrDhcp					19

#define E_GrP4MngErrBattery				24

#define E_GrP4MngDpcUpiParaCnt		6

#define	E_GrP4MngVmRerunMax				(E_GrP4MngVmCnt * 8)
#define	E_GrP4MngVmSliceMax				3000

// message command
#define E_GrP4MngCmdQuadModeSet		1		/* quad mode set : St_GrP4MngMsgQuadModeSet */
#define E_GrP4MngCmdQuadModeNext	2		/* quad mode next : St_GrP4MngMsgQuadMovePara */
#define E_GrP4MngCmdQuadModePrev	3		/* quad mode previous : St_GrP4MngMsgQuadMovePara */
#define E_GrP4MngCmdQuadPageNext	4		/* quad page next : St_GrP4MngMsgQuadMovePara */
#define E_GrP4MngCmdQuadModeGet		5		/* quad mode get : St_GrP4MngMsgQuadModeGet */
#define E_GrP4MngCmdAsmDcall			6		/* assembly DCALL : Ptr_GrP4MngMsgAsmDcall */
#define E_GrP4MngCmdDpcUpi				7		/* dispatched UPI : Ptr_GrP4MngMsgDpcUpi */
#define E_GrP4MngCmdErrSetSetOne	8		/* error set one : Ptr_GrP4MngMsgErrSetOne */
#define E_GrP4MngCmdErrSetClrMap	9		/* error clear map : Ptr_GrP4MngMsgErrClrMap */
#define E_GrP4MngCmdSetupSave			10	/* setup save : St_GrP4MngMsgBase */

//====================================================================
//global macro

//====================================================================
//global type


typedef struct St_GrP4MngConMng
{
	__u8	LastLiveQuadMode;
	__u8	LastLiveQuadPage;
}	*Ptr_GrP4MngConMng;

typedef struct St_GrP4MngMsgBase
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrP4MngMsgBase;

typedef struct St_GrP4MngMsgQuadModeSet
{
	St_GrP4MngMsgBase	Hd;
	__u8	ConIdx;
	__u8	Mode;
	__u8	Page;
	__u8	_rsvd0;
	__u32	Fcc;
}	*Ptr_GrP4MngMsgQuadModeSet;

typedef struct St_GrP4MngMsgQuadMovePara
{
	St_GrP4MngMsgBase	Hd;
	__u8	ConIdx;
	__u8	_rsvd0;
	__u16	_rsvd1;
	__u32	Fcc;
}	*Ptr_GrP4MngMsgQuadMovePara;

typedef struct St_GrP4MngMsgQuadModeGet
{
	St_GrP4MngMsgBase	Hd;

	__u8	ConIdx;
	__u8	_rsvd0;
	__u16	_rsvd1;

	__u8*	PtrRtMode;
	__u8*	PtrRtPage;
}	*Ptr_GrP4MngMsgQuadModeGet;

typedef struct St_GrP4MngMsgAsmDcall
{
	St_GrP4MngMsgBase	Hd;
	__u16	CodeId;
}	*Ptr_GrP4MngMsgAsmDcall;

typedef struct St_GrP4MngMsgDpcUpi
{
	St_GrP4MngMsgBase	Hd;
	__u8	ConIdx;
	__u16	WinScptId;
	__u16	ObjScptId;
	__u16	UpiCmd;
	__s32		Para[E_GrP4MngDpcUpiParaCnt+1];
}	*Ptr_GrP4MngMsgDpcUpi;

typedef struct St_GrP4MngMsgErrSetOne 
{
	St_GrP4MngMsgBase	Hd;
	__u8	ErrCode;
	BOOL8	IsOn;
}	*Ptr_GrP4MngMsgErrSetOne;

typedef struct St_GrP4MngMsgErrClrMap 
{
	St_GrP4MngMsgBase	Hd;
	__u32	Map;
}	*Ptr_GrP4MngMsgErrClrMap;


//====================================================================
//global class
class	Cls_GrP4Mng	:	public	Cls_GrTaskCli
{
	protected:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_HndlTimer;
		__u32		m_MsgAloc;

		Cls_GrP4ConBase*	m_ConTbl[E_GrP4MngConMaxCnt];
		St_GrP4MngConMng	m_ConMng[E_GrP4MngConMaxCnt];

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

		Ptr_GrP4ScptCfgPrj	m_PtrCfgPrj;
		Ptr_GrP4ScptCfgRtl	m_PtrCfgRtl;
		Ptr_GrP4ScptCfgMdl	m_PtrCfgMdl;

		// window
		__u32		m_WinCnt;
		Ptr_GrP4ScptWinInfo	m_PtrWinTbl;

		// page
		__u32		m_PageCnt;
		Ptr_GrP4ScptPageInfo	m_PtrPageTbl;

		// font
		Cls_GrP4Font*	m_FontMng[E_GrP4MngFontMaxCnt];		// font manager table

		// text
		Ptr_GrP4ScptTxtItm	m_PtrTxtTbl;
		St_GrP4ScptTxtItm		m_TxtScptTbl[E_GrP4MngTextScptMaxCnt];

		// image
		Ptr_GrGdib*	m_PtrImgTbl;		// image position table
		Ptr_GrGdib	m_ImgScptTbl[E_GrP4MngImgScptMaxCnt];
		Ptr_GrGdib	m_ImgUserTbl[E_GrP4MngImgUserMaxCnt];

		// paint script
		__u32	m_PstCnt;
		Ptr_GrP4ScptPstItm	m_PtrPstTbl;	// paint script count

		// assembly
		__u32	m_AsmCnt;
		Ptr_GrP4ScptAsmInfo	m_PtrAsmTbl;			// code table pointer

		// global variable
		St_GrP4Gvs	m_Gvs;
		__s32	m_GvuTbl[E_GrP4MngGvuMaxCnt];
		__u8	m_SdaBuf[E_GrP4MngSdaSize];		// setup data area
		__u8	m_RgstBuf[E_GrP4MngRgstSize];	// registry buffer area

		// virtual machine
		//__u8		m_VmIdx;					// VM run index
		BOOL8			m_IsVmReqRerun;		// VM running requirement
		St_GrP4AsmVm	m_VmTbl[E_GrP4MngVmCnt];

		// VM runtime
		__u16		m_VmCsLim;			// code segment limit count	
		Ptr_GrP4AsmOpBase		m_PtrVmCs;
		Cls_GrP4DoWin*	m_VmHostWin;
		Cls_GrP4DoBase*	m_VmHostObj;

		// holiday catalog
		__u32		m_HdayCtlgPageSize;
		__u32		m_HdayCtlgPageIdx;
		__u32		m_HdayCtlgPageCnt;

		// PTZ
		Cls_GrPtzV2Mng*	m_PtzMng;

		// controller
		Cls_GrP4SioMng*	m_SioMng;

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
		BOOL8	LcScptDecCfgPrj( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecCfgRtl( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecMdlAtb( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecPstTbl( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecPstScpt( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecWinTbl( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecWinScpt( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecPageTbl( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecPageScpt( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecAsmTbl( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecAsmCode( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecImgData( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecTextData( Ptr_GrP4ScptDataHd A_PtrDtHd );
		BOOL8	LcScptDecFontData( Ptr_GrP4ScptDataHd A_PtrDtHd );

		void	LcImgAlphaChgRgb32( __u8 A_AlphaVal );
		void	LcPstAlphaChgRgb32( __u8 A_AlphaVal );

		void	LcPtzSetup( void );
		void	LcSioSetup( void );

		void	LcAtLockSetup( void );
		virtual	void	RtlAtLockProc( void );

		WCHAR*	LcChToWstr( WCHAR* A_StrTg, __u8 A_Ch );
		WCHAR*	LcUserToWstr( WCHAR* A_StrTg, __u8 A_UserIdx );
		WCHAR*	LcIp4ToWstr( WCHAR* A_StrTg, __u32 A_Ip4 );
		WCHAR*	LcChMapToWstr(WCHAR* A_StrTg, __u32 A_ChMap);

		void	LcProcMsgUiMsg( Ptr_GrP4MsgHd A_PtrMsg );
		void	LcProcMsgBrdcst( Ptr_GrP4MsgHd A_PtrMsg );		// message console broad cast
		void	LcProcMsgFormat( Ptr_GrTaskMsgDiskFormat A_PtrMsg );	// disk format message
		void	LcProcMsgQuadChgReq( Ptr_GrTaskMsgQuadChgReq A_PtrMsg );		// quad change message
		void	LcProcMsgEvtCall( Ptr_GrP4EvtCallMsg A_PtrMsg );
		void	LcProcMsgFwUpgdDone( Ptr_GrTaskMsgFwUpgdEnd A_PtrMsg );
		void	LcProcMsgNetOnOff( BOOL8 A_IsOn );
		void	LcProcMsgPlayStatChg( void );
		void	LcProcMsgUsbChg( void );
		void	LcProcMsgCmsCliChg( void );
		void	LcProcMsgBkupStart( void );
		void	LcProcMsgBkupEnd( void );
		void	LcProcMsgBkupErr( void );
		void	LcProcMsgErrSetOne( Ptr_GrP4MngMsgErrSetOne A_PtrMsg );
		void	LcProcMsgErrClrMap( Ptr_GrP4MngMsgErrClrMap A_PtrMsg );
		void	LcProcMsgSetupSave( void );
		void	LcProcMsgLogBkupStart( void );
		void	LcProcMsgLogBkupEnd( void );
		void	LcProcMsgLogBkupError( void );

		BOOL8	LcUiDecWindow( __u16 A_ScptId, __u8 A_ConIdx, __u16 A_RefPosWin );			// window decode
		__u32	LcUiDecSlave( Cls_GrP4DoBase* A_ObjPrnt, void* A_PtrScpt, BOOL8 A_IsWithPrnt );		// decode slave object
		Cls_GrP4DoBase*	LcUiObjCreate( __u8 A_Class, __u16 A_ScptId, Ptr_GrRect A_PtrRc, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt );
		void	LcTickMsgBrdcst( __u32 A_Tick );
		void	LcConRedrawAll( void );
		void	LcWinAllFree( void );
		Ptr_GrP4ScptObjHd	LcGetWinScpt( __u8* A_PtrConIdx, __u16 A_ScptId, Ptr_GrRect A_PtrRtRc, __u8* A_PtrRtRelPos, __u8* A_PtrRtLayer );

		__s32		LcAsmArgLoad(__u8 A_VmIdx,__u8 A_Type, __s32 A_Para);
		void	LcAsmArgStore(__u8 A_VmIdx, __u8 A_Type, __s32 A_Para, __s32 A_Val);
		void	LcAsmOpCond(__u8 A_VmIdx, Ptr_GrP4AsmOpCond A_PtrCode);
		void	LcAsmOpMove(__u8 A_VmIdx, Ptr_GrP4AsmOpMove A_PtrCode);
		void	LcAsmOpFlag(__u8 A_VmIdx, Ptr_GrP4AsmOpFlag A_PtrCode);
		void	LcAsmOpCalc(__u8 A_VmIdx, Ptr_GrP4AsmOpCalc A_PtrCode);
		void	LcAsmOpNot(__u8 A_VmIdx, Ptr_GrP4AsmOpNot A_PtrCode);
		void	LcAsmOpDirJump(__u8 A_VmIdx, Ptr_GrP4AsmOpDirJump A_PtrCode);
		void	LcAsmOpRefJump(__u8 A_VmIdx, Ptr_GrP4AsmOpRefJump A_PtrCode);
		void	LcAsmOpRelJump(__u8 A_VmIdx, Ptr_GrP4AsmOpRelJump A_PtrCode);
		void	LcAsmOpOfsJump(__u8 A_VmIdx, Ptr_GrP4AsmOpOfsJump A_PtrCode);
		void	LcAsmOpTxtClr(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtClear A_PtrCode);
		void	LcAsmOpTxtId(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtIdGet A_PtrCode);
		void	LcAsmOpTxtDec(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtDec A_PtrCode);
		void	LcAsmOpTxtHex(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtHex A_PtrCode);
		void	LcAsmOpTxtCharAdd(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtChar A_PtrCode);
		void	LcAsmOpTxtCopy(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtCopy A_PtrCode);
		void	LcAsmOpTxtSetSetup(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtSetStp A_PtrCode);
		void	LcAsmOpTxtGetSetup(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtGetStp A_PtrCode);
		void	LcAsmOpSetupCopy(__u8 A_VmIdx, Ptr_GrP4AsmOpSetupCopy A_PtrCode);
		void	LcAsmOpSetupFill(__u8 A_VmIdx, Ptr_GrP4AsmOpSetupFill A_PtrCode);
		void	LcAsmOpWindow(__u8 A_VmIdx, Ptr_GrP4AsmOpWindow A_PtrCode);
		void	LcAsmOpUpi(__u8 A_VmIdx, Ptr_GrP4AsmOpUpi A_PtrCode);
		void	LcAsmOpApi(__u8 A_VmIdx, Ptr_GrP4AsmOpApi A_PtrCode);
		void	LcAsmOpNotify(__u8 A_VmIdx, Ptr_GrP4AsmOpNotify A_PtrCode);
		void	LcAsmOpQuery(__u8 A_VmIdx, Ptr_GrP4AsmOpQuery A_PtrCode);
		void	LcAsmOpTxtTime(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtTime A_PtrCode);
		void	LcAsmOpTxtCmp(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtCmp A_PtrCode);
		void	LcAsmOpTxtLen(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtLen A_PtrCode);
		void	LcAsmOpWait(__u8 A_VmIdx, Ptr_GrP4AsmOpWait A_PtrCode);
		void	LcAsmOpLabel(__u8 A_VmIdx, Ptr_GrP4AsmOpLabel A_PtrCode);
		void	LcAsmOpLblJump(__u8 A_VmIdx, Ptr_GrP4AsmOpLblJump A_PtrCode);
		void	LcAsmOpReturn(__u8 A_VmIdx, Ptr_GrP4AsmOpBase A_PtrCode);
		void	LcAsmOpDcall(__u8 A_VmIdx, Ptr_GrP4AsmOpDcall A_PtrCode);
		void	LcAsmOpStpGet(__u8 A_VmIdx, Ptr_GrP4AsmOpStpGet A_PtrCode);
		void	LcAsmOpStpPut(__u8 A_VmIdx, Ptr_GrP4AsmOpStpPut A_PtrCode);

		void	LcDpcCodeRun( __u16 A_CodeId );

		__s32	LcVmFindEmptySlot(void);
		void	LcVmProcTimer(void);

		void	LcVmReset( void );
		BOOL8	LcVmRparaIn(__u8 A_VmIdx, Ptr_GrP4MngVmRpara A_PtrRpara);
		BOOL8	LcVmRparaOut(__u8 A_VmIdx, Ptr_GrP4MngVmRpara A_PtrRpara);

		void	LcProcDpcUpi( Ptr_GrP4MngMsgDpcUpi A_PtrMsg );

		// VM inheritable
		virtual	void	RtlVmDoOneCmd(__u8 A_VmIdx);
		virtual	void	RtlVmPumpAll(void);
		virtual	void	RtlVmDoSlice( __u8 A_VmIdx );
		virtual	void	RtlVmFault(__u8 A_VmIdx);	// VM fault

		//task inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
		Cls_GrP4Mng( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrP4Mng( void );

		// script
		BOOL8	ScriptSet( void* A_PtrScpt, __u32 A_Size );		// set script pointer

		// information
		Ptr_GrP4ScptCfgPrj	ConfigPrjPtrGet( void );
		Ptr_GrP4ScptCfgRtl	ConfigRtlPtrGet( void );
		Ptr_GrP4ScptCfgMdl	ConfigMdlAtbGet( void );

		// resource
		__u32	PixelFormatGet(void);
		Ptr_GrGdib	ImgGet( __u16 A_Id );
		Ptr_GrP4ScptPstData	PstDataGet( __u16 A_Id, __u8* A_PtrRtCnt );		// paint script get
		WCHAR*	TextPtrGet( __u16 A_Id, __u8 A_Lang = E_GrP4MngLangAuto );
		Ptr_GrP4ScptAsmInfo	AsmPtrGet( __u16 A_Id );
		Cls_GrP4Font*	FontGet( __u8 A_Id );
		__u32	CursorColorGet( void );
		void	PtzMngSet( Cls_GrPtzV2Mng* A_ObjMng );
		Cls_GrPtzV2Mng*	PtzMngGet( void );
		void	SioMngSet( Cls_GrP4SioMng* A_ObjMng );
		Cls_GrP4SioMng*	SioMngGet( void );
		
		__u8	LangSelGet( void );		// selected language get
		void	LangSelSet( __u8 A_Idx );	// select language

		BOOL8	PageDec( __u16 A_PageId, Cls_GrP4DoBase* A_Prnt );
		__u16	PageAsmOpenGet( __u16 A_PageId );
		__u16	PageAsmCloseGet( __u16 A_PageId );
		__u16	PageEscKeyFcsIdGet( __u16 A_PageId );

		void*	SetupDataPtrGet( __u16 A_Ofs );
		void*	GvuTblGet( void );
		void*	GvsPtrGet( void );
		void*	RegistryPtrGet( void );

		// console
		Cls_GrP4ConBase*	ConsoleGet( __u8 A_ConIdx = E_GrP4ConIdxFocus );
		BOOL8	ConsoleRegist( Cls_GrP4ConBase* A_ObjCon, __u8 A_Idx );
		BOOL8	ConsoleUnregist( Cls_GrP4ConBase* A_ObjCon );

		// UI message
		BOOL8	UiMsgPost( Ptr_GrP4MsgHd A_PtrMsg, __u32 A_Size );
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
		__u8	VkbdTxtPageCntGet( __u8 A_Lang = E_GrP4MngLangAuto );
		WCHAR	VkbdTxtKeyCodeGet( __u8 A_Page, __u8 A_Key, __u8 A_Lang = E_GrP4MngLangAuto );
		BOOL8	VkbdTxtRun( Cls_GrP4DoBase* A_Host, WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd, BOOL8 A_IsAdvPasswd );
		BOOL8	VkbdNumRun( Cls_GrP4DoBase* A_Host,  WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd );
		BOOL8	VkbdRun( Cls_GrP4DoBase* A_Host, WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd, BOOL8 A_IsNumType, BOOL8 A_IsAdvPasswd );

		// virtual machine
		virtual	BOOL8	VmRunCode( __u16 A_CodeId, Cls_GrP4DoBase* A_ObjHost, Ptr_GrP4MngVmRpara A_PtrRpara = NULL, BOOL8 A_IsRunNow = FALSE );
		BOOL8	VmReqEvtCall( __u16 A_CodeId, Cls_GrP4DoBase* A_ObjHost, Ptr_GrP4MngVmRpara A_PtrRpara = NULL );

		BOOL8	DpcUpiCall( __u8 A_ConIdx, __u16 A_WinScptId, __u16 A_ObjScptId, __u16 A_UpiCmd, 
			__s32 A_Para1 = 0, __s32 A_Para2 = 0, __s32 A_Para3 = 0, __s32 A_Para4 = 0, 
			__s32 A_Para5 = 0, __s32 A_Para6 = 0 );

		__s32	SysValueGet(__u8 A_Type, __u32 A_Para);
		void	SysValueSet(__u8 A_Type, __u32 A_Para, __s32 A_Val);


		// message box
		BOOL8	NotifyBox( __u8 A_ConIdx, __u16 A_WinScptId, WCHAR* A_StrTitle, WCHAR* A_StrCont, __u8 A_Time, __u16 A_RefPosWin );
		BOOL8	NotifyBox(__u8 A_ConIdx, __u16 A_WinScptId, __u16 A_TxtIdTitle, __u16 A_TxtIdCont, __u8 A_Time, __u16 A_RefPosWin);

		// a/v status
		void	LiveAtbSet( __u8 A_Ch, __u8 A_Atb );
		__u8*	LiveAtbTblPtrGet( void );

		__u8*	PlayAtbTblPtrGet( void );

		BOOL8	IsPlayMode( void );

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
		void*	UiScptDecThis(Cls_GrP4DoBase* A_Obj, void* A_PtrScpt);
		__u32	UiScptDecRemain(Cls_GrP4DoBase* A_Obj, void* A_PtrScpt);

		// catalog
		BOOL8	CtlgLoad( __u8 A_Id, __s32* A_PtrParaTbl, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt );
		BOOL8	CtlgPageNext( __u8 A_Id, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt, __u32* A_PtrRtPagePos );
		BOOL8	CtlgPagePrev( __u8 A_Id, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt, __u32* A_PtrRtPagePos );
		BOOL8	CtlgItemValueGet( __u32 A_Hndl, __u8 A_ElemCode, __s32 A_AddVal, __s32* A_PtrRtVal );
		BOOL8	CtlgItemStrGet( __u32 A_Hndl, __u8 A_ElemCode, __s32 A_AddVal, WCHAR* A_StrRt );

		// make text
		BOOL8	EvtLogToWstr(Ptr_GrDvrLogItm A_PtrLog, WCHAR* A_PtrRtStr);

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
		void	RtlPlayModeSet( BOOL8 A_IsPlayMode );
		void	RtlDateDispFmtUpdt( void );
		BOOL8	RtlLogIn( __u8 A_Id, WCHAR* A_StrPass, WCHAR* A_StrDual );
		void	RtlLogOut( void );
		BOOL8	RtlPasswdRuleChk( void );
		void	RtlTryRecEmgc( BOOL8 A_IsOn );
		void	RtlDispAutoSeqSet( BOOL8 A_IsAtSeq );
		void	RtlProcObjRelease( Cls_GrP4DoBase* A_Obj );
		BOOL8	RtlVmWakeup(__u8 A_VmIdx, Ptr_GrP4MngVmRpara A_PtrRpara = NULL);

		// editor
#ifdef GR_P4_EDITOR
		virtual	__u32	DvlConResX( __u8 A_ConIdx );
		virtual	__u32	DvlConResY( __u8 A_ConIdx );
#endif


};

//====================================================================
//global function
BOOL8	GrP4MngInit( Cls_GrTaskSvr* A_TaskSvr );
BOOL8	GrP4MngFinish( void );
void	GrP4MngGetWinIdInfo( __u16 A_WinId, __u8* A_PtrRtLayer, __u8* A_PtrRtSlot );
__u16	GrP4MngMakeWinId( __u8 A_Layer, __u8 A_Slot, __u8 A_Seq );
__u32	GrP4MngMakeCtlgHndl( __u8 A_Id, __u32 A_ItemIdx );
__u32	GrP4MngCtlgHndlToIdx( __u32 A_CtlgHndl, __u32* A_PtrRtId );

//====================================================================
//out Environment conditional
#endif

