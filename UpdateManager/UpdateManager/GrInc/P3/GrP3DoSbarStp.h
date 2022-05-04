/*
	Gause Platform slider bar setup data

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSbarStp
#define	_EPC_GrP3DoSbarStp

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>
#include	<P3/GrP3DoSbarBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSbarStpPara
{
	__u16	ValAdr;			// value address
	__u16	AddMult;

	__u8	ValType;
	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	_rsvd2;

	__u32	AdderVal;

}	*Ptr_GrP3DoSbarStpPara;

//====================================================================
//global class
class	Cls_GrP3DoSbarStp	:	public	Cls_GrP3DoSbarBase
{
	protected:
		St_GrP3DoSbarStpPara	m_ParaSbarStp;

		// local

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP3DoSbarStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSbarStp( void );

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

