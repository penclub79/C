/*
Platform 5 TAB Standard

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoTabStd
#define	_EPC_GrP5UoTabStd

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoTabBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class

class Cls_GrP5UoTabStd : public Cls_GrP5UoTabBase
{
	protected:

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoTabStd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoTabStd(void);

		// basic information

		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

