/*
	Gause Platform query window

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoQuery
#define	_EPC_GrP4DoQuery

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>
#include	<P4/GrP4DoWin.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class
class	Cls_GrP4DoQuery	:	public	Cls_GrP4DoWin
{
	protected:
		Def_WstrTag		m_StrTitle;
		Def_WstrShort	m_StrCont;

		// local
		
		// event
		virtual	void	ProcEvtBtnUp( Cls_GrP4DoBase* A_Obj );

		// editor only
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update

		// inherited

	public:
		Cls_GrP4DoQuery(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, __u8 A_Layer, void* A_Console,
			WCHAR* A_StrCont, WCHAR* A_StrTitle,__u16 A_RefPosWin );
		virtual	~Cls_GrP4DoQuery( void );


		//runtime
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

