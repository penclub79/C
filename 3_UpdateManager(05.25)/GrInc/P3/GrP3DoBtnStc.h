/*
	Gause Platform button static

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoBtnStc
#define	_EPC_GrP3DoBtnStc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoBtnStcStatDisable		0
#define E_GrP3DoBtnStcStatNormal		1
#define E_GrP3DoBtnStcStatFocus			2
#define E_GrP3DoBtnStcStatPush			3

#define E_GrP3DoBtnStcStatCnt				4

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoBtnStcPara
{
	__u16	PntStat[E_GrP3DoBtnStcStatCnt];					// paint background

	__u16	AsmBtnDown;
	__u16	AsmBtnUp;

	__u16	EvtHostId;			// event host id
	__u16	TxtId;

}	*Ptr_GrP3DoBtnStcPara;

//====================================================================
//global class
class	Cls_GrP3DoBtnStc	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoBtnStcPara	m_ParaDoBtnStc;

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


		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoBtnStc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoBtnStc( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

