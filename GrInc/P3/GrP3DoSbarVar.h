/*
	Gause Platform slider bar intra variable

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSbarVar
#define	_EPC_GrP3DoSbarVar

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>
#include	<P3/GrP3DoSbarBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSbarVarPara
{
	__u32	_rsvd0;
}	*Ptr_GrP3DoSbarVarPara;

//====================================================================
//global class
class	Cls_GrP3DoSbarVar	:	public	Cls_GrP3DoSbarBase
{
	protected:
		St_GrP3DoSbarVarPara	m_ParaSbarVar;

		// local

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump

		// inherited

	public:
		Cls_GrP3DoSbarVar( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSbarVar( void );

		void	ValueSet( __s32 A_Value );
		__s32	ValueGet( void );

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

