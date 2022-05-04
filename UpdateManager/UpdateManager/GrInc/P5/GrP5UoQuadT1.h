/*
Platform 5 quad type 1

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoQuadT1
#define	_EPC_GrP5UoQuadT1

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoQuadBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoQuadT1Para
{
	St_GrP5FontDrawStyle	NameTpara;
	__u8		NameFontId;
	__u8		_rsvd0;
	__u16		RecDoImg;
	St_GrP5RelRect	NameRc;
	St_GrP5RelRect	RecDoRc;
	__u16		SenImg;
	__u16		MtnImg;
	__u16		RsenImg;
	__u16		RmtnImg;
	St_GrP5RelRect	SenRc;
	St_GrP5RelRect	MtnRc;
	St_GrP5RelRect	PtzRc;
	St_GrP5RelRect	AdoRc;
	__u16		PtzImg;
	__u16		AdoImg;
	St_GrP5RelRect	IaRc;
	__u16		IaImg;
	__u16		_rsvd1;

}	*Ptr_GrP5UoQuadT1Para;

//====================================================================
//global class

class Cls_GrP5UoQuadT1	: public Cls_GrP5UoQuadBase
{
	protected:

		St_GrP5UoQuadT1Para	m_ParaUoQuadT1;
		St_GrP5SkinCtlQuadT1	m_SkinCtlQuadT1;

		St_GrP5SkinQuadDrawInfo	m_DreqPrv;		// draw request previous

		// function
		void	LcDrawReqPrev(void);

		// inherite
		virtual	void	RtlScrRectBld(void);

		// event system
		virtual	void	EvtScptDecDone(void);
		virtual	void	EvtTimerTick(void);
		virtual	void	EvtQuadChanged(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoQuadT1(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoQuadT1(void);

		// basic information

		// control
		BOOL8	IsShowName(void);
		BOOL8	IsShowIconRec(void);
		BOOL8	IsShowIconEvt(void);
		BOOL8	IsShowIconPtz(void);
		BOOL8	IsShowIconAudio(void);
		BOOL8	IsRecIconEvtShow(void);

		void	ShowNameSet(BOOL8 A_IsShow);
		void	ShowIconRec(BOOL8 A_IsShow);
		void	ShowIconEvt(BOOL8 A_IsShow);
		void	ShowIconPtz(BOOL8 A_IsShow);
		void	ShowIconAudio(BOOL8 A_IsShow);
		void	RecIconEvtShowSet(BOOL8 A_IsOn);

		// inherited
		virtual	void	ReqDraw(BOOL8 A_IsForce);
		virtual	void	ReqDrawByChg(void);

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

