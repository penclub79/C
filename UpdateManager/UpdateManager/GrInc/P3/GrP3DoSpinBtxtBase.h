/*
	GAUSE Platform spin buffer text base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinBtxtBase
#define	_EPC_GrP3DoSpinBtxtBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoSpinBtxtStatDisable	0
#define E_GrP3DoSpinBtxtStatNormal	1
#define E_GrP3DoSpinBtxtStatFocus		2
#define E_GrP3DoSpinBtxtStatControl	3

#define E_GrP3DoSpinBtxtStatCnt			4

#define E_GrP3DoSpinBtxtItemMax			32

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinBtxtBasePara
{
	__u16	PntStat[E_GrP3DoSpinBtxtStatCnt];					// paint status

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	__u16	AsmEvtValChg;

	BOOL8	IsValRotate;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u8	_rsvd3;

}	*Ptr_GrP3DoSpinBtxtBasePara;

//====================================================================
//global class
class	Cls_GrP3DoSpinBtxtBase	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoSpinBtxtBasePara	m_ParaSpinBtxtBase;

		BOOL8	m_IsCtrlMode;		// control mode
		__u32	m_Value;
		__u32	m_TxtCnt;
		Def_WstrTag	m_StrTbl[E_GrP3DoSpinBtxtItemMax];

		// local
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );
		virtual	void	LcNotifyValChg( void );
		void	LcValIncrease( void );
		void	LcValDecrease( void );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// event
		virtual	void	ProcEvtFocusGet( void );
		virtual	void	ProcEvtFocusLoss( void );
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtWheelUp( void );
		virtual	BOOL8	ProcEvtWheelDown( void );

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoSpinBtxtBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinBtxtBase( void );

		// inherited

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );
		virtual	void	RtlInit( void );	// initialize

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

