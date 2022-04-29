/*
	Gause Platform spin paint

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinPnt
#define	_EPC_GrP3DoSpinPnt

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance
#define E_GrP3DoSpinPntItemCnt		16

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinPntPara
{
	__u16	PntList[E_GrP3DoSpinPntItemCnt];					// paint list
	__u16	TxtList[E_GrP3DoSpinPntItemCnt];					// text list

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	BOOL8	IsValRotate;		// value rotate
	__u8	ItemCnt;				// registed item count

	__u16	AsmEvtValChg;		// event assembly
	__u16	_rsvd0;

}	*Ptr_GrP3DoSpinPntPara;

//====================================================================
//global class
class	Cls_GrP3DoSpinPnt	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoSpinPntPara	m_ParaDoSpinPnt;

		BOOL8	m_IsCtrlMode;		// control mode
		Def_WstrTag	m_StrTxt;
		__u8		m_Value;			// image index

		// local
		void	LcValTxtUpdt( void );
		void	LcNotifyValChg( void );
		void	LcValIncrease( void );
		void	LcValDecrease( void );
		void	LcValueUpdt( BOOL8 A_IsNotify );

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
		Cls_GrP3DoSpinPnt( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinPnt( void );

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

