/*
	Gause Platform check button base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoChkbBase
#define	_EPC_GrP3DoChkbBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoChkbBaseStatFalseDis	0
#define E_GrP3DoChkbBaseStatFalse			1
#define E_GrP3DoChkbBaseStatFalseFcs	2
#define E_GrP3DoChkbBaseStatFalsePush	3
#define E_GrP3DoChkbBaseStatTrueDis		4
#define E_GrP3DoChkbBaseStatTrue			5
#define E_GrP3DoChkbBaseStatTrueFcs		6
#define E_GrP3DoChkbBaseStatTruePush	7

#define E_GrP3DoChkbBaseStatCnt				8

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoChkbBasePara
{
	__u16	PntStat[E_GrP3DoChkbBaseStatCnt];					// paint background

	__u16	AsmChgVal;
	__u16	_rsvd0;
}	*Ptr_GrP3DoChkbBasePara;

//====================================================================
//global class
class	Cls_GrP3DoChkbBase	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoChkbBasePara	m_ParaDoChkbBase;

		BOOL8	m_IsPush;
		BOOL8	m_IsVal;

		// local
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );
		virtual	void	LcNotifyValChg( void );

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
		Cls_GrP3DoChkbBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoChkbBase( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

