/*
Platform 5 arrow base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoArwBase
#define	_EPC_GrP5UoArwBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoArwBasePara
{
	__u32	Margin;
	__u32	MinSize;
	BOOL8	IsKeyCover;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrP5UoArwBasePara;

typedef struct St_GrP5UoArwItem
{
	St_GrRect	RcPnt;
}	*Ptr_GrP5UoArwItem;

//====================================================================
//global class

class Cls_GrP5UoArwBase	: public Cls_GrP5UoBase
{
	protected:

		__s32		m_EditIdx;		// item edit index

		St_GrRect	m_RcCus;		// now cursor rect

		St_GrP5UoArwBasePara	m_ParaUoArwBase;
		St_GrP5SkinCtlArwBase	m_SkinCtlArwBase;

		St_GrP5UoArwItem			m_ArwTbl[E_GrP5SkinArwMaxCnt];

		// local

		void	LcDrawCusRect(__s32 A_IdxX, __s32 A_IdxY);
		void	LcDrawItemRect(__u32 A_ArwIdx);

		void	LcDoItemWork(void);

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	void	EvtKeyFocusGet(__u32 A_Para);
		virtual	void	EvtKeyFocusLoss(void);
		virtual	void	EvtMseEnter(void);
		virtual	void	EvtMseLeave(void);

		virtual	BOOL8	EvtKeyDown(__u8 A_Key);	/* not need inerite */
		virtual	BOOL8	EvtKeyUp(__u8 A_Key);	/* not need inerite */

		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoArwBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoArwBase(void);

		// basic information
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

		// control

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

