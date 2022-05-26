/*
	GAUSE Platform grid motion setup

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoGridMtnStp
#define	_EPC_GrP3DoGridMtnStp

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P3/GrP3Base.h>
#include <P3/GrP3DoGridBase.h>

//====================================================================
//constance

#define E_GrP3DoGridMtnStpSensCnt		2

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoGridMtnStpPara
{
	__u16	PntSens[E_GrP3DoGridMtnStpSensCnt];

	__u16	PntMark;
	__u16	PntSel;

	BOOL8	IsSenseDraw;
	__u8	SwViewDelay;
	__u16	StpAddr;//setup address

	__u16	TxtPrefix;
	__u16	TxtSuffix;

	__s32		DispValAdd;
}	*Ptr_GrP3DoGridMtnStpPara;

//====================================================================
//global class
class	Cls_GrP3DoGridMtnStp	:	public	Cls_GrP3DoGridBase
{
	protected:
		St_GrP3DoGridMtnStpPara	m_ParaGridMtnStp;

		__u8	m_Ch;					// channel
		BOOL8	m_IsSel;			// mouse left button down
		BOOL8	m_IsSoftView;
		__u8	m_SwViewDelay;

		__u16	m_SelStX;
		__u16	m_SelStY;
		__u16	m_SelEdX;
		__u16	m_SelEdY;

		__u32*	m_PtrStp;			// setup address
		__u32*	m_PtrSens;		// sensed map

		__u32	m_ChkMtn[32];

		Def_WstrTag	m_StrMtn;
		__u8	m_Value;				// last value
		__u16	m_Suffix;
		__u16	m_Prefix;

		void	LcChEnvUpdt( void );
		
		virtual	void	LcProcKeyItemSel( void );						// key item select
		virtual	void	LcPaintSelect( void );							// paint select area
		virtual	void	LcPainItems( void );								// paint item area

		__u8	LcMapGet( __u16 A_X, __u16 A_Y );		// value get
		void	LcMapSet( __u16 A_X, __u16 A_Y, __u8 A_Val );
		__u8	LcSenseGet( void );
		void	LcSenseSet( __u8 A_Sense );
		void	LcCfgMapUpdt( void );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcEvtMseLeave( void );
		virtual	BOOL8	ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );
		virtual	BOOL8	ProcEvtWheelUp( void );
		virtual	BOOL8	ProcEvtWheelDown( void );

	public:
		Cls_GrP3DoGridMtnStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoGridMtnStp( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );
};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

