/*
	GAUSE MTD tool


*/

#include <GrMtdTool.h>

#include <GrDebug.h>

#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <errno.h>


//====================================================================
// local const


//====================================================================
// local type

//====================================================================
// local macro



//====================================================================
// global var



//====================================================================
// local functions

//--------------------------------------------------------------------
BOOL8	GrMtdNandOpen( Ptr_GrMtdNandCtrl A_PtrCtrl, __u8 A_MtdIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_StrName	Tv_StrDev;
		char*	Tv_StrWk;
	// code --------------------
		Tv_Result	=	FALSE;

		// init
		A_PtrCtrl->BlkOfs		=	0;
		A_PtrCtrl->InOfs		=	0;
		A_PtrCtrl->ReadBlk	=	0;
		A_PtrCtrl->ReadOfs	=	0;
		
		// make MTD file name
		Tv_StrWk	=	GrStrCopy( Tv_StrDev, "/dev/mtd" );
		GrStrIntToStr( Tv_StrWk, (__s32)A_MtdIdx );
		// MTD handle open
#ifdef LINUX_APP
		DbgMsgPrint( "GrMtdNandOpen - try open %s \n", Tv_StrDev );
		A_PtrCtrl->Hndl	=	open( Tv_StrDev, O_RDWR | O_SYNC );
		if ( 0 <= A_PtrCtrl->Hndl )
		{
			// get memory info
			DbgMsgPrint( "GrMtdNandOpen - try ioctl MEMGETINFO %s \n", Tv_StrDev );
			if ( 0 == ioctl( A_PtrCtrl->Hndl, MEMGETINFO, &A_PtrCtrl->MemInfo ) )
			if ( TRUE )
			{
				DbgMsgPrint( "GrMtdNandOpen - %s has been writesize = %d , oob size = %d \n", Tv_StrDev, A_PtrCtrl->MemInfo.writesize, A_PtrCtrl->MemInfo.oobsize );
				// check NAND type
				if (	( (16 == A_PtrCtrl->MemInfo.oobsize) && (512 == A_PtrCtrl->MemInfo.writesize) ) || 
							( (8 == A_PtrCtrl->MemInfo.oobsize) && (256 == A_PtrCtrl->MemInfo.writesize) ) || 
							( (64 == A_PtrCtrl->MemInfo.oobsize) && (2048 == A_PtrCtrl->MemInfo.writesize) ) || 
							( (128 == A_PtrCtrl->MemInfo.oobsize) && (4096 == A_PtrCtrl->MemInfo.writesize) ) 
					)
				{
					Tv_Result	=	TRUE;
				}
				else
				{
					DbgMsgPrint( "GrMtdNandOpen - none standard ( %d - %d ) NAND device.\n", A_PtrCtrl->MemInfo.oobsize, A_PtrCtrl->MemInfo.writesize );
				}
			}
			else
			{
				DbgMsgPrint( "GrMtdNandOpen - MEMGETINFO fail.\n" );
			}
			
			if ( !Tv_Result )
			{
				close( A_PtrCtrl->Hndl );
				A_PtrCtrl->Hndl	=	-1;
			}
		}
#else
		A_PtrCtrl->Fc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl( Tv_StrDev, TRUE, FALSE );
		if ( A_PtrCtrl->Fc->IsOpened() )
		{
			A_PtrCtrl->MemInfo.size				=	(__u32)A_PtrCtrl->Fc->GetSize();
			A_PtrCtrl->MemInfo.writesize	=	2048;		// emulation page size
			A_PtrCtrl->MemInfo.oobsize		=	64;
			A_PtrCtrl->MemInfo.erasesize	=	0x20000;
			Tv_Result	=	TRUE;
		}
		else
		{
			delete A_PtrCtrl->Fc;
			A_PtrCtrl->Fc	=	NULL;
		}

#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrMtdNandClose( Ptr_GrMtdNandCtrl A_PtrCtrl )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		if ( 0 <= A_PtrCtrl->Hndl )
		{
			close( A_PtrCtrl->Hndl );
			A_PtrCtrl->Hndl	=	-1;
			Tv_Result	=	TRUE;
		}
#else
		if ( NULL != A_PtrCtrl->Fc )
		{
			delete	A_PtrCtrl->Fc;
			A_PtrCtrl->Fc	=	NULL;
			Tv_Result	=	TRUE;
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrMtdNandUnitSizeGet( Ptr_GrMtdNandCtrl A_PtrCtrl )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

#ifdef LINUX_APP
		if ( 0 <= A_PtrCtrl->Hndl )
#else
		if ( NULL != A_PtrCtrl->Fc )
#endif
		{
			Tv_Result	=	A_PtrCtrl->MemInfo.writesize;
		}
		else
		{
			DbgMsgPrint( "GrMtdNandUnitSizeGet - control was not open.\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrMtdNandEraseAll( Ptr_GrMtdNandCtrl A_PtrCtrl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Pos;
#ifdef LINUX_APP
		loff_t	Tv_Ofs;
		__s32		Tv_Ret;
		erase_info_t	Tv_Erase;
		__u32	Tv_RetryCnt;
		BOOL8	Tv_IsRetry;
#else
		__u8	Tv_FillBuf[64];
		__u32	Tv_WriteSize;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		if ( 0 <= A_PtrCtrl->Hndl )
		{
			Tv_Result	=	TRUE;
			Tv_Erase.length	=	A_PtrCtrl->MemInfo.erasesize;
			Tv_Pos			=	0;
			Tv_RetryCnt	=	0;
			Tv_IsRetry	=	FALSE;
			while ( Tv_Pos < A_PtrCtrl->MemInfo.size )
			{
				// init
				Tv_IsRetry	=	FALSE;
				// check bad info
				Tv_Ofs	=	Tv_Pos;
				Tv_Ret	=	ioctl(A_PtrCtrl->Hndl, MEMGETBADBLOCK, &Tv_Ofs);
				if ( 0 == Tv_Ret )
				{
					// try erase block
					Tv_Erase.start	=	Tv_Pos;
					if ( 0 != ioctl(A_PtrCtrl->Hndl, MEMERASE, &Tv_Erase) != 0)
					{
						// erase failure
						DbgMsgPrint( "GrMtdNandErase - erase fail at %08X.\n", Tv_Pos );
						Tv_IsRetry	=	TRUE;
					}
				}
				else if ( 0 < Tv_Ret )
				{
					// bad block
					DbgMsgPrint( "GrMtdNandErase - bad block found at %08X.\n", Tv_Pos );
				}
				else
				{
					// bad block check not support
					DbgMsgPrint( "GrMtdNandErase - MEMGETBADBLOCK not support.\n" );
					Tv_Result	=	FALSE;
					break;
				}
				
				// next
				if ( Tv_IsRetry )
				{
					Tv_RetryCnt ++;
					if ( E_GrMtdNandRetryCnt <= Tv_RetryCnt )
					{
						DbgMsgPrint( "GrMtdNandErase - erase skip at %08X.\n", Tv_Pos );
						Tv_Pos	=	Tv_Pos + A_PtrCtrl->MemInfo.erasesize;
						Tv_RetryCnt	=	0;
					}
				}
				else
				{
					Tv_Pos	=	Tv_Pos + A_PtrCtrl->MemInfo.erasesize;
					Tv_RetryCnt	=	0;
				}
			}
		}
#else
		if ( NULL != A_PtrCtrl->Fc )
		{
			if ( (0 == A_PtrCtrl->BlkOfs) && (0 == A_PtrCtrl->InOfs) )
			{
				// erase buffer
				GrDumyFillB( Tv_FillBuf, sizeof(Tv_FillBuf), 0xFF );
				Tv_Pos	=	0;
				if ( 0 == A_PtrCtrl->Fc->Seek( 0 ) )
				{
					Tv_Result	=	TRUE;
					while ( Tv_Pos < A_PtrCtrl->MemInfo.size )
					{
						Tv_WriteSize	=	A_PtrCtrl->MemInfo.size - Tv_Pos;
						if ( sizeof(Tv_FillBuf) < Tv_WriteSize )
						{
							Tv_WriteSize	=	sizeof(Tv_FillBuf);
						}
						if ( Tv_WriteSize != A_PtrCtrl->Fc->Write( Tv_FillBuf, Tv_WriteSize ) )
						{
							DbgMsgPrint( "GrMtdNandErase - write error.\n" );
							Tv_Result	=	FALSE;
							break;
						}
						
						// next
						Tv_Pos	=	Tv_Pos + Tv_WriteSize;
					}
				}
				else
				{
					DbgMsgPrint( "GrMtdNandErase - seek error.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "GrMtdNandErase - already working.\n" );
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrMtdNandErase( Ptr_GrMtdNandCtrl A_PtrCtrl, __u32 A_Size )
{
		// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Pos;
#ifdef LINUX_APP
		loff_t	Tv_Ofs;
		__s32		Tv_Ret;
		erase_info_t	Tv_Erase;
		__u32	Tv_RetryCnt;
		BOOL8	Tv_IsRetry;
#else
		__u8	Tv_FillBuf[64];
		__u32	Tv_WriteSize;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		if ( 0 <= A_PtrCtrl->Hndl )
		{
			Tv_Result	=	TRUE;
			Tv_Erase.length	=	A_PtrCtrl->MemInfo.erasesize;
			Tv_Pos			=	0;
			Tv_RetryCnt	=	0;
			Tv_IsRetry	=	FALSE;
			while ( Tv_Pos < A_Size )
			{
				// init
				Tv_IsRetry	=	FALSE;
				// check bad info
				Tv_Ofs	=	Tv_Pos;
				Tv_Ret	=	ioctl(A_PtrCtrl->Hndl, MEMGETBADBLOCK, &Tv_Ofs);
				if ( 0 == Tv_Ret )
				{
					// try erase block
					Tv_Erase.start	=	Tv_Pos;
					if ( 0 != ioctl(A_PtrCtrl->Hndl, MEMERASE, &Tv_Erase) != 0)
					{
						// erase failure
						DbgMsgPrint( "GrMtdNandErase - erase fail at %08X.\n", Tv_Pos );
						Tv_IsRetry	=	TRUE;
					}
				}
				else if ( 0 < Tv_Ret )
				{
					// bad block
					DbgMsgPrint( "GrMtdNandErase - bad block found at %08X.\n", Tv_Pos );
				}
				else
				{
					// bad block check not support
					DbgMsgPrint( "GrMtdNandErase - MEMGETBADBLOCK not support.\n" );
					Tv_Result	=	FALSE;
					break;
				}
				
				// next
				if ( Tv_IsRetry )
				{
					Tv_RetryCnt ++;
					if ( E_GrMtdNandRetryCnt <= Tv_RetryCnt )
					{
						DbgMsgPrint( "GrMtdNandErase - erase skip at %08X.\n", Tv_Pos );
						Tv_Pos	=	Tv_Pos + A_PtrCtrl->MemInfo.erasesize;
						Tv_RetryCnt	=	0;
					}
				}
				else
				{
					Tv_Pos	=	Tv_Pos + A_PtrCtrl->MemInfo.erasesize;
					Tv_RetryCnt	=	0;
				}
			}
		}
#else
		if ( NULL != A_PtrCtrl->Fc )
		{
			if ( (0 == A_PtrCtrl->BlkOfs) && (0 == A_PtrCtrl->InOfs) )
			{
				// erase buffer
				GrDumyFillB( Tv_FillBuf, sizeof(Tv_FillBuf), 0xFF );
				Tv_Pos	=	0;
				if ( 0 == A_PtrCtrl->Fc->Seek( 0 ) )
				{
					Tv_Result	=	TRUE;
					while ( Tv_Pos < A_Size )
					{
						Tv_WriteSize	=	A_Size - Tv_Pos;
						if ( sizeof(Tv_FillBuf) < Tv_WriteSize )
						{
							Tv_WriteSize	=	sizeof(Tv_FillBuf);
						}
						if ( Tv_WriteSize != A_PtrCtrl->Fc->Write( Tv_FillBuf, Tv_WriteSize ) )
						{
							DbgMsgPrint( "GrMtdNandErase - write error.\n" );
							Tv_Result	=	FALSE;
							break;
						}
						
						// next
						Tv_Pos	=	Tv_Pos + Tv_WriteSize;
					}
				}
				else
				{
					DbgMsgPrint( "GrMtdNandErase - seek error.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "GrMtdNandErase - already working.\n" );
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrMtdNandWrite( Ptr_GrMtdNandCtrl A_PtrCtrl, void* A_PtrData )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsWrite;
		__u32	Tv_Pos;

#ifdef LINUX_APP
		BOOL8	Tv_IsBad;
		__s32		Tv_Rval;
		loff_t	Tv_Ofs;
		__u32	Tv_RetryCnt;
		ssize_t	Tv_WtSize;
#else
		
#endif
	// code --------------------
		Tv_Result	=	FALSE;


		
#ifdef LINUX_APP
		if ( 0 <= A_PtrCtrl->Hndl )
		{
			// check bad block
			while ( (A_PtrCtrl->BlkOfs + A_PtrCtrl->InOfs) < A_PtrCtrl->MemInfo.size )
			{
				// check block start
				Tv_IsBad	=	FALSE;
				if ( 0 == A_PtrCtrl->InOfs )
				{
					// check bad block
					Tv_Ofs	=	A_PtrCtrl->BlkOfs;
					Tv_Rval	=	ioctl(A_PtrCtrl->Hndl, MEMGETBADBLOCK, &Tv_Ofs);
					if ( 0 > Tv_Rval )
					{
						DbgMsgPrint( "GrMtdNandWrite - can not support MEMGETBADBLOCK .\n" );
						Tv_Result	=	FALSE;
						break;
					}
					else if ( 0 < Tv_Rval )
					{
						Tv_IsBad	=	TRUE;
					}
				}

				// write
				if ( !Tv_IsBad )
				{
					Tv_IsWrite	= FALSE;
					// try write
					for ( Tv_RetryCnt=0;Tv_RetryCnt < E_GrMtdNandRetryCnt;Tv_RetryCnt++ )
					{
						Tv_Pos		=	A_PtrCtrl->BlkOfs + A_PtrCtrl->InOfs;
						Tv_WtSize	=	pwrite( A_PtrCtrl->Hndl, A_PtrData, A_PtrCtrl->MemInfo.writesize, Tv_Pos );
						if ( A_PtrCtrl->MemInfo.writesize == Tv_WtSize )
						{
							// success
							Tv_IsWrite	 = TRUE;
							break;
						}
						// retry
						DbgMsgPrint( "GrMtdNandWrite - retry data writing at %8X.\n", Tv_Pos );
					}

					if(Tv_IsWrite)
					{
						// next
						A_PtrCtrl->InOfs	=	A_PtrCtrl->InOfs + A_PtrCtrl->MemInfo.writesize;
						if ( A_PtrCtrl->InOfs >= A_PtrCtrl->MemInfo.erasesize )
						{
							A_PtrCtrl->BlkOfs	=	A_PtrCtrl->BlkOfs + A_PtrCtrl->MemInfo.erasesize;
							A_PtrCtrl->InOfs	=	0;
						}

						Tv_Result	=	TRUE;
						break;
					}
				}
				// bad , so next block
				DbgMsgPrint("GrMtdNandWrite - write failed. skip bad block = 0x%x, size = %d\n", A_PtrCtrl->BlkOfs, A_PtrCtrl->MemInfo.erasesize);
				A_PtrCtrl->BlkOfs	=	A_PtrCtrl->BlkOfs + A_PtrCtrl->MemInfo.erasesize;
				A_PtrCtrl->InOfs	=	0;
			}
		}
#else
		if ( NULL != A_PtrCtrl->Fc )
		{
			if ( (A_PtrCtrl->BlkOfs + A_PtrCtrl->InOfs) < A_PtrCtrl->MemInfo.size )
			{
				// try write
				Tv_Pos		=	A_PtrCtrl->BlkOfs + A_PtrCtrl->InOfs;
				if ( Tv_Pos == (__u32)A_PtrCtrl->Fc->Seek( Tv_Pos ) )
				{
					if ( A_PtrCtrl->MemInfo.writesize == A_PtrCtrl->Fc->Write( A_PtrData, A_PtrCtrl->MemInfo.writesize ) )
					{
						// next
						A_PtrCtrl->InOfs	=	A_PtrCtrl->InOfs + A_PtrCtrl->MemInfo.writesize;
						if ( A_PtrCtrl->InOfs >= A_PtrCtrl->MemInfo.erasesize )
						{
							A_PtrCtrl->BlkOfs	=	A_PtrCtrl->BlkOfs + A_PtrCtrl->MemInfo.erasesize;
							A_PtrCtrl->InOfs	=	0;
						}
						Tv_Result	=	TRUE;
					}
				}
				
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrMtdNandTotalSizeGet( Ptr_GrMtdNandCtrl A_PtrCtrl )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

#ifdef LINUX_APP
		if ( 0 <= A_PtrCtrl->Hndl )
#else
		if ( NULL != A_PtrCtrl->Fc )
#endif
		{
			Tv_Result	=	A_PtrCtrl->MemInfo.size;
		}
		else
		{
			DbgMsgPrint( "GrMtdNandTotalSizeGet - control was not open.\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrMtdNandReadPosReset( Ptr_GrMtdNandCtrl A_PtrCtrl )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		if ( 0 <= A_PtrCtrl->Hndl )
		{
			Tv_Result	=	TRUE;
		}
#else
		if ( NULL != A_PtrCtrl->Fc )
		{
			Tv_Result	=	TRUE;
		}
#endif
		A_PtrCtrl->ReadBlk	=	0;
		A_PtrCtrl->ReadOfs	=	0;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrMtdNandSeqRead( Ptr_GrMtdNandCtrl A_PtrCtrl, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsBlkOk;
		__s32		Tv_Remain;
		__u32	Tv_Ofs;
		__u32	Tv_ReadSize;
		__u32	Tv_ReadAble;
		void*	Tv_PtrBuf;
		__s32		Tv_RetVal;
#ifdef LINUX_APP
		loff_t	Tv_ChkBlk;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

		// check handle able
#ifdef LINUX_APP
		if ( 0 <= A_PtrCtrl->Hndl )
#else
		if ( NULL != A_PtrCtrl->Fc )
#endif
		{
			Tv_Result	=	TRUE;
			Tv_Remain	=	(__s32)A_Size;
			Tv_PtrBuf	=	A_PtrBuf;
			while ( 0 != Tv_Remain )
			{
				// check read able
				if ( A_PtrCtrl->ReadBlk >= A_PtrCtrl->MemInfo.size )
				{
					Tv_Result	=	FALSE;
					DbgMsgPrint( "GrMtdNandSeqRead - out of size.\n" );
					break;
				}
				// check block offset
				Tv_IsBlkOk	=	TRUE;
#ifdef LINUX_APP
				if ( 0 == A_PtrCtrl->ReadOfs )
				{
					Tv_ChkBlk	=	A_PtrCtrl->ReadBlk;
					Tv_RetVal	=	ioctl(A_PtrCtrl->Hndl, MEMGETBADBLOCK, &Tv_ChkBlk );
					if ( 0 < Tv_RetVal )
					{
						// bad block
						DbgMsgPrint( "GrMtdNandSeqRead - bad block found at %08X.\n", A_PtrCtrl->ReadBlk );
						// go next block
						A_PtrCtrl->ReadBlk	=	A_PtrCtrl->ReadBlk + A_PtrCtrl->MemInfo.erasesize;
					}
					else if ( 0 > Tv_RetVal )
					{
						// bad block check not support
						DbgMsgPrint( "GrMtdNandSeqRead - MEMGETBADBLOCK not support.\n" );
					}
				}
#endif
				// read block
				if ( Tv_IsBlkOk )
				{
					Tv_ReadSize	=	(__u32)Tv_Remain;
					Tv_ReadAble	=	A_PtrCtrl->MemInfo.erasesize - A_PtrCtrl->ReadOfs;
					if ( Tv_ReadAble < Tv_ReadSize )
					{
						Tv_ReadSize	=	Tv_ReadAble;
					}
					Tv_Ofs			=	A_PtrCtrl->ReadBlk + A_PtrCtrl->ReadOfs;
#ifdef LINUX_APP
					if ( 0 > lseek( A_PtrCtrl->Hndl, (long)Tv_Ofs, SEEK_SET ) )
					{
						Tv_Result	=	FALSE;
						DbgMsgPrint( "GrMtdNandRead - seek error.\n" );
						break;
					}
					Tv_RetVal	=	read( A_PtrCtrl->Hndl, Tv_PtrBuf, (__s32)Tv_ReadSize );
					if ( 0 > Tv_RetVal  )
					{
						DbgMsgPrint( "GrMtdNandRead - reading failure, retry\n" );
						Tv_Result	=	FALSE;
						break;
					}
#else
					if ( (__s64)Tv_Ofs != A_PtrCtrl->Fc->Seek( (__s64)Tv_Ofs ))
					{
						Tv_Result	=	FALSE;
						DbgMsgPrint( "GrMtdNandRead - seek error.\n" );
						break;
					}
					Tv_RetVal	=	(__s32)A_PtrCtrl->Fc->Read( Tv_PtrBuf, Tv_ReadSize );
					if ( (__u32)Tv_RetVal != Tv_ReadSize )
					{
						Tv_Result	=	FALSE;
						DbgMsgPrint( "GrMtdNandRead - read error.\n" );
						break;
					}
#endif
					// go next
					A_PtrCtrl->ReadOfs	=	A_PtrCtrl->ReadOfs + (__u32)Tv_RetVal;
					Tv_Remain	=	Tv_Remain - Tv_RetVal;
					Tv_PtrBuf	=	(void*)( (__s32)Tv_PtrBuf + Tv_RetVal );
					if ( A_PtrCtrl->ReadOfs >= A_PtrCtrl->MemInfo.erasesize )
					{
						A_PtrCtrl->ReadBlk	=	A_PtrCtrl->ReadBlk + A_PtrCtrl->MemInfo.erasesize;
						A_PtrCtrl->ReadOfs	=	0;
					}
				}	//	if ( Tv_IsBlkOk )
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrMtdReadFromNand(__u8 A_MtdIdx, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_StrName	Tv_StrDev;
		char*	Tv_StrWk;
		__s32		Tv_RetVal;
#ifdef LINUX_APP
		int	Tv_Hndl;
#endif

	// code --------------------
		Tv_Result	=	FALSE;

		// make MTD file name
		Tv_StrWk	=	GrStrCopy(Tv_StrDev, "/dev/mtd");
		GrStrIntToStr(Tv_StrWk, (__s32)A_MtdIdx);
		// MTD handle open
#ifdef LINUX_APP
		//DbgMsgPrint( "GrMtdNandOpen - try open %s \n", Tv_StrDev );
		Tv_Hndl	=	open(Tv_StrDev, O_RDWR | O_SYNC);
		if(0 <= Tv_Hndl)
		{
			if(0 <= lseek(Tv_Hndl, (long)A_Ofs, SEEK_SET))
			{
				Tv_RetVal	=	read(Tv_Hndl, A_PtrBuf, (__s32)A_Size);
				if(0 <= Tv_RetVal)
				{
					Tv_Result	=	TRUE;
				}
				else
				{
					DbgMsgPrint("GrMtdNandRead - reading failure, retry\n");
				}
			}
			else
			{
				DbgMsgPrint("GrMtdReadFromNand - seek error.\n");
			}

			close(Tv_Hndl);
		}
#endif

		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	GrMtdWriteToNand(__u8 A_MtdIdx, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_StrName	Tv_StrDev;
		char*	Tv_StrWk;
		__u32	Tv_Remain;
		__s32	Tv_RetVal;
		void*	Tv_PtrBuf;
#ifdef LINUX_APP
		int	Tv_Hndl;
		struct  mtd_info_user	Tv_MemInfo;
		erase_info_t	Tv_Erase;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

		// make MTD file name
		Tv_StrWk	=	GrStrCopy(Tv_StrDev, "/dev/mtd");
		GrStrIntToStr(Tv_StrWk, (__s32)A_MtdIdx);

#ifdef LINUX_APP
		//DbgMsgPrint( "GrMtdNandOpen - try open %s \n", Tv_StrDev );
		Tv_Hndl	=	open(Tv_StrDev, O_RDWR | O_SYNC);
		if(0 <= Tv_Hndl)
		{
			if(0 == ioctl(Tv_Hndl, MEMGETINFO, &Tv_MemInfo))
			{
				// check offset and size
				if((0 != A_Size) && (0 == (A_Ofs % Tv_MemInfo.erasesize)) && (0 == (A_Size % Tv_MemInfo.erasesize)))
				{
					// do erase
					Tv_Erase.length	=	Tv_MemInfo.erasesize;

					Tv_Remain	=	A_Size;
					Tv_Erase.start	=	A_Ofs;
					while(0 != Tv_Remain)
					{
						if(0 != ioctl(Tv_Hndl, MEMERASE, &Tv_Erase))
						{
							// erase failure
							DbgMsgPrint("GrMtdWriteToNand - erase fail at %08X.\n", Tv_Erase.start);
						}
						// next
						Tv_Remain				=	Tv_Remain - Tv_MemInfo.erasesize;
						Tv_Erase.start	=	Tv_Erase.start + Tv_MemInfo.erasesize;
					}

					// do write
					if(0 <= lseek(Tv_Hndl, (long)A_Ofs, SEEK_SET))
					{
						Tv_Remain	=	A_Size;
						Tv_PtrBuf	=	A_PtrBuf;

						while(0 != Tv_Remain)
						{
							Tv_RetVal	=	write(Tv_Hndl, Tv_PtrBuf, (__s32)Tv_MemInfo.writesize);
							if(0 <= Tv_RetVal)
							{
								Tv_Result	=	TRUE;
							}
							else
							{
								DbgMsgPrint("GrMtdWriteToNand - write failure\n");
								break;
							}

							// next
							Tv_Remain		=	Tv_Remain - Tv_MemInfo.writesize;
							Tv_PtrBuf		=	(void*)((Def_GrCalPtr)Tv_PtrBuf + (Def_GrCalPtr)Tv_MemInfo.writesize);
						}

					}
					else
					{
						DbgMsgPrint("GrMtdWriteToNand - seek error.\n");
					}

				}
				else
				{
					DbgMsgPrint("GrMtdWriteToNand - bad offset and size\n");
				}
			}
			else
			{
				DbgMsgPrint("GrMtdWriteToNand - MEMGETINFO failure\n");
			}

			close(Tv_Hndl);
		}
#endif

		return	Tv_Result;

}
//--------------------------------------------------------------------
