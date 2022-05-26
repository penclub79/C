/*
Platform 5 spin setup text register integer

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSpinStxtRint
#define	_EPC_GrP5UoSpinStxtRint

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoSpinStxtBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoSpinStxtRintPara
{
	__u16	RintVal;
	__u16	_rsvd0;

}	*Ptr_GrP5UoSpinStxtRintPara;

//====================================================================
//global class

class Cls_GrP5UoSpinStxtRint	: public Cls_GrP5UoSpinStxtBase
{
	protected:

		St_GrP5UoSpinStxtRintPara	m_ParaUoSpinStxtRint;

		// inherited
		virtual	void	ValueRead(void);
		virtual	void	ValueWrite(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoSpinStxtRint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoSpinStxtRint(void);

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

