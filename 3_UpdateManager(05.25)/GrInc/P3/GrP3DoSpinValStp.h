/*
	GAUSE Platform spin value setup

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinValStp
#define	_EPC_GrP3DoSpinValStp

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoSpinValBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinValStpPara
{
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	ValType;
	__u8	_rsvd0;
	__u16	ValAdr;			// value address
	__u16	AddMult;
	__u32	AdderVal;
}	*Ptr_GrP3DoSpinValStpPara;

//====================================================================
//global class
class	Cls_GrP3DoSpinValStp	:	public	Cls_GrP3DoSpinValBase
{
	protected:
		St_GrP3DoSpinValStpPara	m_ParaSpinValStp;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP3DoSpinValStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinValStp( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

