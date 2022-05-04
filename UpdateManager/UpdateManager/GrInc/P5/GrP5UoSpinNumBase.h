/*
Platform 5 spin number base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSpinNumBase
#define	_EPC_GrP5UoSpinNumBase

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

#define	E_GrP5UoSpinNumValRplTxtMax			2

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoSpinNumBasePara
{
	BOOL8	IsHex;
	BOOL8	IsFixDigit;
	__u8	MaxDigit;
	__u8	_rsvd0;
	__s32	ValMin;
	__s32	ValMax;
	__s32	ValStep;
	__s32	ValAdder;
	__u16	RplTxtId[E_GrP5UoSpinNumValRplTxtMax];
	__s32	RplVal[E_GrP5UoSpinNumValRplTxtMax];
	__u16	PrefixTxtId;
	__u16	SurfixTxtId;
}	*Ptr_GrP5UoSpinNumBasePara;

//====================================================================
//global class

class Cls_GrP5UoSpinNumBase	: public Cls_GrP5UoSpinBase
{
	protected:

		St_GrP5UoSpinNumBasePara	m_ParaUoSpinNumBase;

		__s32	m_Value;
		WCHAR*	m_StrTxt;

		virtual	BOOL8	DoIncrease(void);
		virtual	BOOL8	DoDecrease(void);

		virtual	void	ValueRead(void);

		// paint
		
		// inherited

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoSpinNumBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoSpinNumBase(void);

		// basic information
		
		// control
		__s32	ValueMinGet(void);
		__s32	ValueMaxGet(void);
		virtual	void	ValueMinSet(__s32 A_Value);
		virtual	void	ValueMaxSet(__s32 A_Value);

		BOOL8	ValueRelpaceTxtGet(__u32 A_Idx, __s32* A_PtrRtVal, __u16* A_PtrRtTxtId);
		BOOL8	ValueRelpaceTxtSet(__u32 A_Idx, __s32 A_Val, __u16 A_TxtId);

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

