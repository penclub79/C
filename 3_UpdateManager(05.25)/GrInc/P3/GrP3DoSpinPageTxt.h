/*
	Gause Platform spin page text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinPageTxt
#define	_EPC_GrP3DoSpinPageTxt

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoSpinPageTxtStatDisable	0
#define E_GrP3DoSpinPageTxtStatNormal		1
#define E_GrP3DoSpinPageTxtStatFocus		2
#define E_GrP3DoSpinPageTxtStatControl	3

#define E_GrP3DoSpinPageTxtStatCnt			4

#define E_GrP3DoSpinPageTxtItemMax			16

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinPageTxtPara
{
	__u16	PntStat[E_GrP3DoSpinPageTxtStatCnt];					// paint status

	__u16	FrameId;			// frame script id
	__u8	FrmLayer;
	BOOL8	IsMoveFcs;		// chage focus after change page

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	__u8	ItemCnt;
	__u8	IsValRotate;

	__u16	TxtTbl[E_GrP3DoSpinPageTxtItemMax];
	__u16	PageTbl[E_GrP3DoSpinPageTxtItemMax];

}	*Ptr_GrP3DoSpinPageTxtPara;

//====================================================================
//global class
class	Cls_GrP3DoSpinPageTxt	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoSpinPageTxtPara	m_ParaDoSpinPageTxt;

		BOOL8	m_IsCtrlMode;		// control mode
		__u8	m_PageIdx;			// page index

		// local
		void	LcPageInc( void );
		void	LcPageDec( void );
		void	LcPageUpdt( void );

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
		Cls_GrP3DoSpinPageTxt( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinPageTxt( void );

		__u8	PageIdxGet( void );
		void	PageIdxSet( __u8 A_PageIdx );

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

