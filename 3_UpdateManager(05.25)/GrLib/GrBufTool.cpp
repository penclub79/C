/*
 variable queue

 */


//====================================================================
// uses
#ifndef	LINUX_DRV

#include	<GrBufTool.h>
#include	<GrDebug.h>

#endif

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
void		GrBufQueInit( struct St_GrBufQue* A_PtrBuf, __s32 A_ItemSize, __s32 A_BufSize )
{
	// local -------------------
	// code --------------------
		A_PtrBuf->InPos				=	0;
		A_PtrBuf->OutPos			=	0;

		//DbgMsgPrint( "GrBufQueInit - itemsize:%d bufsizef:%d\n", A_ItemSize, A_BufSize );

		A_PtrBuf->SizeItem		=	A_ItemSize;
		A_PtrBuf->BufSize			=	A_BufSize - sizeof(struct St_GrBufQue);
		//patch buffer size
		A_PtrBuf->BufSize			=	A_PtrBuf->BufSize - (A_PtrBuf->BufSize % A_ItemSize);
}
//--------------------------------------------------------------------
void*		GrBufQueGetInPtr( struct St_GrBufQue* A_PtrBuf )
{
	// local -------------------
		void*		Tv_Result;
		__s32			Tv_InSize;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//check add able
		Tv_InSize		=	A_PtrBuf->InPos - A_PtrBuf->OutPos;
		if ( 0 > Tv_InSize )
		{
			Tv_InSize	=	Tv_InSize + A_PtrBuf->BufSize;
		}
		if ( A_PtrBuf->SizeItem < (A_PtrBuf->BufSize - Tv_InSize) )
		{
			//calculate in address
			Tv_Result	=	(void*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)(sizeof(struct St_GrBufQue) + A_PtrBuf->InPos));
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufQueIn( struct St_GrBufQue* A_PtrBuf )
{
	// local -------------------
		__s32			Tv_InPos;
	// code --------------------
		//insert 
		Tv_InPos		=	A_PtrBuf->InPos + A_PtrBuf->SizeItem;
		if ( A_PtrBuf->BufSize <= Tv_InPos )
		{
			Tv_InPos	=	0;
		}
		//update
		A_PtrBuf->InPos		=	Tv_InPos;
}
//--------------------------------------------------------------------
void*		GrBufQueGetOutPtr( struct St_GrBufQue* A_PtrBuf )
{
	// local -------------------
		void*		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//check empty
		if ( A_PtrBuf->InPos != A_PtrBuf->OutPos )
		{
			Tv_Result		=	(void*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)(sizeof(struct St_GrBufQue) + A_PtrBuf->OutPos));
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrBufQueOut( struct St_GrBufQue* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_OutPos;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check empty
		if ( A_PtrBuf->InPos != A_PtrBuf->OutPos )
		{
			//increase out point
			Tv_OutPos	=	A_PtrBuf->OutPos + A_PtrBuf->SizeItem;
			if ( A_PtrBuf->BufSize <= Tv_OutPos) 
			{
				Tv_OutPos	=	0;
			}
			//update in position
			A_PtrBuf->OutPos	=	Tv_OutPos;
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufQueClear( struct St_GrBufQue* A_PtrBuf )
{
		A_PtrBuf->OutPos	=	A_PtrBuf->InPos;
}
//--------------------------------------------------------------------
__u32		GrBufQueGetInCount(struct St_GrBufQue* A_PtrBuf)
{
	// local -------------------
		__s32	Tv_InSize;
	// code --------------------
		Tv_InSize		=	A_PtrBuf->InPos - A_PtrBuf->OutPos;
		if(0 > Tv_InSize)
		{
			Tv_InSize	=	Tv_InSize + A_PtrBuf->BufSize;
		}

		return	(__u32)Tv_InSize / A_PtrBuf->SizeItem;
}
//--------------------------------------------------------------------
void		GrBufVqInit(	struct St_GrBufVarQue* A_PtrBuf, __u32 A_BufSize, __u32 A_AlignSize )
{
		A_PtrBuf->AlgSize		=	A_AlignSize;
		if(E_GrBufVqHdSize > A_AlignSize)
		{
			A_PtrBuf->AlgSize		=	E_GrBufVqHdSize;
		}
		A_PtrBuf->AlgMask		=	~(A_PtrBuf->AlgSize -1);
		A_PtrBuf->Ofs				=	A_PtrBuf->AlgMask & (sizeof(struct St_GrBufVarQue) + (A_PtrBuf->AlgSize - 1));
		A_PtrBuf->BufSize		=	A_BufSize & A_PtrBuf->AlgMask;
		A_PtrBuf->InPos			=	A_PtrBuf->Ofs;
		A_PtrBuf->OutPos			=	A_PtrBuf->Ofs;

}
//--------------------------------------------------------------------
void*		GrBufVqGetInPtr( struct St_GrBufVarQue* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		void*		Tv_Result;
		__u32		Tv_OutPos;		//backup out position - for synchronize
		__u32		Tv_PktSize;
		__u32*	Tv_PtrHd;			//header pointer
		BOOL8		Tv_IsOk;
	// code --------------------
		//init
		Tv_Result		=	NULL;
		Tv_OutPos		=	A_PtrBuf->OutPos;
		Tv_IsOk			=	FALSE;
		Tv_PtrHd		=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->InPos);
		Tv_PktSize	=	A_PtrBuf->AlgMask & (A_PtrBuf->AlgSize + A_Size + A_PtrBuf->AlgSize - 1);

		*Tv_PtrHd		=	0;		//reset header

		if(A_PtrBuf->InPos >= Tv_OutPos)
		{
			if((A_PtrBuf->InPos + Tv_PktSize) < A_PtrBuf->BufSize )
			{
				// able
				Tv_IsOk		=	TRUE;
			}
			else if(((A_PtrBuf->InPos + Tv_PktSize) == A_PtrBuf->BufSize) && (A_PtrBuf->Ofs < Tv_OutPos))
			{
				// able
				Tv_IsOk		=	TRUE;
			}
			else
			{
				// check front size
				if((A_PtrBuf->Ofs + Tv_PktSize) < Tv_OutPos)
				{
					// mark end
					*Tv_PtrHd	=	E_GrBufVqHdIdEnd;

					// change in position
					A_PtrBuf->InPos	=	A_PtrBuf->Ofs;
					Tv_PtrHd		=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->InPos);

					*Tv_PtrHd		=	0;

					Tv_IsOk			=	TRUE;

				}
			}
		}
		else
		{
			if((A_PtrBuf->InPos + Tv_PktSize) < Tv_OutPos)
			{
				Tv_IsOk	=	TRUE;
			}
		}

		if ( Tv_IsOk )
		{
			*Tv_PtrHd	=	E_GrBufVqHdIdNormal | Tv_PktSize;
			Tv_Result	=	(void*)((Def_GrCalPtr)Tv_PtrHd + (Def_GrCalPtr)A_PtrBuf->AlgSize);
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrBufVqIn( struct St_GrBufVarQue* A_PtrBuf )
{
	// local -------------------
		__u32*	Tv_PtrHd;
		__u32		Tv_Size;
		__u32		Tv_InPos;
	// code --------------------
		//init
		Tv_PtrHd		=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->InPos);
		//check current header
		if ( E_GrBufVqHdIdNormal == (E_GrBufVqHdIdMask & (*Tv_PtrHd) ) )
		{
			//insert
			Tv_Size		=	E_GrBufVqHdSizeMask & (*Tv_PtrHd);
			Tv_InPos	=	A_PtrBuf->InPos + Tv_Size;
			if ( A_PtrBuf->BufSize <= Tv_InPos )
			{
				Tv_InPos	=	A_PtrBuf->Ofs;		//first position
			}
			A_PtrBuf->InPos	=	Tv_InPos;
		}
}
//--------------------------------------------------------------------
void		GrBufVqSizeIn(struct St_GrBufVarQue* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		__u32*	Tv_PtrHd;
		__u32		Tv_Size;
		__u32		Tv_InPos;
		__u32		Tv_NewSize;
	// code --------------------
		//init
		Tv_PtrHd		=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->InPos);
		//check current header
		if(E_GrBufVqHdIdNormal == (E_GrBufVqHdIdMask & (*Tv_PtrHd)))
		{
			//insert
			Tv_Size			=	E_GrBufVqHdSizeMask & (*Tv_PtrHd);
			Tv_NewSize	=	A_PtrBuf->AlgMask & (A_PtrBuf->AlgSize + A_Size + A_PtrBuf->AlgSize - 1);
			if(Tv_Size >= Tv_NewSize)
			{
				// update new size
				Tv_Size			=	Tv_NewSize;
				*Tv_PtrHd		=	E_GrBufVqHdIdNormal | Tv_Size;

				Tv_InPos	=	A_PtrBuf->InPos + Tv_Size;
				if(A_PtrBuf->BufSize <= Tv_InPos)
				{
					Tv_InPos	=	A_PtrBuf->Ofs;		//first position
				}
				A_PtrBuf->InPos	=	Tv_InPos;
			}
		}
}
//--------------------------------------------------------------------
void*		GrBufVqGetOutPtr( struct St_GrBufVarQue* A_PtrBuf )
{
	// local -------------------
		void*		Tv_Result;
		__u32		Tv_InPos;
		__u32*	Tv_PtrHd;
		BOOL8		Tv_IsOk;
	// code --------------------
		//init
		Tv_Result			=	NULL;
		Tv_InPos			=	A_PtrBuf->InPos;		//backup in positon - for sync
		Tv_IsOk				=	FALSE;

		//release end position
		if ( Tv_InPos != A_PtrBuf->OutPos )
		{
			//check endmark
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->OutPos);
			if ( E_GrBufVqHdIdEnd == (*Tv_PtrHd) )
			{
				A_PtrBuf->OutPos	=	A_PtrBuf->Ofs;
				if ( Tv_InPos != A_PtrBuf->OutPos )
				{
					//success
					Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->OutPos);
					Tv_IsOk		=	TRUE;
				}
			}
			else
			{
				//success
				Tv_IsOk		=	TRUE;
			}
			//success
			if ( Tv_IsOk )
			{
				Tv_Result	=	(void*)((Def_GrCalPtr)Tv_PtrHd + (Def_GrCalPtr)A_PtrBuf->AlgSize);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*		GrBufVqGetPtrByPos(struct St_GrBufVarQue* A_PtrBuf, __u32 A_Pos, __u32* A_PtrRtCurPos)
{
	// local -------------------
		void*		Tv_Result;
		__u32		Tv_InPos;
		__u32		Tv_WkPos;
		__u32*	Tv_PtrHd;
		BOOL8		Tv_IsOk;
	// code --------------------
		Tv_Result			=	NULL;
		Tv_InPos			=	A_PtrBuf->InPos;		//backup in positon - for sync
		Tv_WkPos			=	A_Pos;							// working position
		Tv_IsOk				=	FALSE;

		//release end position
		if (Tv_InPos != Tv_WkPos)
		{
			Tv_IsOk		=	TRUE;
			//check endmark
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_WkPos);
			if (E_GrBufVqHdIdEnd == (*Tv_PtrHd))
			{
				Tv_IsOk		=	FALSE;
				Tv_WkPos	=	A_PtrBuf->Ofs;
				if (Tv_InPos != Tv_WkPos)
				{
					//success
					Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_WkPos);
					Tv_IsOk		=	TRUE;
				}
			}
			//success
			if (Tv_IsOk)
			{
				Tv_Result	=	(void*)((Def_GrCalPtr)Tv_PtrHd + (Def_GrCalPtr)A_PtrBuf->AlgSize);
			}
			// update 
			*A_PtrRtCurPos	=	Tv_WkPos;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrBufVqOut( struct St_GrBufVarQue* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_InPos;
		__u32*	Tv_PtrHd;
		__u32		Tv_Size;
		__u32		Tv_OutPos;
	// code --------------------
		//init
		Tv_Result			=	FALSE;

		//thread sync
		Tv_InPos			=	A_PtrBuf->InPos;		//backup in positon - for sync
		Tv_OutPos			=	A_PtrBuf->OutPos;
		//check empty
		if ( Tv_InPos != Tv_OutPos )
		{
			//get header
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_OutPos);
			Tv_Size	=	E_GrBufVqHdSizeMask & (*Tv_PtrHd);
			Tv_OutPos	=	Tv_OutPos + Tv_Size;
			if ( Tv_OutPos >= A_PtrBuf->BufSize )
			{
				Tv_OutPos	=	A_PtrBuf->Ofs;
			}
			Tv_Result	=	TRUE;
			//remove end block
			if ( Tv_InPos != Tv_OutPos )
			{
				Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_OutPos);
				if ( E_GrBufVqHdIdEnd == (*Tv_PtrHd) )
				{
					Tv_OutPos	=	A_PtrBuf->Ofs;
				}
			}
		}
		//update out pos
		A_PtrBuf->OutPos	=	Tv_OutPos;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrBufVqGetNextPosByPos(struct St_GrBufVarQue* A_PtrBuf, __u32 A_Pos, __u32* A_PtrRtNextPos)
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_InPos;
		__u32*	Tv_PtrHd;
		__u32		Tv_Size;
		__u32		Tv_OutPos;
	// code --------------------
		//init
		Tv_Result			=	FALSE;

		//thread sync
		Tv_InPos			=	A_PtrBuf->InPos;		//backup in positon - for sync
		Tv_OutPos			=	A_Pos;
		//check empty
		if (Tv_InPos != Tv_OutPos)
		{
			//get header
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_OutPos);
			Tv_Size	=	E_GrBufVqHdSizeMask & (*Tv_PtrHd);
			Tv_OutPos	=	Tv_OutPos + Tv_Size;
			if (Tv_OutPos >= A_PtrBuf->BufSize)
			{
				Tv_OutPos	=	A_PtrBuf->Ofs;
			}
			Tv_Result	=	TRUE;
			//remove end block
			if (Tv_InPos != Tv_OutPos)
			{
				Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_OutPos);
				if (E_GrBufVqHdIdEnd == (*Tv_PtrHd))
				{
					Tv_OutPos	=	A_PtrBuf->Ofs;
				}
			}
		}
		//update out pos
		if (Tv_Result)
		{
			*A_PtrRtNextPos	=	Tv_OutPos;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufVqReset( struct St_GrBufVarQue* A_PtrBuf )
{
		A_PtrBuf->OutPos		=	A_PtrBuf->InPos;
}
//--------------------------------------------------------------------
__u32		GrBufVqGetUseSize( struct St_GrBufVarQue* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_OutPos;
		__u32	Tv_InPos;
	// code --------------------
		Tv_OutPos	=	A_PtrBuf->OutPos;
		Tv_InPos	=	A_PtrBuf->InPos;
		if ( Tv_InPos < Tv_OutPos )
		{
			Tv_Result	=	(A_PtrBuf->BufSize + Tv_InPos - Tv_OutPos) - A_PtrBuf->Ofs;
		}
		else
		{
			Tv_Result	=	Tv_InPos - Tv_OutPos;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufRqInit(	struct St_GrBufRefQue* A_PtrBuf, __u32 A_BufSize, __u32 A_AlignSize )
{
		A_PtrBuf->AlgSize		=	A_AlignSize;
		A_PtrBuf->AlgMask		=	~(A_AlignSize -1);
		A_PtrBuf->Ofs				=	A_PtrBuf->AlgMask & ( sizeof(struct St_GrBufRefQue) + (A_AlignSize - 1) );
		A_PtrBuf->BufSize		=	A_BufSize & A_PtrBuf->AlgMask;
		A_PtrBuf->InPos			=	A_PtrBuf->Ofs;
		A_PtrBuf->OutPos		=	A_PtrBuf->Ofs;
		A_PtrBuf->RefPos		=	A_PtrBuf->Ofs;
}
//--------------------------------------------------------------------
void*		GrBufRqGetInPtr( struct St_GrBufRefQue* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		void*		Tv_Result;
		__u32		Tv_OutPos;		//backup out position - for synchronize
		__u32*	Tv_PtrHd;			//header pointer
		__u32		Tv_PosNext;		//next position
		BOOL8		Tv_IsOk;
	// code --------------------
		//init
		Tv_Result		=	NULL;
		Tv_OutPos		=	A_PtrBuf->OutPos;
		Tv_IsOk			=	FALSE;
		Tv_PtrHd		=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->InPos);
		Tv_PosNext	=	A_PtrBuf->AlgMask & 
			( A_PtrBuf->InPos + A_PtrBuf->AlgSize + A_Size + A_PtrBuf->AlgSize - 1 );

		*Tv_PtrHd		=	0;		//reset header

		//check type
		if ( A_PtrBuf->InPos >= Tv_OutPos )
		{
			//check limit
			if ( A_PtrBuf->Ofs == Tv_OutPos )
			{
				//out position first
				if ( Tv_PosNext < A_PtrBuf->BufSize )
				{
					//able
					Tv_IsOk		=	TRUE;
				}
			}
			else
			{
				//not out position first
				if ( Tv_PosNext <= A_PtrBuf->BufSize )
				{
					Tv_IsOk			=	TRUE;
				}
				else
				{
					//end mark
					*Tv_PtrHd	=	E_GrBufVqHdIdEnd;
					A_PtrBuf->InPos	=	A_PtrBuf->Ofs;		//reset in position
					Tv_PtrHd		=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->InPos);
					Tv_PosNext	=	A_PtrBuf->AlgMask & 
						( A_PtrBuf->InPos + A_PtrBuf->AlgSize + A_Size + A_PtrBuf->AlgSize - 1 );
				}
			}
		}

		if ( (!Tv_IsOk) && (A_PtrBuf->InPos < Tv_OutPos) )
		{
			//reverse in / out position
			if ( Tv_PosNext < Tv_OutPos )
			{
				Tv_IsOk	=	TRUE;
			}
		}

		if ( Tv_IsOk )
		{
			*Tv_PtrHd	=	E_GrBufVqHdIdNormal | (Tv_PosNext - A_PtrBuf->InPos);
			Tv_Result	=	(void*)((Def_GrCalPtr)Tv_PtrHd + (Def_GrCalPtr)A_PtrBuf->AlgSize);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufRqIn( struct St_GrBufRefQue* A_PtrBuf )
{
	// local -------------------
		__u32*	Tv_PtrHd;
		__u32		Tv_Size;
		__u32		Tv_InPos;
	// code --------------------
		//init
		Tv_PtrHd		=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->InPos);
		//check current header
		if ( E_GrBufVqHdIdNormal == (E_GrBufVqHdIdMask & (*Tv_PtrHd) ) )
		{
			//insert
			Tv_Size		=	E_GrBufVqHdSizeMask & (*Tv_PtrHd);
			Tv_InPos	=	A_PtrBuf->InPos + Tv_Size;
			if ( A_PtrBuf->BufSize <= Tv_InPos )
			{
				Tv_InPos	=	A_PtrBuf->Ofs;		//first position
			}
			A_PtrBuf->InPos	=	Tv_InPos;
		}
}
//--------------------------------------------------------------------
void*		GrBufRqGetOutPtr( struct St_GrBufRefQue* A_PtrBuf )
{
	// local -------------------
		void*		Tv_Result;
		__u32		Tv_InPos;
		__u32*	Tv_PtrHd;
		BOOL8		Tv_IsOk;
	// code --------------------
		//init
		Tv_Result			=	NULL;
		Tv_InPos			=	A_PtrBuf->InPos;		//backup in positon - for sync
		Tv_IsOk				=	FALSE;

		//release end position
		if ( Tv_InPos != A_PtrBuf->OutPos )
		{
			//check endmark
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->OutPos);
			if ( E_GrBufVqHdIdEnd == (*Tv_PtrHd) )
			{
				A_PtrBuf->OutPos	=	A_PtrBuf->Ofs;
				if ( Tv_InPos != A_PtrBuf->OutPos )
				{
					//success
					Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->OutPos);
					Tv_IsOk		=	TRUE;
				}
			}
			else
			{
				//success
				Tv_IsOk		=	TRUE;
			}
			//success
			if ( Tv_IsOk )
			{
				Tv_Result	=	(void*)((Def_GrCalPtr)Tv_PtrHd + (Def_GrCalPtr)A_PtrBuf->AlgSize);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrBufRqOut( struct St_GrBufRefQue* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_InPos;
		__u32*	Tv_PtrHd;
		__u32		Tv_Size;
		__u32		Tv_OutPos;
	// code --------------------
		//init
		Tv_Result			=	FALSE;

		//thread sync
		Tv_InPos			=	A_PtrBuf->InPos;		//backup in positon - for sync
		Tv_OutPos			=	A_PtrBuf->OutPos;
		//check empty
		if ( Tv_InPos != Tv_OutPos )
		{
			//get header
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_OutPos);
			Tv_Size	=	E_GrBufVqHdSizeMask & (*Tv_PtrHd);
			Tv_OutPos	=	Tv_OutPos + Tv_Size;
			if ( Tv_OutPos >= A_PtrBuf->BufSize )
			{
				Tv_OutPos	=	A_PtrBuf->Ofs;
			}
			Tv_Result	=	TRUE;
			//remove end block
			if ( Tv_InPos != Tv_OutPos )
			{
				Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_OutPos);
				if ( E_GrBufVqHdIdEnd == (*Tv_PtrHd) )
				{
					Tv_OutPos	=	A_PtrBuf->Ofs;
				}
			}
		}
		//update out pos
		A_PtrBuf->OutPos	=	Tv_OutPos;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufRqReset( struct St_GrBufRefQue* A_PtrBuf )
{
		A_PtrBuf->OutPos		=	A_PtrBuf->InPos;
		A_PtrBuf->RefPos		=	A_PtrBuf->OutPos;
}
//--------------------------------------------------------------------
__u32		GrBufRqGetUseSize( struct St_GrBufRefQue* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_OutPos;
		__u32	Tv_InPos;
	// code --------------------
		Tv_OutPos	=	A_PtrBuf->OutPos;
		Tv_InPos	=	A_PtrBuf->InPos;
		if ( Tv_InPos < Tv_OutPos )
		{
			Tv_Result	=	(A_PtrBuf->BufSize + Tv_InPos - Tv_OutPos) - A_PtrBuf->Ofs;
		}
		else
		{
			Tv_Result	=	Tv_InPos - Tv_OutPos;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*		GrBufRqGetRefPtr( struct St_GrBufRefQue* A_PtrBuf )
{
	// local -------------------
		void*		Tv_Result;
		__u32		Tv_InPos;
		__u32*	Tv_PtrHd;
		BOOL8		Tv_IsOk;
	// code --------------------
		//init
		Tv_Result			=	NULL;
		Tv_InPos			=	A_PtrBuf->InPos;		//backup in positon - for sync
		Tv_IsOk				=	FALSE;

		//release end position
		if ( Tv_InPos != A_PtrBuf->RefPos )
		{
			//check endmark
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->RefPos);
			if ( E_GrBufVqHdIdEnd == (*Tv_PtrHd) )
			{
				A_PtrBuf->RefPos	=	A_PtrBuf->Ofs;
				if ( Tv_InPos != A_PtrBuf->RefPos )
				{
					//success
					Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->RefPos);
					Tv_IsOk		=	TRUE;
				}
			}
			else
			{
				//success
				Tv_IsOk		=	TRUE;
			}
			//success
			if ( Tv_IsOk )
			{
				Tv_Result	=	(void*)((Def_GrCalPtr)Tv_PtrHd + (Def_GrCalPtr)A_PtrBuf->AlgSize);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufRqScanReset( struct St_GrBufRefQue* A_PtrBuf )
{
	// local -------------------
	// code --------------------
		A_PtrBuf->ScnPos	=	A_PtrBuf->OutPos;		// reset scan position
}
//--------------------------------------------------------------------
void*		GrBufRqGetScanPtr( struct St_GrBufRefQue* A_PtrBuf )
{
	// local -------------------
		void*	Tv_Result;
		__u32*	Tv_PtrHd;
		BOOL8		Tv_IsOk;
	// code --------------------
		// init
		Tv_Result			=	NULL;
		Tv_IsOk				=	FALSE;

		// check end scan positon
		if ( A_PtrBuf->ScnPos != A_PtrBuf->RefPos )
		{
			//check endmark
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->ScnPos);
			if ( E_GrBufVqHdIdEnd == (*Tv_PtrHd) )
			{
				A_PtrBuf->ScnPos	=	A_PtrBuf->Ofs;
				if ( A_PtrBuf->ScnPos != A_PtrBuf->RefPos )
				{
					//success
					Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->ScnPos);
					Tv_IsOk		=	TRUE;
				}
			}
			else
			{
				//success
				Tv_IsOk		=	TRUE;
			}
			//success
			if ( Tv_IsOk )
			{
				Tv_Result	=	(void*)((Def_GrCalPtr)Tv_PtrHd + (Def_GrCalPtr)A_PtrBuf->AlgSize);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrBufRqScanNext( struct St_GrBufRefQue* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_ScnPos;
		__u32*	Tv_PtrHd;
		__u32		Tv_Size;
		__u32		Tv_RefPos;
	// code --------------------
		//init
		Tv_Result			=	FALSE;

		//thread sync
		Tv_ScnPos			=	A_PtrBuf->ScnPos;
		Tv_RefPos			=	A_PtrBuf->RefPos;
		//check empty
		if ( Tv_ScnPos != Tv_RefPos )
		{
			//get header
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_ScnPos);
			Tv_Size	=	E_GrBufVqHdSizeMask & (*Tv_PtrHd);
			if ( 0 != Tv_Size )
			{
				Tv_ScnPos	=	Tv_ScnPos + Tv_Size;
				if ( Tv_ScnPos >= A_PtrBuf->BufSize )
				{
					Tv_ScnPos	=	A_PtrBuf->Ofs;
				}
				Tv_Result			=	TRUE;
				//remove end block
				if ( Tv_ScnPos != Tv_RefPos )
				{
					Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_ScnPos);
					if ( E_GrBufVqHdIdEnd == (*Tv_PtrHd) )
					{
						Tv_ScnPos	=	A_PtrBuf->Ofs;
					}
				}
			}
		}
		//update out pos
		A_PtrBuf->ScnPos	=	Tv_ScnPos;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrBufRqRefNext( struct St_GrBufRefQue* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_InPos;
		__u32*	Tv_PtrHd;
		__u32		Tv_Size;
		__u32		Tv_RefPos;
	// code --------------------
		//init
		Tv_Result			=	FALSE;

		//thread sync
		Tv_InPos			=	A_PtrBuf->InPos;		//backup in positon - for sync
		Tv_RefPos			=	A_PtrBuf->RefPos;
		//check empty
		if ( Tv_InPos != Tv_RefPos )
		{
			//get header
			Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_RefPos);
			Tv_Size	=	E_GrBufVqHdSizeMask & (*Tv_PtrHd);
			Tv_RefPos	=	Tv_RefPos + Tv_Size;
			if ( Tv_RefPos >= A_PtrBuf->BufSize )
			{
				Tv_RefPos	=	A_PtrBuf->Ofs;
			}
			Tv_Result			=	TRUE;
			//remove end block
			if ( Tv_InPos != Tv_RefPos )
			{
				Tv_PtrHd	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_RefPos);
				if ( E_GrBufVqHdIdEnd == (*Tv_PtrHd) )
				{
					Tv_RefPos	=	A_PtrBuf->Ofs;
				}
			}
		}
		//update out pos
		A_PtrBuf->RefPos	=	Tv_RefPos;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufRqRefReset( struct St_GrBufRefQue* A_PtrBuf )
{
		A_PtrBuf->RefPos	=	A_PtrBuf->OutPos;
}
//--------------------------------------------------------------------
void		GrBufLaInit( struct St_GrBufLnkAlcMng* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		__u32	Tv_HeadSize;
		__u32	Tv_ItmMax;
		__u32	Tv_HdBlkCnt;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnk;
		__u32	Tv_WkIdx;
		__u32	Tv_WkCnt;
		__u32*	Tv_PtrWk;
	// code --------------------
		Tv_ItmMax		=	(A_BufSize >> E_GrBufLaPageShift);
		Tv_HeadSize	=	sizeof(struct St_GrBufLnkAlcMng) + (sizeof(struct St_GrBufLnkAlcItm) * A_PtrBuf->ItmMax);
		Tv_HdBlkCnt	=	(Tv_HeadSize + E_GrBufLaPageUnit - 1) >> E_GrBufLaPageShift;
		Tv_ItmMax		=	Tv_ItmMax - Tv_HdBlkCnt;
		A_PtrBuf->ItmMax	=	(__u16)Tv_ItmMax;
		A_PtrBuf->PosBase	=	Tv_HdBlkCnt << E_GrBufLaPageShift;
		A_PtrBuf->MaxLnk	=	(__u16)( (A_BufSize - A_PtrBuf->PosBase) >> E_GrBufLaPageShift );
#ifndef LINUX_DRV
		if ( 0x10000 <= Tv_ItmMax )
		{
			DbgMsgPrint("GrBufLaInit - buffer size over.\n");
		}
		if ( 8 != (sizeof(struct St_GrBufLnkAlcItm)) )
		{
			DbgMsgPrint("GrBufLaInit - link item size mismatch = %d.\n", (sizeof(struct St_GrBufLnkAlcItm)) );
		}
#endif
		//clear table
		Tv_PtrWk	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufLnkAlcMng));
		Tv_WkCnt	=	Tv_ItmMax << 1;
		for ( Tv_WkIdx=0;Tv_WkIdx < Tv_WkCnt;Tv_WkIdx++ )
		{
			*Tv_PtrWk	=	0;
			Tv_PtrWk ++;
		}
		//register 
		Tv_PtrLnk	=	(struct St_GrBufLnkAlcItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufLnkAlcMng));
		Tv_PtrLnk->Cnt	=	A_PtrBuf->MaxLnk;
		Tv_PtrLnk->Next	=	0;		//last
		Tv_PtrLnk->Pos	=	0;		//first
		Tv_PtrLnk->Atb	=	E_GrBufLaAtbFree;		//free space
}
//--------------------------------------------------------------------
void		GrBufLaReset( struct St_GrBufLnkAlcMng* A_PtrBuf )
{
	// local -------------------
		struct St_GrBufLnkAlcItm*	Tv_PtrLnk;
	// code --------------------
		//reset
		Tv_PtrLnk	=	(struct St_GrBufLnkAlcItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufLnkAlcMng));
		Tv_PtrLnk->Cnt	=	A_PtrBuf->MaxLnk;
		Tv_PtrLnk->Next	=	0;		//last
		Tv_PtrLnk->Pos	=	0;		//first
		Tv_PtrLnk->Atb	=	E_GrBufLaAtbFree;		//free space
}
//--------------------------------------------------------------------
void*		GrBufLaAlloc( struct St_GrBufLnkAlcMng* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		void*	Tv_Result;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkBase;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkNow;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkNxt;
		__u32	Tv_ReqBlkCnt;
		__u16	Tv_WkIdx;
		__u16	Tv_DivIdx;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkDiv;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_ReqBlkCnt	=	( (A_Size + E_GrBufLaPageUnit - 1) >> E_GrBufLaPageShift );
		//get first link
		Tv_PtrLnkBase	=	(struct St_GrBufLnkAlcItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufLnkAlcMng));
		//find free link
		Tv_PtrLnkNow	=	Tv_PtrLnkBase;
		while ( TRUE )
		{
			//check free link
			if ( E_GrBufLaAtbFree == Tv_PtrLnkNow->Atb )
			{
				//collection free area
				while ( 0 != Tv_PtrLnkNow->Next )
				{
					//get next
					Tv_PtrLnkNxt	=	&(Tv_PtrLnkBase[Tv_PtrLnkNow->Next]);
					//check next is free
					if ( E_GrBufLaAtbFree != Tv_PtrLnkNxt->Atb )
					{
						break;
					}
					//merge
					Tv_PtrLnkNow->Next	=	Tv_PtrLnkNxt->Next;	//change chain
					Tv_PtrLnkNow->Cnt		=	Tv_PtrLnkNow->Cnt + Tv_PtrLnkNxt->Cnt;	//merge size
					Tv_PtrLnkNxt->Atb		=	E_GrBufLaAtbNone;		//remove 
				}
				//check size
				if ( Tv_PtrLnkNow->Cnt >= (__u16)Tv_ReqBlkCnt )
				{
					//found
					if ( Tv_PtrLnkNow->Cnt > (__u16)Tv_ReqBlkCnt )
					{
						//find empty slot
						Tv_PtrLnkDiv	=	NULL;
						for ( Tv_WkIdx=0;Tv_WkIdx < A_PtrBuf->ItmMax;Tv_WkIdx++ )
						{
							if ( E_GrBufLaAtbNone == Tv_PtrLnkBase[Tv_WkIdx].Atb )
							{
								//found
								Tv_PtrLnkDiv	=	&(Tv_PtrLnkBase[Tv_WkIdx]);
								Tv_DivIdx			=	Tv_WkIdx;
								break;
							}
						}
						if ( NULL == Tv_PtrLnkDiv )
						{
#ifndef LINUX_DRV
							DbgMsgPrint("GrBufLaAlloc - not enough link.\n");
#endif
							break;
						}
						//do divide
						Tv_PtrLnkDiv->Next	=	Tv_PtrLnkNow->Next;
						Tv_PtrLnkDiv->Cnt		=	Tv_PtrLnkNow->Cnt - (__u16)Tv_ReqBlkCnt;
						Tv_PtrLnkDiv->Pos		=	Tv_PtrLnkNow->Pos + (__u16)Tv_ReqBlkCnt;
						Tv_PtrLnkDiv->Atb		=	E_GrBufLaAtbFree;

						Tv_PtrLnkNow->Cnt		=	(__u16)Tv_ReqBlkCnt;
						Tv_PtrLnkNow->Next	=	Tv_DivIdx;							
					}
					//change used
					Tv_PtrLnkNow->Atb	=	E_GrBufLaAtbUsed;
					Tv_Result	=	(void*)((Def_GrCalPtr)A_PtrBuf + (((Def_GrCalPtr)Tv_PtrLnkNow->Pos) << E_GrBufLaPageShift) + (Def_GrCalPtr)A_PtrBuf->PosBase);
					break;
				}
			}
			//check next able
			if ( 0 == Tv_PtrLnkNow->Next )
			{
				//finish
				break;
			}
			//get next
			Tv_PtrLnkNow	=	&(Tv_PtrLnkBase[Tv_PtrLnkNow->Next]);
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufLaFree( struct St_GrBufLnkAlcMng* A_PtrBuf, void* A_PtrAloc )
{
	// local -------------------
		__u32	Tv_Pos;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkBase;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkNow;
	// code --------------------
#ifndef LINUX_DRV
		if(((Def_GrCalPtr)A_PtrBuf + A_PtrBuf->PosBase) > (Def_GrCalPtr)A_PtrAloc)
		{
			DbgMsgPrint("GrBufLaFree - bad allocate address.\n");
			return;
		}
#endif
		Tv_Pos	=	(__u32)((Def_GrCalPtr)A_PtrAloc - (Def_GrCalPtr)A_PtrBuf) - A_PtrBuf->PosBase;
		Tv_Pos	=	Tv_Pos >> E_GrBufLaPageShift;
		//find
		Tv_PtrLnkBase	=	(struct St_GrBufLnkAlcItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufLnkAlcMng));
		//find free link
		Tv_PtrLnkNow	=	Tv_PtrLnkBase;
		while ( TRUE )
		{
			//check position
			if ( Tv_Pos == Tv_PtrLnkNow->Pos )
			{
#ifndef LINUX_DRV
				if ( E_GrBufLaAtbUsed != Tv_PtrLnkNow->Atb )
				{
					DbgMsgPrint("GrBufLaFree - bad link type.\n");
				}
#endif
				//found - release
				Tv_PtrLnkNow->Atb	=	E_GrBufLaAtbFree;
				break;
			}
			//check next able
			if ( 0 == Tv_PtrLnkNow->Next )
			{
#ifndef LINUX_DRV
				DbgMsgPrint("GrBufLaFree - link not found.\n");
#endif
				//finish
				break;
			}
			//get next
			Tv_PtrLnkNow	=	&(Tv_PtrLnkBase[Tv_PtrLnkNow->Next]);
		}
}
//--------------------------------------------------------------------
BOOL8		GrBufLaIsExist( struct St_GrBufLnkAlcMng* A_PtrBuf, void* A_PtrAloc )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Pos;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkBase;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkNow;
	// code --------------------
		Tv_Result	=	FALSE;
