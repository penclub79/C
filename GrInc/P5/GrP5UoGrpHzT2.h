/*
Platform 5 graph horizontal time map type 2

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoGrpHzT2
#define	_EPC_GrP5UoGrpHzT2

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrTimeTool.h>
#include <GrGdibObj.h>

#include <Dvr/GrDvrBase.h>

#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoGrpHzBase.h>


//====================================================================
//constance

#define	E_GrP5UoGrpHzT2HzCellMax					60
#define	E_GrP5UoGrpHzT2TxtBufCnt					4

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoGrpHzT2Para
{
	St_GrP5FontDrawStyle	RulerTxtStyle;
	
	__u8	RulerFontId; 
	__u8	RulerHeight;
	__u8	ViewLineCnt;	// view line count
	BOOL8	IsMergeCh;		// merge all channel

	BOOL8	IsAutoCus;		// auto cursor position
	__u8	RulerTxtWidth;	// ruler text width
	__u16	RegCusTime;

}	*Ptr_GrP5UoGrpHzT2Para;

//====================================================================
//global class

class Cls_GrP5UoGrpHzT2 : public Cls_GrP5UoGrpHzBase
{
	protected:

		St_GrP5UoGrpHzT2Para	m_ParaUoGrpHzT2;
		St_GrP5SkinCtlGrpHzT2	m_SkinCtlGrpHzT2;

		WCHAR*	m_TxtBufs[E_GrP5UoGrpHzT2TxtBufCnt];

		__u32	m_TimeLastOvEvt;

		// inherited
		void	LcTxtBufInit(void);

		void	LcRedrawCus(void);
		void	LcCusTimeSet(Def_GrTime A_Time);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		virtual	BOOL8	IsKeyAble(void);
		virtual	__u8	ItemValueGet(__u32 A_IdxX, __u32 A_IdxY);

		// event system
		virtual	void	EvtTimerTick(void);	/* not need inerite ,call by GrP5TimerRegist */
		virtual	void	EvtScptDecDone(void);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

	public:
		Cls_GrP5UoGrpHzT2(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoGrpHzT2(void);

		// basic information

		// control
		void	MapBufferSet(__u8* A_PtrMap, __u32 A_TimeDay);
		void	ChannelStartSet(__u8 A_Ch);
		__u8	ChannelStartGet(void);
		void	TimeSet(Def_GrTime A_Time);
		Def_GrTime	TimeGet(void);
		void	ZoomLvlSet(__u8 A_Level);
		__u8	ZoomLvlGet(void);
		void	ScrollLeft(__u32 A_Step);
		void	ScrollRight(__u32 A_Step);
		void	ScrollByTime(Def_GrTime A_Time);

		void	LineCntSet(__u8 A_Cnt);
		__u8	LineCntGet(void);

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

