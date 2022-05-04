/*
	configuration Tools	
		
*/

//====================================================================
// uses
#include <GrVerFile.h>
#include <stdlib.h>
#include <GrFileCtrl.h>
#include <GrFileTool.h>

//====================================================================
//local type
typedef	struct St_GrVerFileHd 
{
	__u32		Fcc;
	__u32		Ver;
}	*Ptr_GrVerFileHd;

//====================================================================
//local function
BOOL8	GrVerFileLcChk( Cls_GrFileCtrl* A_ObjFl, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size );
BOOL8	GrVerFileLcVerChk( Cls_GrFileCtrl* A_ObjFl, __u32 A_Fcc, __u32 A_Ver );

//====================================================================
//var

//====================================================================
BOOL8	GrVerFileLcChk( Cls_GrFileCtrl* A_ObjFl, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrVerFileHd	Tv_Hd;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//load header
		A_ObjFl->Seek( 0 );
		if ( (sizeof(Tv_Hd)) == A_ObjFl->Read( &Tv_Hd, sizeof(Tv_Hd) ) )
		{
			//check size
			if ( (A_Size + sizeof(Tv_Hd) ) == A_ObjFl->GetSize() )
			{
				//check fcc and version
				if ( (A_Fcc == Tv_Hd.Fcc) && ( A_Ver == Tv_Hd.Ver ) )
				{
					//current file
					Tv_Result		=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrVerFileChk(char* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size)
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrFileCtrl*	Tv_ObjFl;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//open file
		Tv_ObjFl		=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
		if ( Tv_ObjFl->IsOpened() )
		{
			Tv_Result		=	GrVerFileLcChk( Tv_ObjFl, A_Fcc, A_Ver, A_Size );
		}
		delete	Tv_ObjFl;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrVerFileLcVerChk( Cls_GrFileCtrl* A_ObjFl, __u32 A_Fcc, __u32 A_Ver )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrVerFileHd	Tv_Hd;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//load header
		A_ObjFl->Seek( 0 );
		if ( (sizeof(Tv_Hd)) == A_ObjFl->Read( &Tv_Hd, sizeof(Tv_Hd) ) )
		{
			//check fcc and version
			if ( (A_Fcc == Tv_Hd.Fcc) && ( A_Ver == Tv_Hd.Ver ) )
			{
				//current file
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrVerFileLoadAll(char* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrFileCtrl*		Tv_ObjFile;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//create file contorl
		Tv_ObjFile	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
		if ( Tv_ObjFile->IsOpened() )
		{
			if ( GrVerFileLcChk( Tv_ObjFile, A_Fcc, A_Ver, A_Size ) )
			{
				Tv_ObjFile->Seek( sizeof(St_GrVerFileHd) );
				//load contents
				if ( A_Size == Tv_ObjFile->Read( A_PtrBuf, A_Size ) )
				{
					//success
					Tv_Result		=	TRUE;
				}
			}
		}

		delete	Tv_ObjFile;
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrVerFileLoadAble(char* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf, BOOL8 A_IsNoEmu)
{
	// local -------------------
		__u32	Tv_Result;
		Cls_GrFileCtrl*		Tv_ObjFile;
	// code --------------------
		//init
		Tv_Result		=	0;
		//create file contorl
		Tv_ObjFile	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE, FALSE, A_IsNoEmu );
		if ( Tv_ObjFile->IsOpened() )
		{
			if ( GrVerFileLcVerChk( Tv_ObjFile, A_Fcc, A_Ver ) )
			{
				Tv_ObjFile->Seek( sizeof(St_GrVerFileHd) );
				//load contents
				Tv_Result	=	Tv_ObjFile->Read( A_PtrBuf, A_Size );
			}
		}
		delete	Tv_ObjFile;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrVerFileLoadAble( WCHAR* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf, BOOL8 A_IsNoEmu )
{
	// local -------------------
		__u32	Tv_Result;
		Cls_GrFileCtrl*		Tv_ObjFile;
	// code --------------------
		//init
		Tv_Result		=	0;
		//create file contorl
		Tv_ObjFile	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE, FALSE, A_IsNoEmu );
		if ( Tv_ObjFile->IsOpened() )
		{
			if ( GrVerFileLcVerChk( Tv_ObjFile, A_Fcc, A_Ver ) )
			{
				Tv_ObjFile->Seek( sizeof(St_GrVerFileHd) );
				//load contents
				Tv_Result	=	Tv_ObjFile->Read( A_PtrBuf, A_Size );
			}
		}
		delete	Tv_ObjFile;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrVerFileSaveAll(char* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrFileCtrl*		Tv_ObjFile;
		St_GrVerFileHd		Tv_FlHd;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//delete already exist file
		if ( GrFileIsExist( A_StrFn ) )
		{
			GrFileDelete( A_StrFn );
		}

		//create file contorl
		Tv_ObjFile	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE );
		if ( Tv_ObjFile->IsOpened() )
		{
			//write header
			Tv_FlHd.Fcc				=	A_Fcc;
			Tv_FlHd.Ver				=	A_Ver;
			if ( sizeof(Tv_FlHd) == Tv_ObjFile->Write( (void*)&Tv_FlHd, sizeof(Tv_FlHd) ) )
			{
				//write contents
				if ( A_Size == Tv_ObjFile->Write( A_PtrBuf, A_Size ) )
				{
					//success
					Tv_Result		=	TRUE;
				}
			}
		}

		delete	Tv_ObjFile;
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrVerFileSaveAll( WCHAR* A_StrFn, __u32 A_Fcc, __u32 A_Ver, __u32 A_Size, void* A_PtrBuf, BOOL8 A_IsNoEmu )
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrFileCtrl*		Tv_ObjFile;
		St_GrVerFileHd		Tv_FlHd;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//delete already exist file
		if ( GrFileIsExist( A_StrFn, A_IsNoEmu ) )
		{
			GrFileDelete( A_StrFn, A_IsNoEmu );
		}

		//create file contorl
		Tv_ObjFile	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE, FALSE, A_IsNoEmu );
		if ( Tv_ObjFile->IsOpened() )
		{
			//write header
			Tv_FlHd.Fcc				=	A_Fcc;
			Tv_FlHd.Ver				=	A_Ver;
			if ( sizeof(Tv_FlHd) == Tv_ObjFile->Write( (void*)&Tv_FlHd, sizeof(Tv_FlHd) ) )
			{
				//write contents
				if ( A_Size == Tv_ObjFile->Write( A_PtrBuf, A_Size ) )
				{
					//success
					Tv_Result		=	TRUE;
				}
			}
		}

		delete	Tv_ObjFile;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrVerFileLoadPart(char* A_StrFn, void* A_PtrBuf, __u32 A_Pos, __u32 A_PartSize)
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrFileCtrl*		Tv_ObjFile;
		__s64	Tv_Fptr;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//create file contorl
		Tv_ObjFile	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
		if ( Tv_ObjFile->IsOpened() )
		{
			Tv_Fptr		=	(__s64)(sizeof(St_GrVerFileHd) + A_Pos);
			if ( Tv_Fptr == Tv_ObjFile->Seek( Tv_Fptr ) )
			{
				//load
				if ( A_PartSize == Tv_ObjFile->Read( A_PtrBuf, A_PartSize ) )
				{
					Tv_Result		=	TRUE;
				}
			}
		}

		delete	Tv_ObjFile;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrVerFileSavePart(char* A_StrFn, void* A_PtrBuf, __u32 A_Pos, __u32 A_PartSize)
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrFileCtrl*		Tv_ObjFile;
		__s64	Tv_Fptr;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//create file contorl
		Tv_ObjFile	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, FALSE );
		if ( Tv_ObjFile->IsOpened() )
		{
			Tv_Fptr		=	(__s64)(sizeof(St_GrVerFileHd) + A_Pos);
			if ( Tv_Fptr == Tv_ObjFile->Seek( Tv_Fptr ) )
			{
				//write
				if ( A_PartSize == Tv_ObjFile->Write( A_PtrBuf, A_PartSize ) )
				{
					Tv_Result		=	TRUE;
				}
			}
		}

		delete	Tv_ObjFile;

		return	Tv_Result;
}
//--------------------------------------------------------------------


