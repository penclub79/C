/*
	GAUSE Platform edit local

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoEdtLoc
#define	_EPC_GrP4DoEdtLoc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>
#include <P4/GrP4DoEdtBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoEdtLocPara
{
	__u16	EvtHostId;
	__u16	_rsvd0;
}	*Ptr_GrP4DoEdtLocPara;

//====================================================================
//global class
class	Cls_GrP4DoEdtLoc	:	public	Cls_GrP4DoEdtBase
{
	protected:
		St_GrP4DoEdtLocPara	m_ParaEdtLoc;

		Def_WstrShort	m_StrTxt;

		// local
		virtual	WCHAR*	LcValAdrGet( void );		// value address get
		virtual	void	ProcValChg( void );				// value change

		// event

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited

	public:
		Cls_GrP4DoEdtLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoEdtLoc( void );

		// runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

