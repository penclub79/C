/*
Platform 5 UI control event base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5Mng.h>

#include <GrDumyTool.h>

#include <P5/GrP5UoBase.h>
#include <P5/GrP5WinBase.h>

#include <GrDebug.h>

//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5CtrlEvtBase::Cls_GrP5CtrlEvtBase(void)
{
	// local -------------------
	// code --------------------
		// init

}
//--------------------------------------------------------------------
Cls_GrP5CtrlEvtBase::~Cls_GrP5CtrlEvtBase(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtScptDecDone(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtCloseWin(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtVarChg(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_ValType, __u16 A_ValId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtPageDecDone(__u16 A_CtrlId, __u16 A_ObjId, __u16 A_ScptId)
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtPageQueryChange(__u16 A_CtrlId, __u16 A_ObjId)
{
	return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtBtnPress(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtBtnRelease(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtBtnValChg(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtListItemSel(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtTblItemSel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_Line, __s32 A_Colum)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtSpinValChanged(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtSpinClickCont(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtSlidValChanged(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtArwValChanged(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtCalDaySel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_TimeSel)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtGraphItemSel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_IdxX, __u32 A_IdxY)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtGraphCusOver(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_Dir)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtVkbdInput(__u16 A_CtrlId, WCHAR* A_Str, __u32 A_MaxLen, __u32 A_UserData)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtVkbdCancel(__u16 A_CtrlId, WCHAR* A_Str, __u32 A_MaxLen, __u32 A_UserData)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtGridItemSel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_IdxX, __u32 A_IdxY)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtGridItemAreaSel(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_StX, __u32 A_StY, __u32 A_EdX, __u32 A_EdY)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtAim(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, BOOL8 A_IsBtnPush)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtDir(__u16 A_CtrlId, __u16 A_ObjId, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown)
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtKeyDown(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_Key)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtKeyUp(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_Key)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtMseLbtnDown(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtMseLbtnUp(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtMseRbtnDown(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtMseRbtnUp(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtMseMbtnDown(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtMseMbtnUp(__u16 A_CtrlId, __u16 A_ObjId, __s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtMseWheelUp(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_WheelIdx)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtMseWheelDown(__u16 A_CtrlId, __u16 A_ObjId, __u8 A_WheelIdx)
{
	return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtCbxItemSel(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtQuadChgByUser(__u16 A_CtrlId, __u16 A_ObjId)
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5CtrlEvtBase::EvtMenuClick(__u16 A_CtrlId, __u16 A_ObjId, __u32 A_ItemId, __u16 A_Squad, __u32 A_UserData)
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP5CtrlEvtBase::EvtDragArea(__u16 A_CtrlId, __u16 A_ObjId, Ptr_GrRect A_PtrRcDrag, Ptr_GrRect A_PtrRcObj)
{

}
//--------------------------------------------------------------------
