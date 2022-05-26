/*
Platform 5 Button virtual key

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnVkey
#define	_EPC_GrP5UoBtnVkey

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

typedef struct St_GrP5UoBtnVkeyPara
{
	St_GrP5FontDrawStyle	TxtPara;
	St_GrP5RelRect	TxtRc;
	__u8		FontId;
	__u8		_rsvd0;
	__u16		_rsvd1;
	__u32		_rsvd2;
	__u32		_rsvd3;
	__u32		_rsvd4;
	WCHAR		Code[E_GrP5ScptLanguageMaxCnt][E_GrP5VkbdShftMax];

}	*Ptr_GrP5UoBtnVkeyPara;

//====================================================================
//global class

class Cls_GrP5UoBtnVkey	: public Cls_GrP5UoBtnBase
{
	protected:

		St_GrP5UoBtnVkeyPara	m_ParaUoBtnVkey;
		St_GrP5SkinCtlBtnVkey	m_SkinCtlBtnVkey;

		BOOL8	m_IsPrntVkbd;

		// inherited
		virtual	void	DoEvtBtnPress(void);
		virtual	void	DoEvtBtnRelease(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoBtnVkey(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoBtnVkey(void);

		// basic information

		// control
		void	VkeyLanguageSet(__u32 A_LangIdx, __u32 A_ShftIdx);

		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

