/*
	Gause Platform check button static setup bool8

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoChkbSgb
#define	_EPC_GrP3DoChkbSgb

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
typedef	struct	St_GrP3DoChkbSgbPara
{
	__u16	TxtIdFalse;
	__u16	TxtIdTrue;

	__u16	ValIdx;			// value index
	__u16	_rsvd;
}	*Ptr_GrP3DoChkbSgbPara;

//====================================================================
//global class
class	Cls_GrP3DoChkbSgb	:	public	Cls_GrP3DoChkbBase
{
	protected:
		St_GrP3DoChkbSgbPara	m_ParaChkbSgb;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP3DoChkbSgb( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoChkbSgb( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

