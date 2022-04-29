/*
Platform 5 Window popup base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5WinPopBase
#define	_EPC_GrP5WinPopBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5WinBase.h>
#include <P5/GrP5UoListBase.h>
#include <P5/GrP5UoListRef.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5WinPopBaseItemHeightMin		8

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class

class Cls_GrP5WinPopBase	: public Cls_GrP5WinBase
{
	protected:

		__u16	m_PrntPopObjId;
		__u16	m_SlvPopObjId;

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5WinPopBase(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_PrntPopupObjId);
		virtual	~Cls_GrP5WinPopBase(void);

		// basic information
		__u16	ParentPopupIdGet(void);
		__u16	SlavePopupIdGet(void);
		
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