#ifndef LINUX_DRV
		if(((Def_GrCalPtr)A_PtrBuf + A_PtrBuf->PosBase) > (Def_GrCalPtr)A_PtrAloc)
		{
			DbgMsgPrint("GrBufLaFree - bad allocate address.\n");
			return	FALSE;
		}
#endif
		Tv_Pos	=	(__u32)((Def_GrCalPtr)A_PtrAloc - (Def_GrCalPtr)A_PtrBuf) - A_PtrBuf->PosBase;
		Tv_Pos	=	Tv_Pos >> E_GrBufLaPageShift;
		//find
		Tv_PtrLnkBase	=	(struct St_GrBufLnkAlcItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufLnkAlcMng));
		//find free link
		Tv_PtrLnkNow	=	Tv_PtrLnkBase;
		while ( TRUE )
		{
			//check position
			if ( Tv_Pos == Tv_PtrLnkNow->Pos )
			{
#ifndef LINUX_DRV
				if ( E_GrBufLaAtbUsed != Tv_PtrLnkNow->Atb )
				{
					DbgMsgPrint("GrBufLaFree - bad link type.\n");
				}
#endif
				//found
				Tv_Result	=	TRUE;
				break;
			}
			//check next able
			if ( 0 == Tv_PtrLnkNow->Next )
			{
				//finish
				break;
			}
			//get next
			Tv_PtrLnkNow	=	&(Tv_PtrLnkBase[Tv_PtrLnkNow->Next]);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufLaInfo( struct St_GrBufLnkAlcMng* A_PtrBuf, __u32* A_PtrRtLnkCnt, __u32* A_PtrRtFree, 
										__u32* A_PtrRtUsed )
{
	// local -------------------
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkBase;
		struct St_GrBufLnkAlcItm*	Tv_PtrLnkNow;
		__u32	Tv_LnkCnt;
		__u32	Tv_Free;
		__u32	Tv_Used;
	// code --------------------
		Tv_LnkCnt		=	0;
		Tv_Free			=	0;
		Tv_Used			=	0;

		//scan
		Tv_PtrLnkBase	=	(struct St_GrBufLnkAlcItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufLnkAlcMng));
		//find free link
		Tv_PtrLnkNow	=	Tv_PtrLnkBase;
		while ( TRUE )
		{
			//check type
			Tv_LnkCnt ++;
			switch( Tv_PtrLnkNow->Atb )
			{
				case	E_GrBufLaAtbFree:
					Tv_Free	=	Tv_Free + (__u32)Tv_PtrLnkNow->Cnt;
					break;
				case	E_GrBufLaAtbUsed:
					Tv_Used	=	Tv_Used + (__u32)Tv_PtrLnkNow->Cnt;
					break;
			}

			//check next able
			if ( 0 == Tv_PtrLnkNow->Next )
			{
				//finish
				break;
			}
			//get next
			Tv_PtrLnkNow	=	&(Tv_PtrLnkBase[Tv_PtrLnkNow->Next]);
		}

		//update result
		*A_PtrRtLnkCnt	=	Tv_LnkCnt;
		*A_PtrRtFree		=	Tv_Free;
		*A_PtrRtUsed		=	Tv_Used;
}
//--------------------------------------------------------------------
void		GrBufMaInit( struct St_GrBufMaMng* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		__u32*	Tv_PtrMtbl;
		__u32	Tv_WkIdx;
		__u32	Tv_BufSize;
		__u32	Tv_HdSize;
		__u32	Tv_HdPgCnt;
	// code --------------------
		//calculate buffer
		Tv_BufSize	=	(A_BufSize & E_GrBufMaPageMask) - E_GrBufMaPageSize; 
		A_PtrBuf->PageMax	=	Tv_BufSize >> E_GrBufMaPageShft;
		Tv_HdSize		=	(sizeof(struct St_GrBufMaMng) + ( A_PtrBuf->PageMax * sizeof(struct St_GrBufMaItm) ) + E_GrBufMaPageSize - 1) 
			& E_GrBufMaPageMask;
		Tv_HdPgCnt	=	Tv_HdSize >> E_GrBufMaPageShft;
		A_PtrBuf->PageMax	=	A_PtrBuf->PageMax - Tv_HdPgCnt + 1;
		A_PtrBuf->BufPos	=	Tv_HdSize;

		//init map
		Tv_PtrMtbl	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufMaMng));
		for ( Tv_WkIdx=0;Tv_WkIdx < A_PtrBuf->PageMax;Tv_WkIdx++ )
		{
			*Tv_PtrMtbl	=	0;
			Tv_PtrMtbl ++;
		}
		
}
//--------------------------------------------------------------------
void*		GrBufMaAlloc( struct St_GrBufMaMng* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		void*	Tv_Result;
		struct	St_GrBufMaItm*	Tv_PtrMtbl;
		__u32	Tv_ReqPgCnt;
		__u32	Tv_PgIdx;
		__u32	Tv_FindPgId;
		__u32	Tv_FindPgCnt;
		BOOL8	Tv_Isfound;
	// code --------------------
		Tv_Result	=	NULL;
		//calculate page count
		Tv_ReqPgCnt	=	(A_Size + E_GrBufMaPageSize - 1) >> E_GrBufMaPageShft;
		//check size
		if ( 0 != Tv_ReqPgCnt )
		{
			//get map table pointer
			Tv_PtrMtbl	=	(struct	St_GrBufMaItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufMaMng));
			//find allocate able area
			Tv_PgIdx	=	0;
			Tv_FindPgCnt	=	0;
			Tv_FindPgId		=	0;
			Tv_Isfound		=	FALSE;
			while ( A_PtrBuf->PageMax > Tv_PgIdx )
			{
				//find empty
				if ( (0 == Tv_PtrMtbl[Tv_PgIdx].LockCnt) && (0 == Tv_PtrMtbl[Tv_PgIdx].Id) )
				{
					//check continuous
					Tv_FindPgCnt ++;
					if ( 0 == Tv_FindPgId )
					{
						Tv_FindPgId	=	Tv_PgIdx + 1;
					}
					//check found
					if ( Tv_ReqPgCnt <= Tv_FindPgCnt )
					{
						//found
						Tv_Isfound	=	TRUE;
						break;
					}
				}
				else
				{
					//reset
					Tv_FindPgId		=	0;
					Tv_FindPgCnt	=	0;
				}
				//next
				Tv_PgIdx ++;
			}

			//check found
			if ( Tv_Isfound )
			{
				//allocate memory
				Tv_FindPgCnt	=	0;
				Tv_PgIdx			=	Tv_FindPgId - 1;
				Tv_PtrMtbl[Tv_PgIdx].LockCnt	=	1;		//mark lock count , first page
				while (Tv_FindPgCnt < Tv_ReqPgCnt)
				{
					Tv_PtrMtbl[Tv_PgIdx].Id				=	(__u16)Tv_FindPgId;
					//next
					Tv_FindPgCnt ++;
					Tv_PgIdx ++;
				}

				//calculate address
				Tv_Result	=	(void*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)A_PtrBuf->BufPos + (((Def_GrCalPtr)Tv_FindPgId - 1) << E_GrBufMaPageShft));
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufMaLock( struct St_GrBufMaMng* A_PtrBuf, void* A_PtrAloc )
{
	// local -------------------
		struct	St_GrBufMaItm*	Tv_PtrMtbl;
		__u32		Tv_PgIdx;
	// code --------------------
		//check able block address
		if((Def_GrCalPtr)A_PtrAloc > (Def_GrCalPtr)A_PtrBuf)
		{
			//get block index
			Tv_PgIdx		=	(__u32)(((Def_GrCalPtr)A_PtrAloc - (Def_GrCalPtr)A_PtrBuf - (Def_GrCalPtr)A_PtrBuf->BufPos) >> E_GrBufMaPageShft);
			if ( Tv_PgIdx < A_PtrBuf->PageMax )
			{
				//get map table pointer
				Tv_PtrMtbl	=	(struct	St_GrBufMaItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufMaMng));
				//check id
				if ( (Tv_PgIdx + 1) == Tv_PtrMtbl[Tv_PgIdx].Id) 
				{
					if ( 0 != Tv_PtrMtbl[Tv_PgIdx].LockCnt )
					{
						// increase lock count
						Tv_PtrMtbl[Tv_PgIdx].LockCnt ++;
					}
#ifndef LINUX_DRV
					else
					{
						DbgMsgPrint( "GrBufMaLock - page already free. \n" );
					}
#endif
				}
#ifndef LINUX_DRV
				else
				{
					DbgMsgPrint( "GrBufMaLock - page is not master. \n" );
				}
#endif
			}
#ifndef LINUX_DRV
			else
			{
				DbgMsgPrint( "GrBufMaLock - allocate address over. \n" );
			}
#endif
		}
#ifndef LINUX_DRV
		else
		{
			DbgMsgPrint( "GrBufMaLock - bad allocate address. \n" );
		}
#endif
}
//--------------------------------------------------------------------
void		GrBufMaFree( struct St_GrBufMaMng* A_PtrBuf, void* A_PtrAloc )
{
	// local -------------------
		struct	St_GrBufMaItm*	Tv_PtrMtbl;
		__u32		Tv_PgIdx;
		__u32		Tv_AlocId;
	// code --------------------
		//check able block address
		if((Def_GrCalPtr)A_PtrAloc > (Def_GrCalPtr)A_PtrBuf)
		{
			//get block index
			Tv_PgIdx		=	(__u32)(((Def_GrCalPtr)A_PtrAloc - (Def_GrCalPtr)A_PtrBuf - A_PtrBuf->BufPos) >> E_GrBufMaPageShft);
			if ( Tv_PgIdx < A_PtrBuf->PageMax )
			{
				//get map table pointer
				Tv_PtrMtbl	=	(struct	St_GrBufMaItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufMaMng));
				//check id
				if ( (Tv_PgIdx + 1) == Tv_PtrMtbl[Tv_PgIdx].Id) 
				{
					if ( 0 != Tv_PtrMtbl[Tv_PgIdx].LockCnt )
					{
						//decrease lock counter
						Tv_PtrMtbl[Tv_PgIdx].LockCnt --;
						// check need free
						if ( 0 == Tv_PtrMtbl[Tv_PgIdx].LockCnt )
						{
							//try free
							Tv_AlocId	=	Tv_PgIdx + 1;
							while ( Tv_PgIdx < A_PtrBuf->PageMax )
							{
								if ( Tv_AlocId != Tv_PtrMtbl[Tv_PgIdx].Id )
								{
									break;
								}
								// reset
								Tv_PtrMtbl[Tv_PgIdx].LockCnt	=	0;
								Tv_PtrMtbl[Tv_PgIdx].Id				=	0;
								//next
								Tv_PgIdx ++;
							}
						}
					}
#ifndef LINUX_DRV
					else
					{
						DbgMsgPrint( "GrBufMaFree - page already free. \n" );
					}
#endif
				}
#ifndef LINUX_DRV
				else
				{
					DbgMsgPrint( "GrBufMaFree - page is not master. \n" );
				}
#endif
			}
#ifndef LINUX_DRV
			else
			{
				DbgMsgPrint( "GrBufMaFree - allocate address over. \n" );
			}
#endif
		}
