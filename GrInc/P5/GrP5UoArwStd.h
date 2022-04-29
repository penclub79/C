/*
Platform 5 arrow base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoArwStd
#define	_EPC_GrP5UoArwStd

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoArwBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoArwStdPara
{
	__u32	ClrRect[E_GrP5SkinArwStdRectMaxCnt];
}	*Ptr_GrP5UoArwStdPara;


typedef struct St_GrP5UoArwStdRctLstItem
{
	St_GrRect	Rect;
}	*Ptr_GrP5UoArwStdRctLstItem;

typedef struct St_GrP5UoArwStdRctLstCtl
{
	__s32	MaxX;
	__s32	MaxY;
	__u32	TblCnt;
	Ptr_GrByteRect	PtrTbl;

}	*Ptr_GrP5UoArwStdRctLstCtl;


//====================================================================
//global class

class Cls_GrP5UoArwStd	: public Cls_GrP5UoArwBase
{
	protected:

		St_GrP5UoArwStdPara	m_ParaUoArwStd;
		St_GrP5SkinCtlArwStd	m_SkinCtlArwStd;


		// local
		St_GrP5UoArwStdRctLstCtl	m_RctCtl;

		void	LcTmrRectDisp(void);

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	void	EvtTimerTick(void);	/* not need inerite ,call by GrP5TimerRegist */

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoArwStd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoArwStd(void);

		// basic information


		// control
		void	EditIndexSet(__s32 A_Idx);
		__s32	EditIndexGet(void);

		BOOL8	ItemInfoSet(__u32 A_Idx, __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Val);
		BOOL8	ItemInfoGet(__u32 A_Idx, __s32* A_PtrRtStX, __s32* A_PtrRtStY, __s32* A_PtrRtEdX, __s32* A_PtrRtEdY,
			__u32* A_PtrRtVal);

		BOOL8	ItemValueSet(__u32 A_Idx, __u32 A_Val);
		__u32	ItemValueGet(__u32 A_Idx);

		void	RectangeListSet(__u8 A_MaxX, __u8 A_MaxY, __u32 A_TblCnt, Ptr_GrByteRect A_PtrTbl);

		// runtime

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
		virtual	void	DvlPreDelete(void);		// prepare delete
#endif

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

