/*
 GAUSE file system version 2 DEMUX

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <Dvr/Fs/V2/GrDvrFs2Dmux.h>

#include	<stdlib.h>
#include <Dvr/Fs/V2/GrDvrFs2Disk.h>

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
Cls_GrDvrFs2Dmux::Cls_GrDvrFs2Dmux( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs2Mng* A_FsMng, Ptr_GrDvrFs2SeekCtrl A_PtrSeekCtrl, BOOL8 A_IsSafe ):
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		//init
		m_TaskSvr	=	A_TaskSvr;
		// m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		//init
		m_FsMng				=	A_FsMng;
		m_IsSafe			=	A_IsSafe;

		m_DevType			=	E_GrDvrRecDevNoraml;

		m_PtrSeekCtrl	=	A_PtrSeekCtrl;

		m_PtrFrm			=	NULL;
}
//--------------------------------------------------------------------
Cls_GrDvrFs2Dmux::~Cls_GrDvrFs2Dmux()
{
	// local -------------------
	// code --------------------

		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::FrmNext( void )
{
	// local -------------------
		__u8	Tv_BufIdx;
		__u32	Tv_Prgs;
		__u32	Tv_Readed;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		__u32	Tv_FrmSize;
	// code --------------------
		// init
		Tv_BufIdx	=	m_PtrSeekCtrl->BufIdx;
		Tv_Prgs		=	m_PtrSeekCtrl->Buf[Tv_BufIdx].Prgs;
		Tv_Readed	=	m_PtrSeekCtrl->Buf[Tv_BufIdx].Readed;

		// get frame size
		Tv_FrmSize	=	E_GrDvrFs2StrmAlignSize;
		if ( (Tv_Readed - Tv_Prgs ) >= sizeof(St_GrDvrAvHd) )
		{
			Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_PtrSeekCtrl->Buf[Tv_BufIdx].Ptr + Tv_Prgs );
			if ( E_GrDvrFs2StrmFcc == Tv_PtrAvHd->Fcc )
			{
				Tv_FrmSize	=	E_GrDvrFs2StrmAlignMask & ( Tv_PtrAvHd->DataSize + sizeof(St_GrDvrAvHd) + E_GrDvrRecTailMarkSize + E_GrDvrFs2StrmAlignSize - 1 );
				if ( E_GrDvrFs2DmuxStrmLimite < Tv_FrmSize )
				{
					Tv_FrmSize	=	E_GrDvrFs2StrmAlignSize;
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs2Dmux::FrmNext - bad FCC date.\n" );
			}
		}

		// move next
		if ( (Tv_Prgs + Tv_FrmSize) <= Tv_Readed )
		{
			m_PtrSeekCtrl->Buf[Tv_BufIdx].Prgs	=	Tv_Prgs + Tv_FrmSize;
			LcFrmPtrUpdt();
			m_PtrSeekCtrl->Stat	=	E_GrDvrSeekStatNormal;
		}
		else
		{
			m_PtrFrm	=	NULL;
			// load next
			if ( PrepareLoadFw() )
			{
				if ( m_IsSafe )
				{
					m_FsMng->SafeReadForward( m_DevType, m_PtrSeekCtrl );
				}
				else
				{
					m_FsMng->ImmReadForward( m_DevType, m_PtrSeekCtrl );
				}
				AfterLoadFw();

				LcFrmPtrUpdt();		// update frame
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::FrmPrev( void )
{
	// local -------------------
		__u8	Tv_BufIdx;
		__u32	Tv_Prgs;
		__u32	Tv_Readed;
		__u32	Tv_Tail;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		__u32	Tv_FrmSize;
		__u32	Tv_EstmSize;
	// code --------------------
		// init
		Tv_BufIdx	=	m_PtrSeekCtrl->BufIdx;
		Tv_Prgs		=	m_PtrSeekCtrl->Buf[Tv_BufIdx].Prgs;
		Tv_Readed	=	m_PtrSeekCtrl->Buf[Tv_BufIdx].Readed;

		// get previous frame
		Tv_FrmSize	=	E_GrDvrFs2StrmAlignSize;
		if ( sizeof(St_GrDvrAvHd) <= Tv_Prgs )
		{
			// get tail
			Tv_Tail			=	*( (__u32*)( (__u32)m_PtrSeekCtrl->Buf[Tv_BufIdx].Ptr + Tv_Prgs - E_GrDvrRecTailMarkSize ) );
			// check tail
			if ( E_GrDvrRecTailMarkId == ( E_GrDvrRecTailMarkMask & Tv_Tail )  )
			{
				// get frame size
				Tv_EstmSize	=	Tv_Tail & E_GrDvrRecTailSizeMask;
				if ( (E_GrDvrFs2DmuxStrmLimite >= Tv_EstmSize) && (0 == (Tv_EstmSize & (~E_GrDvrFs2StrmAlignMask))) )
				{
					// check size over
					if ( Tv_Prgs >= Tv_EstmSize )
					{
						// get header
						Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_PtrSeekCtrl->Buf[Tv_BufIdx].Ptr + Tv_Prgs - Tv_EstmSize );
						if ( E_GrDvrFs2StrmFcc == Tv_PtrAvHd->Fcc )
						{
							Tv_FrmSize	=	E_GrDvrFs2StrmAlignMask & ( Tv_PtrAvHd->DataSize + sizeof(St_GrDvrAvHd) + E_GrDvrRecTailMarkSize + E_GrDvrFs2StrmAlignSize - 1 );
							if ( (Tv_EstmSize != Tv_FrmSize) || (E_GrDvrFs2DmuxStrmLimite < Tv_FrmSize) )
							{
								Tv_FrmSize	=	E_GrDvrFs2StrmAlignSize;
							}
						}
					}
					else
					{
						// read previous
						Tv_FrmSize	=	Tv_EstmSize;
					}
				}
			}
		}

		// move next
		if ( Tv_Prgs >= Tv_FrmSize )
		{
			m_PtrSeekCtrl->Buf[Tv_BufIdx].Prgs	=	Tv_Prgs - Tv_FrmSize;
			LcFrmPtrUpdt();
		}
		else
		{
			m_PtrFrm	=	NULL;
			// load previous
			if ( PrepareLoadBw() )
			{
				if ( m_IsSafe )
				{
					m_FsMng->SafeReadBackward( m_DevType, m_PtrSeekCtrl );
				}
				else
				{
					m_FsMng->ImmReadBackward( m_DevType, m_PtrSeekCtrl );
				}
				AfterLoadBw();

				LcFrmPtrUpdt();		// update frame
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::SeekBegin( void )
{
	// local -------------------
	// code --------------------
		if ( PrepareLoadFw() )
		{
			if ( m_IsSafe )
			{
				m_FsMng->SafeReadBegin( m_DevType, m_PtrSeekCtrl );
			}
			else
			{
				m_FsMng->ImmReadBegin( m_DevType, m_PtrSeekCtrl );
			}
			AfterLoadFw();

			LcFrmPtrUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::SeekEnd( void )
{
	// local -------------------
	// code --------------------
		if ( PrepareLoadBw() )
		{
			if ( m_IsSafe )
			{
				m_FsMng->SafeReadEnd( m_DevType, m_PtrSeekCtrl );
			}
			else
			{
				m_FsMng->ImmReadEnd( m_DevType, m_PtrSeekCtrl );
			}
			AfterLoadBw();

			LcFrmPtrUpdt();
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::SeekByHndlFw( __u64 A_Hndl )
{
	// local -------------------
	// code --------------------
		if ( PrepareLoadFw() )
		{
			if ( m_IsSafe )
			{
				m_FsMng->SafeReadByHndlFw( m_DevType, m_PtrSeekCtrl, A_Hndl );
			}
			else
			{
				m_FsMng->ImmReadByHndlFw( m_DevType, m_PtrSeekCtrl, A_Hndl );
			}
			AfterLoadFw();

			LcFrmPtrUpdt();
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::SeekByHndlBw( __u64 A_Hndl )
{
	// local -------------------
	// code --------------------
		if ( PrepareLoadBw() )
		{
			if ( m_IsSafe )
			{
				m_FsMng->SafeReadByHndlBw( m_DevType, m_PtrSeekCtrl, A_Hndl );
			}
			else
			{
				m_FsMng->ImmReadByHndlBw( m_DevType, m_PtrSeekCtrl, A_Hndl );
			}
			AfterLoadBw();

			LcFrmPtrUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::SeekByMinFw( Def_GrTime A_Time )
{
	// local -------------------
	// code --------------------
		if ( PrepareLoadFw() )
		{
			if ( m_IsSafe )
			{
				m_FsMng->SafeReadByMinFw( m_DevType, m_PtrSeekCtrl, A_Time );
			}
			else
			{
				m_FsMng->ImmReadByMinFw( m_DevType, m_PtrSeekCtrl, A_Time );
			}
			AfterLoadFw();

			LcFrmPtrUpdt();
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::SeekByMinBw( Def_GrTime A_Time )
{
	// local -------------------
	// code --------------------
		if ( PrepareLoadBw() )
		{
			if ( m_IsSafe )
			{
				m_FsMng->SafeReadByMinBw( m_DevType, m_PtrSeekCtrl, A_Time );
			}
			else
			{
				m_FsMng->ImmReadByMinBw( m_DevType, m_PtrSeekCtrl, A_Time );
			}
			AfterLoadBw();

			LcFrmPtrUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::LcFrmPtrUpdt( void )
{
	// local -------------------
		__u8	Tv_BufIdx;
		__u32	Tv_Prgs;
		__u32	Tv_Readed;
		Ptr_GrDvrAvHd	Tv_PtrHd;
		__u32	Tv_PktSize;
		__u32	Tv_Tail;
	// code --------------------
		// init
		m_PtrFrm	=	NULL;
		Tv_BufIdx	=	m_PtrSeekCtrl->BufIdx;
		Tv_Prgs		=	m_PtrSeekCtrl->Buf[Tv_BufIdx].Prgs;
		Tv_Readed	=	m_PtrSeekCtrl->Buf[Tv_BufIdx].Readed;

		// check current range
		if ( (Tv_Prgs + sizeof(St_GrDvrAvHd)) <= Tv_Readed )
		{
			Tv_PtrHd	=	(Ptr_GrDvrAvHd)( (__u32)m_PtrSeekCtrl->Buf[Tv_BufIdx].Ptr + Tv_Prgs );
			// check FCC
			if ( E_GrDvrFs2StrmFcc == Tv_PtrHd->Fcc )
			{
				// check size
				Tv_PktSize	=	E_GrDvrFs2StrmAlignMask & ( sizeof(St_GrDvrAvHd) + Tv_PtrHd->DataSize + E_GrDvrRecTailMarkSize + E_GrDvrFs2StrmAlignSize - 1 );
				if ( (Tv_PktSize + Tv_Prgs) <= Tv_Readed  )
				{
					// check tail
					Tv_Tail	=	*( (__u32*)( (__u32)Tv_PtrHd + Tv_PktSize - E_GrDvrRecTailMarkSize ) );
					if ( E_GrDvrRecTailMarkId == (E_GrDvrRecTailMarkMask & Tv_Tail) )
					{
						if ( Tv_PktSize == (E_GrDvrRecTailSizeMask & Tv_Tail) )
						{
							// found
							m_PtrFrm	=	(void*)Tv_PtrHd;
							// check av type
							if ( E_GrDvrAvTypVdoB >= Tv_PtrHd->Type )
							{
								if ( E_GrDvrMaxChCnt <= Tv_PtrHd->Ch )
								{
									m_PtrFrm	=	NULL;
								}
							}

						}	//	if ( Tv_PktSize == (E_GrDvrRecTailSizeMask & Tv_Tail) )
					}
				}	//	if ( (Tv_PktSize + Tv_Prgs) <= Tv_Readed  )

			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Dmux::PrepareLoadFw( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::AfterLoadFw( void )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Dmux::PrepareLoadBw( void )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Dmux::AfterLoadBw( void )
{

}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Dmux::SeekStatGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrSeekStatNormal;
		if ( NULL != m_PtrSeekCtrl )
		{
			Tv_Result	=	m_PtrSeekCtrl->Stat;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
