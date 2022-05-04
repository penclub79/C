/*
	GAUSE Platform edit setup string

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoEdtStpStr
#define	_EPC_GrP3DoEdtStpStr

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
typedef	struct	St_GrP3DoEdtStpStrPara
{
	__u16	ValAdr;					// value address
	__u16	_rsvd0;
	BOOL8	IsAdder;				// address adder use
	__u8	AdderType;			// adder type
	__u16	AddMult;				// adder multiple
	__u32	AdderVal;				// adder value
}	*Ptr_GrP3DoEdtStpStrPara;

//====================================================================
//global class
class	Cls_GrP3DoEdtStpStr	:	public	Cls_GrP3DoEdtBase
{
	protected:
		St_GrP3DoEdtStpStrPara	m_ParaEdtStpStr;

		// local
		virtual	WCHAR*	LcValAdrGet( void );		// value address get

		// event

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited

	public:
		Cls_GrP3DoEdtStpStr( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoEdtStpStr( void );

		// runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