#ifndef LINUX_DRV
		else
		{
			DbgMsgPrint( "GrBufMaFree - bad allocate address. \n" );
		}
#endif
}
//--------------------------------------------------------------------
void		GrBufMaRemove( struct St_GrBufMaMng* A_PtrBuf, void* A_PtrAloc )
{
	// local -------------------
		struct	St_GrBufMaItm*	Tv_PtrMtbl;
		__u32		Tv_PgIdx;
		__u32		Tv_AlocId;
	// code --------------------
		//check able block address
		if((Def_GrCalPtr)A_PtrAloc > (Def_GrCalPtr)A_PtrBuf)
		{
			//get block index
			Tv_PgIdx		=	(__u32)(((Def_GrCalPtr)A_PtrAloc - (Def_GrCalPtr)A_PtrBuf - A_PtrBuf->BufPos) >> E_GrBufMaPageShft);
			if ( Tv_PgIdx < A_PtrBuf->PageMax )
			{
				//get map table pointer
				Tv_PtrMtbl	=	(struct	St_GrBufMaItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufMaMng));
				//check id
				if ( (Tv_PgIdx + 1) == Tv_PtrMtbl[Tv_PgIdx].Id) 
				{
					if ( 0 != Tv_PtrMtbl[Tv_PgIdx].LockCnt )
					{
						//try free
						Tv_AlocId	=	Tv_PgIdx + 1;
						while ( Tv_PgIdx < A_PtrBuf->PageMax )
						{
							if ( Tv_AlocId != Tv_PtrMtbl[Tv_PgIdx].Id )
							{
								break;
							}
							// reset
							Tv_PtrMtbl[Tv_PgIdx].LockCnt	=	0;
							Tv_PtrMtbl[Tv_PgIdx].Id				=	0;
							//next
							Tv_PgIdx ++;
						}
					}
#ifndef LINUX_DRV
					else
					{
						DbgMsgPrint( "GrBufMaRemove - page already free. \n" );
					}
#endif
				}
#ifndef LINUX_DRV
				else
				{
					DbgMsgPrint( "GrBufMaRemove - page is not master. \n" );
				}
#endif
			}
#ifndef LINUX_DRV
			else
			{
				DbgMsgPrint( "GrBufMaRemove - allocate address over. \n" );
			}
#endif
		}
