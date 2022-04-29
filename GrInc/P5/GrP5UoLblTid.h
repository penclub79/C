/*
Platform 5 Label Text Id

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoLblTid
#define	_EPC_GrP5UoLblTid

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

typedef struct St_GrP5UoLblTidPara
{
	__u16	TxtId;
	__u16	_rsvd0;
}	*Ptr_GrP5UoLblTidPara;

//====================================================================
//global class

class Cls_GrP5UoLblTid : public Cls_GrP5UoLblBase
{
	protected:

		St_GrP5UoLblTidPara	m_ParaUoLblTid;

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoLblTid(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoLblTid(void);

		// basic information

		// control
		__u16	TextIdGet(void);
		void	TextIdSet(__u16 A_TxtId);

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

