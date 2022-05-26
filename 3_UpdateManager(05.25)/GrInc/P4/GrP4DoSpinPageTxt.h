/*
	Gause Platform spin page text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinPageTxt
#define	_EPC_GrP4DoSpinPageTxt

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoSpinPageTxtStatDisable	0
#define E_GrP4DoSpinPageTxtStatNormal		1
#define E_GrP4DoSpinPageTxtStatFocus		2
#define E_GrP4DoSpinPageTxtStatControl	3

#define E_GrP4DoSpinPageTxtStatCnt			4

#define E_GrP4DoSpinPageTxtItemMax			16

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinPageTxtPara
{
	__u16	PntStat[E_GrP4DoSpinPageTxtStatCnt];					// paint status

	__u16	FrameId;			// frame script id
	__u8	FrmLayer;
	BOOL8	IsMoveFcs;		// chage focus after change page

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsVertical;
	__u8	ItemCnt;
	__u8	IsValRotate;

	__u16	TxtTbl[E_GrP4DoSpinPageTxtItemMax];
	__u16	PageTbl[E_GrP4DoSpinPageTxtItemMax];

}	*Ptr_GrP4DoSpinPageTxtPara;

//====================================================================
//global class
class	Cls_GrP4DoSpinPageTxt	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoSpinPageTxtPara	m_ParaDoSpinPageTxt;

		BOOL8	m_IsCtrlMode;		// control mode
		__u8	m_PageIdx;			// page index

		// local
		void	LcPageInc( void );
		void	LcPageDec( void );
		void	LcPageUpdt( void );

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
		Cls_GrP4DoSpinPageTxt(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinPageTxt( void );

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

