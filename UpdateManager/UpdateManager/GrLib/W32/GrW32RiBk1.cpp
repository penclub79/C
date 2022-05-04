/*
	dat backup file manager ver 1
 */

//====================================================================
// uses
#include	<GrLnxApp.h>

#include	<GrTypeBase.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>

#include	<stdlib.h>
#include	<W32/GrW32RiBk1.h>
#include	<GrFile64.h>
#include	<Dvr/GrDvrFls16v1.h>

//====================================================================
//local const
#define E_GrW32RiBk1FileFilter	"*.dat"

#define E_GrW32RiBk1ChukChkSize	0x60000
#define E_GrW32RiBk1FptrStpChuk	(E_GrW32RiBk1MaxChukSize - E_GrW32RiBk1ChukChkSize)

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrW32RiBk1::Cls_GrW32RiBk1( void )
{
	// local -------------------
	// code --------------------
		//init
		m_FileCnt			=	0;
		GrDumyZeroMem( &m_FileTbl, sizeof(m_FileTbl) );
}
//--------------------------------------------------------------------
Cls_GrW32RiBk1::~Cls_GrW32RiBk1()
{
}
//--------------------------------------------------------------------
void		Cls_GrW32RiBk1::InitFileTbl( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		Cls_GrFile64*		Tv_ObjFl;
		BOOL8		Tv_IsFirst;
		__u64		Tv_FlSize;
	// code --------------------
		if ( 0 != m_FileCnt )
		{
			Tv_IsFirst		=	TRUE;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_FileCnt;Tv_WkIdx++ )
			{
				Tv_ObjFl	=	(Cls_GrFile64*)new Cls_GrFile64( m_FileTbl[Tv_WkIdx].StrFn, FALSE, FALSE );
				if ( Tv_ObjFl->IsOpened() )
				{
					Tv_FlSize		=	Tv_ObjFl->GetSize();
					//load head
					Tv_ObjFl->Seek( 0 );
					if ( sizeof(St_GrW32RiBk1FileHead) == Tv_ObjFl->Read( &m_FileTbl[Tv_WkIdx].Fh, sizeof(St_GrW32RiBk1FileHead) ) )
					{
						if ( E_GrW32RiBk1FccFileHead == m_FileTbl[Tv_WkIdx].Fh.Fcc )
						{
							//update total time range
							if (Tv_IsFirst)
							{
								//update time rule
								GrTimeSetTimeRule( &m_FileTbl[Tv_WkIdx].Fh.TimeRule );
								//update time
								m_TimeRng			=	m_FileTbl[Tv_WkIdx].Fh.TimeRng;
								//decode password
								LcPassDec();
								Tv_IsFirst		=	FALSE;
							}
							else
							{
								if ( m_TimeRng.TimeSt > m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeSt )
								{
									m_TimeRng.TimeSt =	m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeSt;
								}
								if ( m_TimeRng.TimeEd < m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeEd )
								{
									m_TimeRng.TimeEd =	m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeEd;
								}
							}
						}
					}
				}
				delete	Tv_ObjFl;
			}
			//check all file bad
			if ( Tv_IsFirst )
			{
				m_FileCnt		=	0;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrW32RiBk1::RecGetAllTimeRng( Ptr_GrTimeRng A_PtrRtTmRng, BOOL8 A_IsInst, BOOL8 A_IsBackup )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (0 != m_FileCnt) && (!A_IsInst) )
		{
			*A_PtrRtTmRng		=	m_TimeRng;
			Tv_Result				=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrW32RiBk1::FindFrmCnt( void* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrDvrFls16v1FrmHd	Tv_PtrFh;
		__u32		Tv_PrcSize;
	// code --------------------
		//init
		Tv_Result		=	0;
		//find
		Tv_PrcSize	=	0;
		Tv_PtrFh		=	(Ptr_GrDvrFls16v1FrmHd)A_PtrBuf;
		while ( Tv_PrcSize < A_BufSize )
		{
			Tv_PrcSize	=	Tv_PrcSize + Tv_PtrFh->Size;
			Tv_PtrFh		=	(Ptr_GrDvrFls16v1FrmHd)( (__u32)Tv_PtrFh + Tv_PtrFh->Size );
			Tv_Result ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrW32RiBk1::RecReadChunk( BOOL8 A_IsInst, Def_GrTime A_Time, __u32 A_TimeType, 
											__u32* A_PtrRtFrmCnt, void* A_PtrChunk, BOOL8 A_IsBackup )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
		__u32		Tv_FileIdx;
		Def_GrTime	Tv_FindGap;
		Def_GrTime	Tv_NowGap;
		Cls_GrFile64*	Tv_ObjFl;
		__u32		Tv_ChukIdx;
		__u32		Tv_ReadSize;
		Ptr_GrW32RiBk1FileHead	Tv_PtrItm;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able
		if ( (0 != m_FileCnt) && (!A_IsInst) )
		{
			//find file
			for ( Tv_WkIdx=0;Tv_WkIdx < m_FileCnt;Tv_WkIdx++ )
			{
				if ( 0 != m_FileTbl[Tv_WkIdx].Fh.ChukCnt )
				{
					//in time
					if ( 0 != ( E_GrTimeFindRngIn & A_TimeType ) )
					{
						//check in time
						if (	(A_Time >= m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeSt ) && 
									(A_Time <= m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeEd )
								)
						{
							//finde
							Tv_Result		=	TRUE;
							Tv_FileIdx	=	Tv_WkIdx;
							break;
						}
					}
					//over time
					if ( 0 != ( E_GrTimeFindRngGreate & A_TimeType ) )
					{
						if ( A_Time < m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeEd )
						{
							Tv_NowGap	=	m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeEd - A_Time;
							if ( Tv_Result )
							{
								if ( Tv_FindGap > Tv_NowGap )
								{
									Tv_FindGap		=	Tv_NowGap;
									Tv_FileIdx		=	Tv_WkIdx;
								}
							}
							else
							{
								Tv_FindGap		=	Tv_NowGap;
								Tv_FileIdx		=	Tv_WkIdx;
							}
							Tv_Result		=	TRUE;
						}
					}
					//small time
					if ( 0 != ( E_GrTimeFindRngSmall & A_TimeType ) )
					{
						if ( A_Time > m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeSt )
						{
							Tv_NowGap	=	A_Time - m_FileTbl[Tv_WkIdx].Fh.TimeRng.TimeSt;
							if ( Tv_Result )
							{
								if ( Tv_FindGap > Tv_NowGap )
								{
									Tv_FindGap		=	Tv_NowGap;
									Tv_FileIdx		=	Tv_WkIdx;
								}
							}
							else
							{
								Tv_FindGap		=	Tv_NowGap;
								Tv_FileIdx		=	Tv_WkIdx;
							}
							Tv_Result		=	TRUE;
						}
					}
				}
			}

			//find chunk index
			if ( Tv_Result )
			{
				//init
				Tv_Result		=	FALSE;
				Tv_PtrItm	=	&m_FileTbl[Tv_FileIdx].Fh;
				//find chunk
				for ( Tv_WkIdx=0;Tv_WkIdx < Tv_PtrItm->ChukCnt;Tv_WkIdx++ )
				{
					//in time
					if ( 0 != ( E_GrTimeFindRngIn & A_TimeType ) )
					{
						//check in time
						if (	(A_Time >= Tv_PtrItm->ChukTbl[Tv_WkIdx].TimeRng.TimeSt ) && 
									(A_Time <= Tv_PtrItm->ChukTbl[Tv_WkIdx].TimeRng.TimeEd )
							)
						{
							//finde
							Tv_Result		=	TRUE;
							Tv_ChukIdx	=	Tv_WkIdx;
							break;
						}
					}
					//over time
					if ( 0 != ( E_GrTimeFindRngGreate & A_TimeType ) )
					{
						if ( A_Time < Tv_PtrItm->ChukTbl[Tv_WkIdx].TimeRng.TimeEd )
						{
							Tv_NowGap	=	Tv_PtrItm->ChukTbl[Tv_WkIdx].TimeRng.TimeEd - A_Time;
							if ( Tv_Result )
							{
								if ( Tv_FindGap > Tv_NowGap )
								{
									Tv_FindGap		=	Tv_NowGap;
									Tv_ChukIdx		=	Tv_WkIdx;
								}
							}
							else
							{
								Tv_FindGap		=	Tv_NowGap;
								Tv_ChukIdx		=	Tv_WkIdx;
							}
							Tv_Result		=	TRUE;
						}
					}
					//small time
					if ( 0 != ( E_GrTimeFindRngSmall & A_TimeType ) )
					{
						if ( A_Time > Tv_PtrItm->ChukTbl[Tv_WkIdx].TimeRng.TimeSt )
						{
							Tv_NowGap	=	A_Time - Tv_PtrItm->ChukTbl[Tv_WkIdx].TimeRng.TimeSt;
							if ( Tv_Result )
							{
								if ( Tv_FindGap > Tv_NowGap )
								{
									Tv_FindGap		=	Tv_NowGap;
									Tv_ChukIdx		=	Tv_WkIdx;
								}
							}
							else
							{
								Tv_FindGap		=	Tv_NowGap;
								Tv_ChukIdx		=	Tv_WkIdx;
							}
							Tv_Result		=	TRUE;
						}
					}
				}
			}

			//load file
			if ( Tv_Result )
			{
				Tv_ObjFl	=	(Cls_GrFile64*)new Cls_GrFile64( m_FileTbl[Tv_FileIdx].StrFn, FALSE, FALSE );
				if ( Tv_ObjFl->IsOpened() )
				{
					Tv_ReadSize		=	Tv_PtrItm->ChukTbl[Tv_ChukIdx].ContSize;
					Tv_ObjFl->Seek( Tv_PtrItm->ChukTbl[Tv_ChukIdx].Fptr );
					if ( Tv_ReadSize == Tv_ObjFl->Read( A_PtrChunk, Tv_ReadSize ) )
					{
						//calculate frame count
						*A_PtrRtFrmCnt		=	FindFrmCnt( A_PtrChunk, Tv_ReadSize );
					}
					else
					{
						//bad data
						Tv_Result		=	FALSE;
					}
				}
				delete	Tv_ObjFl;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrW32RiBk1::OpenByPath( __s8* A_StrPath )
{
	// local -------------------
		WIN32_FIND_DATA		Tv_FileFindData;
		Def_StrShort			Tv_StrFind;
		HANDLE						Tv_HndlFind;
	// code --------------------
		//init
		m_FileCnt			=	0;
		GrDumyZeroMem( &m_FileTbl, sizeof(m_FileTbl) );

		//search file list
		GrStrCopy( Tv_StrFind, A_StrPath );
		GrStrFnAddBkSlsh( Tv_StrFind );
		GrStrCat( Tv_StrFind, E_GrW32RiBk1FileFilter );
		Tv_HndlFind		=	FindFirstFile( Tv_StrFind, &Tv_FileFindData );
		if ( INVALID_HANDLE_VALUE != Tv_HndlFind )
		{
			//continue find
			do 
			{
				//add item
				GrStrCopy( m_FileTbl[m_FileCnt].StrFn, A_StrPath );
				GrStrFnAddBkSlsh( m_FileTbl[m_FileCnt].StrFn );
				GrStrCat( m_FileTbl[m_FileCnt].StrFn, Tv_FileFindData.cFileName );
				//go next
				m_FileCnt ++;
			} while( FindNextFile( Tv_HndlFind, &Tv_FileFindData ) );

			//finish
			FindClose( Tv_HndlFind );
		}

		InitFileTbl();

}
//--------------------------------------------------------------------
BOOL8	Cls_GrW32RiBk1::FindFccAv( void* A_PtrBuf, __u32 A_BufSize, __u32* A_PtrRtOfs )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8*		Tv_PtrChk;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check buf size
		if (sizeof(St_GrDvrFls16v1FrmHd) <= A_BufSize  )
		{
			//find frame
			Tv_PtrChk		=	(__u8*)A_PtrBuf;
			for ( Tv_WkIdx=0;Tv_WkIdx <= (A_BufSize - sizeof(St_GrDvrFls16v1FrmHd));Tv_WkIdx++ )
			{
				if (	( E_GrDvrFls16v1VdoFccId == Ptr_GrDvrFls16v1FrmHd(Tv_PtrChk)->FccId ) ||
							( E_GrDvrFls16v1AdoFccId == Ptr_GrDvrFls16v1FrmHd(Tv_PtrChk)->FccId ) )
				{
					//finded
					Tv_Result		=	TRUE;
					*A_PtrRtOfs	=	Tv_WkIdx;
					break;
				}
				//next
				Tv_PtrChk ++;				
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrW32RiBk1::IsPassExist( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( 0 != m_FileCnt )
		{
			if ( GrStrIsExist( m_FileTbl[0].Fh.StrPass ) )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrW32RiBk1::GetPassword( __s8* A_StrRt )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( IsPassExist() )
		{
			GrStrCopy( A_StrRt, m_FileTbl[0].Fh.StrPass );
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrW32RiBk1::Reset( void )
{
		m_FileCnt			=	0;
		GrDumyZeroMem( &m_FileTbl, sizeof(m_FileTbl) );
		m_TimeRng.TimeSt		=	0;
		m_TimeRng.TimeEd		=	0;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrW32RiBk1::GetChName( __u32 A_Ch, __s8* A_StrRt )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (E_GrW32RiBk1MaxChCnt > A_Ch) && (0 != m_FileCnt) )
		{
			GrStrCopy( A_StrRt, m_FileTbl[0].Fh.StrId[A_Ch] );
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrW32RiBk1::LcPassDec( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrW32RiBk1PassMax;Tv_WkIdx++ )
		{
			if ( 0 == m_FileTbl[0].Fh.StrPass[Tv_WkIdx] )
			{
				break;
			}
			m_FileTbl[0].Fh.StrPass[Tv_WkIdx]	=	m_FileTbl[0].Fh.StrPass[Tv_WkIdx] ^ E_GrW32RiBk1PassXorVal;
		}
}
//--------------------------------------------------------------------

