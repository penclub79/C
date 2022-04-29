/*
	GAUSE Platform edit setup string

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoEdtStpStr
#define	_EPC_GrP4DoEdtStpStr

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
typedef	struct	St_GrP4DoEdtStpStrPara
{
	__u16	ValAdr;					// value address
	__u16	_rsvd0;
	BOOL8	IsAdder;				// address adder use
	__u8	AdderType;			// adder type
	__u16	AddMult;				// adder multiple
	__u32	AdderVal;				// adder value
}	*Ptr_GrP4DoEdtStpStrPara;

//====================================================================
//global class
class	Cls_GrP4DoEdtStpStr	:	public	Cls_GrP4DoEdtBase
{
	protected:
		St_GrP4DoEdtStpStrPara	m_ParaEdtStpStr;

		// local
		virtual	WCHAR*	LcValAdrGet( void );		// value address get

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
		Cls_GrP4DoEdtStpStr(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoEdtStpStr( void );

		// runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

