/*
	GAUSE Platform basic input output system

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4Bios
#define	_EPC_GrP4Bios

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4AsmBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global function

// configuration
void	GrP4BiosCfgAssign(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

// hardware
void	GrP4BiosHwAlphaUpdt(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4BiosAdoLiveChSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4BiosAdoLiveVolSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4BiosAdoPlayChSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4BiosAdoPlayVolSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

// record
void	GrP4BiosGroupMinMapGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);


//====================================================================
//out Environment conditional
#endif

