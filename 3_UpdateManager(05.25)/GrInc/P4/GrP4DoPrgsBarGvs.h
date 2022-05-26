/*
	Gause Platform prograss bar GVS

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoPrgsBarGvs
#define	_EPC_GrP4DoPrgsBarGvs

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoPrgsBarBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoPrgsBarGvsPara
{
	__u16	ValAdr;			// value address
	__u16	AddMult;

	__u8	ValType;
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	_rsvd2;

	__u32	AdderVal;

}	*Ptr_GrP4DoPrgsBarGvsPara;

//====================================================================
//global class
class	Cls_GrP4DoPrgsBarGvs	:	public	Cls_GrP4DoPrgsBarBase
{
	protected:
		St_GrP4DoPrgsBarGvsPara	m_ParaPrgsBarGvs;

		void*	m_PtrGvs;

		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

	public:
		Cls_GrP4DoPrgsBarGvs(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoPrgsBarGvs( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

