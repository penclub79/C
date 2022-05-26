/*
	Gause Platform notify window

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoNotify
#define	_EPC_GrP3DoNotify

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
class	Cls_GrP3DoNotify	:	public	Cls_GrP3DoWin
{
	protected:
		Def_WstrTag		m_StrTitle;
		Def_WstrShort	m_StrCont;
		__u32					m_Timer;

		// local
		
		// event
		virtual	void	ProcEvtBtnUp( Cls_GrP3DoBase* A_Obj );
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

		// editor only
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update

		// inherited

	public:
		Cls_GrP3DoNotify( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, void* A_Console,
			WCHAR* A_StrCont, WCHAR* A_StrTitle, __u8 A_Timer );
		virtual	~Cls_GrP3DoNotify( void );


		//runtime
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

