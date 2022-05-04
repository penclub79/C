

//====================================================================
// uses

#include <Win\GrWinClipboard.h>

#include <GrDumyTool.h>
#include <GrBufTool.h>
#include <GrStrTool.h>

//====================================================================
//const


//====================================================================
//global var


//====================================================================
//--------------------------------------------------------------------
UINT	GrWinClipboardRegistFormat(WCHAR* A_StrFmt)
{
	return	RegisterClipboardFormatW(A_StrFmt);
}
//--------------------------------------------------------------------
BOOL8	GrWinClipboardCopy(UINT A_HndlFormat, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8		Tv_Result;
		HGLOBAL	Tv_HndlHeap;
		void*		Tv_PtrAlloc;
	// code --------------------
		Tv_Result	=	FALSE;

		if((0 != A_Size) && (NULL != A_PtrBuf))
		{
			if(OpenClipboard(NULL))
			{
				if(EmptyClipboard())
				{
					Tv_HndlHeap	=	GlobalAlloc(GMEM_MOVEABLE, A_Size);
					if(NULL != Tv_HndlHeap)
					{
						Tv_PtrAlloc	=	GlobalLock(Tv_HndlHeap);
						if(NULL != Tv_PtrAlloc)
						{
							// copy
							GrDumyCopyMem(Tv_PtrAlloc, A_PtrBuf, A_Size);

							GlobalUnlock(Tv_HndlHeap);

							if(NULL != SetClipboardData(A_HndlFormat, Tv_HndlHeap))
							{
								// success
								Tv_Result	=	TRUE;
							}
						}
						// check faile
						if(!Tv_Result)
						{
							GlobalFree(Tv_HndlHeap);
						}
					}
				}
				CloseClipboard();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrWinClipboardPast(UINT A_HndlFormat, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
		HGLOBAL	Tv_HndlHeap;
		void*		Tv_PtrAlloc;
	// code --------------------
		Tv_Result	=	FALSE;

		if((0 != A_Size) && (NULL != A_PtrBuf))
		{
			if(OpenClipboard(NULL))
			{
				// get data
				Tv_HndlHeap	=	GetClipboardData(A_HndlFormat);
				if(NULL != Tv_HndlHeap)
				{
					Tv_PtrAlloc	=	GlobalLock(Tv_HndlHeap);
					if(NULL != Tv_PtrAlloc)
					{
						// copy
						GrDumyCopyMem(A_PtrBuf, Tv_PtrAlloc, A_Size);

						Tv_Result	=	TRUE;

						GlobalUnlock(Tv_HndlHeap);
					}
				}
				CloseClipboard();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrWinClipboardAdvCopy(UINT A_HndlFormat, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8		Tv_Result;
		HGLOBAL	Tv_HndlHeap;
		SIZE_T	Tv_AllocSize;
		Ptr_GrWinClipboardAdvHead	Tv_PtrAdv;
		void*		Tv_PtrAlloc;
	// code --------------------
		Tv_Result	=	FALSE;

		if((0 != A_Size) && (NULL != A_PtrBuf))
		{
			if(OpenClipboard(NULL))
			{
				if(EmptyClipboard())
				{
					Tv_AllocSize	=	sizeof(St_GrWinClipboardAdvHead)+ (SIZE_T)A_Size;
					Tv_HndlHeap	=	GlobalAlloc(GMEM_MOVEABLE, Tv_AllocSize);
					if(NULL != Tv_HndlHeap)
					{
						Tv_PtrAdv	=	(Ptr_GrWinClipboardAdvHead)GlobalLock(Tv_HndlHeap);
						if(NULL != Tv_PtrAdv)
						{
							// make ADV header
							Tv_PtrAdv->Fcc		=	E_GrWinClipboardFccAdv;
							Tv_PtrAdv->Size	=	A_Size;

							// copy
							Tv_PtrAlloc	=	(void*)((Def_GrCalPtr)Tv_PtrAdv + sizeof(St_GrWinClipboardAdvHead));
							GrDumyCopyMem(Tv_PtrAlloc, A_PtrBuf, A_Size);

							GlobalUnlock(Tv_HndlHeap);

							if(NULL != SetClipboardData(A_HndlFormat, Tv_HndlHeap))
							{
								// success
								Tv_Result	=	TRUE;
							}
						}
						// check faile
						if(!Tv_Result)
						{
							GlobalFree(Tv_HndlHeap);
						}
					}
				}
				CloseClipboard();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrWinClipboardAdvGetSize(UINT A_HndlFormat)
{
	// local -------------------
		__u32	Tv_Result;
		HGLOBAL	Tv_HndlHeap;
		Ptr_GrWinClipboardAdvHead	Tv_PtrAdv;
	// code --------------------
		Tv_Result	=	0;

		if(OpenClipboard(NULL))
		{
			// get data
			Tv_HndlHeap	=	GetClipboardData(A_HndlFormat);
			if(NULL != Tv_HndlHeap)
			{
				Tv_PtrAdv	=	(Ptr_GrWinClipboardAdvHead)GlobalLock(Tv_HndlHeap);
				if(NULL != Tv_PtrAdv)
				{
					// check advance format
					if(E_GrWinClipboardFccAdv == Tv_PtrAdv->Fcc)
					{
						Tv_Result	=	Tv_PtrAdv->Size;
					}
					GlobalUnlock(Tv_HndlHeap);
				}
			}
			CloseClipboard();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrWinClipboardAdvPast(UINT A_HndlFormat, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		__u32	Tv_Result;
		HGLOBAL	Tv_HndlHeap;
		Ptr_GrWinClipboardAdvHead	Tv_PtrAdv;
		void*		Tv_PtrAlloc;
	// code --------------------
		Tv_Result	=	0;

		if((0 != A_Size) && (NULL != A_PtrBuf))
		{
			if(OpenClipboard(NULL))
			{
				// get data
				Tv_HndlHeap	=	GetClipboardData(A_HndlFormat);
				if(NULL != Tv_HndlHeap)
				{
					Tv_PtrAdv	=	(Ptr_GrWinClipboardAdvHead)GlobalLock(Tv_HndlHeap);
					if(NULL != Tv_PtrAdv)
					{
						// check advance format
						if(E_GrWinClipboardFccAdv == Tv_PtrAdv->Fcc)
						{
							Tv_Result	=	Tv_PtrAdv->Size;
							if(Tv_Result > A_Size)
							{
								Tv_Result	=	A_Size;
							}

							// copy
							Tv_PtrAlloc	=	(void*)((Def_GrCalPtr)Tv_PtrAdv + sizeof(St_GrWinClipboardAdvHead));
							GrDumyCopyMem(A_PtrBuf, Tv_PtrAlloc, A_Size);

						}
						GlobalUnlock(Tv_HndlHeap);
					}
				}
				CloseClipboard();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

