/*
	Gause Platform radio button base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoRbtnBase
#define	_EPC_GrP3DoRbtnBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoRbtnBaseStatNrmDis		0
#define E_GrP3DoRbtnBaseStatNrm				1
#define E_GrP3DoRbtnBaseStatNrmFcs		2
#define E_GrP3DoRbtnBaseStatNrmPush		3
#define E_GrP3DoRbtnBaseStatSelDis		4
#define E_GrP3DoRbtnBaseStatSel				5
#define E_GrP3DoRbtnBaseStatSelFcs		6
#define E_GrP3DoRbtnBaseStatSelPush		7

#define E_GrP3DoRbtnBaseStatCnt				8

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoRbtnBasePara
{
	__u16	PntStat[E_GrP3DoRbtnBaseStatCnt];					// paint background

	__u16	AsmPush;
	__u16	_rsvd0;

	__u16	TxtIdNrm;
	__u16	TxtIdSel;

	__s32		Value;

}	*Ptr_GrP3DoRbtnBasePara;

//====================================================================
//global class
class	Cls_GrP3DoRbtnBase	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoRbtnBasePara	m_ParaRbtnBase;

		BOOL8	m_IsPush;
		BOOL8	m_IsVal;

		// local
		virtual	void	LcDoSelect( void );

		// event
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	void	ProcEvtFocusLoss( void );


		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoRbtnBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoRbtnBase( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

