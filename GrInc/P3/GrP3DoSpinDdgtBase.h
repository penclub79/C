/*
	Gause Platform spin decimal digit base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinDdgtBase
#define	_EPC_GrP3DoSpinDdgtBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoSpinDdgtStatDisable	0
#define E_GrP3DoSpinDdgtStatNormal	1
#define E_GrP3DoSpinDdgtStatFocus		2
#define E_GrP3DoSpinDdgtStatControl	3

#define E_GrP3DoSpinDdgtStatCnt			4

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinDdgtBasePara
{
	__u16	PntStat[E_GrP3DoSpinDdgtStatCnt];					// paint status

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsValRotate;		// value rotate
	BOOL8	IsCipherRotate;
	__u8	IsRelExtDigit;

	__u32	MinVal;
	__u32	MaxVal;

	__u16	AsmEvtValChg;
	__u16	_rsvd1;

}	*Ptr_GrP3DoSpinDdgtBasePara;

//====================================================================
//global class
class	Cls_GrP3DoSpinDdgtBase	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoSpinDdgtBasePara	m_ParaDoSpinDdgtBase;

		BOOL8	m_IsCtrlMode;		// control mode
		Def_WstrTag	m_StrTxt;
		__u32		m_Value;
		__u8		m_CusPos;
		__u8		m_MaxCipher;

		// local
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );
		virtual	void	LcNotifyValChg( void );

		void	LcCipherUpdt( void );
		void	LcValTxtUpdt( void );
		void	LcCipherInc( void );
		void	LcCipherDec( void );
		BOOL8	LcCusRight( void );
		BOOL8	LcCusLeft( void );

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
		virtual	BOOL8	ProcEvtMseMbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtWheelUp( void );
		virtual	BOOL8	ProcEvtWheelDown( void );
		virtual	BOOL8	ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb );
		virtual	void	ProcEvtVkbdEditEnd( void );

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoSpinDdgtBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinDdgtBase( void );

		void	ValControlSet( BOOL8 A_IsCtrl );

		// inherited

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );
		virtual	void	RtlInit( void );	// initialize

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

