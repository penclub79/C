/*
	file utility - on WIN32

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrFileTool
#define	_EPC_GrFileTool
//====================================================================
// uses

#ifdef	LINUX_APP
	#include	<sys/vfs.h>
	#include	<stdio.h>
	#include	<dirent.h>
#endif
	
//====================================================================
//constance

#define	E_GrFileAtbTypeMask			00170000	

#define	E_GrFileAtbTypeDir			0040000
#define	E_GrFileAtbTypeLink			0120000
#define	E_GrFileAtbTypeNormal		0100000
#define	E_GrFileAtbTypeBlkDev		0060000
#define	E_GrFileAtbTypeChrDev		0020000


//====================================================================
//global macro


//====================================================================
//global type

typedef	struct St_GrFileListA
{
#ifdef LINUX_APP
	DIR*	PtrDir;
#else
	WIN32_FIND_DATAA	FindInfo;
	HANDLE	Hndl;
#endif
	Def_StrName	StrExt;
}	*Ptr_GrFileListA;

//====================================================================
//class

	
//====================================================================
//global function

//file
BOOL8		GrFileIsExist( char* A_StrFn, BOOL8 A_IsNoEmu = FALSE );
BOOL8		GrFileIsExist( WCHAR* A_StrFn, BOOL8 A_IsNoEmu = FALSE );

BOOL8		GrFileDelete( char* A_StrFn, BOOL8 A_IsNoEmu = FALSE );
BOOL8		GrFileDelete( WCHAR* A_StrFn, BOOL8 A_IsNoEmu = FALSE );

BOOL8		GrFileRename(char* A_StrTg, char* A_StrSr, BOOL8 A_IsNoEmu = FALSE);
BOOL8		GrFileRename(WCHAR* A_StrTg, WCHAR* A_StrSr, BOOL8 A_IsNoEmu = FALSE);

__u64		GrFileGetSize( char* A_StrFn, BOOL8 A_IsNoEmu = FALSE );
__u64		GrFileGetSize( WCHAR* A_StrFn, BOOL8 A_IsNoEmu = FALSE );

BOOL8		GrFileCopy(char* A_StrSrFile, char* A_StrTgFile, BOOL8 A_IsNoEmu = FALSE);
BOOL8		GrFileCopy( WCHAR* A_StrSrFile, WCHAR* A_StrTgFile, BOOL8 A_IsNoEmu = FALSE );

BOOL8		GrFileGetAtb(char* A_StrFn, __u32* A_PtrRtMode, __u32* A_PtrRtPara, BOOL8 A_IsNoEmu = FALSE);
BOOL8		GrFileGetAtb(WCHAR* A_StrFn, __u32* A_PtrRtMode, __u32* A_PtrRtPara, BOOL8 A_IsNoEmu = FALSE);

//dir
BOOL8		GrFileIsExistDir( char* A_StrFn, BOOL8 A_IsNoEmu = FALSE );
BOOL8		GrFileIsExistDir( WCHAR* A_StrFn, BOOL8 A_IsNoEmu = FALSE );

BOOL8		GrFileDelDir( char* A_StrFn, BOOL8 A_IsNoEmu = FALSE );
BOOL8		GrFileDelDir( WCHAR* A_StrFn, BOOL8 A_IsNoEmu = FALSE );

BOOL8		GrFileCreateDir(char* A_StrPath, BOOL8 A_IsNoEmu = FALSE, __u32 A_Owner = 0777);
BOOL8		GrFileCreateDir(WCHAR* A_StrPath, BOOL8 A_IsNoEmu = FALSE, __u32 A_Owner = 0777);

//char only
BOOL8		GrFileListOpen(char* A_StrPath, Ptr_GrFileListA A_PtrFind, char* A_StrExt, BOOL8 A_IsNoEmu = FALSE);

BOOL8		GrFileListGet(Ptr_GrFileListA A_PtrFind, char* A_StrRt);

BOOL8		GrFileListClose( Ptr_GrFileListA A_PtrFind );

// partition
__u64		GrFilePartSize(char* A_PtrDev, __u64* A_PtrRtFree);		//partition size


//====================================================================
//out Enviroment conditional
#endif
