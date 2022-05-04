/*
	Gause Platform check button static setup bool8

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoChkbSgb
#define	_EPC_GrP4DoChkbSgb

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoChkbBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoChkbSgbPara
{
	__u16	TxtIdFalse;
	__u16	TxtIdTrue;

	__u16	ValIdx;			// value index
	__u16	_rsvd;
}	*Ptr_GrP4DoChkbSgbPara;

//====================================================================
//global class
class	Cls_GrP4DoChkbSgb	:	public	Cls_GrP4DoChkbBase
{
	protected:
		St_GrP4DoChkbSgbPara	m_ParaChkbSgb;

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
		Cls_GrP4DoChkbSgb(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoChkbSgb( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

