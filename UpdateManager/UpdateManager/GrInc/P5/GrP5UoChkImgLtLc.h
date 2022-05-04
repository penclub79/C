/*
Platform 5 Button check box base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoChkImgLtLc
#define	_EPC_GrP5UoChkImgLtLc

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoChkImgLtBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

#if 0
typedef struct St_GrP5UoChkImgLtLc
{
	St_GrP5RelRect	MarkRc;
}	*Ptr_GrP5UoChkImgLtLc;
#endif

//====================================================================
//global class

class Cls_GrP5UoChkImgLtLc	: public Cls_GrP5UoChkImgLtBase
{
	protected:

		//St_GrP5UoChkImgLtLc	m_ParaUoChkImgLtLc;

		// inherited
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoChkImgLtLc(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoChkImgLtLc(void);

		// basic information

		// control

		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

