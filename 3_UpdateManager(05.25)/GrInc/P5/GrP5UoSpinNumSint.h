/*
Platform 5 spin number setup integer

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSpinNumSint
#define	_EPC_GrP5UoSpinNumSint

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

typedef struct St_GrP5UoSpinNumSintPara
{
	__u16	StpAdr;
	__u8	VarSize;
	__u8	_rsvd0;
}	*Ptr_GrP5UoSpinNumSintPara;

//====================================================================
//global class

class Cls_GrP5UoSpinNumSint	: public Cls_GrP5UoSpinNumBase
{
	protected:

		St_GrP5UoSpinNumSintPara	m_ParaUoSpinNumSint;

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
		Cls_GrP5UoSpinNumSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoSpinNumSint(void);

		// basic information
		
		// control
		void	SetupAdrSet(__u16 A_Adr);
		__u16	SetupAdrGet(__u16 A_Adr);

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

