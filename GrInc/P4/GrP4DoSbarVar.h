/*
	Gause Platform slider bar intra variable

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSbarVar
#define	_EPC_GrP4DoSbarVar

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>
#include	<P4/GrP4DoSbarBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSbarVarPara
{
	__u32	_rsvd0;
}	*Ptr_GrP4DoSbarVarPara;

//====================================================================
//global class
class	Cls_GrP4DoSbarVar	:	public	Cls_GrP4DoSbarBase
{
	protected:
		St_GrP4DoSbarVarPara	m_ParaSbarVar;

		// local

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump
#endif

		// inherited

	public:
		Cls_GrP4DoSbarVar(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSbarVar( void );

		// inherited

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

