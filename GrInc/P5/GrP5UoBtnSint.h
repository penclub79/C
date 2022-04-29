/*
Platform 5 Button setup value

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnSint
#define	_EPC_GrP5UoBtnSint

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

typedef struct St_GrP5UoBtnSintPara
{
	St_GrP5FontDrawStyle	TxtPara;
	St_GrP5RelRect	TxtRc;
	__u16	StpAdr;
	__u8	ValSize;
	__u8	FontId;

	BOOL8	IsPushEdit;
	BOOL8	IsSign;
	BOOL8	IsHex;
	BOOL8	IsObserve;

	__s32	ValMin;
	__s32	ValMax;

}	*Ptr_GrP5UoBtnSintPara;

//====================================================================
//global class

class Cls_GrP5UoBtnSint	: public Cls_GrP5UoBtnBase
{
	protected:

		St_GrP5UoBtnSintPara	m_ParaUoBtnSint;

		__s32	m_Value;
		BOOL8	m_IsHex;

		BOOL8	LcDispValUpdt(BOOL8 A_IsForce);
		void	LcSetupValWrite(__s32 A_Val);

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
		Cls_GrP5UoBtnSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoBtnSint(void);

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

