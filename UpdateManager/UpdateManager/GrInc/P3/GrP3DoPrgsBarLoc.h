/*
	Gause Platform prograss bar local

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoPrgsBarLoc
#define	_EPC_GrP3DoPrgsBarLoc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoPrgsBarBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
/*
typedef	struct	St_GrP3DoPrgsBarLocPara
{
	__u16	EvtHostId;
	__u16	_rsvd0;

}	*Ptr_GrP3DoPrgsBarLocPara;
*/

//====================================================================
//global class
class	Cls_GrP3DoPrgsBarLoc	:	public	Cls_GrP3DoPrgsBarBase
{
	protected:
		//St_GrP3DoPrgsBarLocPara	m_ParaPrgsBarLoc;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited

	public:
		Cls_GrP3DoPrgsBarLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoPrgsBarLoc( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

