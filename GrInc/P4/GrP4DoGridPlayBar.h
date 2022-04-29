/*
	GAUSE Platform grid play time bar
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoGridPlayBar
#define	_EPC_GrP4DoGridPlayBar

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P4/GrP4Base.h>
#include <P4/GrP4DoGridBase.h>
#include <GrTimeTool.h>
#include <Dvr/GrDvrBase.h>

//====================================================================
//constance
#define E_GrP4DoGridPlayBarItemCnt		8

#define E_GrP4DoGridPlayBarMapCnt			60


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoGridPlayBarPara
{
	__u16	PntVal[E_GrP4DoGridPlayBarItemCnt];		// value paint table

	__u8	ValTbl[E_GrP4DoGridPlayBarItemCnt];		// item value count

	__u16	GvuDev;
	__u16	GvuTime;

	__u16	AsmTimeSel;
	__u16	_rsvd1;
}	*Ptr_GrP4DoGridPlayBarPara;

typedef	struct	St_GrP4DoGridPlayBarInfo
{
	__u8	ChCnt;
	__u8	MinMap[E_GrTimeMinOfHour][E_GrDvrMaxChCnt];
} *Ptr_GrP4DoGridPlayBarInfo;

//====================================================================
//global class
class	Cls_GrP4DoGridPlayBar	:	public	Cls_GrP4DoGridBase
{
	protected:
		St_GrP4DoGridPlayBarPara	m_ParaGridPlayBar;

		__s32*	m_PtrGvu;		// GVU table pointer

		__u32	m_Time;
		__u32 m_GridTime;

		__u16	m_Year;
		__u8	m_Month;
		__u8	m_Day;
		__u8	m_Hour;
		__u8	m_Min;

		St_GrP4DoGridPlayBarInfo m_BarInfo;

		BOOL8	LcTimeUpdate(__u32 A_Time);
		void	LcGetMinMap(__u32 A_Time);

		virtual	void	LcPainItems( void );								// paint item area
		virtual	void	LcProcKeyItemSel(__u32 A_SelTime);

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		//virtual	void	ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

	public:
		Cls_GrP4DoGridPlayBar(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoGridPlayBar( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize
};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

