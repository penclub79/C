/*
	Gause Platform spin number base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinNumBase
#define	_EPC_GrP3DoSpinNumBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoSpinNumStatDisable	0
#define E_GrP3DoSpinNumStatNormal		1
#define E_GrP3DoSpinNumStatFocus		2
#define E_GrP3DoSpinNumStatControl	3

#define E_GrP3DoSpinNumStatCnt			4

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinNumBasePara
{
	__u16	PntStat[E_GrP3DoSpinNumStatCnt];					// paint status

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

}	*Ptr_GrP3DoSpinNumBasePara;

//====================================================================
//global class
class	Cls_GrP3DoSpinNumBase	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoSpinNumBasePara	m_ParaDoSpinNumBase;

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
		Cls_GrP3DoSpinNumBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinNumBase( void );

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

