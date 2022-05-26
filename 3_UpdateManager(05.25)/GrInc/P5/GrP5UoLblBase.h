/*
Platform 5 Label base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoLblBase
#define	_EPC_GrP5UoLblBase

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

typedef struct St_GrP5UoLblBasePara
{
	St_GrP5FontDrawStyle	TxtPara;
	__u8		FontId;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd3;
	St_GrP5RelRect	TxtRc;
}	*Ptr_GrP5UoLblBasePara;

//====================================================================
//global class

class Cls_GrP5UoLblBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoLblBasePara	m_ParaUoLblBase;
		St_GrP5SkinCtlLblBase	m_SkinCtlLblBase;

		// event system
		virtual	void	EvtScptDecDone(void);


#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoLblBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoLblBase(void);

		// basic information

		
		// runtime
		void	RtlStatSet(__u32 A_Stat);

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

