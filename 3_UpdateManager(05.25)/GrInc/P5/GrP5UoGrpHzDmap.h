/*
Platform 5 graph horizontal day map

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoGrpHzDmap
#define	_EPC_GrP5UoGrpHzDmap

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

#define	E_GrP5UoGrpHzDmapModeDayView				0
#define	E_GrP5UoGrpHzDmapModeHourView				1

#define	E_GrP5UoGrpHzDmapHzDayDivUnitMin		3
#define	E_GrP5UoGrpHzDmapHzDayDivUnitMax		6

#define	E_GrP5UoGrpHzDmapHzCellMax					(E_GrTimeMinOfDay / E_GrP5UoGrpHzDmapHzDayDivUnitMin)

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoGrpHzDmapPara
{
	__u8	ViewLineCnt;	// view line count
	__u8	DayMapUnit;
	__u16	RegCusTime;
}	*Ptr_GrP5UoGrpHzDmapPara;

//====================================================================
//global class

class Cls_GrP5UoGrpHzDmap : public Cls_GrP5UoGrpHzBase
{
	protected:

		St_GrP5UoGrpHzDmapPara	m_ParaUoGrpHzDmap;
		St_GrP5SkinCtlGrpHzDmap	m_SkinCtlGrpHzDmap;

		__u8	m_ViewMap[E_GrDvrMaxChCnt][E_GrP5UoGrpHzDmapHzCellMax];
		__u8*	m_PtrMap;
		
		__u16	m_Year;
		__u8	m_Month;
		__u8	m_Day;
		__u8	m_Hour;

		__u8	m_Mode;
		__u8	m_StartCh;

		__u32	m_DivDayUnit;

		// inherited
		void	LcViewMapUpdate(void);
		void	LcSubViewMapDayUpdate(void);
		void	LcSubViewMapHourUpdate(void);
		__u32	LcMapIdxCalcBegin(void);

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
		Cls_GrP5UoGrpHzDmap(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoGrpHzDmap(void);

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
		__u8	ModeGet(void);
		void	ModeSet(__u8 A_Mode);

		Def_GrTime	SelectTimeGet(void);
		void	SelectTimeSetByHm(__u8 A_Hour, __u8 A_Min);
		void	SelectTimeSetByTime(Def_GrTime A_Time);

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