#ifndef LINUX_DRV
		else
		{
			DbgMsgPrint( "GrBufMaRemove - bad allocate address. \n" );
		}
#endif

}
//--------------------------------------------------------------------
BOOL8		GrBufMaInfo( struct St_GrBufMaMng* A_PtrBuf, __u32* A_PtrUsed, __u32* A_PtrFree )
{
	// local -------------------
		BOOL8		Tv_Result;
		struct	St_GrBufMaItm*	Tv_PtrMtbl;
		__u32		Tv_PgIdx;
		__u32		Tv_AlocCnt;
		__u32		Tv_FreeCnt;
		__u32		Tv_PrntPg;
	// code --------------------
		Tv_Result	=	TRUE;

		//get map table pointer
		Tv_PtrMtbl	=	(struct	St_GrBufMaItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufMaMng));
		//calculate
		Tv_AlocCnt	=	0;
		Tv_FreeCnt	=	0;
		for ( Tv_PgIdx=0;Tv_PgIdx < A_PtrBuf->PageMax;Tv_PgIdx++ )
		{
			if ( 0 != Tv_PtrMtbl[Tv_PgIdx].Id )
			{
				//check master page
				if ( (Tv_PgIdx + 1) == Tv_PtrMtbl[Tv_PgIdx].Id )
				{
					//check lock count
					if ( 0 != Tv_PtrMtbl[Tv_PgIdx].LockCnt )
					{
						Tv_AlocCnt ++;
					}
					else
					{
						//bad page
						Tv_Result	=	FALSE;
#ifndef LINUX_DRV
						DbgMsgPrint( "GrBufMaInfo - master page lock count zero. Page = %d, Master = %d, Lock = %d \n", Tv_PgIdx, Tv_PtrMtbl[Tv_PgIdx].Id ,Tv_PtrMtbl[Tv_PgIdx].LockCnt );
#endif
					}
				}
				else
				{
					//check parent page
					Tv_PrntPg	=	(__u32)Tv_PtrMtbl[Tv_PgIdx].Id - 1;
					if ( (Tv_PrntPg < Tv_PgIdx) && (Tv_PtrMtbl[Tv_PgIdx].Id == Tv_PtrMtbl[Tv_PrntPg].Id) && (0 != Tv_PtrMtbl[Tv_PrntPg].LockCnt) )
					{
						//allocate page
						Tv_AlocCnt ++;
					}
					else
					{
						//bad parent page
						Tv_Result	=	FALSE;
#ifndef LINUX_DRV
						DbgMsgPrint( "GrBufMaInfo - bad parent page. Page = %d, Master = %d, Lock = %d \n", Tv_PgIdx, Tv_PtrMtbl[Tv_PgIdx].Id ,Tv_PtrMtbl[Tv_PgIdx].LockCnt );
#endif
					}
				}
			}
			else
			{
				if ( 0 != Tv_PtrMtbl[Tv_PgIdx].LockCnt )
				{
					//bad page 
					Tv_Result	=	FALSE;
#ifndef LINUX_DRV
					DbgMsgPrint( "GrBufMaInfo - free page lock counter exist!. Page = %d, Master = %d, Lock = %d \n", Tv_PgIdx, Tv_PtrMtbl[Tv_PgIdx].Id ,Tv_PtrMtbl[Tv_PgIdx].LockCnt );
#endif
				}
				else
				{
					//free page increase
					Tv_FreeCnt ++;
				}
			}
		}
		
		//update result
		*A_PtrUsed	=	Tv_AlocCnt;
		*A_PtrFree	=	Tv_FreeCnt;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufMaReset( struct St_GrBufMaMng* A_PtrBuf )
{
	// local -------------------
		__u32*	Tv_PtrMtbl;
		__u32	Tv_WkIdx;
	// code --------------------

		//init map
		Tv_PtrMtbl	=	(__u32*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufMaMng));
		for ( Tv_WkIdx=0;Tv_WkIdx < A_PtrBuf->PageMax;Tv_WkIdx++ )
		{
			*Tv_PtrMtbl	=	0;
			Tv_PtrMtbl ++;
		}

}
//--------------------------------------------------------------------
void		GrBufMaCorrect( struct St_GrBufMaMng* A_PtrBuf )
{
	// local -------------------
		struct	St_GrBufMaItm*	Tv_PtrMtbl;
		__u32		Tv_PgIdx;
		__u32		Tv_PrntPg;
	// code --------------------
		//get map table pointer
		Tv_PtrMtbl	=	(struct	St_GrBufMaItm*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufMaMng));
		//calculate
		for ( Tv_PgIdx=0;Tv_PgIdx < A_PtrBuf->PageMax;Tv_PgIdx++ )
		{
			if ( 0 != Tv_PtrMtbl[Tv_PgIdx].Id )
			{
				//check master page
				if ( (Tv_PgIdx + 1) == Tv_PtrMtbl[Tv_PgIdx].Id )
				{
					//check lock count
					if ( 0 == Tv_PtrMtbl[Tv_PgIdx].LockCnt )
					{
						//bad page
#ifndef LINUX_DRV
						DbgMsgPrint( "GrBufMaCorrect - master page lock count zero. Page = %d, Master = %d, Lock = %d \n", Tv_PgIdx, Tv_PtrMtbl[Tv_PgIdx].Id ,Tv_PtrMtbl[Tv_PgIdx].LockCnt );
#endif
						Tv_PtrMtbl[Tv_PgIdx].Id				=	0;
						Tv_PtrMtbl[Tv_PgIdx].LockCnt	=	0;
					}
				}
				else
				{
					//check parent page
					Tv_PrntPg	=	(__u32)Tv_PtrMtbl[Tv_PgIdx].Id - 1;
					if ( (Tv_PrntPg >= Tv_PgIdx) || (Tv_PtrMtbl[Tv_PgIdx].Id != Tv_PtrMtbl[Tv_PrntPg].Id) || (0 == Tv_PtrMtbl[Tv_PrntPg].LockCnt) )
					{
						//bad parent page
#ifndef LINUX_DRV
						DbgMsgPrint( "GrBufMaCorrect - bad parent page. Page = %d, Master = %d, Lock = %d \n", Tv_PgIdx, Tv_PtrMtbl[Tv_PgIdx].Id ,Tv_PtrMtbl[Tv_PgIdx].LockCnt );
#endif
						Tv_PtrMtbl[Tv_PgIdx].Id				=	0;
						Tv_PtrMtbl[Tv_PgIdx].LockCnt	=	0;
					}
				}
			}
			else
			{
				if ( 0 != Tv_PtrMtbl[Tv_PgIdx].LockCnt )
				{
					//bad page 
#ifndef LINUX_DRV
					DbgMsgPrint( "GrBufMaCorrect - free page lock counter exist!. Page = %d, Master = %d, Lock = %d \n", Tv_PgIdx, Tv_PtrMtbl[Tv_PgIdx].Id ,Tv_PtrMtbl[Tv_PgIdx].LockCnt );
#endif
					Tv_PtrMtbl[Tv_PgIdx].Id				=	0;
					Tv_PtrMtbl[Tv_PgIdx].LockCnt	=	0;
				}
			}
		}

}
//--------------------------------------------------------------------
void		GrBufCsInit( struct St_GrBufCsMng* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_BufSize;
		__u32		Tv_BlkSize;
		__u32		Tv_BlkShft;
		__u32		Tv_ChkSize;
		__u32		Tv_BlkCnt;
	// code --------------------
		// item clear
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrBufCsBlkMaxCnt;Tv_WkIdx++ )
		{
			A_PtrBuf->Items[Tv_WkIdx]	=	0;
		}
