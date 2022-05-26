/*
	Gause Platform button static

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoBtnStc
#define	_EPC_GrP4DoBtnStc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoBtnStcStatDisable		0
#define E_GrP4DoBtnStcStatNormal		1
#define E_GrP4DoBtnStcStatFocus			2
#define E_GrP4DoBtnStcStatPush			3

#define E_GrP4DoBtnStcStatCnt				4

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoBtnStcPara
{
	__u16	PntStat[E_GrP4DoBtnStcStatCnt];					// paint background

	__u16	AsmBtnDown;
	__u16	AsmBtnUp;

	__u16	EvtHostId;			// event host id
	__u16	TxtId;

}	*Ptr_GrP4DoBtnStcPara;

//====================================================================
//global class
class	Cls_GrP4DoBtnStc	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoBtnStcPara	m_ParaDoBtnStc;

		BOOL8	m_IsPush;

		// local
		void	LcDoBtnDownEvt( void );
		void	LcOdBtnUpEvt( void );

		// event
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	void	ProcEvtFocusLoss( void );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP4DoBtnStc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoBtnStc( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

