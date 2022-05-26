/*
Platform 5 graph horizontal time map type 1

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoGrpHzT1
#define	_EPC_GrP5UoGrpHzT1

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

#define	E_GrP5UoGrpHzT1HzCellMax					60

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoGrpHzT1Para
{
	St_GrP5FontDrawStyle	RulerTxtStyle;
	St_GrP5FontDrawStyle	IndTxtStyle;

	__u8	RulerFontId;
	__u8	RulerHeight;
	__u16	RulerUnitWidth;

	__u8	IndFontId;
	__u8	CellWidth;
	__u16	IndWidth;

	__u8	ViewLineCnt;	// view line count
	BOOL8	IsDayMode;
	__u16	RegCusTime;

	__u16	RelHourGrp;
	__u16	_rsvd1;

}	*Ptr_GrP5UoGrpHzT1Para;

//====================================================================
//global class

class Cls_GrP5UoGrpHzT1 : public Cls_GrP5UoGrpHzBase
{
	protected:

		St_GrP5UoGrpHzT1Para	m_ParaUoGrpHzT1;
		St_GrP5SkinCtlGrpHzT1	m_SkinCtlGrpHzT1;

		__u8	m_ViewMap[E_GrDvrMaxChCnt][E_GrP5UoGrpHzT1HzCellMax];
		__u8*	m_PtrMap;
		
		__u16	m_Year;
		__u8	m_Month;
		__u8	m_Day;
		__u8	m_Hour;

		__u8	m_StartCh;

		Cls_GrP5UoBase*	m_RelGrp;

		// inherited
		void	LcViewMapUpdate(void);
		void	LcSubViewMapDayUpdate(void);
		void	LcSubViewMapHourUpdate(void);
		__u32	LcMapIdxCalcBegin(void);
		void	LcRedrawCursorArea(__s32 A_Idx);
		virtual	void	LcDoCusPosChg(__s32 A_X, __s32 A_Y);
		virtual	void	DoEvtGrpItemClick(void);

		void	LcParaValUpdate(void);

		Def_GrTime	LcTimeCalcByIndex(__u32 A_IdxX);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);
		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

	public:
		Cls_GrP5UoGrpHzT1(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoGrpHzT1(void);

		// basic information
		virtual	__u8	ItemValueGet(__u32 A_IdxX, __u32 A_IdxY);

		// control
		void	MapBufferSet(__u8* A_PtrMap);
		__u8	HourGet(void);
		void	HourSet(__u8 A_Hour);
		void	ChannelStartSet(__u8 A_Ch);
		__u8	ChannelStartGet(void);
		void	TimeBaseSet(Def_GrTime A_Time);
		Def_GrTime	TimeBaseGet(void);
		BOOL8	IsDayMode(void);
		void	ModeSet(BOOL8 A_IsDayMode);

		Def_GrTime	SelectTimeGet(void);
		void	SelectTimeSetByHm(__u8 A_Hour, __u8 A_Min);
		void	SelectTimeSetByTime(Def_GrTime A_Time);

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

