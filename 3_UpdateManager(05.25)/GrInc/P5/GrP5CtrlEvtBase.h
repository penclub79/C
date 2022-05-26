/*
	Platform 5 control event

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5CtrlEvtBase
#define	_EPC_GrP5CtrlEvtBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>

#include <GrGdibObj.h>

#include <P5/GrP5Msg.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//global class

class Cls_GrP5CtrlEvtBase	
{
	protected:


	public:
		Cls_GrP5CtrlEvtBase(void);
		virtual	~Cls_GrP5CtrlEvtBase(void);

		// event
		virtual	void	EvtScptDecDone(__u16 A_CtrlId,__u16 A_ObjId );	// object decoded done
		virtual	void	EvtCloseWin(__u16 A_CtrlId, __u16 A_ObjId);		// close window or tab page
		virtual	void	EvtVarChg(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_ValType, __u16 A_ValId);	// variable value change
		virtual	void	EvtPageDecDone(__u16 A_CtrlId, __u16 A_ObjId, __u16 A_ScptId);	// tab page decoded
		virtual	BOOL8	EvtPageQueryChange(__u16 A_CtrlId, __u16 A_ObjId);	// tab quary page change

		virtual	void	EvtBtnPress(__u16 A_CtrlId, __u16 A_ObjId);	// button press
		virtual	void	EvtBtnRelease(__u16 A_CtrlId, __u16 A_ObjId);	// button release , button click
		virtual	void	EvtBtnValChg(__u16 A_CtrlId, __u16 A_ObjId);	// button value change

		virtual	void	EvtCbxItemSel(__u16 A_CtrlId, __u16 A_ObjId);	// combo box item select
		virtual	void	EvtListItemSel(__u16 A_CtrlId, __u16 A_ObjId);	// list item select
		virtual	void	EvtTblItemSel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_Line, __s32 A_Colum);	// table item select
		virtual	void	EvtSpinValChanged(__u16 A_CtrlId, __u16 A_ObjId);
		virtual	void	EvtSpinClickCont(__u16 A_CtrlId, __u16 A_ObjId);
		virtual	void	EvtSlidValChanged(__u16 A_CtrlId, __u16 A_ObjId);	// slide value change
		virtual	void	EvtArwValChanged(__u16 A_CtrlId, __u16 A_ObjId);	// arrow value changed

		virtual	void	EvtCalDaySel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_TimeSel);

		virtual	void	EvtGridItemSel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_IdxX, __u32 A_IdxY);
		virtual	void	EvtGridItemAreaSel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_StX, __u32 A_StY,__u32 A_EdX,__u32 A_EdY);
		virtual	void	EvtGraphItemSel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_IdxX, __u32 A_IdxY);
		virtual	void	EvtGraphCusOver(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_Dir);

		virtual	void	EvtVkbdInput(__u16 A_CtrlId,WCHAR* A_Str,__u32 A_MaxLen,__u32 A_UserData);
		virtual	void	EvtVkbdCancel(__u16 A_CtrlId, WCHAR* A_Str, __u32 A_MaxLen, __u32 A_UserData);

		virtual	void	EvtAim(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, BOOL8 A_IsBtnPush);
		virtual	void	EvtDir(__u16 A_CtrlId, __u16 A_ObjId, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown);

		virtual	void	EvtQuadChgByUser(__u16 A_CtrlId, __u16 A_ObjId);	// quad change by user control

		virtual	void	EvtDragArea(__u16 A_CtrlId, __u16 A_ObjId, Ptr_GrRect A_PtrRcDrag, Ptr_GrRect A_PtrRcObj);

		virtual	BOOL8	EvtKeyDown(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_Key);
		virtual	BOOL8	EvtKeyUp(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_Key);

		virtual	BOOL8	EvtMseLbtnDown(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseRbtnDown(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseRbtnUp(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseMbtnDown(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseMbtnUp(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseWheelUp(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_WheelIdx);
		virtual	BOOL8	EvtMseWheelDown(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_WheelIdx);

		virtual	BOOL8	EvtMenuClick(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_ItemId, __u16 A_Squad, __u32 A_UserData);
};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

