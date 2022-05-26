/*
Platform 5 graph horizontal base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoGrpHzBase
#define	_EPC_GrP5UoGrpHzBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <GrTimeTool.h>

#include <GrGdibObj.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoGrpHzBasePara
{
	__u8	LineHeight;
	BOOL8	IsDoExistOnly;
	BOOL8	IsEvtOutRange;
	__u8	_rsvd0;
}	*Ptr_GrP5UoGrpHzBasePara;

//====================================================================
//global class

class Cls_GrP5UoGrpHzBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoGrpHzBasePara	m_ParaUoGrpHzBase;
		St_GrP5SkinCtlGrpHzBase	m_SkinCtlGrpHzBase;

		virtual	void	LcDoCusPosChg(__s32 A_X, __s32 A_Y);
		void	LcDoEvtCusOver(__u32 A_Dir);

		BOOL8	LcCursorVtCalcRect(__u32 A_IdxX, Ptr_GrRect A_PtrRtRc);

		void	LcRedrawCusArea(__u32 A_IdxX, __u32 A_IdxY);

		virtual	void	DoEvtGrpItemClick(void);

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

		virtual	BOOL8	EvtKeyDown(__u8 A_Key);
		virtual	BOOL8	EvtKeyUp(__u8 A_Key);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

	public:
		Cls_GrP5UoGrpHzBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoGrpHzBase(void);

		virtual	__u8	ItemValueGet(__u32 A_IdxX, __u32 A_IdxY) = 0;
		
		// basic information
		void	SelectIdxGet(__s32* A_PtrRtSelX, __s32* A_PtrRtSelY);
		void	CusPosGet(__s32* A_PtrRtX, __s32* A_PtrRtY);

		// control
		void	SelectIdxSet(__s32 A_IdxX, __s32 A_IdxY);
		void	DoExistOnlySet(BOOL8 A_IsOn);

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

