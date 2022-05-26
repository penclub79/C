/*
	Gause Platform spin text base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinTxtBase
#define	_EPC_GrP3DoSpinTxtBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoSpinTxtStatDisable	0
#define E_GrP3DoSpinTxtStatNormal		1
#define E_GrP3DoSpinTxtStatFocus		2
#define E_GrP3DoSpinTxtStatControl	3

#define E_GrP3DoSpinTxtStatCnt			4

#define E_GrP3DoSpinTxtItemMax			32

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinTxtBasePara
{
	__u16	PntStat[E_GrP3DoSpinTxtStatCnt];					// paint status

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	__u16	AsmEvtValChg;

	__u8	TxtCnt;		// text count
	BOOL8	IsValRotate;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u16	TxtTbl[E_GrP3DoSpinTxtItemMax];
}	*Ptr_GrP3DoSpinTxtBasePara;

//====================================================================
//global class
class	Cls_GrP3DoSpinTxtBase	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoSpinTxtBasePara	m_ParaDoSpinTxtBase;

		BOOL8	m_IsCtrlMode;		// control mode
		__u32	m_Value;

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
		Cls_GrP3DoSpinTxtBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinTxtBase( void );

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

