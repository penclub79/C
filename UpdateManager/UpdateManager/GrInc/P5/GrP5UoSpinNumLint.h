/*
Platform 5 spin number local integer

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSpinNumLint
#define	_EPC_GrP5UoSpinNumLint

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoSpinNumBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

#if 0
typedef struct St_GrP5UoSpinNumLintPara
{
	__u16	StpAdr;
	__u8	VarSize;
	__u8	_rsvd0;
}	*Ptr_GrP5UoSpinNumLintPara;
#endif

//====================================================================
//global class

class Cls_GrP5UoSpinNumLint	: public Cls_GrP5UoSpinNumBase
{
	protected:

		//St_GrP5UoSpinNumLintPara	m_ParaUoSpinNumLint;

		// inherited

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoSpinNumLint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoSpinNumLint(void);

		// basic information
		
		// control
		__s32	ValueGet(void);
		void	ValueSet(__s32 A_Value);

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

