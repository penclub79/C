/*
	standard debug

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDebug
#define	_EPC_GrDebug

//====================================================================
// uses
#ifdef	LINUX_APP
	#include	<stdio.h>
#else
#ifdef _CONSOLE
#include	<stdio.h>
#endif
#endif

//====================================================================
//constance

#define E_DbgDumpUnitByte			0
#define E_DbgDumpUnitWord			1
#define E_DbgDumpUnitDword		2

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

//====================================================================
//global function
#ifdef	LINUX_APP

#ifdef GR_DBG_PRT_RPLC
#define	DbgMsgPrint	GR_DBG_PRT_RPLC

void	GR_DBG_PRT_RPLC(char* A_Str,...);

#else
	#define	DbgMsgPrint	printf
#endif

#else

#ifdef _CONSOLE

#define	DbgMsgPrint	printf

#else

	#ifdef GR_NO_DBG_PRT
		inline	void	DbgMsgPrint( __char* A_Str, ... )
		{
		}
	#else
		extern	void	DbgMsgPrint( __in_z __format_string const char * A_StrFmt, ... );
	#endif

#endif
#endif

#ifndef LINUX_DRV
		void	DbgMemDump( void* A_Ptr, unsigned int A_Size, unsigned int A_Unit = E_DbgDumpUnitByte );
#endif

//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif
