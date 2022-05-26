/*
Platform 5 progress base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoPrgsRint
#define	_EPC_GrP5UoPrgsRint

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoPrgsBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoPrgsRintPara
{
	__u16		RintMax;
	__u16		RintNow;
}	*Ptr_GrP5UoPrgsRintPara;

//====================================================================
//global class

class Cls_GrP5UoPrgsRint	: public Cls_GrP5UoPrgsBase
{
	protected:

		St_GrP5UoPrgsRintPara	m_ParaUoPrgsRint;

		virtual	void	PrgsValGet(__u32* A_PtrRtMax, __u32* A_PtrRtNow);

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoPrgsRint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoPrgsRint(void);

		// basic information

		
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

