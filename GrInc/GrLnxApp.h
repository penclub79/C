/*
	Linux on Win32 header

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_LnxAppOnW32
#define	_EPC_LnxAppOnW32

//====================================================================
// uses

#if 0
#ifndef LINUX_APP
#ifdef DEBUG

#define	new	DEBUG_NEW

#endif
#endif
#endif

#if 0
#ifdef _CRTDBG_MAP_ALLOC
	#include <stdlib.h> 
	#include <crtdbg.h>
	#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
#endif

#define	GR_TASKMSG_DEBUG

//====================================================================
//constance

#ifdef	LINUX_APP
	#ifdef	WIN32
		#define		__GNUC__	4
		#define		__GNUC_MINOR__	1
		#define		_GNU_SOURCE
		#define		__extension__
		#define		__USE_BSD

		#define		__signed__	signed
		#define		__attribute__( A_Para )		
		#define		__inline__
		#define		inline
		#define		__value
		#define		__asm__( A_Para )
		#define		__volatile__
		#define		__builtin_expect
		#define		__typeof__( A_Type )	A_Type	
		#define		__const	const

		#define		__STDC__
	#else
		#ifdef LINUX_EMU
			#define		inline
		#endif
	#endif		//ifdef		WIN32
#else
#ifndef _MFC_VER
	#include <Windows.h>
#endif

#endif

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//global function

//====================================================================
//out Environment conditional

#endif		//#ifndef	_EPC_LnxAppOnW32

