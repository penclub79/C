/*
	Gause Platform check button static setup mask

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoChkbSsm
#define	_EPC_GrP3DoChkbSsm

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
typedef	struct	St_GrP3DoChkbSsmPara
{
	__u16	TxtIdFalse;
	__u16	TxtIdTrue;

	BOOL8	IsAdder;
	__u8	AdderType;
	__u8	_rsvd0;
	__u8	BitPos;
	__u16	ValAdr;			// value address
	__u16	AddMult;
	__u32	AdderVal;
}	*Ptr_GrP3DoChkbSsmPara;

//====================================================================
//global class
class	Cls_GrP3DoChkbSsm	:	public	Cls_GrP3DoChkbBase
{
	protected:
		St_GrP3DoChkbSsmPara	m_ParaChkbSsm;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP3DoChkbSsm( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoChkbSsm( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

