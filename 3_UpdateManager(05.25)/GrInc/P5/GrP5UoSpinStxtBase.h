/*
Platform 5 spin setup text base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSpinStxtBase
#define	_EPC_GrP5UoSpinStxtBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoSpinBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoSpinStxtBasePara
{
	__u16	StpAdr;
	__u8	Len;
	__u8	Cnt;

	BOOL8	IsEdit;			// text edit able
	BOOL8	IsCalc;			// calculate address mode
	BOOL8	IsZeroText;
	__u8	_rsvd0;

	__u32	MulVal;
	__u32	AddVal;
	__u16	TextIdZero;
	__u16	__rsvd1;

}	*Ptr_GrP5UoSpinStxtBasePara;

//====================================================================
//global class

class Cls_GrP5UoSpinStxtBase	: public Cls_GrP5UoSpinBase
{
	protected:

		St_GrP5UoSpinStxtBasePara	m_ParaUoSpinStxtBase;

		__u32	m_ValIdx;		// value index

		// inherited
		virtual	BOOL8	DoIncrease(void);
		virtual	BOOL8	DoDecrease(void);

		virtual	void	ValueRead(void);

		virtual	void	ProcSpinClickCont(void);
				
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);
		virtual	void	EvtVkbdInput(void);
		virtual	void	EvtVkbdCancel(void);

	public:
		Cls_GrP5UoSpinStxtBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoSpinStxtBase(void);

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

