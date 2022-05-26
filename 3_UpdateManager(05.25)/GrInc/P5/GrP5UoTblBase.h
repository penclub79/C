/*
Platform 5 List base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoTblBase
#define	_EPC_GrP5UoTblBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5TblItemDfltMaxCnt					4
#define	E_GrP5TblItemDfltHeight					24

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoTblBasePara
{
	St_GrP5FontDrawStyle	TxtPara;
	__u8	FontId;
	__u8	ItemHeight;
	BOOL8	_rsvd0;
	__u8	ColumCnt;

	__u16	ItemMaxCnt;
	BOOL8	IsLineSel;
	BOOL8	_rsvd1;

	__u16	ColSize[E_GrP5SkinTblColumMaxCnt];
	BOOL8	IsFixPitch[E_GrP5SkinTblColumMaxCnt];
	__s8	CharPitch[E_GrP5SkinTblColumMaxCnt];
	__s8	GapHz[E_GrP5SkinTblColumMaxCnt];
	__s8	TxtMgnX[E_GrP5SkinTblColumMaxCnt];
	__s8	TxtMgnY[E_GrP5SkinTblColumMaxCnt];
}	*Ptr_GrP5UoTblBasePara;

//====================================================================
//global class

class Cls_GrP5UoTblBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoTblBasePara	m_ParaUoTblBase;
		St_GrP5SkinCtlTblBase	m_SkinCtlTblBase;

		__u16			m_EvtHostId;			// event host object id

		// paint

		BOOL8	ItemIdxGetByPos(__s32 A_ScrX, __s32 A_ScrY, __s32* A_PtrRtLine, __s32* A_PtrRtColum);
		BOOL8	ItemRectGetByIdx(__u32 A_Line, __s32 A_Colum, Ptr_GrRect A_PtrRc);

		void	NotifyItemSel(void);

		BOOL8	ProcKeyEnter(void);
		BOOL8	ProcKeyUp(void);
		BOOL8	ProcKeyDown(void);
		BOOL8	ProcKeyLeft(void);
		BOOL8	ProcKeyRight(void);

		// inherited
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	void	EvtKeyFocusGet(__u32 A_Para);
		virtual	void	EvtKeyFocusLoss(void);
		virtual	void	EvtMseEnter(void);
		virtual	void	EvtMseLeave(void);

		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtKeyDown(__u8 A_Key);
		virtual	BOOL8	EvtKeyUp(__u8 A_Key);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

	public:
		Cls_GrP5UoTblBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		
		virtual	~Cls_GrP5UoTblBase(void);

		// basic information


		// control
		void	ItemClear(void);
		__u32	ItemCountGet(void);

		void	CursorPosGet(__s32* A_PtrRtCusX, __s32* A_PtrRtCusY);
		void	CursorPosSet(__s32 A_CusX, __s32 A_CusY);

		void	SelectPosGet(__s32* A_PtrRtSelX, __s32* A_PtrRtSelY);
		void	SelectPosSet(__s32 A_SelX, __s32 A_SelY);

		void	ItemMaxCountSet(__u32 A_MaxCnt);
		void	EventHostIdSet(__u16 A_ObjId);

		virtual	__s32	ItemAdd(void);
		BOOL8	ItemSetTextLocal(__u32 A_Line, __u32 A_Colum, WCHAR* A_StrTxt, __u8 A_AlignHz = E_GrAlignCenter, __u8 A_AlignVt = E_GrAlignCenter);
		BOOL8	ItemSetImage(__u32 A_Line, __u32 A_Colum, __u16 A_ImgId, BOOL8 A_IsSprite, __u8 A_AlignHz = E_GrAlignCenter, __u8 A_AlignVt = E_GrAlignCenter);
		BOOL8	ItemSetTextId(__u32 A_Line, __u32 A_Colum, __u16 A_TxtId, __u8 A_AlignHz = E_GrAlignCenter, __u8 A_AlignVt = E_GrAlignCenter);
		BOOL8	ItemSetValDec(__u32 A_Line, __u32 A_Colum, __u32 A_Value, __u8 A_Digit = 0, __u8 A_AlignHz = E_GrAlignCenter, __u8 A_AlignVt = E_GrAlignCenter);
		BOOL8	ItemSetValHex(__u32 A_Line, __u32 A_Colum, __u32 A_Value, __u8 A_Digit = 0, __u8 A_AlignHz = E_GrAlignCenter, __u8 A_AlignVt = E_GrAlignCenter);
		BOOL8	ItemSetTime(__u32 A_Line, __u32 A_Colum, Def_GrTime A_Time, BOOL8 A_IsNoTime = FALSE, BOOL8 A_IsNoDate = FALSE,
			BOOL8 A_IsNoSec = FALSE, __u8 A_GapDate = 1, __u8 A_GapTime = 1, __u8 A_DivGap = 2, BOOL8 A_IsDivider = TRUE,
			BOOL8 A_IsFix = TRUE, BOOL8 A_IsApm = FALSE, WCHAR* A_StrAm = NULL, WCHAR* A_StrPm = NULL, __u8 A_AlignHz = E_GrAlignCenter, __u8 A_AlignVt = E_GrAlignCenter);
				
		WCHAR*	ItemGetText(__u32 A_Line, __u32 A_Colum);
		__u16		ItemGetResourceId(__u32 A_Line, __u32 A_Colum);
		BOOL8		ItemGetValue(__u32 A_Line, __u32 A_Colum,__u32* A_PtrRtVal);

		// runtime


#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif
};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

