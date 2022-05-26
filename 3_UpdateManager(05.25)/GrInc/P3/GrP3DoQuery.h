/*
	Gause Platform query window

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoQuery
#define	_EPC_GrP3DoQuery

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>
#include	<P3/GrP3DoWin.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class
class	Cls_GrP3DoQuery	:	public	Cls_GrP3DoWin
{
	protected:
		Def_WstrTag		m_StrTitle;
		Def_WstrShort	m_StrCont;

		// local
		
		// event
		virtual	void	ProcEvtBtnUp( Cls_GrP3DoBase* A_Obj );

		// editor only
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update

		// inherited

	public:
		Cls_GrP3DoQuery( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, void* A_Console,
			WCHAR* A_StrCont, WCHAR* A_StrTitle );
		virtual	~Cls_GrP3DoQuery( void );


		//runtime
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

