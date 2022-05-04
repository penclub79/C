/*
	Gause Platform virtual keyboard base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoVkbdBase
#define	_EPC_GrP3DoVkbdBase

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
class	Cls_GrP3DoVkbdBase	:	public	Cls_GrP3DoWin
{
	protected:
		__u32		m_MaxLen;
		WCHAR*	m_StrPtr;
		__u32		m_CusPos;
		__u32		m_Length;
		Def_WstrShort	m_StrPass;		// password display
		BOOL8		m_IsPasswd;
		
		Cls_GrP3DoBase*	m_Host;

		// local
		virtual	void	ProcVkbdKey( __u32 A_KeyIdx );		// virtual key process
		virtual	void	ProcTxtUpdt( void );							// text updated

		// API
		void	VkClear( void );
		void	VkCusHome( void );
		void	VkCusEnd( void );
		void	VkCusLeft( void );
		void	VkCusRight( void );
		void	VkCodeIns( WCHAR A_Code, BOOL8 A_IsMoveCus );
		void	VkCodeOvwt( WCHAR A_Code, BOOL8 A_IsMoveCus );
		void	VkBackspace( void );
		void	VkDelete( void );

		// event
		virtual	void	ProcEvtBtnUp( Cls_GrP3DoBase* A_Obj );
		virtual	BOOL8	ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );

		// editor only
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update

		// inherited

	public:
		Cls_GrP3DoVkbdBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, Cls_GrP3DoBase* A_Host,
			WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd );
		virtual	~Cls_GrP3DoVkbdBase( void );


		//runtime
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

