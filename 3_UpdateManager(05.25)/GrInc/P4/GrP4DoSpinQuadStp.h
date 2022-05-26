/*
	GAUSE Platform spin quad setup

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinQuadStp
#define	_EPC_GrP4DoSpinQuadStp

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoSpinQuadStpStatDisable	0
#define E_GrP4DoSpinQuadStpStatNormal		1
#define E_GrP4DoSpinQuadStpStatFocus		2
#define E_GrP4DoSpinQuadStpStatControl	3

#define E_GrP4DoSpinQuadStpStatCnt			4


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinQuadStpPara
{
	__u16	PntStat[E_GrP4DoSpinQuadStpStatCnt];					// paint status

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	__u8	Cipher;
	__u8	_rsvd0;

	__u8	QuadMode;
	__u8	ViewIdx;
	__u16	PrefixTxtId;

	__u16	SuffixTxtId;
	__u16	AsmEvtValChg;

	__u16	RelPnt;			// relate paint object id
	__u16	_rsvd1;

}	*Ptr_GrP4DoSpinQuadStpPara;

//====================================================================
//global class
class	Cls_GrP4DoSpinQuadStp	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoSpinQuadStpPara	m_ParaSpinQuadStp;

		Def_WstrTag	m_StrTxt;
		__u8*	m_PtrCfgPage;		// quad mode configuration

		BOOL8	m_IsCtrlMode;		// control mode
		__u8	m_Value;
		__u8	m_Orignal;			// original value

		__u8	m_ViewCnt;
		__u8	m_CfgViewOfs;		// configuration view index
		__u8	m_VdoChCnt;
		__u8	m_ChSt;					// channel start
		__u8	m_ChRmCnt;			// channel remain count

		void	LcCfgPosUpdt( void );		// configuration position update
		void	LcChgVal( __u8 A_NewVal );

		void	LcValLoad( void );
		void	LcValSave( void );
		void	LcValTxtUpdt( void );
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
		Cls_GrP4DoSpinQuadStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinQuadStp( void );

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

