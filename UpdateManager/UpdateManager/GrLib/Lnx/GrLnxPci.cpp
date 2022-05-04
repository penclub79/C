/*
	linux pci resource tool
*/

//====================================================================
// uses
#include	<Lnx/GrLnxPci.h>
#include	<GrStrTool.h>
#include	<GrDumyTool.h>
#include	<GrFileCtrl.h>
#include	<stdlib.h>

//====================================================================
//local const
#define E_GrLnxPciStrPciPath		"/proc/bus/pci/devices"

#define E_GrLnxPciDevBufSize		0x1000

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrLnxPci::Cls_GrLnxPci()
{
	// local -------------------
		void*		Tv_PtrBuf;
		Cls_GrFileCtrl*	Tv_ObjFile;
		__u32		Tv_Size;
	// code --------------------
		//init
		m_DevCnt			=	0;
		GrDumyZeroMem( m_DevTbl, sizeof(m_DevTbl) );
		//parsing device
		Tv_ObjFile	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( E_GrLnxPciStrPciPath, FALSE, FALSE );
		if ( Tv_ObjFile->IsOpened() )
		{
			Tv_PtrBuf		=	(void*)malloc( E_GrLnxPciDevBufSize );
			if ( NULL != Tv_PtrBuf )
			{
				GrDumyZeroMem( Tv_PtrBuf, E_GrLnxPciDevBufSize );
				//load
				Tv_ObjFile->Seek( 0 );
				Tv_Size = Tv_ObjFile->Read( Tv_PtrBuf, E_GrLnxPciDevBufSize );
				if ( 0 != Tv_Size )
				{
					DevsPars( Tv_PtrBuf );
				}
				//release
				free( Tv_PtrBuf );
			}
		}
		delete	Tv_ObjFile;

}
//--------------------------------------------------------------------
Cls_GrLnxPci::~Cls_GrLnxPci()
{
}
//--------------------------------------------------------------------
void	Cls_GrLnxPci::DevsPars( void* A_PtrBuf )
{
	// local -------------------
		St_GrStrPars	Tv_PrsLn;
		Def_StrLong		Tv_StrDev;
	// code --------------------
		GrStrPrsInit( (__s8*)A_PtrBuf, &Tv_PrsLn );
		while ( GrStrPrsLine( &Tv_PrsLn, Tv_StrDev ) )
		{
			PrsLine( Tv_StrDev );
		}
}
//--------------------------------------------------------------------
void	Cls_GrLnxPci::PrsLine( __s8* A_Str )
{
	// local -------------------
		St_GrStrPars	Tv_PrsWd;
		Def_StrTag		Tv_StrItm;
		Ptr_GrLnxPciInfo	Tv_PtrInfo;
		__u32		Tv_WkIdx;
	// code --------------------
		GrStrPrsInit( A_Str, &Tv_PrsWd );
		Tv_PtrInfo		=	&m_DevTbl[m_DevCnt];
		//bus
		if ( GrStrPrsWord( &Tv_PrsWd, Tv_StrItm ) )
		{
			//pvid
			if ( GrStrPrsWord( &Tv_PrsWd, Tv_StrItm ) )
			{
				if ( GrStrHexStrToDword( Tv_StrItm, &Tv_PtrInfo->Vpid ) )
				{
					//irq
					if ( GrStrPrsWord( &Tv_PrsWd, Tv_StrItm ) )
					{
						if ( GrStrHexStrToDword( Tv_StrItm, &Tv_PtrInfo->Irq ) )
						{
							//resource
							for ( Tv_WkIdx=0;Tv_WkIdx < E_GrLnxPciMaxRsrcCnt;Tv_WkIdx++ )
							{
								Tv_PtrInfo->Rsrc[Tv_WkIdx]		=	0;
								if ( GrStrPrsWord( &Tv_PrsWd, Tv_StrItm ) )
								{
									GrStrHexStrToDword( Tv_StrItm, &Tv_PtrInfo->Rsrc[Tv_WkIdx] );
								}
							}
							//success
							m_DevCnt ++;
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrLnxPci::FindDevFirst( __u32 A_Vid, __u32 A_Pid, Ptr_GrLnxPciFind A_PtrFind )
{
	// local -------------------
	// code --------------------
		//init
		A_PtrFind->Vpid			=	(A_Vid << 16) | A_Pid;
		A_PtrFind->FindIdx	=	0;
		return	FindDevContinue( A_PtrFind );
}
//--------------------------------------------------------------------
__u32	Cls_GrLnxPci::FindDevContinue( Ptr_GrLnxPciFind A_PtrFind )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrLnxPciInfo	Tv_PtrItm;
	// code --------------------
		//init
		Tv_Result		=	0;
		//check able
		while ( m_DevCnt > A_PtrFind->FindIdx )
		{
			//get item
			Tv_PtrItm	=	&m_DevTbl[A_PtrFind->FindIdx];
			//next
			A_PtrFind->FindIdx ++;
			//check match
			if ( Tv_PtrItm->Vpid == A_PtrFind->Vpid )
			{
				//finded
				Tv_Result		=	(__u32)Tv_PtrItm;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrLnxPci::GetListFirst( Ptr_GrLnxPciFind A_PtrFind )
{
		A_PtrFind->Vpid			=	0;
		A_PtrFind->FindIdx	=	0;
		return		GetListContinue( A_PtrFind );
}
//--------------------------------------------------------------------
__u32	Cls_GrLnxPci::GetListContinue( Ptr_GrLnxPciFind A_PtrFind )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrLnxPciInfo	Tv_PtrItm;
	// code --------------------
		//init
		Tv_Result		=	0;

		if ( m_DevCnt > A_PtrFind->FindIdx )
		{
			//get item
			Tv_PtrItm	=	&m_DevTbl[A_PtrFind->FindIdx];
			//next
			A_PtrFind->FindIdx ++;
			//check match
			if ( Tv_PtrItm->Vpid == A_PtrFind->Vpid )
			{
				//finded
				Tv_Result		=	(__u32)Tv_PtrItm;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrLnxPci::GetDevCnt( void )
{
		return	m_DevCnt;
}
//--------------------------------------------------------------------
__u32	Cls_GrLnxPci::IrqGet( __u32 A_Hndl )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrLnxPciInfo	Tv_PtrItm;
	// code --------------------
		//init
		Tv_Result		=	0;

		//check item
		if ( 0 != A_Hndl )
		{
			//init
			Tv_PtrItm		=	(Ptr_GrLnxPciInfo)A_Hndl;
			Tv_Result		=	Tv_PtrItm->Irq;			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrLnxPci::RsrcGet( __u32 A_Hndl, __u32 A_Idx, BOOL8* A_PtrRtIsIo )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrLnxPciInfo	Tv_PtrItm;
	// code --------------------
		//init
		Tv_Result			=	0;
		*A_PtrRtIsIo	=	FALSE;

		//check item
		if ( (0 != A_Hndl) && (E_GrLnxPciMaxRsrcCnt > A_Idx) )
		{
			//init
			Tv_PtrItm		=	(Ptr_GrLnxPciInfo)A_Hndl;
			Tv_Result		=	Tv_PtrItm->Rsrc[A_Idx];
			if ( 0 != ( 1 & Tv_Result) )
			{
				Tv_Result			=	Tv_Result & ~1;
				*A_PtrRtIsIo	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrLnxPci::VidGet( __u32 A_Hndl )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrLnxPciInfo	Tv_PtrItm;
	// code --------------------
		//init
		Tv_Result		=	0;

		//check item
		if ( 0 != A_Hndl )
		{
			//init
			Tv_PtrItm		=	(Ptr_GrLnxPciInfo)A_Hndl;
			Tv_Result		=	((Tv_PtrItm->Vpid) >> 16) & 0xFFFF;			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrLnxPci::PidGet( __u32 A_Hndl )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrLnxPciInfo	Tv_PtrItm;
	// code --------------------
		//init
		Tv_Result		=	0;

		//check item
		if ( 0 != A_Hndl )
		{
			//init
			Tv_PtrItm		=	(Ptr_GrLnxPciInfo)A_Hndl;
			Tv_Result		=	Tv_PtrItm->Vpid & 0xFFFF;			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

