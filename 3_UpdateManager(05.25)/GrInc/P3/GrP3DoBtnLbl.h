/*
	Gause Platform button label

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoBtnLbl
#define	_EPC_GrP3DoBtnLbl

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance
#define E_GrP3DoBtnLblStrMaxLen			256

#define E_GrP3DoBtnLblStatDisable		0
#define E_GrP3DoBtnLblStatNormal		1
#define E_GrP3DoBtnLblStatFocus			2
#define E_GrP3DoBtnLblStatPush			3

#define E_GrP3DoBtnLblStatCnt				4

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoBtnLblPara
{
	__u16	PntStat[E_GrP3DoBtnLblStatCnt];					// paint background

	__u16	AsmBtnDown;
	__u16	AsmBtnUp;

	__u16	EvtHostId;			// event host id
	__u16	_rsvd0;

}	*Ptr_GrP3DoBtnLblPara;

//====================================================================
//global class
class	Cls_GrP3DoBtnLbl	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoBtnLblPara	m_ParaDoBtnLbl;

		WCHAR	m_StrTxt[E_GrP3DoBtnLblStrMaxLen];
		BOOL8	m_IsPush;

		// local
		void	LcDoBtnDownEvt( void );
		void	LcDoBtnUpEvt( void );

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
		Cls_GrP3DoBtnLbl( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoBtnLbl( void );

		void	TextSet( WCHAR* A_Str );
		
		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

