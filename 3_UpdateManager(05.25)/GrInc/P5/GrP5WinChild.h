/*
Platform 5 Window child

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5WinChild
#define	_EPC_GrP5WinChild

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5WinBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class

class Cls_GrP5WinChild	: public Cls_GrP5WinBase
{
	protected:


		// event system
		virtual	void	EvtPaintProc(void);

	public:
		Cls_GrP5WinChild(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5WinChild(void);

		// basic information

		// basic control
		virtual	BOOL8	CloseRequest(void);

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

