/*
	Gause Platform prograss bar GVS

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoPrgsBarGvs
#define	_EPC_GrP3DoPrgsBarGvs

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
typedef	struct	St_GrP3DoPrgsBarGvsPara
{
	__u16	ValAdr;			// value address
	__u16	AddMult;

	__u8	ValType;
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	_rsvd2;

	__u32	AdderVal;

}	*Ptr_GrP3DoPrgsBarGvsPara;

//====================================================================
//global class
class	Cls_GrP3DoPrgsBarGvs	:	public	Cls_GrP3DoPrgsBarBase
{
	protected:
		St_GrP3DoPrgsBarGvsPara	m_ParaPrgsBarGvs;

		void*	m_PtrGvs;

		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

	public:
		Cls_GrP3DoPrgsBarGvs( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoPrgsBarGvs( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

