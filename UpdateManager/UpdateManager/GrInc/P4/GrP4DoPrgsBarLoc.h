/*
	Gause Platform prograss bar local

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoPrgsBarLoc
#define	_EPC_GrP4DoPrgsBarLoc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoPrgsBarBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
/*
typedef	struct	St_GrP4DoPrgsBarLocPara
{
	__u16	EvtHostId;
	__u16	_rsvd0;

}	*Ptr_GrP4DoPrgsBarLocPara;
*/

//====================================================================
//global class
class	Cls_GrP4DoPrgsBarLoc	:	public	Cls_GrP4DoPrgsBarBase
{
	protected:
		//St_GrP4DoPrgsBarLocPara	m_ParaPrgsBarLoc;

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited

	public:
		Cls_GrP4DoPrgsBarLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoPrgsBarLoc( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

