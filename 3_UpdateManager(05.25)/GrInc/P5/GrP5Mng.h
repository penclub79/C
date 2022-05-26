/*
	Platform 5 manager

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5Mng
#define	_EPC_GrP5Mng

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <P5/GrP5Script.h>
#include <P5/GrP5Text.h>
#include <P5/GrP5Msg.h>

#include <FontV3/GrFontV3DrawBase.h>

#include <GrLayerData.h>

//====================================================================
//constance

#define	E_GrP5MngTimerPeriod				100

#define	E_GrP5MngDblClkTimeCnt			5

#define	E_GrP5MngMseHideTimeCnt			100

#define	E_GrP5MngTooltipPopTime			10
#define	E_GrP5MngTooltipKeepTime		20

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5MngTooltipCtl
{
	__u16	ObjIdNew;
	__u16	ObjIdNow;
	__u16	ObjIdPop;

	__u16	TxtId;

	__u32	TimePop;
	__u32	TimeKeep;
}	*Ptr_GrP5MngTooltipCtl;


//====================================================================
//global class

class Cls_GrP5Mng	: public Cls_GrTaskCli
{
	protected:
		Cls_GrTaskSvr*	m_TskSvr;

		//__u32	m_MsgAloc;
		__u32	m_TmrMain;

		__u32	m_MseHideTimer;

		__u32	m_AtLogoutTimer;
		__u32	m_AtLogoutExpire;

		// script info
		Ptr_GrP5ScptUiInfoBasic	m_PtrUiInfoBasic;

		// mouse
#if P5_DBLCLK
		BOOL8	m_IsLbtnDblClkChk;
		__u32	m_LbtnDblClkTimer;
#endif

		// font
		Cls_GrFontV3DrawBase*	m_FontTbl[E_GrP5ScptFontMaxCnt];

		// image
		Ptr_GrGdib	m_ImgScptTbl[E_GrP5ScptImgMaxCnt];
		Ptr_GrGdib	m_ImgUserTbl[E_GrP5ImgUserMaxCnt];
		
		// script window table
		Ptr_GrP5ScptObjHead	m_ScptWinTbl[E_GrP5ScptWinMaxCnt];

		// text table
		WCHAR*	m_TxtFixTbl[E_GrP5ScptLanguageMaxCnt][E_GrP5TxtDedicatedCnt];
		WCHAR*	m_TxtUserTbl[E_GrP5ScptLanguageMaxCnt][E_GrP5ScptTxtUserMaxCnt];

		// virtual keyboard table
		Ptr_GrP5VkbdCfg	m_PtrVkbdCfg;		// virtual keyboard configuration

		// tooltip
		St_GrP5MngTooltipCtl	m_Tooltip;

		// local function
		void	FontFreeAll(void);

		void	ExcFontSet(__u32 A_FontId, void* A_PtrData, __u32 A_DataSize);
		void	ExcImageSet(__u32 A_ImgId, void* A_PtrData, __u32 A_DataSize);
		void	ExcWinScptSet(__u32 A_ScptId, void* A_PtrData, __u32 A_DataSize);
		void	ExcSkinCfgSet(void* A_PtrData, __u32 A_DataSize);
		void	ExcTextSet(__u32 A_TxtId, __u32 A_Language, void* A_PtrData, __u32 A_DataSize);
		void	ExcVkbdCfg(void* A_PtrData);

		void	MseCursorOnSet(BOOL8 A_IsOn);

		void	ProcPosMse(__s32 A_X, __s32 A_Y);

		BOOL8	ProcMsgTypeCon(Ptr_GrP5MsgConHd A_PtrMsg);
		BOOL8	ProcMsgTypeUi(Ptr_GrP5MsgUiHd A_PtrMsg);
		void	ProcMsgConRegist(Ptr_GrP5MsgConRegist A_PtrMsg);
		void	ProcMsgConUnregist(Ptr_GrP5MsgConUnregist A_PtrMsg);
		void	ProcMsgInpKey(Ptr_GrP5MsgInpKey A_PtrMsg);
		void	ProcMsgInpMsePosRel(Ptr_GrP5MsgInpMsePosRel A_PtrMsg);
		void	ProcMsgInpMsePosAbs(Ptr_GrP5MsgInpMsePosAbs A_PtrMsg);
		void	ProcMsgInpMseBtn(Ptr_GrP5MsgInpMseBtn A_PtrMsg);
		void	ProcMsgInpMseWheel(Ptr_GrP5MsgInpMseWheel A_PtrMsg);
		void	ProcMsgKeyFocusUoSet(Ptr_GrP5MsgKeyFcsUoSet A_PtrMsg);
		void	ProcMsgQuadChanged(Ptr_GrTaskMsgQuadChgDone A_PtrMsg);
		void	ProcMsgAtLogoutSet(Ptr_GrP5MsgAtLogoutSet A_PtrMsg);

		void	ProcTimerInput(void);
		void	ProcTimerCtrl(void);
		void	ProcTimerObj(void);
		void	ProcTimerCon(void);
		void	ProcTimerWin(void);

		void	LcTimerTooltip(void);

		BOOL8	ImmPntCodeBld(__u8 A_ConIdx);

		virtual	BOOL8	RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg);		/* return = message processed */
		//virtual	BOOL8	RtlBkgProc(void);											/* return = background processed */
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrP5Mng( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrP5Mng(void);

		// status
		__u32	PixelFormatGet(void);

		// input
		void	InputKey(__u8 A_DevId, __u8 A_Key, BOOL8 A_IsPush);
		void	InputMsePosRel(__s32 A_X, __s32 A_Y);
		void	InputMsePosAbs(__s32 A_X, __s32 A_Y);
		void	InputMseBtn(__u8 A_BtnIdx, BOOL8 A_IsPush);
		void	InputMseWheel(__u8 A_WheelIdx, BOOL8 A_IsDown);

		void	AutoLogoutReq(__u32 A_ExpiredSec);

		// script
		void	Reset(void);	/* Do not this function on Editor mode */
		BOOL8	ExcScriptSet(void* A_PtrExc);		// excute script set
		void*	ScriptGet(__u16 A_ScptId);	// ui script get

		// font
		Cls_GrP5Font*	FontGet(__u8 A_FontId);

		// image
		Ptr_GrGdib	ImageGet(__u16 A_ImgId);
		BOOL8	ImageUserSet(__u16 A_ImgId, Ptr_GrGdib A_PtrGdib);

		// text
		WCHAR*	TextGet(__u16 A_TxtId, __u8 A_LanguageId = E_GrP5TxtLanguageIdAuto);

		// virtual keyboard
		__u16	VkbdGetScptId(__u32 A_Type);

		// paint
		BOOL8	ReqPntCodeBld(__u8 A_ConIdx);

		// message
		BOOL8	UiMsgPost(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_MsgSize);
		BOOL8	UiMsgSend(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_MsgSize);

		// runtime
		void	RtlTooltipUpdate(__u16 A_ObjId);

};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

