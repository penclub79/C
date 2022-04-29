/*
	GAUSE Platform radio button Global Variable User

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoRbtnGvu
#define	_EPC_GrP3DoRbtnGvu

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
typedef	struct	St_GrP3DoRbtnGvuPara
{
	__u16	ValIdx;			// value index
	__u16	_rsvd0;
}	*Ptr_GrP3DoRbtnGvuPara;

//====================================================================
//global class
class	Cls_GrP3DoRbtnGvu	:	public	Cls_GrP3DoRbtnBase
{
	protected:
		St_GrP3DoRbtnGvuPara	m_ParaRbtnGvu;

		// local
		__s32*		m_PtrGvu;
		
		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// event
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

		// inherited
		virtual	void	LcDoSelect( void );

	public:
		Cls_GrP3DoRbtnGvu( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoRbtnGvu( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

