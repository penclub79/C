/*
	Gause Platform time control

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoTimeCtrl
#define	_EPC_GrP4DoTimeCtrl

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoTimeCtrlYearIdx				0
#define E_GrP4DoTimeCtrlMonthIdx			1
#define E_GrP4DoTimeCtrlDayIdx				2
#define E_GrP4DoTimeCtrlHourIdx				3
#define E_GrP4DoTimeCtrlMinIdx				4
#define E_GrP4DoTimeCtrlSecIdx				5
#define E_GrP4DoTimeCtrlDateDiv1Idx		6
#define E_GrP4DoTimeCtrlDateDiv2Idx		7

#define E_GrP4DoTimeCtrlSubCtrlCnt		8

#define	E_GrP4DoTimeCtrlElementCnt		6		/* time element count */


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoTimeCtrlPara
{
	__u16	PntBg;					// paint background
	BOOL8	IsDispose;			// dispose automatically
	__u8	_rsvd0;

	__u16	CtrlTbl[E_GrP4DoTimeCtrlSubCtrlCnt];

	__u16	DateGap;
	__u16	AsmEvtValChg;		// value change event

}	*Ptr_GrP4DoTimeCtrlPara;

//====================================================================
//global class
class	Cls_GrP4DoTimeCtrl	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoTimeCtrlPara	m_ParaDoTimeCtrl;

		Cls_GrP4DoBase*	m_ObjTbl[E_GrP4DoTimeCtrlSubCtrlCnt];

		__s32		m_HeadPosX;

		__u32	m_Time;			// time value

		__u16	m_ElemTm[E_GrP4DoTimeCtrlElementCnt];

		// local
		void	LcCtrlListUpdt( void );		// build control list
		void	LcDisposeCtrls( void );
		BOOL8	LcDisposeItem( __u8 A_Idx, __s32* A_PtrPosX );
		void	LcFocusTgUpdt( void );
		void	LcEvtHostUpdt( void );

		void	LcCtrlValUpdt( void );		// control value update
		void	LcDayLimiteUpdt( void );	// day limit update
		void	LcValueUpdt( void );			// value update by element

		void	ProcSpinValChg( __u16 A_ObjId );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump

		// inherited
		virtual	void	ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg );
		virtual	void	ProcPaint( void );		// do painting script
		virtual	void	ProcEvtSysStat( __u8 A_Type );

	public:
		Cls_GrP4DoTimeCtrl(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoTimeCtrl( void );

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

