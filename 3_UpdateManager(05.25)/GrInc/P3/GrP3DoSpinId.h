/*
	GAUSE Platform spin authority id

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinId
#define	_EPC_GrP3DoSpinId

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoSpinIdStatDisable		0
#define E_GrP3DoSpinIdStatNormal		1
#define E_GrP3DoSpinIdStatFocus			2
#define E_GrP3DoSpinIdStatControl		3

#define E_GrP3DoSpinIdStatCnt				4


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinIdPara
{
	__u16	PntStat[E_GrP3DoSpinIdStatCnt];					// paint status

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	__u8	IsValRotate;
	__u8	_rsvd0;

	__u16	AsmEvtValChg;
	__u16	EvtHostId;
}	*Ptr_GrP3DoSpinIdPara;

//====================================================================
//global class
class	Cls_GrP3DoSpinId	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoSpinIdPara	m_ParaSpinId;

		BOOL8	m_IsCtrlMode;		// control mode
		__u8	m_Value;
		__u8	m_UserIdCnt;

		// local
		void	LcValLoad( void );
		void	LcNotifyValChg( void );

		void	LcValIncrease( void );
		void	LcValDecrease( void );


		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcEvtFocusGet( void );
		virtual	void	ProcEvtFocusLoss( void );
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtWheelUp( void );
		virtual	BOOL8	ProcEvtWheelDown( void );

		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoSpinId( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinId( void );

		void	EvtHostIdSet( __u16 A_ObjId );
		__u16	EvtHostIdGet( void );

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

