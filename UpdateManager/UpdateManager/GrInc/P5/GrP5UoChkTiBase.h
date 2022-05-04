/*
Platform 5 Button check box base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoChkTiBase
#define	_EPC_GrP5UoChkTiBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoBtnTid.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoChkTiBasePara
{
	St_GrP5RelRect	MarkRc;
}	*Ptr_GrP5UoChkTiBasePara;

//====================================================================
//global class

class Cls_GrP5UoChkTiBase	: public Cls_GrP5UoBtnTid
{
	protected:

		St_GrP5UoChkTiBasePara	m_ParaUoChkTiBase;

		virtual	void	LcValueWrite(void);
		virtual	void	LcValueRead(void);

		// inherited
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	DoEvtBtnRelease(void);

		virtual	void	EvtPaintProc(void);
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoChkTiBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoChkTiBase(void);

		// basic information

		// control
		void	ValueSet(BOOL8 A_IsSet);
		BOOL8	ValueGet(void);

		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

