/*
	Gause Platform virtual keyboard number

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoVkbdNum
#define	_EPC_GrP4DoVkbdNum

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>
#include	<P4/GrP4DoVkbdBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class
class	Cls_GrP4DoVkbdNum	:	public	Cls_GrP4DoVkbdBase
{
	protected:
		
		// local

		// event


		// editor only

		// inherited
		virtual	void	ProcVkbdKey( __u32 A_KeyIdx );		// virtual key process

	public:
		Cls_GrP4DoVkbdNum( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, __u8 A_Layer, Cls_GrP4DoBase* A_Host,
			WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd );
		virtual	~Cls_GrP4DoVkbdNum( void );

		//runtime

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

