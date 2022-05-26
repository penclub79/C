/*
	Gause Platform button page

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoBtnPage
#define	_EPC_GrP4DoBtnPage

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>
#include	<Dvr/GrDvrBase.h>

//====================================================================
//constance
#define	E_GrP4DoBtnPageStatCnt			6

#define	E_GrP4DoBtnPageStatDisable	0
#define	E_GrP4DoBtnPageStatNormal		1
#define	E_GrP4DoBtnPageStatOnCus		2
#define	E_GrP4DoBtnPageStatSelect		3
#define	E_GrP4DoBtnPageStatSelCus		4
#define	E_GrP4DoBtnPageStatPush			5

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoBtnPagePara
{
	__u16	PntStatTbl[E_GrP4DoBtnPageStatCnt];							// status paint table
	__u16	FrameId;			// relate frame id
	__u16	PageId;				// target page id
	__u8	FrmLayer;			// target frame layer
	BOOL8	IsUpLayer;		// go to up layer
	BOOL8	IsMovFcs;			// move focus when after change page.
	__u8	_rsvd0;
	__u16	TxtId;				// main text id
	__u16	_rsvd1;
}	*Ptr_GrP4DoBtnPagePara;

//====================================================================
//global class
class	Cls_GrP4DoBtnPage	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoBtnPagePara	m_ParaDoBtnPage;


		// runtime
		BOOL8	m_IsPush;			// push status
		BOOL8	m_IsSelect;		// selected

		// local
		void	LcSelStatUpdt( void );		// relationship status update
		void	LcReqFrmPageChg( void );	// request frame page change

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump
#endif

		// event
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );
		virtual	void	ProcEvtFocusGet( void );
		virtual	void	ProcEvtFocusLoss( void );
		virtual	void	ProcEvtFramePageChged( __u16 A_FrmadId, __u16 A_PageId );

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP4DoBtnPage(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoBtnPage( void );

		// inherited

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

