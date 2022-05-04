/*
	Gause Platform virtual keyboard number

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoVkbdNum
#define	_EPC_GrP3DoVkbdNum

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>
#include	<P3/GrP3DoVkbdBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class
class	Cls_GrP3DoVkbdNum	:	public	Cls_GrP3DoVkbdBase
{
	protected:
		
		// local

		// event


		// editor only

		// inherited
		virtual	void	ProcVkbdKey( __u32 A_KeyIdx );		// virtual key process

	public:
		Cls_GrP3DoVkbdNum( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, Cls_GrP3DoBase* A_Host,
			WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd );
		virtual	~Cls_GrP3DoVkbdNum( void );

		//runtime

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

