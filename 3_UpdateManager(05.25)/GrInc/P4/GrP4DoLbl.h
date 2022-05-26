/*
	Gause Platform window

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoLbl
#define	_EPC_GrP4DoLbl

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoLblStrMaxLen			256


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoLblPara
{
	__u16	PntBg;					// paint background
	__u16	_rsvd0;
}	*Ptr_GrP4DoLblPara;

//====================================================================
//global class
class	Cls_GrP4DoLbl	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoLblPara	m_ParaDoLbl;
		WCHAR	m_StrTxt[E_GrP4DoLblStrMaxLen];

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP4DoLbl(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoLbl( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

