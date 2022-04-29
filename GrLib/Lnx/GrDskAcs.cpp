/*
	disk direct access
*/

//====================================================================
// uses
#include	<Lnx/GrDskAcs.h>
#include	<GrStrTool.h>

#include	<sys/types.h>			/* unistd.h needs this */
#include	<unistd.h>				/* contains read/write */
#include	<fcntl.h>
#include	<linux/hdreg.h>
#include	<sys/ioctl.h>
#include	<linux/fs.h>

//====================================================================
//local const
#define E_GrDskSctSize		512
#define E_GrDskSctAdrMsk	(~(512 - 1))

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrDskAcs::Cls_GrDskAcs( __s8* A_StrDev, __s8* A_StrIdf )
{
	// local -------------------
		__s32			Tv_HndlDev;
	// code --------------------
		//init
		m_SctSize			=	0;
		m_SctCnt			=	0;
		m_TotalSize		=	0;
		m_IsExist			=	FALSE;
		m_IsGood			=	FALSE;
		GrStrCopy( m_StrDev, A_StrDev );
		GrStrCopy( m_StrAcc, "/dev/" );
		GrStrCat( m_StrAcc, A_StrDev );
		GrStrCopy( m_StrIdf, A_StrIdf );

		//open disk
		Tv_HndlDev	=	open( m_StrAcc, O_RDWR );
		if ( 0 <= Tv_HndlDev )
		{
			//get disk identify
			if ( 0 <= ioctl( Tv_HndlDev, BLKGETSIZE64, &m_TotalSize ) )
			{
				//update info
				m_SctSize			=	E_GrDskSctSize;
				//calculate sector count
				m_SctCnt			=	m_TotalSize / (__u64)m_SctSize;

				//success, exist disk
				m_IsExist			=	TRUE;
				m_IsGood			=	TRUE;
			}
			close( Tv_HndlDev );
		}
	
}
//--------------------------------------------------------------------
Cls_GrDskAcs::~Cls_GrDskAcs()
{
}
//--------------------------------------------------------------------
__u32		Cls_GrDskAcs::GetSectorSize( void )
{
	return	m_SctSize;
}
//--------------------------------------------------------------------
__u64		Cls_GrDskAcs::GetTotalSector( void )
{
	return	m_SctCnt;
}
//--------------------------------------------------------------------
__u64		Cls_GrDskAcs::GetTotalSize( void )
{
	return		m_TotalSize;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrDskAcs::IsExist( void )
{
	return	m_IsExist;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrDskAcs::IsGood( void )
{
	return	m_IsGood;
}
//--------------------------------------------------------------------
__s8*		Cls_GrDskAcs::GetStrDev( void )
{
	return	m_StrDev;
}
//--------------------------------------------------------------------
__s8*		Cls_GrDskAcs::GetStrIdf( void )
{
	return	m_StrIdf;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrDskAcs::ReadSector( __u64 A_SctPos, __u32 A_SctCnt, void* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_HndlDev;
		__s32			Tv_SzRead;
		__u64		Tv_Fptr;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//chek exist
		if ( m_IsExist && m_IsGood )
		{
			//open disk
			Tv_HndlDev	=	open( m_StrAcc, O_RDWR );
			if ( 0 <= Tv_HndlDev )
			{
				Tv_Fptr		=	A_SctPos * E_GrDskSctSize;
				if ( -1 != lseek64( Tv_HndlDev, (off64_t)Tv_Fptr, SEEK_SET ) )
				{
					//read
					Tv_SzRead	=	A_SctCnt * m_SctSize;
					if ( Tv_SzRead == read( Tv_HndlDev, A_PtrBuf, Tv_SzRead) )
					{
						//success
						Tv_Result		=	TRUE;
					}
					else
					{
						m_IsGood		=	FALSE;
					}
				}
				close( Tv_HndlDev );
			}

			//update good
			if ( !Tv_Result )
			{
				m_IsGood	=	FALSE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrDskAcs::WriteSector( __u64 A_SctPos, __u32 A_SctCnt, void* A_PtrBuf )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_HndlDev;
		__s32			Tv_SzWrite;
		__u64		Tv_Fptr;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check exist
		if ( m_IsExist && m_IsGood )
		{
			//open disk
			Tv_HndlDev	=	open( m_StrAcc, O_RDWR );
			if ( 0 <= Tv_HndlDev )
			{
				Tv_Fptr		=	A_SctPos * E_GrDskSctSize;
				if ( -1 != lseek64( Tv_HndlDev, (off64_t)Tv_Fptr, SEEK_SET ) )
				{
					//read
					Tv_SzWrite	=	A_SctCnt * m_SctSize;
					if ( Tv_SzWrite == write( Tv_HndlDev, A_PtrBuf, Tv_SzWrite) )
					{
						//success
						Tv_Result		=	TRUE;
					}
					else
					{
						m_IsGood		=	FALSE;
					}
				}
				close( Tv_HndlDev );
			}
			//update good
			if ( !Tv_Result )
			{
				m_IsGood	=	FALSE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		Cls_GrDskAcs::CalcSctCntBySize( __u32 A_Size )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result		=	A_Size / m_SctSize;
		if ( 0 != (A_Size % m_SctSize) )
		{
			Tv_Result	++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

