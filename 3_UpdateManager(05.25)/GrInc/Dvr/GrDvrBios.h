/*
	DVR BIOS 
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrDvrBios
#define	_EPC_GrDvrBios

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrTimeTool.h>
#include <Dvr/GrDvrBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//global function

BOOL8	GrDvrBiosRecDevNameGet( __u8 A_Idx, char* A_StrRt );			// record device name get
BOOL8	GrDvrBiosRecDevModelNameGet(__u8 A_Idx, char* A_StrRt);
void	GrDvrBiosLogAdd(Ptr_GrDvrLogItm A_PtrLog);

//====================================================================
//out Environment conditional
#endif

