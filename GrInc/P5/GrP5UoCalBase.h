/*
Platform 5 calendar base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoCalBase
#define	_EPC_GrP5UoCalBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <GrTimeTool.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoCalBasePara
{
	St_GrP5FontDrawStyle	TxtPara;
	__u8		FontId;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
	St_GrP5RelRect	TxtRc;
}	*Ptr_GrP5UoCalBasePara;

//====================================================================
//global class

class Cls_GrP5UoCalBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoCalBasePara	m_ParaUoCalBase;
		St_GrP5SkinCtlCalBase	m_SkinCtlCalBase;

		__u8			m_StartWeek;
		__u8			m_DayCnt;

		// paint

		BOOL8	LcDayToItemIdx(__u8 A_Day, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);

		virtual	void	DoEvtCalDayClick(void);

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
		virtual	void	EvtMseEnter(void);
		virtual	void	EvtMseLeave(void);

		virtual	BOOL8	EvtKeyDown(__u8 A_Key);	/* not need inerite */
		virtual	BOOL8	EvtKeyUp(__u8 A_Key);	/* not need inerite */

		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

	public:
		Cls_GrP5UoCalBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoCalBase(void);

		// basic information

		// control
		void	CalendarSetTime(__u32 A_Time);
		void	CalendarSetDate(__u16 A_Year, __u8 A_Month);
		void	CalendarGetDate(__u16* A_PtrRtYear, __u8* A_PtrRtMonth);

		__s8	SelectDayGet(void);
		void	SelectDaySet(__u8 A_Day);
		Def_GrTime	SelectTimeGet(void);

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

