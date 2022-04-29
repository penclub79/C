/*
	Gause Platform window

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoLbl
#define	_EPC_GrP3DoLbl

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoLblStrMaxLen			256


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoLblPara
{
	__u16	PntBg;					// paint background
	__u16	_rsvd0;
}	*Ptr_GrP3DoLblPara;

//====================================================================
//global class
class	Cls_GrP3DoLbl	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoLblPara	m_ParaDoLbl;
		WCHAR	m_StrTxt[E_GrP3DoLblStrMaxLen];

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoLbl( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoLbl( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

