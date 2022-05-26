/*
Platform 5 Button text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnLtxt
#define	_EPC_GrP5UoBtnLtxt

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

typedef struct St_GrP5UoBtnLtxtPara
{
	St_GrP5FontDrawStyle	TxtPara;
	St_GrP5RelRect	TxtRc;

	__u8	TxtLen;
	__u8	FontId;
	BOOL8	IsPushEdit;
	__u8	VkbdType;

}	*Ptr_GrP5UoBtnLtxtPara;

//====================================================================
//global class

class Cls_GrP5UoBtnLtxt	: public Cls_GrP5UoBtnBase
{
	protected:

		St_GrP5UoBtnLtxtPara	m_ParaUoBtnLtxt;

		// inherited
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	EvtScptDecDone(void);
		virtual	void	DoEvtBtnPress(void);
		virtual	void	DoEvtBtnRelease(void);
		virtual	void	EvtVkbdInput(void);

	public:
		Cls_GrP5UoBtnLtxt(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoBtnLtxt(void);

		// basic information

		// control
		void	TextGet(WCHAR* A_StrRtTxt);
		void	TextSet(WCHAR* A_StrTxt);

		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

