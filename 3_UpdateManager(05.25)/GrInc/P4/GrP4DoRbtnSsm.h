/*
	GAUSE Platform radio button Setup mask

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoRbtnSsm
#define	_EPC_GrP4DoRbtnSsm

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoRbtnBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoRbtnSsmPara
{
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	_rsvd0;
	__u8	BitPos;
	__u16	ValAdr;			// value address
	__u16	AddMult;
	__u32	AdderVal;
}	*Ptr_GrP4DoRbtnSsmPara;

//====================================================================
//global class
class	Cls_GrP4DoRbtnSsm	:	public	Cls_GrP4DoRbtnBase
{
	protected:
		St_GrP4DoRbtnSsmPara	m_ParaRbtnSsm;

		// local
		__u8*		m_PtrStp;
		
		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// event
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

		// inherited
		virtual	void	LcDoSelect( void );

	public:
		Cls_GrP4DoRbtnSsm(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoRbtnSsm( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

