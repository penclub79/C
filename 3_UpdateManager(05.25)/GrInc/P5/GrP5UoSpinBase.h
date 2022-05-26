/*
Platform 5 combo box base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSpinBase
#define	_EPC_GrP5UoSpinBase

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

typedef struct St_GrP5UoSpinBasePara
{
	St_GrP5FontDrawStyle	TxtPara;

	__u8	FontId;
	BOOL8	IsNotRotate;	// value ratate block
	BOOL8	IsKeyCover;
	BOOL8	IsVertical;

	__u16	RelLbl;
	__u16	_rsvd1;

	St_GrP5RelRect	TxtRc;		// text rect

}	*Ptr_GrP5UoSpinBasePara;

//====================================================================
//global class

class Cls_GrP5UoSpinBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoSpinBasePara	m_ParaUoSpinBase;
		St_GrP5SkinCtlSpinBase	m_SkinCtlSpinBase;

		__u16			m_EvtHostId;			// event host object id

		__u8	BtnIdxGetByScrPos(__s32 A_X, __s32 A_Y);

		virtual	BOOL8	DoIncrease(void);
		virtual	BOOL8	DoDecrease(void);
		virtual	void	ValueRead(void);
		virtual	void	ValueWrite(void);

		void	DoEvtSpinValChg(void);
		void	DoEvtSpinClickCont(void);

		virtual	void	ProcSpinClickCont(void);

		// paint

		// inherited
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtPaintProc(void);

		virtual	void	EvtScptDecDone(void);

		virtual	void	EvtKeyFocusGet(__u32 A_Para);
		virtual	void	EvtKeyFocusLoss(void);
		virtual	void	EvtMseEnter(void);
		virtual	void	EvtMseLeave(void);

		virtual	BOOL8	EvtKeyDown(__u8 A_Key);
		virtual	BOOL8	EvtKeyUp(__u8 A_Key);
		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseWheelUp(__u8 A_WheelIdx);
		virtual	BOOL8	EvtMseWheelDown(__u8 A_WheelIdx);

		virtual	void	EvtTimerTick(void);

	public:
		Cls_GrP5UoSpinBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoSpinBase(void);

		// basic information
		
		// control

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

