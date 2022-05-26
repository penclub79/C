/*
	Gause Platform spin number base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinNumBase
#define	_EPC_GrP4DoSpinNumBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoSpinNumStatDisable	0
#define E_GrP4DoSpinNumStatNormal		1
#define E_GrP4DoSpinNumStatFocus		2
#define E_GrP4DoSpinNumStatControl	3

#define E_GrP4DoSpinNumStatCnt			4

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinNumBasePara
{
	__u16	PntStat[E_GrP4DoSpinNumStatCnt];					// paint status

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	__u8	Cipher;
	BOOL8	IsValRotate;		// value rotate

	__s32		MinVal;
	__s32		MaxVal;
	__s32		Step;

	__u16	ZeroTxtId;
	__u16	PrefixTxtId;

	__u16	SuffixTxtId;
	__u16	AsmEvtValChg;

	__s32		DispAddVal;

}	*Ptr_GrP4DoSpinNumBasePara;

//====================================================================
//global class
class	Cls_GrP4DoSpinNumBase	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoSpinNumBasePara	m_ParaDoSpinNumBase;

		BOOL8	m_IsCtrlMode;		// control mode
		Def_WstrTag	m_StrTxt;
		__s32		m_Value;
		__s32		m_Orignal;			// original value

		// local
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

		void	LcValTxtUpdt( void );
		virtual	void	LcNotifyValChg( void );
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
		Cls_GrP4DoSpinNumBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinNumBase( void );

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

