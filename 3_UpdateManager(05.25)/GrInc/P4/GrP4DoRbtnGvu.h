/*
	GAUSE Platform radio button Global Variable User

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoRbtnGvu
#define	_EPC_GrP4DoRbtnGvu

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
typedef	struct	St_GrP4DoRbtnGvuPara
{
	__u16	ValIdx;			// value index
	__u16	_rsvd0;
}	*Ptr_GrP4DoRbtnGvuPara;

//====================================================================
//global class
class	Cls_GrP4DoRbtnGvu	:	public	Cls_GrP4DoRbtnBase
{
	protected:
		St_GrP4DoRbtnGvuPara	m_ParaRbtnGvu;

		// local
		__s32*		m_PtrGvu;
		
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
		Cls_GrP4DoRbtnGvu(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoRbtnGvu( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

