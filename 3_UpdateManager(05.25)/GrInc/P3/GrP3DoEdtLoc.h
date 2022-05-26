/*
	GAUSE Platform edit local

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoEdtLoc
#define	_EPC_GrP3DoEdtLoc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>
#include <P3/GrP3DoEdtBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoEdtLocPara
{
	__u16	EvtHostId;
	__u16	_rsvd0;
}	*Ptr_GrP3DoEdtLocPara;

//====================================================================
//global class
class	Cls_GrP3DoEdtLoc	:	public	Cls_GrP3DoEdtBase
{
	protected:
		St_GrP3DoEdtLocPara	m_ParaEdtLoc;

		Def_WstrShort	m_StrTxt;

		// local
		virtual	WCHAR*	LcValAdrGet( void );		// value address get
		virtual	void	ProcValChg( void );				// value change

		// event

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited

	public:
		Cls_GrP3DoEdtLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoEdtLoc( void );

		// runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

