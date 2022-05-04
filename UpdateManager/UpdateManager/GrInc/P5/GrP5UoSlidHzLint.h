/*
Platform 5 UI scroll bar standard

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSlidHzLint
#define	_EPC_GrP5UoSlidHzLint

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoSlidHzBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

#if 0
typedef struct St_GrP5UoSlidHzLintPara
{
	BOOL8		IsFlat;
	BOOL8		IsTrsprnt;
	BOOL8		IsSkipKey;
	__u8		SkinStyle;
}	*Ptr_GrP5UoSlidHzLintPara;
#endif

//====================================================================
//global class

class Cls_GrP5UoSlidHzLint : public Cls_GrP5UoSlidHzBase
{
	protected:

		//St_GrP5UoSlidHzLintPara	m_ParaUoSlidHzLint;

		// local

		// inherited

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system

		virtual	void	EvtScptDecDone(void);


	public:
		Cls_GrP5UoSlidHzLint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect );
		virtual	~Cls_GrP5UoSlidHzLint(void);

		// basic information


		// control

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

