/*
Platform 5 Button base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnBase
#define	_EPC_GrP5UoBtnBase

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

typedef struct St_GrP5UoBtnBasePara
{
	__u8		PumpPeriod;
	BOOL8		IsSkipKey;
	__u8		_rsvd2;

	__u16		EvtHostId;
	__u16		RelLbl;
}	*Ptr_GrP5UoBtnBasePara;

//====================================================================
//global class

class Cls_GrP5UoBtnBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoBtnBasePara	m_ParaUoBtnBase;
		St_GrP5SkinCtlBtnBase	m_SkinCtlBtnBase;

		__u16			m_HostObjId;

		__u8			m_PumpPrgs;

		virtual	void	DoEvtBtnPress(void);
		virtual	void	DoEvtBtnRelease(void);

		// inherited
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	void	EvtKeyFocusGet(__u32 A_Para);
		virtual	void	EvtKeyFocusLoss(void);

		virtual	BOOL8	EvtKeyDown(__u8 A_Key);
		virtual	BOOL8	EvtKeyUp(__u8 A_Key);

		virtual	void	EvtMseEnter(void);
		virtual	void	EvtMseLeave(void);

		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

		virtual	void	EvtTimerTick(void);

	public:
		Cls_GrP5UoBtnBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoBtnBase(void);

		// basic information

		// control
		BOOL8	IsSelected(void);
		void	SelectSet(BOOL8 A_IsSet);

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

