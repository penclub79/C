/*
	GAUSE Platform spin buffer text setup

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinBtxtStp
#define	_EPC_GrP3DoSpinBtxtStp

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoSpinBtxtBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinBtxtStpPara
{
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	ValType;
	__u8	_rsvd0;
	__u16	ValAdr;			// value address
	__u16	AddMult;
	__u32	AdderVal;
}	*Ptr_GrP3DoSpinBtxtStpPara;

//====================================================================
//global class
class	Cls_GrP3DoSpinBtxtStp	:	public	Cls_GrP3DoSpinBtxtBase
{
	protected:
		St_GrP3DoSpinBtxtStpPara	m_ParaSpinBtxtStp;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP3DoSpinBtxtStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinBtxtStp( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

