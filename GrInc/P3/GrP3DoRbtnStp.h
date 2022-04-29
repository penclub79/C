/*
	Gause Platform radio button setup

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoRbtnStp
#define	_EPC_GrP3DoRbtnStp

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoRbtnBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoRbtnStpPara
{
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	ValType;
	__u8	_rsvd0;
	__u16	ValAdr;			// value address
	__u16	AddMult;
	__u32	AdderVal;
}	*Ptr_GrP3DoRbtnStpPara;

//====================================================================
//global class
class	Cls_GrP3DoRbtnStp	:	public	Cls_GrP3DoRbtnBase
{
	protected:
		St_GrP3DoRbtnStpPara	m_ParaRbtnStp;

		// local

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// event
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

		// inherited
		virtual	void	LcDoSelect( void );

	public:
		Cls_GrP3DoRbtnStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoRbtnStp( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

