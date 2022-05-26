/*
Platform 5 Button text id

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnTid
#define	_EPC_GrP5UoBtnTid

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

typedef struct St_GrP5UoBtnTidPara
{
	St_GrP5FontDrawStyle	TxtPara;
	__u16		TxtId;		// text id
	__u8		FontId;
	__u8		_rsvd0;
	St_GrP5RelRect	TxtRc;

}	*Ptr_GrP5UoBtnTidPara;

//====================================================================
//global class

class Cls_GrP5UoBtnTid	: public Cls_GrP5UoBtnBase
{
	protected:

		St_GrP5UoBtnTidPara	m_ParaUoBtnTid;

		// text
		WCHAR*	m_StrTxt;		// button base text

		// inherited
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoBtnTid(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoBtnTid(void);

		// basic information

		// control
		void	TextIdSet(__u16 A_TxtId);

		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

