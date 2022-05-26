//--------------------------------------------------------------------
/*
	Linux module memory management
*/

#ifndef	_EPC_GrLxmMem
#define	_EPC_GrLxmMem

//====================================================================
// uses
#include	<linux/mm.h>

//====================================================================
//constance
#define _NO_VERSION__ 

//====================================================================
//global macro


//====================================================================
//global type

//====================================================================
//global function

void*	GrLxmPageAlloc( __u32 A_Size );
void	GrLxmPageFree( void* A_Ptr, __u32 A_Size );

__u32	GrLxmCalcPageSize( __u32 A_Size );
	
//====================================================================
//out Enviroment conditional
#endif

