/*
Platform 5 UI slider bar standard

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSlidBase
#define	_EPC_GrP5UoSlidBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5SkinBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoSlidBasePara
{
	__s32	ValMin;
	__s32	ValMax;
	__s32	ValStep;
	__u16	RelLbl;
	__u16	_rsvd0;
}	*Ptr_GrP5UoSlidBasePara;

//====================================================================
//global class

class Cls_GrP5UoSlidBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoSlidBasePara	m_ParaUoSlidBase;

		__s32	m_Value;

		BOOL8	m_IsPush;

		// paint
		__u32	m_PntStat;
		__s32	m_PntPosAreaSize;		// by hz vt

		St_GrRect	m_PntRcBtnPos;

		//St_GrP5SkinSlidInfo	m_PntInfo;

		// local
		__s32	LcCalcValToPos(__s32 A_Val);
		__s32	LcCalcPosToVal(__s32 A_Pos);

		void	LcValueCorrect(void);

		// inheritable
		virtual	void	EvtSlidBtnPosMove(__s32 A_X, __s32 A_Y);
		virtual	void	RtlValueRead(void);
		virtual	void	RtlValueWrite(void);
		virtual	void	NotifyValChg(void);

		// inherited
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system

		virtual	void	EvtScptDecDone(void);

		virtual	void	EvtMseEnter(void);
		virtual	void	EvtMseLeave(void);
		virtual	void	EvtKeyFocusGet(__u32 A_Para);
		virtual	void	EvtKeyFocusLoss(void);

		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

	public:
		Cls_GrP5UoSlidBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect );
		virtual	~Cls_GrP5UoSlidBase(void);

		// basic inforwlqmation


		// control
		virtual	BOOL8	ValueRangeSet(__s32 A_Begin, __s32 A_End);
		virtual	BOOL8	ValueSet(__s32 A_Val);
		__s32	ValueGet(void);

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

