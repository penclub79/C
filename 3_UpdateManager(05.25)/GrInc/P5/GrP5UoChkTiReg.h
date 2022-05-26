/*
Platform 5 checkbox text id register

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoChkTiReg
#define	_EPC_GrP5UoChkTiReg

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoChkTiBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoChkTiRegPara
{
	__u16	RegIdx;
	__u8	Bit;
	__u8	_rsvd0;
}	*Ptr_GrP5UoChkTiRegPara;

//====================================================================
//global class

class Cls_GrP5UoChkTiReg : public Cls_GrP5UoChkTiBase
{
	protected:

		St_GrP5UoChkTiRegPara	m_ParaUoChkTiReg;

		// inherited
		virtual	void	LcValueWrite(void);
		virtual	void	LcValueRead(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoChkTiReg(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoChkTiReg(void);

		// basic information



		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

