/*
Platform 5 Button tab button base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnTabBase
#define	_EPC_GrP5UoBtnTabBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoBtnBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoBtnTabBasePara
{
	__u16		TabScptId;
	__u16		PageScptId;
	BOOL8		IsPush;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
}	*Ptr_GrP5UoBtnTabBasePara;

//====================================================================
//global class

class Cls_GrP5UoBtnTabBase	: public Cls_GrP5UoBtnBase
{
	protected:

		St_GrP5UoBtnTabBasePara	m_ParaUoBtnTabBase;

		Cls_GrP5UoBase*	HostTabCtrlGet(void);

		// inherited
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// draw

		// event 
		virtual	void	DoEvtBtnRelease(void);

	public:
		Cls_GrP5UoBtnTabBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoBtnTabBase(void);

		// basic information
		BOOL8	IsTabSelected(void);

		// basic command
		void	DoTabCommand(void);

		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

