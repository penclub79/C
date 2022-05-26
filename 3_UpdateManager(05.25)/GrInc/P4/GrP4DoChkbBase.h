/*
	Gause Platform check button base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoChkbBase
#define	_EPC_GrP4DoChkbBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoChkbBaseStatFalseDis	0
#define E_GrP4DoChkbBaseStatFalse			1
#define E_GrP4DoChkbBaseStatFalseFcs	2
#define E_GrP4DoChkbBaseStatFalsePush	3
#define E_GrP4DoChkbBaseStatTrueDis		4
#define E_GrP4DoChkbBaseStatTrue			5
#define E_GrP4DoChkbBaseStatTrueFcs		6
#define E_GrP4DoChkbBaseStatTruePush	7

#define E_GrP4DoChkbBaseStatCnt				8

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoChkbBasePara
{
	__u16	PntStat[E_GrP4DoChkbBaseStatCnt];					// paint background

	__u16	AsmChgVal;
	__u16	_rsvd0;
}	*Ptr_GrP4DoChkbBasePara;

//====================================================================
//global class
class	Cls_GrP4DoChkbBase	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoChkbBasePara	m_ParaDoChkbBase;

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

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump
#endif

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP4DoChkbBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoChkbBase( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

