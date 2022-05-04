/*
Platform 5 Label local text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoLblTimeView
#define	_EPC_GrP5UoLblTimeView

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

typedef struct St_GrP5UoLblTimeViewPara
{
	St_GrP5RelRect	RcIconDst;
	BOOL8	IsPrgs;
	BOOL8	IsSpriteImg;
	__u16	ImgDst;
}	*Ptr_GrP5UoLblTimeViewPara;

//====================================================================
//global class

class Cls_GrP5UoLblTimeView : public Cls_GrP5UoLblTimeBase
{
	protected:

		St_GrP5UoLblTimeViewPara	m_ParaUoLblTimeView;

		BOOL8	m_IsDst;
		Def_GrTime	m_TimeVal;

		void	LcTimeValueUpdate(void);

		// event system
		virtual	void	EvtScptDecDone(void);
		virtual	void	EvtTimerTick(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoLblTimeView(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoLblTimeView(void);

		// basic information
		Def_GrTime	TimeGet(void);
		void	TimeSet(Def_GrTime A_Time);

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

