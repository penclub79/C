/*
Platform 5 calendar mark

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoCalMark
#define	_EPC_GrP5UoCalMark

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoCalBase.h>
#include <GrTimeTool.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoCalMarkPara
{
	BOOL8	IsCanSelOnMark;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrP5UoCalMarkPara;

//====================================================================
//global class

class Cls_GrP5UoCalMark	: public Cls_GrP5UoCalBase
{
	protected:

		St_GrP5UoCalMarkPara	m_ParaUoCalMark;

		// inherited

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);


	public:
		Cls_GrP5UoCalMark(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoCalMark(void);

		// basic information

		// control
		void	ValueMapSet(__u32 A_Map);
		__u32	ValueMapGet(void);
		

		// runtime


#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif
};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

