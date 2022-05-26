/*
	Gause Platform virtual keyboard base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoVkbdBase
#define	_EPC_GrP4DoVkbdBase

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
class	Cls_GrP4DoVkbdBase	:	public	Cls_GrP4DoWin
{
	protected:
		__u32		m_MaxLen;
		WCHAR*	m_StrPtr;
		__u32		m_CusPos;
		__u32		m_Length;
		Def_WstrShort	m_StrPass;		// password display
		BOOL8		m_IsPasswd;
		
		Cls_GrP4DoBase*	m_Host;

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
		virtual	void	ProcEvtBtnUp( Cls_GrP4DoBase* A_Obj );
		virtual	BOOL8	ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );

		// editor only
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update

		// inherited

	public:
		Cls_GrP4DoVkbdBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect,__u8 A_PosRel, __u8 A_Layer, Cls_GrP4DoBase* A_Host,
			WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd );
		virtual	~Cls_GrP4DoVkbdBase( void );


		//runtime
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

