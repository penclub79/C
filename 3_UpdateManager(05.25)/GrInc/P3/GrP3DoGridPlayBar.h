/*
	GAUSE Platform grid play time bar
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoGridPlayBar
#define	_EPC_GrP3DoGridPlayBar

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P3/GrP3Base.h>
#include <P3/GrP3DoGridBase.h>
#include <GrTimeTool.h>
#include <Dvr/GrDvrBase.h>

//====================================================================
//constance

#define E_GrP3DoGridPlayBarItemCnt		8

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoGridPlayBarPara
{
	__u16	PntVal[E_GrP3DoGridPlayBarItemCnt];		// value paint table

	__u8	ValTbl[E_GrP3DoGridPlayBarItemCnt];		// item value count

	__u16	GvuDev;
	__u16	GvuTime;

	__u16	AsmTimeSel;
	__u16	_rsvd1;
}	*Ptr_GrP3DoGridPlayBarPara;

//====================================================================
//global class
class	Cls_GrP3DoGridPlayBar	:	public	Cls_GrP3DoGridBase
{
	protected:
		St_GrP3DoGridPlayBarPara	m_ParaGridPlayBar;

		__s32*	m_PtrGvu;		// GVU table pointer

		__u32	m_Time;
		__u32 m_GridTime;

		__u16	m_Year;
		__u8	m_Month;
		__u8	m_Day;
		__u8	m_Hour;
		__u8	m_Min;

		__u8	m_MinMap[E_GrTimeMinOfHour];

		BOOL8	LcTimeUpdate(__u32 A_Time);
		void	LcGetMinMap(__u32 A_Time);

		virtual	void	LcPainItems( void );								// paint item area
		virtual	void	LcProcKeyItemSel(__u32 A_SelTime);

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

	public:
		Cls_GrP3DoGridPlayBar( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoGridPlayBar( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

