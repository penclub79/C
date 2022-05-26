/*
Platform 5 Label local text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoLblTimeReg
#define	_EPC_GrP5UoLblTimeReg

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoLblTimeBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoLblTimeRegPara
{
	__u16	RegTime;
	BOOL8	IsObserve;
	BOOL8	IsSpriteImg;
	__u16	ImgDstId;
	__u16	_rsvd0;
	St_GrP5RelRect	RcIconDst;
}	*Ptr_GrP5UoLblTimeRegPara;

//====================================================================
//global class

class Cls_GrP5UoLblTimeReg : public Cls_GrP5UoLblTimeBase
{
	protected:

		St_GrP5UoLblTimeRegPara	m_ParaUoLblTimeReg;

		BOOL8	m_IsDst;
		__u32	m_TimeVal;

		void	LcTimeValueUpdate(void);
		void	LcTimeDispUpdate(Def_GrTime A_Time);

		// event system
		virtual	void	EvtScptDecDone(void);
		virtual	void	EvtTimerTick(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoLblTimeReg(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoLblTimeReg(void);

		// basic information

		// control
		Def_GrTime	TimeGet(void);

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

