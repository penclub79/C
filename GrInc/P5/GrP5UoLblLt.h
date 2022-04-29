/*
Platform 5 Label local text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoLblLt
#define	_EPC_GrP5UoLblLt

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoLblBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

#if 0
typedef struct St_GrP5UoLblLtPara
{
	__u16	TxtId;
	__u16	_rsvd0;
}	*Ptr_GrP5UoLblLtPara;

#endif

//====================================================================
//global class

class Cls_GrP5UoLblLt : public Cls_GrP5UoLblBase
{
	protected:

		//St_GrP5UoLblLtPara	m_ParaUoLblLt;

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoLblLt(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoLblLt(void);

		// basic information

		// control
		void	TextSet(WCHAR* A_Str);

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

