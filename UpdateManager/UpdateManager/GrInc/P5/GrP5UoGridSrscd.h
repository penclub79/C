/*
Platform 5 grid setup schedule

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoGridSrscd
#define	_EPC_GrP5UoGridSrscd

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoGridBase.h>
#include <GrTimeTool.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5UoGridSrscdHolydayCnt		2
#define	E_GrP5UoGridSrscdChSize				(E_GrTimeHourOfDay * (E_GrP5UoGridSrscdHolydayCnt + E_GrTimeDayOfWeek))

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoGridSrscdPara
{
	__u16	StpAdr;
	BOOL8	IsHourUnit;
	__u8	HolydayCnt;

	BOOL8	IsHzWeek;
	BOOL8	IsHolydayOnly;
	__u16	_rsvd1;
}	*Ptr_GrP5UoGridSrscdPara;

//====================================================================
//global class

class Cls_GrP5UoGridSrscd	: public Cls_GrP5UoGridBase
{
	protected:

		St_GrP5UoGridSrscdPara	m_ParaUoGridSrscd;

		__u8	m_Ch;
		__u8	m_DrawVal;

		void	CellValueSet(__u32 A_X, __u32 A_Y, __u8 A_Val);
		void	LcCellCntUpdate(void);

		// inherited

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	DoEvtGrpItemOneSel(void);
		virtual	void	DoEvtGrpItemAreaSel(void);

		virtual	void	DrawReqItemDrag(__u32 A_IdxStX, __u32 A_IdxStY, __u32 A_IdxEdX, __u32 A_IdxEdY);

		virtual	BOOL8	EvtKeyUp(__u8 A_Key);

		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoGridSrscd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoGridSrscd(void);

		// basic information

		// control
		BOOL8	CellXyCntSet(__u32 A_CntX, __u32 A_CntY);
		virtual	__u32	CellValueGet(__u32 A_IdxX, __u32 A_IdxY);

		void	ChannelSet(__u8 A_Ch);
		__u8	ChannelGet(void);

		void	DrawValueSet(__u8 A_Val);
		__u8	DrawValueGet(void);
		void	GridClear(void);
		void	GridFill(void);

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

