/*
Platform 5 UI object base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBase
#define	_EPC_GrP5UoBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>

#include <GrGdibObj.h>

#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Msg.h>

//====================================================================
//constance

#define	E_GrP5UoUserDataCnt					4

#define	E_GrP5UoHierarchyMax				8

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoHierarchy
{
	__u16		ClassId;
	__u16		ParaSize;
	void*		PtrPara;
	void*		PtrSkinCtl;		// skin control
}	*Ptr_GrP5UoHierarchy;

typedef struct St_GrP5UoBasePara
{
	__u32		UserData[E_GrP5UoUserDataCnt];

	__u16		CtrlId;
	BOOL8		IsVisible;
	BOOL8		IsEnable;

	__u32		UserId;

	__u16		KeyFcs[E_GrP5DirCnt];

	BOOL8		IsCtlEvtKey;
	BOOL8		IsCtlEvtMse;
	__u16		TtTxtid;			// tooltip text id

	St_GrP5SkinPara	SkinPara;

}	*Ptr_GrP5UoBasePara;

//====================================================================
//global class

class Cls_GrP5UoBase
{
	protected:

		__u8		m_ConIdx;

		__u16		m_ScptId;
		__u16		m_ObjId;

		__u16		m_ClassId;

		__u32		m_InhHierCnt;
		St_GrP5UoHierarchy	m_InhHierTbl[E_GrP5UoHierarchyMax];
		St_GrRect	m_RcSkinBase;		// skin base rect

		St_GrP5RelRect	m_RcRel;

		St_GrP5UoBasePara	m_ParaUoBase;

		Cls_GrP5UoBase*	m_ObjPrnt;
		Cls_GrP5UoBase*	m_ObjWin;			// parent window
		//Cls_GrP5UoBase*	m_ObjFrm;			// parent frame
		Cls_GrP5UoBase*	m_ObjNext;
		Cls_GrP5UoBase*	m_ObjSlave;

		Cls_GrP5SkinBase*	m_SkinMng;

#ifdef GR_P5_EDITOR
		__u32	m_DvlScptHdPara;				// script head para
		void*	m_DvlPtrScptBuf;
		__u32	m_DvlScptPrgs;

		BOOL8	m_DvlIsNotScpt;
#endif

		// mouse

		// paint
		St_GrRect	m_RcScr;			// screen rect
		//Cls_GrGdib*	m_GdibScr;	// GDIB screen

		// dvelope tool
#ifdef GR_P5_EDITOR
		void	DvlParaInfoAdd(void* A_PtrAdd, __u32 A_AddCnt, void* A_PtrBasis);
		void	DvlParaInfoRelPosAdd(WCHAR* A_StrSep, void* A_PtrRelPos);
		void	DvlParaInfoTxtParaAdd(WCHAR* A_StrSep, void* A_PtrTxtPara);
		void	DvlScptParaAdd(__u16 A_ClassId, void* A_PtrPara, __u16 A_Size);
		void	DvlScptParaEncode(void);	// parametter encode

		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		void	LcChkNeedKfcCor(void);
		void	LcPntReqByRect(Ptr_GrRect A_PtrRect);	// paint rect
		void	LcSkinCtlUpdate(void);

		// tools
		void	ClassRegist(__u16 A_ClassId,void* A_PtrPara, __u32 A_ParaSize, void* A_PtrSkinCtl);
		void	UoSlaveFreeAll(void);
		void	MseCapture(void);
		void	MseRelese(void);

		// event system
		virtual	void	EvtPaintProc(void);		// pre paint event

		// event input
		virtual	void	EvtScptDecDone(void);


	public:
		Cls_GrP5UoBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);	// for slave object
		virtual	~Cls_GrP5UoBase(void);

		// basic information
		__u16	ScptIdGet(void);
		__u16	ObjIdGet(void);
		__u16	CtrlIdGet(void);
		__u16	ClassIdGet(void);
		__u32	UserIdGet(void);
		__u16	TooltipTextIdGet(void);

		virtual	BOOL8	IsCanKeyInput(void);
		virtual	BOOL8	IsCanMseInput(void);

		BOOL8	IsEnable(void);
		BOOL8	IsVisible(void);
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

		virtual	BOOL8	IsCanEnable(void);
		virtual	BOOL8	IsCanVisible(void);

		BOOL8	IsOnFocuse(void);

		__u32	UserDataGet(__u32 A_Idx);
		void	UserDataSet(__u32 A_Idx, __u32 A_Val);

		BOOL8	IsInheritClass(__u16 A_ClassId);
		Ptr_GrP5UoHierarchy	InheritClassInfoGet(__u16 A_ClassId);

		BOOL8	ScrRectGet(Ptr_GrRect A_PtrRtRect);
		BOOL8	ParentScrRectGet(Ptr_GrRect A_PtrRtRect);

		__u8	ConsoleIdxGet(void);

		// object information
		Cls_GrP5UoBase*	ParentWinGet(void);
		Cls_GrP5UoBase*	ParentObjGet(void);
		Cls_GrP5UoBase*	NextObjGet(void);
		Cls_GrP5UoBase*	PrevObjGet(void);
		Cls_GrP5UoBase*	SlaveObjGet(void);
		Cls_GrP5UoBase*	ParentClassObjectGet(__u16 A_ClassId);

		// basic control
		virtual	void	ReqDraw(BOOL8 A_IsForce);
		BOOL8	PosMoveAbsolute(__s32 A_X, __s32 A_Y, BOOL8 A_IsRedraw);
		BOOL8	PosMoveRelate(__s32 A_X, __s32 A_Y, BOOL8 A_IsRedraw);
		BOOL8	SizeChangeAbsolute(__s32 A_X, __s32 A_Y, BOOL8 A_IsRedraw);
		BOOL8	SizeChangeRelate(__s32 A_X, __s32 A_Y, BOOL8 A_IsRedraw);
		BOOL8	PosSizeChange(__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsPosRel, __s32 A_SizeX, __s32 A_SizeY, BOOL8 A_IsSizeRel, BOOL8 A_IsRedraw);

		void	ReqKeyFocusSet(void);

		BOOL8	IsHasParent(__u16 A_ObjId);

		void	EnableSet(BOOL8 A_IsEnable);
		void	VisiableSet(BOOL8 A_IsVisible);
		void	KeyFocusSet(__u8 A_Dir, __u16 A_ScptId);
		__u16	KeyFocusGet(__u8 A_Dir);
		Cls_GrP5UoBase*	FindSlaveNearByDir(Ptr_GrRect A_PtrRcBase, __u8 A_Dir, BOOL8 A_IsKeyAbleOnly, __u16 A_ExcpObjId);
		Cls_GrP5UoBase*	FindSlaveFarByDir(Ptr_GrRect A_PtrRcBase, __u8 A_Dir, BOOL8 A_IsKeyAbleOnly, __u16 A_ExcpObjId);

		// evnet - input
		virtual	BOOL8	EvtKeyDown(__u8 A_Key);	
		virtual	BOOL8	EvtKeyUp(__u8 A_Key);

		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseRbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseRbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseMbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseMbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseWheelUp(__u8 A_WheelIdx);
		virtual	BOOL8	EvtMseWheelDown(__u8 A_WheelIdx);

		// event - status
		virtual	void	EvtPosSizeChanged(void);
		virtual	void	EvtKeyFocusGet(__u32 A_Para);	/* not need inerite */
		virtual	void	EvtKeyFocusLoss(void);	/* not need inerite */
		virtual	void	EvtMseEnter(void);	/* not need inerite */
		virtual	void	EvtMseLeave(void);	/* not need inerite */
		virtual	void	EvtQuadChanged(void);	/* not need inerite */

		// event scroll
		virtual	void	EvtScrlVtPos(void);	/* not need inerite */
		virtual	void	EvtScrlHzPos(void);	/* not need inerite */

		// event - system
		virtual	void	EvtTimerTick(void);	/* not need inerite ,call by GrP5TimerRegist */
		virtual	void	EvtLanguageChg(void);	/* not need inerite */

		// event - paint

		// event - relate control
		//virtual	void	EvtVkbdInputEnd(void);
		virtual	void	EvtBtnPress(__u16 A_ObjId);
		virtual	void	EvtBtnRelease(__u16 A_ObjId);
		virtual	void	EvtListItemSel(__u16 A_ObjId, __s32 A_ItemIdx);
		virtual	void	EvtPopSelCancel(void);		// popup select cancel
		virtual	void	EvtSpinValChanged(__u16 A_ObjId);
		virtual	void	EvtSpinClickCont(__u16 A_ObjId);
		virtual	void	EvtVkbdInput(void);
		virtual	void	EvtVkbdCancel(void);

		// develop
