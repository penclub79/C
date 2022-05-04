/*
Platform 5 Window standard

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5WinStd
#define	_EPC_GrP5WinStd

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

class Cls_GrP5WinStd	: public Cls_GrP5WinBase
{
	protected:

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5WinStd(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5WinStd(void);

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

