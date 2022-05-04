/*
	Gause Platform check button static setup bool8

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoChkbSsb
#define	_EPC_GrP3DoChkbSsb

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoChkbBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoChkbSsbPara
{
	__u16	TxtIdFalse;
	__u16	TxtIdTrue;

	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u16	ValAdr;			// value address
	__u16	AddMult;
	__u32	AdderVal;
}	*Ptr_GrP3DoChkbSsbPara;

//====================================================================
//global class
class	Cls_GrP3DoChkbSsb	:	public	Cls_GrP3DoChkbBase
{
	protected:
		St_GrP3DoChkbSsbPara	m_ParaChkbSsb;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP3DoChkbSsb( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoChkbSsb( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