#ifndef LINUX_DRV
		if ( 0x8000 != sizeof(struct St_GrBufCsMng) )
		{
			DbgMsgPrint( "GrBufCsInit - bad management struct size 0x%X \n", sizeof(struct St_GrBufCsMng) );
		}
#endif
		// calculate block size
		Tv_BufSize	=	(A_BufSize - sizeof(struct St_GrBufCsMng) );
		Tv_BlkSize	=	Tv_BufSize / E_GrBufCsBlkMaxCnt;
		if ( E_GrBufCsBlkMaxSize <= Tv_BlkSize )
		{
			Tv_BlkShft	=	E_GrBufCsBlkMaxShft;
		}
		else if ( E_GrBufCsBlkMinSize >= Tv_BlkSize )
		{
			Tv_BlkShft	=	E_GrBufCsBlkMinShft;
		}
		else
		{
			Tv_ChkSize	=	E_GrBufCsBlkMinSize;
			Tv_BlkShft	=	E_GrBufCsBlkMinShft;
			while ( Tv_ChkSize < Tv_BlkSize )
			{
				if ( E_GrBufCsBlkMaxSize == Tv_ChkSize )
				{
					break;
				}
				// next
				Tv_ChkSize	=	Tv_ChkSize << 1;
				Tv_BlkShft ++;
			}
		}
		// calculate block count
		Tv_BlkSize	=	1 << Tv_BlkShft;
		Tv_BlkCnt	=	Tv_BufSize / Tv_BlkSize;
		if ( E_GrBufCsBlkMaxCnt < Tv_BlkCnt )
		{
			Tv_BlkCnt	=	E_GrBufCsBlkMaxCnt;
		}
		// update header
		A_PtrBuf->BlkShft	=	(__u16)Tv_BlkShft;
		A_PtrBuf->BlkCnt	=	(__u16)Tv_BlkCnt;
		
}
//--------------------------------------------------------------------
void*		GrBufCsAlloc( struct St_GrBufCsMng* A_PtrBuf, __u32 A_Size, __u16 A_CliMap )
{
	// local -------------------
		void*	Tv_Result;
		__u16	Tv_ReqCnt;
		__u16	Tv_ChkIdx;
		__u32	Tv_BlkSize;
		__u16	Tv_FindIdx;
		__u16	Tv_FindCnt;
		//struct St_GrBufCsItem	Tv_Item;
		__u32	Tv_Val;
	// code --------------------
		// init
		Tv_Result	=	NULL;

		if ( 0 != A_Size )
		{
			// calculate request block count
			Tv_BlkSize	=	1 << (__u32)A_PtrBuf->BlkShft;
			Tv_ReqCnt		=	(__u16)(A_Size >> (__u32)A_PtrBuf->BlkShft);
			if ( 0 != (A_Size & (Tv_BlkSize - 1) ) )
			{
				Tv_ReqCnt ++;
			}
			// find
			Tv_FindIdx	=	0;
			Tv_FindCnt	=	0;
			Tv_ChkIdx		=	0;
			while ( (Tv_ChkIdx + Tv_ReqCnt) <= A_PtrBuf->BlkCnt )
			{
				// get item
				//*((__u32*)&Tv_Item)	=	A_PtrBuf->Items[Tv_ChkIdx];
				Tv_Val	=	A_PtrBuf->Items[Tv_ChkIdx];
				// check zero map
				//if ( 0 == Tv_Item.Map )
				if ( 0 == (0x0000FFFF & Tv_Val) )
				{
					if ( 0 == Tv_FindCnt )
					{
						Tv_FindIdx	=	Tv_ChkIdx;
					}
					Tv_FindCnt ++;
					// check enough count
					if ( Tv_ReqCnt <= Tv_FindCnt )
					{
						// found
						break;
					}
					// next
					Tv_ChkIdx ++;
				}
				else
				{
					Tv_FindCnt	=	0;
					//Tv_ChkIdx		=	Tv_ChkIdx + Tv_Item.Cnt;
					Tv_ChkIdx	=	Tv_ChkIdx + (Tv_Val >> 16);
				}
			}	//	while ( (Tv_ChkIdx + Tv_ReqCnt) <= A_PtrBuf->BlkCnt )

			if ( Tv_FindCnt >= Tv_ReqCnt )
			{
				/*
				Tv_Item.Map	=	A_CliMap;
				Tv_Item.Cnt	=	Tv_ReqCnt;
				A_PtrBuf->Items[Tv_FindIdx]	=	*((__u32*)&Tv_Item);		// update
				*/
				Tv_Val		=	((__u32)A_CliMap) | ((__u32)Tv_ReqCnt << 16);
				A_PtrBuf->Items[Tv_FindIdx]	=	Tv_Val;

				Tv_Result	=	(void*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufCsMng) + ((Def_GrCalPtr)Tv_FindIdx << (Def_GrCalPtr)A_PtrBuf->BlkShft));
				//DbgMsgPrint( "nm:size=%d,req=%d,fi=%d.\n",A_Size,Tv_ReqCnt,Tv_FindIdx );
			}
#ifndef LINUX_DRV
			else
			{
				DbgMsgPrint( "GrBufCsAlloc - not enough buffer.\n" );
			}
#endif
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufCsFree( struct St_GrBufCsMng* A_PtrBuf, void* A_PtrAloc, __u16 A_CliIdx )
{
	// local -------------------
		__u32	Tv_ItmIdx;
		//__u16	Tv_Mask;
		//struct St_GrBufCsItem	Tv_Item;
		__u32	Tv_CliMsk;
		__u32	Tv_Val;
	// code --------------------
		// calculate index
		Tv_ItmIdx	=	(__u32)(((Def_GrCalPtr)A_PtrAloc - ((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufCsMng))) >> (Def_GrCalPtr)A_PtrBuf->BlkShft);
		if ( Tv_ItmIdx < (__u32)A_PtrBuf->BlkCnt )
		{
			// get item
			Tv_Val	=		A_PtrBuf->Items[Tv_ItmIdx];
			//*((__u32*)&Tv_Item)	=	A_PtrBuf->Items[Tv_ItmIdx];
			// check exist
			//Tv_Mask		=	1 << A_CliIdx;
			Tv_CliMsk		=	1 << A_CliIdx;
			//if ( (0 != ( Tv_Mask & Tv_Item.Map )) && ( 0 != Tv_Item.Cnt ) )
			if ( (0 != ( Tv_CliMsk & Tv_Val )) && ( 0 != (0xFFFF0000 & Tv_Val) ) )
			{
				//Tv_Item.Map	=	Tv_Item.Map & (~Tv_Mask);
				Tv_Val	=	Tv_Val & (~Tv_CliMsk);
				//if ( 0 == Tv_Item.Map )
				if ( 0 == (0x0000FFFF & Tv_Val) )
				{
					//Tv_Item.Cnt	=	0;		// all release
					Tv_Val	=	0;
				}
				// update data
				//A_PtrBuf->Items[Tv_ItmIdx]	=	*((__u32*)&Tv_Item);
				A_PtrBuf->Items[Tv_ItmIdx]	=	Tv_Val;
			}
#ifndef LINUX_DRV
			else
			{
				DbgMsgPrint( "GrBufCsFree - item already released.\n" );
			}
#endif
		}
#ifndef LINUX_DRV
		else
		{
			DbgMsgPrint( "GrBufCsFree - out of buffer.\n" );
		}
#endif

}
//--------------------------------------------------------------------
void		GrBufCsClientFree( struct St_GrBufCsMng* A_PtrBuf, __u16 A_CliIdx )
{
	// local -------------------
		__u16	Tv_ItmIdx;
		__u16	Tv_Mask;
		__u16	Tv_JmpCnt;
		struct St_GrBufCsItem	Tv_Item;
	// code --------------------
		Tv_Mask		=	1 << A_CliIdx;
		Tv_ItmIdx	=	0;
		while ( Tv_ItmIdx < A_PtrBuf->BlkCnt )
		{
			// get item
			*((__u32*)&Tv_Item)	=	A_PtrBuf->Items[Tv_ItmIdx];
			// check exist item
			if ( 0 != Tv_Item.Map )
			{
				Tv_JmpCnt	=	Tv_Item.Cnt;
				// check exist client
				if ( 0 != (Tv_Mask & Tv_Item.Map) )
				{
					// release item
					Tv_Item.Map	=	Tv_Item.Map & (~Tv_Mask);
					if ( 0 == Tv_Item.Map )
					{
						// release all
						Tv_Item.Cnt	=	0;
					}
					// update
					A_PtrBuf->Items[Tv_ItmIdx]	=	*((__u32*)&Tv_Item);
				}
				// go next
				Tv_ItmIdx	=	Tv_ItmIdx + Tv_JmpCnt;
			}
			else
			{
				// empty block
				Tv_ItmIdx ++;
			}
		}
}
//--------------------------------------------------------------------
void		GrBufTsInit( struct St_GrBufTsMng* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_BufSize;
		__u32		Tv_BlkSize;
		__u32		Tv_BlkShft;
		__u32		Tv_ChkSize;
		__u32		Tv_BlkCnt;
	// code --------------------
		// init
		A_PtrBuf->BlkAlc		=	0;
		// item clear
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrBufTsBlkMaxCnt;Tv_WkIdx++ )
		{
			A_PtrBuf->Items[Tv_WkIdx].BlkCnt		=	0;
			A_PtrBuf->Items[Tv_WkIdx].MapAloc		=	0;
			A_PtrBuf->Items[Tv_WkIdx].MapRel[0]	=	0;
			A_PtrBuf->Items[Tv_WkIdx].MapRel[1]	=	0;
		}
#ifndef LINUX_DRV
		if ( 0x40000 != sizeof(struct St_GrBufTsMng) )
		{
			DbgMsgPrint( "GrBufTsInit - bad management struct size 0x%X \n", sizeof(struct St_GrBufTsMng) );
		}
#endif
		// calculate block size
		Tv_BufSize	=	(A_BufSize - sizeof(struct St_GrBufTsMng) );
		Tv_BlkSize	=	Tv_BufSize / E_GrBufTsBlkMaxCnt;
		if ( E_GrBufTsBlkMaxSize <= Tv_BlkSize )
		{
			Tv_BlkShft	=	E_GrBufTsBlkMaxSize;
		}
		else if ( E_GrBufTsBlkMinSize >= Tv_BlkSize )
		{
			Tv_BlkShft	=	E_GrBufTsBlkMinSize;
		}
		else
		{
			Tv_ChkSize	=	E_GrBufTsBlkMinSize;
			Tv_BlkShft	=	E_GrBufTsBlkMinShft;
			while ( Tv_ChkSize < Tv_BlkSize )
			{
				if ( E_GrBufTsBlkMaxSize == Tv_ChkSize )
				{
					break;
				}
				// next
				Tv_ChkSize	=	Tv_ChkSize << 1;
				Tv_BlkShft ++;
			}
		}
		// calculate block count
		Tv_BlkSize	=	1 << Tv_BlkShft;
		Tv_BlkCnt	=	Tv_BufSize / Tv_BlkSize;
		if ( E_GrBufTsBlkMaxCnt < Tv_BlkCnt )
		{
			Tv_BlkCnt	=	E_GrBufTsBlkMaxCnt;
		}
		// update header
		A_PtrBuf->BlkShft	=	Tv_BlkShft;
		A_PtrBuf->BlkCnt	=	Tv_BlkCnt;
}
//--------------------------------------------------------------------
void*		GrBufTsAlloc(struct St_GrBufTsMng* A_PtrBuf, __u32 A_Size, __u8 A_OwnerMap)
{
	// local -------------------
		void*	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_Mask;
		__u32	Tv_ReqCnt;
		__u32	Tv_FreCnt;
		__u32	Tv_FreIdx;
		__u8	Tv_RelMap;
		__u8	Tv_ChkMap;
	// code --------------------
		Tv_Result	=	NULL;

		if ( (0 != A_Size) && (0 != A_OwnerMap) )
		{
			Tv_WkIdx	=	0;
			Tv_FreCnt	=	0;
			Tv_FreIdx	=	0;
			Tv_ReqCnt	=	A_Size >> A_PtrBuf->BlkShft;
			Tv_Mask		=	(1 << A_PtrBuf->BlkShft) - 1;
			if ( 0 != ( Tv_Mask & A_Size ) )
			{
				Tv_ReqCnt ++;
			}
			while ( Tv_WkIdx < A_PtrBuf->BlkCnt )
			{
				// check exist block
				if ( 0 != A_PtrBuf->Items[Tv_WkIdx].BlkCnt )
				{
					// check can release
					Tv_RelMap	=	~(A_PtrBuf->Items[Tv_WkIdx].MapRel[0] | A_PtrBuf->Items[Tv_WkIdx].MapRel[1] | A_PtrBuf->Items[Tv_WkIdx].MapRel[2] | A_PtrBuf->Items[Tv_WkIdx].MapRel[3]);
					Tv_ChkMap	=	A_PtrBuf->Items[Tv_WkIdx].MapAloc & Tv_RelMap;
					if ( 0 == Tv_ChkMap )
					{
						// release
						A_PtrBuf->BlkAlc	=	A_PtrBuf->BlkAlc - (__s32)A_PtrBuf->Items[Tv_WkIdx].BlkCnt;
#ifndef LINUX_DRV
						if ( 0 > A_PtrBuf->BlkAlc  )
						{
							DbgMsgPrint( "GrBufTsAlloc - allocate count ( %d ) broken \n", A_PtrBuf->BlkAlc );
						}
#endif
						A_PtrBuf->Items[Tv_WkIdx].MapRel[0]	=	0;
						A_PtrBuf->Items[Tv_WkIdx].MapRel[1]	=	0;
						A_PtrBuf->Items[Tv_WkIdx].MapRel[2]	=	0;
						A_PtrBuf->Items[Tv_WkIdx].MapRel[3]	=	0;
						A_PtrBuf->Items[Tv_WkIdx].MapAloc		=	0;
						A_PtrBuf->Items[Tv_WkIdx].BlkCnt			=	0;
					}
				}

				// check allocatable block
				if ( 0 == A_PtrBuf->Items[Tv_WkIdx].BlkCnt )
				{
					if ( 0 == Tv_FreCnt )
					{
						Tv_FreIdx	=	Tv_WkIdx;
					}
					Tv_FreCnt ++;
					// check request count
					if ( Tv_FreCnt >= Tv_ReqCnt )
					{
						// found, allocate
						A_PtrBuf->Items[Tv_FreIdx].BlkCnt			=	(__u16)Tv_ReqCnt;
						A_PtrBuf->Items[Tv_FreIdx].MapAloc			=	A_OwnerMap;
						A_PtrBuf->Items[Tv_FreIdx].MapRel[0]		=	0;
						A_PtrBuf->Items[Tv_FreIdx].MapRel[1]		=	0;
						A_PtrBuf->Items[Tv_FreIdx].MapRel[2]		=	0;
						A_PtrBuf->Items[Tv_FreIdx].MapRel[3]		=	0;
						A_PtrBuf->BlkAlc	=	A_PtrBuf->BlkAlc + (__s32)Tv_ReqCnt;
#ifndef LINUX_DRV
						if ( (__u32)A_PtrBuf->BlkAlc > A_PtrBuf->BlkCnt  )
						{
							DbgMsgPrint( "GrBufTsAlloc - allocate count ( %d ) over capacity ( %d ) \n", A_PtrBuf->BlkAlc, A_PtrBuf->BlkCnt );
						}
#endif
						// calculate address
						Tv_Result	=	(void*)((Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufTsMng) + (Tv_FreIdx << A_PtrBuf->BlkShft));
						break;
					}
					Tv_WkIdx ++;
				}
				else
				{
					Tv_FreCnt	=	0;
					Tv_WkIdx	=	Tv_WkIdx + (__u32)A_PtrBuf->Items[Tv_WkIdx].BlkCnt;
				}

			}	//	while ( Tv_WkIdx < A_PtrBuf->BlkCnt )

#ifndef LINUX_DRV
			if ( NULL == Tv_Result )
			{
				DbgMsgPrint( "GrBufTsAlloc - neb - aloc %d  req %d \n", A_PtrBuf->BlkAlc, Tv_ReqCnt );
			}
#endif

		}
#ifndef LINUX_DRV
		else
		{
			DbgMsgPrint( "GrBufTsAlloc - bad size ( %d ) or owner map (0x%4X) \n", A_Size, A_OwnerMap );
		}
#endif
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrBufTsFree( struct St_GrBufTsMng* A_PtrBuf, void* A_PtrAlloc, __u16 A_OwnerIdx, __u16 A_ThrdIdx )
{
	// local -------------------
		Def_GrCalPtr	Tv_BaseAdr;
		Def_GrCalPtr	Tv_Dist;
		Def_GrCalPtr	Tv_Mask;
		Def_GrCalPtr	Tv_AlcIdx;
		__u8	Tv_RelMap;
	// code --------------------
		if ( (E_GrBufTsMaxThrdCnt > A_ThrdIdx) && (E_GrBufTsMaxOwnerCnt > A_OwnerIdx) )
		{
			// check address
			Tv_BaseAdr	=	(Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufTsMng);
			if((Def_GrCalPtr)A_PtrAlloc >= Tv_BaseAdr)
			{
				// check address align
				Tv_Dist	=	(Def_GrCalPtr)A_PtrAlloc - Tv_BaseAdr;
				Tv_Mask	=	(1 << A_PtrBuf->BlkShft) - 1;
				if ( 0 == (Tv_Dist & Tv_Mask) )
				{
					Tv_AlcIdx	=	Tv_Dist >> A_PtrBuf->BlkShft;
					// check address over
					if ( Tv_AlcIdx < A_PtrBuf->BlkCnt )
					{
						// check allocated
						Tv_RelMap	=	1 << A_OwnerIdx;
						if (	(0 != A_PtrBuf->Items[Tv_AlcIdx].BlkCnt) && ( 0 != (A_PtrBuf->Items[Tv_AlcIdx].MapAloc & Tv_RelMap)) )
						{
#ifndef LINUX_DRV
							// check over run
							if ((0 != ((A_PtrBuf->Items[Tv_AlcIdx].MapRel[0] | A_PtrBuf->Items[Tv_AlcIdx].MapRel[1] | A_PtrBuf->Items[Tv_AlcIdx].MapRel[2] | A_PtrBuf->Items[Tv_AlcIdx].MapRel[3]) & Tv_RelMap)))
							{
								DbgMsgPrint("GrBufTsFree - requirment over run!\n");
							}
#endif
							// success
							A_PtrBuf->Items[Tv_AlcIdx].MapRel[A_ThrdIdx]	=	A_PtrBuf->Items[Tv_AlcIdx].MapRel[A_ThrdIdx] | Tv_RelMap;
						}
#ifndef LINUX_DRV
						else
						{
							DbgMsgPrint( "GrBufTsFree - block not allocated \n" );
						}
#endif

					}
#ifndef LINUX_DRV
					else
					{
						DbgMsgPrint( "GrBufTsFree - allocate address out of range \n" );
					}
#endif

				}
#ifndef LINUX_DRV
				else
				{
					DbgMsgPrint( "GrBufTsFree - allocate address not aligned \n" );
				}
#endif
			}
#ifndef LINUX_DRV
			else
			{
				DbgMsgPrint( "GrBufTsFree - allocate address smaller than base address \n" );
			}
#endif
		}
#ifndef LINUX_DRV
		else
		{
			DbgMsgPrint( "GrBufTsFree - bad owner index ( %d ) or thread index ( %d ) \n", A_OwnerIdx, A_ThrdIdx );
		}
#endif
}
//--------------------------------------------------------------------
void		GrBufTsMapFree( struct St_GrBufTsMng* A_PtrBuf, void* A_PtrAlloc, __u8 A_OwnerMap, __u16 A_ThrdIdx )
{
	// local -------------------
		Def_GrCalPtr	Tv_BaseAdr;
		Def_GrCalPtr	Tv_Dist;
		Def_GrCalPtr	Tv_Mask;
		Def_GrCalPtr	Tv_AlcIdx;
		__u8	Tv_RelMap;
	// code --------------------
		if ( E_GrBufTsMaxThrdCnt > A_ThrdIdx )
		{
			// check address
			Tv_BaseAdr	=	(Def_GrCalPtr)A_PtrBuf + sizeof(struct St_GrBufTsMng);
			if((Def_GrCalPtr)A_PtrAlloc >= Tv_BaseAdr)
			{
				// check address align
				Tv_Dist	=	(Def_GrCalPtr)A_PtrAlloc - Tv_BaseAdr;
				Tv_Mask	=	(1 << A_PtrBuf->BlkShft) - 1;
				if ( 0 == (Tv_Dist & Tv_Mask) )
				{
					Tv_AlcIdx	=	Tv_Dist >> A_PtrBuf->BlkShft;
					// check address over
					if ( Tv_AlcIdx < A_PtrBuf->BlkCnt )
					{
						// check allocated block
						if ( (0 != A_PtrBuf->Items[Tv_AlcIdx].BlkCnt) && (0 != A_PtrBuf->Items[Tv_AlcIdx].MapAloc) )
						{
							// update
							Tv_RelMap	=	A_PtrBuf->Items[Tv_AlcIdx].MapAloc & A_OwnerMap;
							if ( 0 != Tv_RelMap )
							{
								A_PtrBuf->Items[Tv_AlcIdx].MapRel[A_ThrdIdx]	=	A_PtrBuf->Items[Tv_AlcIdx].MapRel[A_ThrdIdx] | Tv_RelMap;
							}
#ifndef LINUX_DRV
							else
							{
								DbgMsgPrint( "GrBufTsMapFree - bad owner map \n" );
							}
#endif
						}
#ifndef LINUX_DRV
						else
						{
							DbgMsgPrint( "GrBufTsMapFree - block not allocated \n" );
						}
#endif
					}
#ifndef LINUX_DRV
					else
					{
						DbgMsgPrint( "GrBufTsMapFree - allocate address out of range \n" );
					}
#endif

				}
#ifndef LINUX_DRV
				else
				{
					DbgMsgPrint( "GrBufTsMapFree - allocate address not aligned \n" );
				}
#endif
			}
#ifndef LINUX_DRV
			else
			{
				DbgMsgPrint( "GrBufTsMapFree - allocate address ( %08X ) smaller than base address \n", (Def_GrCalPtr)A_PtrAlloc );
			}
#endif
		}
#ifndef LINUX_DRV
		else
		{
			DbgMsgPrint( "GrBufTsMapFree - bad thread index ( %d ) \n", A_ThrdIdx );
		}
#endif
}
//--------------------------------------------------------------------
void		GrBufTsOwnerFree( struct St_GrBufTsMng* A_PtrBuf, __u16 A_OwnerIdx, __u16 A_ThrdIdx )
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u8	Tv_RelMap;
	// code --------------------
		if ( (E_GrBufTsMaxThrdCnt > A_ThrdIdx) && (E_GrBufTsMaxOwnerCnt > A_OwnerIdx) )
		{
			Tv_WkIdx	=	0;
			Tv_RelMap	=	1 << A_OwnerIdx;

			while ( Tv_WkIdx < A_PtrBuf->BlkCnt )
			{
				// check exist
				if ( 0 != A_PtrBuf->Items[Tv_WkIdx].BlkCnt )
				{
					if (	( 0 != (A_PtrBuf->Items[Tv_WkIdx].MapAloc & Tv_RelMap) ) &&
								(0 == ((A_PtrBuf->Items[Tv_WkIdx].MapRel[0] | A_PtrBuf->Items[Tv_WkIdx].MapRel[1] ) & Tv_RelMap))
							)
					{
						// release
						A_PtrBuf->Items[Tv_WkIdx].MapRel[A_ThrdIdx]	=	A_PtrBuf->Items[Tv_WkIdx].MapRel[A_ThrdIdx] | Tv_RelMap;
					}
					// go next
					Tv_WkIdx	=	Tv_WkIdx + A_PtrBuf->Items[Tv_WkIdx].BlkCnt;
				}
				else
				{
					Tv_WkIdx ++;
				}
			}
		}
#ifndef LINUX_DRV
		else
		{
			DbgMsgPrint( "GrBufTsOwnerFree - bad owner index ( %d ) or thread index ( %d ) \n", A_OwnerIdx, A_ThrdIdx );
		}
#endif

}
//--------------------------------------------------------------------
BOOL8		GrBufTsChkFree( struct St_GrBufTsMng* A_PtrBuf, __u32 A_FreeSize )
{
	// local -------------------
		BOOL8	Tv_Result;

		__u16	Tv_RelMap;
		__u16	Tv_ChkMap;

		__u32	Tv_ReqCnt;
		__u32	Tv_WkIdx;
		__u32	Tv_FreCnt;
		__u32	Tv_BlkCnt;
		
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_ReqCnt	=	A_FreeSize >> A_PtrBuf->BlkShft;
		if ( 0 != ( A_FreeSize & ( (1 << A_PtrBuf->BlkShft) - 1) ) )
		{
			Tv_ReqCnt ++;
		}

		Tv_WkIdx	=	0;
		Tv_FreCnt	=	0;
		while ( Tv_WkIdx < A_PtrBuf->BlkCnt )
		{
			// check exist block
			if ( 0 != A_PtrBuf->Items[Tv_WkIdx].BlkCnt )
			{
				// exist block
				// init
				Tv_BlkCnt	=	(__u32)A_PtrBuf->Items[Tv_WkIdx].BlkCnt;

				// check can release
				Tv_RelMap	=	~( A_PtrBuf->Items[Tv_WkIdx].MapRel[0] | A_PtrBuf->Items[Tv_WkIdx].MapRel[1] );
				Tv_ChkMap	=	A_PtrBuf->Items[Tv_WkIdx].MapAloc & Tv_RelMap;
				if ( 0 == Tv_ChkMap )
				{
					// release
					A_PtrBuf->BlkAlc	=	A_PtrBuf->BlkAlc - (__s32)Tv_BlkCnt;
#ifndef LINUX_DRV
					if ( 0 > A_PtrBuf->BlkAlc  )
					{
						DbgMsgPrint( "GrBufTsAlloc - allocate count ( %d ) broken \n", A_PtrBuf->BlkAlc );
					}
#endif
					A_PtrBuf->Items[Tv_WkIdx].MapRel[0]	=	0;
					A_PtrBuf->Items[Tv_WkIdx].MapRel[1]	=	0;
					A_PtrBuf->Items[Tv_WkIdx].MapAloc		=	0;
					A_PtrBuf->Items[Tv_WkIdx].BlkCnt		=	0;
					// update free count
					Tv_FreCnt	=	Tv_FreCnt + Tv_BlkCnt;
				}
				// go next
				Tv_WkIdx	=	Tv_WkIdx + Tv_BlkCnt;
			}
			else
			{
				// zero block
				Tv_FreCnt ++;
				Tv_WkIdx ++;
			}

			// check free count
			/*
			if ( Tv_FreCnt >= Tv_ReqCnt )
			{
				Tv_Result	=	TRUE;
				break;
			}
			*/
		}

		if ( Tv_FreCnt >= Tv_ReqCnt )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

