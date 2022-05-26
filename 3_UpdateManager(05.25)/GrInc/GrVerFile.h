//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrVerFile
#define	_EPC_GrVerFile

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

//====================================================================
//global macro


//====================================================================
//global type

//====================================================================
//global function

BOOL8	GrVerFileLoadAll( char* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf );
BOOL8	GrVerFileSaveAll(char* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf);
BOOL8	GrVerFileChk(char* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size);
BOOL8	GrVerFileLoadPart(char* A_StrFn, void* A_PtrBuf, __u32 A_Pos, __u32 A_PartSize);
BOOL8	GrVerFileSavePart(char* A_StrFn, void* A_PtrBuf, __u32 A_Pos, __u32 A_PartSize);
__u32	GrVerFileLoadAble(char* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf, BOOL8 A_IsNoEmu);
__u32	GrVerFileLoadAble( WCHAR* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf, BOOL8 A_IsNoEmu );
BOOL8	GrVerFileSaveAll( WCHAR* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf, BOOL8 A_IsNoEmu );
	
//====================================================================
//out Enviroment conditional
#endif

