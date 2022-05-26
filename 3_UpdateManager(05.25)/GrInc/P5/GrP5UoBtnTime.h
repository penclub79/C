/*
Platform 5 Button time

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnTime
#define	_EPC_GrP5UoBtnTime

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoBtnBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoBtnTimePara
{
	St_GrP5FontDrawStyle	TxtPara;
	__u8		FontId;
	__u8		_rsvd0;
	__u16		_rsvd1;
	St_GrP5RelRect	TxtRc;

}	*Ptr_GrP5UoBtnTimePara;

//====================================================================
//global class

class Cls_GrP5UoBtnTime	: public Cls_GrP5UoBtnBase
{
	protected:

		St_GrP5UoBtnTimePara	m_ParaUoBtnTime;

		// text
		Def_GrTime	m_TimeLast;

		void	LcBtnTextBld(void);

		// inherited
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	EvtScptDecDone(void);
		virtual	void	EvtTimerTick(void);

	public:
		Cls_GrP5UoBtnTime(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoBtnTime(void);

		// basic information



		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

