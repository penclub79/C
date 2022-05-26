/*
	Platform 5 console

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5Con
#define	_EPC_GrP5Con

//====================================================================
// option

//#define	E_GrP5ConPntReqChk		/* paint request area check */

//#define	E_GrP5ConPntChk		/* paint area arrange check */


//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>

#include <GrGdibObj.h>

#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Msg.h>
#include <FontV3/GrFontV3DrawBase.h>

//====================================================================
//constance

#define	E_GrP5ConPntRcTgCnt						E_GrP5PntSctMaxCnt
#define	E_GrP5ConPntSrStkCnt					16

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//global class

class Cls_GrP5ConBase	
{
	protected:
		__u8			m_ConIdx;		// console index

		BOOL8			m_IsMseObjUpdt;

		BOOL8			m_IsPntExist;

		St_GrRect	m_RcScr;		// draw able rect

		Cls_GrGdib*	m_GdibScr;

		// paint
		St_GrRect	m_PntTgTbl[E_GrP5ConPntRcTgCnt];
		St_GrRect	m_PntSrStk[E_GrP5ConPntSrStkCnt];
		__s32			m_PntSp;		// paint source stack pointer

		// tooltip
		St_GrP5SkinTooltipCtl	m_Tooltip;

		// method
		void	LcDbgPntSrTgChk(void);

		void*	LcMseGlbWinFind(void);
		void*	LcMseGlbAbleObjGetByPos(__s32 A_X,__s32 A_Y);

		void	LcMseObjUpdate(void);

		void	LcPntBldByRect(Ptr_GrRect A_PtrRect);

		BOOL8	LcPntSrAdd(Ptr_GrRect A_PtrRcAdd);
		BOOL8	LcPntTgAdd(Ptr_GrRect A_PtrRect);

		void	LcPntRcMerge(Ptr_GrRect A_PtrTg, Ptr_GrRect A_PtrRef);
		__s32	LcPntTgFindByRect(Ptr_GrRect A_PtrRc);

		void	LcPntArngProc(void);

		void	LcMprcCloseReq(Ptr_GrP5MsgConWinCloseReq A_PtrMsg);

		// event
		virtual	void	EvtTimer(void);
		virtual	void	EvtMseEnter(void);
		virtual	void	EvtMseLeave(void);

	public:
		Cls_GrP5ConBase(__u8 A_ConIdx);
		virtual	~Cls_GrP5ConBase(void);

		// basic information
		void	ScrRectGet(Ptr_GrRect A_PtrRtRect);	// drawable rect get
		BOOL8	IsCanLeave(void);
		Cls_GrGdib*	ScrGdibGet(void);

#ifdef GR_P5_EDITOR
		virtual	BOOL8	DvlIsEdit(void);
#endif

		// paint
		void	PaintReq(Ptr_GrRect A_PtrRect);
		void	PaintAllReq(void);
		void	UiObjScrRectRebuild(void);

		// control
		virtual	__s8	QuadChGetByPos(__s32 A_X, __s32 A_Y);
		virtual	BOOL8	QuadModeSet(__u8 A_Mode, __u8 A_Page);
		virtual	BOOL8	QuadModeGet(__u8* A_PtrRtMode, __u8* A_PtrRtPage);
		virtual	BOOL8	QuadModeNext(void);
		virtual	BOOL8	QuadModePrev(void);
		virtual	BOOL8	QuadPageNext(void);
		virtual	BOOL8	QuadPagePrev(void);
		virtual	BOOL8	QuadModeExchange(__u8 A_RefCh);
		virtual	Ptr_GrRect	QuadLineRcPtrGet(__u32* A_PtrRtCnt);
		virtual	__u32	QuadLineWidthGet(void);
		virtual	void	QuadLineWidthSet(__u32 A_Width);
		virtual	BOOL8	QuadRectSet(Ptr_GrRect A_PtrRc);
		virtual	Ptr_GrRect	QuadViewRcPtrGet(__u32* A_PtrRtCnt);
		virtual	__s8	QuadChGetByViewIdx(__u8 A_Idx);

		virtual	WCHAR*	QuadChNameGet(__u8 A_Ch);
		virtual	__u8	QuadChRecTypeGet(__u8 A_Ch);

		virtual	__u32	QuadRecChMapGet(void);
		virtual	__u32	QuadMotionChMapGet(void);
		virtual	__u32	QuadSensorChMapGet(void);
		virtual	__u32	QuadPtzChMapGet(void);
		virtual	__u32	QuadAdoChMapGet(void);
		virtual	__u32	QuadIaChMapGet(void);

		virtual	void	CursorOnSet(BOOL8 A_IsOn);
		
		void	ReqMouseObjUpdate(void);

		// event
		virtual	void	EvtVdoFormatChanged(void);
		virtual	BOOL8	EvtMseMove(void);
		virtual	BOOL8	EvtMseLbtnDown(void);
		virtual	BOOL8	EvtMseLbtnUp(void);
		virtual	BOOL8	EvtMseLbtnDblclk(void);
		virtual	BOOL8	EvtMseRbtnDown(void);
		virtual	BOOL8	EvtMseRbtnUp(void);
		virtual	BOOL8	EvtMseMbtnDown(void);
		virtual	BOOL8	EvtMseMbtnUp(void);
		virtual	BOOL8	EvtMseWheelUp(__u8 A_WheelIdx);
		virtual	BOOL8	EvtMseWheelDown(__u8 A_WheelIdx);
		virtual	BOOL8	EvtKeyInput(__u8 A_DevId, __u8 A_Key, BOOL8 A_IsPush);

		virtual	void	EvtPaintPrepare(Ptr_GrRect A_PtrRect);
		virtual	void	EvtPaintHwTransfer(Ptr_GrRect A_PtrRect);

		// runtime
		void	RtlTimer(void);
		void	RtlMseEnter(void);	/* console enter - desktop extend mode only */
		void	RtlMseLeave(void);	/* console leave - desktop extend mode only */
		void	RtlMseMove(void);
		void	RtlMseLbtnDown(void);
		void	RtlMseLbtnUp(void);
#ifdef P5_DBLCLK
		void	RtlMseLbtnDblclk(void);
#endif
		void	RtlMseRbtnDown(void);
		void	RtlMseRbtnUp(void);
		void	RtlMseMbtnDown(void);
		void	RtlMseMbtnUp(void);
		void	RtlMseWheelUp(__u8 A_WheelIdx);
		void	RtlMseWheelDown(__u8 A_WheelIdx);

		BOOL8	RtlMsgProc(Ptr_GrP5MsgHd A_PtrMsg);

		void	RtlUiObjReleaseAll(void);

		void	RtlPaintCodeBuild(void);

		void	RtlTooltipSet(Ptr_GrRect A_PtrRcBase,__u16 A_TxtId);

};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

