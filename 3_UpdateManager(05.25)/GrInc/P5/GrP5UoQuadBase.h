/*
Platform 5 quad base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoQuadBase
#define	_EPC_GrP5UoQuadBase

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

typedef struct St_GrP5UoQuadBasePara
{
	BOOL8		IsDrawLine;
	__u8		__rsvd0;
	__u8		__rsvd1;
	__u8		__rsvd2;
	__u32		ClrLine;
}	*Ptr_GrP5UoQuadBasePara;

//====================================================================
//global class

class Cls_GrP5UoQuadBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoQuadBasePara	m_ParaUoQuadBase;

		// function

		void	LcDoEvtQuadChgByUser(void);

		// new inhertable method
		virtual	void	PntQaudAddQuadView(__u8 A_Ch, Ptr_GrRect A_PtrRc);

		// inherite

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

		virtual	void	EvtQuadChanged(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoQuadBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoQuadBase(void);

		// inherite
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

		// basic information

		// control
		virtual	void	ReqDrawByChg(void) = 0;
		
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