#ifdef GR_P5_EDITOR
		// info
		__u32	DvlScptHdParaGet(void);
		void	DvlScptHdParaSet(__u32 A_Val);
		__u32	DvlParaInfoGet(void* A_PtrRtBuf);		// when A_PtrRtBuf is null, count return only

		__u32	DvlScptEncode(void* A_PtrBuf);
		BOOL8	DvlIsNotScpt(void);
		void	DvlNotScptSet(BOOL8 A_IsSet);
		BOOL8	DvlIsCanScptEnc(void);
		Cls_GrP5UoBase*	DvlUoEditableFindByPos(__s32 A_X, __s32 A_Y);

		// ui object implement
		virtual	void	DvlParaInfoAction(__u32 A_Action);
		virtual	void	DvlPreDelete(void);		// prepare delete
		
#endif

		// runtime
		void	RtlNextObjSet(Cls_GrP5UoBase* A_Obj);
		void	RtlSlaveObjSet(Cls_GrP5UoBase* A_Obj);
		virtual	void	RtlScrRectBld(void);

		virtual	void	RtlPaintProc(Cls_GrGdib* A_GdibScr, Ptr_GrRect A_PtrRcScr);

		Cls_GrP5UoBase*	RtlUoFindByPos(__s32 A_X, __s32 A_Y);
		Cls_GrP5UoBase*	RtlUoVisibleFindByPos(__s32 A_X, __s32 A_Y);
		Cls_GrP5UoBase*	RtlUoCanMseFindByPos(__s32 A_X, __s32 A_Y);
		virtual	Cls_GrP5UoBase*	RtlUoFindKeyFocusAble(void);
		Cls_GrP5UoBase*	RtlUoFindByScptId(__u16 A_ScptId);
		Cls_GrP5UoBase*	RtlUoFindNearByDir(Ptr_GrRect A_PtrRcBase, __u8 A_Dir, BOOL8 A_IsKeyAbleOnly, __u16 A_ExcpObjId, Cls_GrP5UoBase* A_RsltObj);
		Cls_GrP5UoBase*	RtlUoFindFarByDir(Ptr_GrRect A_PtrRcBase, __u8 A_Dir, BOOL8 A_IsKeyAbleOnly, __u16 A_ExcpObjId, Cls_GrP5UoBase* A_RsltObj);

		void	RtlScptParaUpdt(void* A_PtrScpt);
		virtual	void	RtlDoEvtScptDecDone(BOOL8 A_IsFocusUpdt = TRUE);
		virtual	BOOL8	RtlProcUiMsg(Ptr_GrP5MsgUiHd A_PtrMsg);
		void	RtlCtrlEvtValChgDo(__u8 A_VarType, __u16 A_VarId);

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

