/*
	GAUSE Platform spin value setup

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinValStp
#define	_EPC_GrP4DoSpinValStp

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoSpinValBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinValStpPara
{
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	ValType;
	__u8	_rsvd0;
	__u16	ValAdr;			// value address
	__u16	AddMult;
	__u32	AdderVal;
}	*Ptr_GrP4DoSpinValStpPara;

//====================================================================
//global class
class	Cls_GrP4DoSpinValStp	:	public	Cls_GrP4DoSpinValBase
{
	protected:
		St_GrP4DoSpinValStpPara	m_ParaSpinValStp;

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP4DoSpinValStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinValStp( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

