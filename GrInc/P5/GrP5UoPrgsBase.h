/*
Platform 5 progress base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoPrgsBase
#define	_EPC_GrP5UoPrgsBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoPrgsBasePara
{
	__u8		FontId;
	BOOL8		IsDrawTxt;
	BOOL8		IsPercentage;
	BOOL8		IsShowTotal;
	__u8		Direction;
	__u8		__rsvd0;
	__u16		SuffixTxtid;
	St_GrP5FontDrawStyle	TxtPara;
}	*Ptr_GrP5UoPrgsBasePara;

//====================================================================
//global class

class Cls_GrP5UoPrgsBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoPrgsBasePara	m_ParaUoPrgsBase;
		St_GrP5SkinCtlPrgsBase	m_SkinCtlPrgsBase;
		
		virtual	void	PrgsValGet(__u32* A_PtrRtMax, __u32* A_PtrRtNow);
		virtual	void	PrgsTxtUpdate(void);

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	void	EvtTimerTick(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoPrgsBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoPrgsBase(void);

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

