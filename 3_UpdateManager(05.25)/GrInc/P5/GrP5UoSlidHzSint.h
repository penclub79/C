/*
Platform 5 UI scroll bar standard

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSlidHzSint
#define	_EPC_GrP5UoSlidHzSint

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoSlidHzBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoSlidHzSintPara
{
	__u16	StpAdr;
	__u8	VarSize;
	__u8	_rsvd0;
}	*Ptr_GrP5UoSlidHzSintPara;

//====================================================================
//global class

class Cls_GrP5UoSlidHzSint : public Cls_GrP5UoSlidHzBase
{
	protected:

		St_GrP5UoSlidHzSintPara	m_ParaUoSlidHzSint;

		// local

		// inherited
		virtual	void	RtlValueRead(void);
		virtual	void	RtlValueWrite(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system

		virtual	void	EvtScptDecDone(void);


	public:
		Cls_GrP5UoSlidHzSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect );
		virtual	~Cls_GrP5UoSlidHzSint(void);

		// basic information


		// control
		void	SetupAdrSet(__u16 A_Adr);
		__u16	SetupAdrGet(__u16 A_Adr);

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

