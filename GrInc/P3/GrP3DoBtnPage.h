/*
	Gause Platform button page

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoBtnPage
#define	_EPC_GrP3DoBtnPage

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>
#include	<Dvr/GrDvrBase.h>

//====================================================================
//constance
#define	E_GrP3DoBtnPageStatCnt			6

#define	E_GrP3DoBtnPageStatDisable	0
#define	E_GrP3DoBtnPageStatNormal		1
#define	E_GrP3DoBtnPageStatOnCus		2
#define	E_GrP3DoBtnPageStatSelect		3
#define	E_GrP3DoBtnPageStatSelCus		4
#define	E_GrP3DoBtnPageStatPush			5

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoBtnPagePara
{
	__u16	PntStatTbl[E_GrP3DoBtnPageStatCnt];							// status paint table
	__u16	FrameId;			// relate frame id
	__u16	PageId;				// target page id
	__u8	FrmLayer;			// target frame layer
	BOOL8	IsUpLayer;		// go to up layer
	BOOL8	IsMovFcs;			// move focus when after change page.
	__u8	_rsvd0;
	__u16	TxtId;				// main text id
	__u16	_rsvd1;
}	*Ptr_GrP3DoBtnPagePara;

//====================================================================
//global class
class	Cls_GrP3DoBtnPage	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoBtnPagePara	m_ParaDoBtnPage;


		// runtime
		BOOL8	m_IsPush;			// push status
		BOOL8	m_IsSelect;		// selected

		// local
		void	LcSelStatUpdt( void );		// relationship status update
		void	LcReqFrmPageChg( void );	// request frame page change

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump

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
		Cls_GrP3DoBtnPage( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoBtnPage( void );

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

