/*
	GAUSE Platform spin authority id

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinId
#define	_EPC_GrP4DoSpinId

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoSpinIdStatDisable		0
#define E_GrP4DoSpinIdStatNormal		1
#define E_GrP4DoSpinIdStatFocus			2
#define E_GrP4DoSpinIdStatControl		3

#define E_GrP4DoSpinIdStatCnt				4


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinIdPara
{
	__u16	PntStat[E_GrP4DoSpinIdStatCnt];					// paint status

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	__u8	IsValRotate;
	__u8	_rsvd0;

	__u16	AsmEvtValChg;
	__u16	EvtHostId;
}	*Ptr_GrP4DoSpinIdPara;

//====================================================================
//global class
class	Cls_GrP4DoSpinId	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoSpinIdPara	m_ParaSpinId;

		BOOL8	m_IsCtrlMode;		// control mode
		__u8	m_Value;
		__u8	m_UserIdCnt;

		// local
		void	LcValLoad( void );
		void	LcNotifyValChg( void );

		void	LcValIncrease( void );
		void	LcValDecrease( void );


		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

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
		Cls_GrP4DoSpinId(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinId( void );

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

