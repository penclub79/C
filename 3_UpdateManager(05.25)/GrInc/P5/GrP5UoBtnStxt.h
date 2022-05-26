/*
Platform 5 Button text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnStxt
#define	_EPC_GrP5UoBtnStxt

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

typedef struct St_GrP5UoBtnStxtPara
{
	St_GrP5FontDrawStyle	TxtPara;
	St_GrP5RelRect	TxtRc;
	__u16	StpAdr;
	__u8	TxtLen;
	__u8	FontId;

	BOOL8	IsPushEdit;
	__u8	VkbdType;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrP5UoBtnStxtPara;

//====================================================================
//global class

class Cls_GrP5UoBtnStxt	: public Cls_GrP5UoBtnBase
{
	protected:

		St_GrP5UoBtnStxtPara	m_ParaUoBtnStxt;

		// text
		void	LcTxtPtrUpdt(void);

		// inherited
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	EvtPaintProc(void);
		virtual	void	EvtScptDecDone(void);
		virtual	void	DoEvtBtnPress(void);
		virtual	void	DoEvtBtnRelease(void);
		virtual	void	EvtVkbdInput(void);

	public:
		Cls_GrP5UoBtnStxt(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoBtnStxt(void);

		// basic information

		// control
		void	SetupAdrSet(__u16 A_Adr);
		__u16	SetupAdrGet(__u16 A_Adr);
		
		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

