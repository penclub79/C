/*
Window clipboard

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrWinClipboard
#define	_EPC_GrWinClipboard

//====================================================================
// uses

#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>


//====================================================================
//constance

#define	E_GrWinClipboardFccAdv		Mac_GrMakeFcc('C','L','I','P')

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrWinClipboardAdvHead
{
	__u32	Fcc;
	__u32	Size;
}	*Ptr_GrWinClipboardAdvHead;

//====================================================================
//class


//====================================================================
//global function

UINT	GrWinClipboardRegistFormat(WCHAR* A_StrFmt);
BOOL8	GrWinClipboardCopy(UINT A_HndlFormat, void* A_PtrBuf, __u32 A_Size);
BOOL8	GrWinClipboardPast(UINT A_HndlFormat, void* A_PtrBuf, __u32 A_Size);

BOOL8	GrWinClipboardAdvCopy(UINT A_HndlFormat, void* A_PtrBuf, __u32 A_Size);
__u32	GrWinClipboardAdvGetSize(UINT A_HndlFormat);
__u32	GrWinClipboardAdvPast(UINT A_HndlFormat, void* A_PtrBuf, __u32 A_Size);

//====================================================================
//out Enviroment conditional
#endif

