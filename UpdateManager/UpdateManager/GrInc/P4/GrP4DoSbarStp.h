/*
	Gause Platform slider bar setup data

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSbarStp
#define	_EPC_GrP4DoSbarStp

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>
#include	<P4/GrP4DoSbarBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSbarStpPara
{
	__u16	ValAdr;			// value address
	__u16	AddMult;

	__u8	ValType;
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	_rsvd2;

	__u32	AdderVal;

	__s32	ValMin;
	__s32	ValMax;

}	*Ptr_GrP4DoSbarStpPara;

//====================================================================
//global class
class	Cls_GrP4DoSbarStp	:	public	Cls_GrP4DoSbarBase
{
	protected:
		St_GrP4DoSbarStpPara	m_ParaSbarStp;

		// local

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump
#endif

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP4DoSbarStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSbarStp( void );

		// inherited

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

