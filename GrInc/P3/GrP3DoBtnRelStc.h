/*
	Gause Platform button relate static

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoBtnRelStc
#define	_EPC_GrP3DoBtnRelStc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoBtnRelStcStatDisable		0
#define E_GrP3DoBtnRelStcStatNormal			1
#define E_GrP3DoBtnRelStcStatFocus			2
#define E_GrP3DoBtnRelStcStatSelect			3
#define E_GrP3DoBtnRelStcStatFcsSel			4
#define E_GrP3DoBtnRelStcStatPush				5

#define E_GrP3DoBtnRelStcStatCnt				6

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoBtnRelStcPara
{
	__u16	PntStat[E_GrP3DoBtnRelStcStatCnt];					// paint background

	__u16	AsmBtnDown;
	__u16	AsmBtnUp;

	__u16	EvtHostId;			// event host id
	__u16	TxtId;

	__u32	Value;

	__u8	RelType;				// relate type
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrP3DoBtnRelStcPara;

//====================================================================
//global class
class	Cls_GrP3DoBtnRelStc	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoBtnRelStcPara	m_ParaBtnRelStc;

		BOOL8	m_IsPush;
		BOOL8	m_IsSel;
		__u32	m_Value;

		// local
		void	LcDoBtnDownEvt( void );
		void	LcDoBtnUpEvt( void );
		void	LcSelStatUpdt( void );	// select status update

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
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

	public:
		Cls_GrP3DoBtnRelStc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoBtnRelStc( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

