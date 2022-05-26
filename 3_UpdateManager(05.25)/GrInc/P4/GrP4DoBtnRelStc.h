/*
	Gause Platform button relate static

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoBtnRelStc
#define	_EPC_GrP4DoBtnRelStc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoBtnRelStcStatDisable		0
#define E_GrP4DoBtnRelStcStatNormal			1
#define E_GrP4DoBtnRelStcStatFocus			2
#define E_GrP4DoBtnRelStcStatSelect			3
#define E_GrP4DoBtnRelStcStatFcsSel			4
#define E_GrP4DoBtnRelStcStatPush				5

#define E_GrP4DoBtnRelStcStatCnt				6

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoBtnRelStcPara
{
	__u16	PntStat[E_GrP4DoBtnRelStcStatCnt];					// paint background

	__u16	AsmBtnDown;
	__u16	AsmBtnUp;

	__u16	EvtHostId;			// event host id
	__u16	TxtId;

	__u32	Value;

	__u8	RelType;				// relate type
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrP4DoBtnRelStcPara;

//====================================================================
//global class
class	Cls_GrP4DoBtnRelStc	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoBtnRelStcPara	m_ParaBtnRelStc;

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

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcPaint( void );		// do painting script
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

	public:
		Cls_GrP4DoBtnRelStc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoBtnRelStc( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

