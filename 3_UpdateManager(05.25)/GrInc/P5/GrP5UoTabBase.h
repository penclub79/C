/*
Platform 5 tab base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoTabBase
#define	_EPC_GrP5UoTabBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5UoTabLvlCnt				4

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoTabBasePara
{
	__u16	ScptIdStart;
	BOOL8	IsContNoSkin;
	__u8	_rsvd0;
	St_GrP5RelRect	ContRc;
	__u32	ContClr;
}	*Ptr_GrP5UoTabBasePara;

typedef struct St_GrP5UoTabStack
{
	__u16	ScptId;			// window script id
	__u16	FocusId;		// last focused script id
}	*Ptr_GrP5UoTabStack;

//====================================================================
//global class

class Cls_GrP5UoTabBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoTabBasePara	m_ParaUoTabBase;
		St_GrP5SkinCtlTabBase	m_SkinCtlTabBase;

		Cls_GrP5UoBase*	m_WinChild;
		St_GrP5UoTabStack	m_PageStack[E_GrP5UoTabLvlCnt];
		__u16	m_StackIdx;

		void	DoEvtPageSet(__u16 A_ScptId);
		void	DoTabPageSet(__u16 A_ScptId, __u16 A_FocusId);
		BOOL8	QueryPageChange(void);

		void	MprcTabPageSet(Ptr_GrP5MsgUiTabPageSet A_PtrMsg);
		void	MprcTabPagePop(void);

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	BOOL8	EvtKeyUp(__u8 A_Key);
		virtual	BOOL8	EvtMseRbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoTabBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoTabBase(void);

		// basic information
		virtual	BOOL8	IsMseAble(void);

		Cls_GrP5UoBase*	ChildWinGet(void);

		// tab control
		void	TabPageSet(__u16 A_ScptId, BOOL8 A_IsPush);
		void	TabPagePop(void);
		__u16	TabNowPageScptIdGet(void);

		// runtime
#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
		virtual	void	DvlPreDelete(void);		// prepare delete
#endif

		virtual	BOOL8	RtlProcUiMsg(Ptr_GrP5MsgUiHd A_PtrMsg);
		virtual	void	RtlDoEvtScptDecDone(void);
};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

