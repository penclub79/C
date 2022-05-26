/*
	Gause Platform spin decimal digit setup

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinDdgtStp
#define	_EPC_GrP4DoSpinDdgtStp

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoSpinDdgtBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinDdgtStpPara
{
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	ValType;
	__u8	_rsvd0;
	__u16	ValAdr;			// value address
	__u16	AddMult;
	__u32	AdderVal;
}	*Ptr_GrP4DoSpinDdgtStpPara;

//====================================================================
//global class
class	Cls_GrP4DoSpinDdgtStp	:	public	Cls_GrP4DoSpinDdgtBase
{
	protected:
		St_GrP4DoSpinDdgtStpPara	m_ParaSpinDdgtStp;

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
		Cls_GrP4DoSpinDdgtStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinDdgtStp( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

