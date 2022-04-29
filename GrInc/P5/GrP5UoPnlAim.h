/*
Platform 5 Label base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoPnlAim
#define	_EPC_GrP5UoPnlAim

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5UoPnlAimUpdtTimeStep		2


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoPnlAimPara
{
	__u8		Level;
	__u8		_rsvd0;
	__u8		_rsvd2;
	__u8		_rsvd3;
}	*Ptr_GrP5UoPnlAimPara;

//====================================================================
//global class

class Cls_GrP5UoPnlAim	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoPnlAimPara	m_ParaUoPnlAim;

		__u8	m_UpdtStep;

		BOOL8	m_IsPosChg;
		BOOL8	m_IsMsePush;
		__s32	m_PosX;
		__s32	m_PosY;

		// local function
		void	LcNotifyAimEvt(__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsBtnPush);

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	void	EvtTimerTick(void);
		virtual	BOOL8	EvtMseWheelUp(__u8 A_WheelIdx);
		virtual	BOOL8	EvtMseWheelDown(__u8 A_WheelIdx);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoPnlAim(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoPnlAim(void);

		// basic information
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

		// control

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

