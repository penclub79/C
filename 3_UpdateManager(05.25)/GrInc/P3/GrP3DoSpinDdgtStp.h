/*
	Gause Platform spin decimal digit setup

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinDdgtStp
#define	_EPC_GrP3DoSpinDdgtStp

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoSpinDdgtBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinDdgtStpPara
{
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	ValType;
	__u8	_rsvd0;
	__u16	ValAdr;			// value address
	__u16	AddMult;
	__u32	AdderVal;
}	*Ptr_GrP3DoSpinDdgtStpPara;

//====================================================================
//global class
class	Cls_GrP3DoSpinDdgtStp	:	public	Cls_GrP3DoSpinDdgtBase
{
	protected:
		St_GrP3DoSpinDdgtStpPara	m_ParaSpinDdgtStp;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP3DoSpinDdgtStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinDdgtStp( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

