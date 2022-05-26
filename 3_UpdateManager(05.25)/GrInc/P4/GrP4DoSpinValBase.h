/*
	GAUSE Platform spin value base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinValBase
#define	_EPC_GrP4DoSpinValBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoSpinValStatDisable	0
#define E_GrP4DoSpinValStatNormal		1
#define E_GrP4DoSpinValStatFocus		2
#define E_GrP4DoSpinValStatControl	3

#define E_GrP4DoSpinValStatCnt			4

#define E_GrP4DoSpinValItemMax			32

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinValBasePara
{
	__u16	PntStat[E_GrP4DoSpinValStatCnt];					// paint status

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	__u16	AsmEvtValChg;

	__u8	ItemCnt;		// text count
	BOOL8	IsValRotate;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u16	TxtTbl[E_GrP4DoSpinValItemMax];
	__u32	ValTbl[E_GrP4DoSpinValItemMax];
}	*Ptr_GrP4DoSpinValBasePara;

//====================================================================
//global class
class	Cls_GrP4DoSpinValBase	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoSpinValBasePara	m_ParaSpinValBase;

		BOOL8	m_IsCtrlMode;		// control mode
		__u8	m_ItemIdx;			// item index
		__u32	m_Value;

		// local
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );
		virtual	void	LcNotifyValChg( void );
		void	LcValToItemIdx( void );	// value to item index
		void	LcItemIdxToVal( void );
		void	LcValIncrease( void );
		void	LcValDecrease( void );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

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
		Cls_GrP4DoSpinValBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinValBase( void );

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

