/*
Platform 5 Label Time base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoLblTimeBase
#define	_EPC_GrP5UoLblTimeBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoLblBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoLblTimeBasePara
{
	__u8	GapDateTime;
	BOOL8	IsNoTime;
	BOOL8	IsNoDate;
	BOOL8	IsNoSec;
}	*Ptr_GrP5UoLblTimeBasePara;

//====================================================================
//global class

class Cls_GrP5UoLblTimeBase : public Cls_GrP5UoLblBase
{
	protected:

		St_GrP5UoLblTimeBasePara	m_ParaUoLblTimeBase;

		BOOL8	m_IsApmShow;

		// event system
		virtual	void	EvtScptDecDone(void);		/* not need inherite */

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoLblTimeBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoLblTimeBase(void);

		// basic information
		BOOL8	IsApmShow(void);

		// control
		void	ApmShowSet(BOOL8 A_IsShow);

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

