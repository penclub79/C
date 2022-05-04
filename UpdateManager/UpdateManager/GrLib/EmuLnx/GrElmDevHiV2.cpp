/*
	emulation linux HiMPI_V2 device
*/

//====================================================================
//include
#include	<EmuLnx/GrElmDevHiV2.h>
#include	<GrDumyTool.h>
#include	<GrBufTool.h>
#include	<GrGdibTool.h>
#include	<GrFileCtrl.h>
#include	<Dvr/GrDvrBase.h>

#include <hi_tde_api.h>
#include <hi_tde_type.h>
#include <hi_tde_errcode.h>

//====================================================================
//local const
#define E_GrElmDevHiV2MaxObjCnt		2

//====================================================================
//local type

//====================================================================
//global var
Cls_GrElmDevHiV2*	V_GrElmDevHiV2Obj[E_GrElmDevHiV2MaxObjCnt]	=	
{	NULL, NULL };
file_operations			V_GrElmDevHiV2Fo;

int		V_GrEmlDevHiV2FbNodeTbl[E_GrElmDevHiV2FbCnt]	=	
{ -1, -1, -1, -1, -1, -1, -1 };

//====================================================================
//functions

int GrEmuDevHiV2_ioctl (struct inode * A_PtrInode, struct file * A_PtrFile, DWORD A_Para1, DWORD A_Para2);
int GrEmuDevHiV2_mmap (struct file * A_PtrFile, struct vm_area_struct * A_PtrVm);
int GrEmuDevHiV2_open (struct inode * A_PtrInode, struct file * A_PtrFile);
int GrEmuDevHiV2_release (struct inode * A_PtrInode, struct file * A_PtrFile);

//--------------------------------------------------------------------
Cls_GrElmDevHiV2::Cls_GrElmDevHiV2( DWORD A_DevIdx, BYTE A_FbIdx ):
Cls_GrElmDevBase( A_DevIdx )
{
	// local -------------------
		cdev	Tv_Cdev;
		BYTE	Tv_WkIdx;
	// code --------------------
		//init
		m_FbIdx	=	A_FbIdx;
		
		cdev_init( &Tv_Cdev, &V_GrElmDevHiV2Fo );
		cdev_add( &Tv_Cdev, m_Node, 1 );		// register ioctl

		m_PtrFb	=	malloc( E_GrElmDevHiV2FbSize );

		V_GrElmDevHiV2Obj[m_FbIdx]	=	this;

		if ( 0 == m_FbIdx )
		{
			// emulate video
			m_EmuVdoCnt		=	0;
			m_EmuVdoWait	=	0;
			m_VencCh			=	0;
			GrDumyZeroMem( m_VencTbl, sizeof(m_VencTbl) );
			for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++)
			{
				m_VencTbl[Tv_WkIdx].IsOn	=	TRUE;
			}
			LcEmuAvLoad();
		}
}
//--------------------------------------------------------------------
Cls_GrElmDevHiV2::~Cls_GrElmDevHiV2()
{
	// local -------------------
	// code --------------------
		if ( NULL != m_PtrFb )
		{
			free( m_PtrFb );
			m_PtrFb	=	NULL;
		}

		V_GrElmDevHiV2Obj[m_FbIdx]	=	NULL;
}
//--------------------------------------------------------------------
void*	Cls_GrElmDevHiV2::Mmap( DWORD A_Offset, DWORD A_Size )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		// check allocated frame buffer
		if ( NULL != m_PtrFb )
		{
			if ( (0 != A_Size) && (E_GrElmDevHiV2FbSize >= (A_Offset + A_Size)) )
			{
				Tv_Result	=	(void*)( (DWORD)m_PtrFb + A_Offset );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int GrEmuDevHiV2_ioctl (struct inode * A_PtrInode, struct file * A_PtrFile, DWORD A_Para1, DWORD A_Para2)
{
	// local -------------------
	// code --------------------
		return	0;
}
//--------------------------------------------------------------------
int GrEmuDevHiV2_mmap (struct file * A_PtrFile, struct vm_area_struct * A_PtrVm)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		if ( NULL != V_GrElmDevHiV2Obj[A_PtrFile->Handle] )
		{
			A_PtrVm->MapAdr	=	V_GrElmDevHiV2Obj[A_PtrFile->Handle]->Mmap( 
				A_PtrVm->vm_pgoff, A_PtrVm->vm_end );
			if ( NULL != A_PtrVm->MapAdr )
			{
				Tv_Result	=	0;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
int GrEmuDevHiV2_open (struct inode * A_PtrInode, struct file * A_PtrFile)
{
	// local -------------------
	// code --------------------

		return	0;
}
//--------------------------------------------------------------------
int GrEmuDevHiV2_release (struct inode * A_PtrInode, struct file * A_PtrFile)
{
	// local -------------------
	// code --------------------
		return	0;
}
//--------------------------------------------------------------------
void	Cls_GrElmDevHiV2::LcEmuAvLoad( void )
{
	// local -------------------
		DWORD		Tv_VdoSize;
		Cls_GrFileCtrl*	Tv_Fc;
		Ptr_GrElmDevHiV2FrmHd	Tv_PtrFrm;
		DWORD		Tv_LoadSize;
		DWORD		Tv_Fptr;
		BOOL		Tv_IsStart;
		void*		Tv_PtrData;
	// code --------------------
		//init
		Tv_VdoSize		=	0;
		m_EmuVdoCnt		=	0;
		Tv_IsStart		=	FALSE;

		Tv_Fc	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( L"e:\\EmuD1\\avdata.dat", FALSE, FALSE, FALSE, TRUE );
		if ( Tv_Fc->IsOpened() )
		{
			//go first frame
			Tv_Fc->Seek( 0x200000 );

			while ( TRUE )
			{
				//check data finished
				if ( (Tv_VdoSize + sizeof(St_GrDvrFrmHdVdo)) >= E_GrElmDevHiV2AvBufSize )
				{
					//finished
					break;
				}
				//load frame
				Tv_PtrFrm		=	(Ptr_GrElmDevHiV2FrmHd)((DWORD)m_EmuVdoBuf + Tv_VdoSize);
				Tv_LoadSize	=	Tv_Fc->Read(Tv_PtrFrm, sizeof(St_GrElmDevHiV2FrmHd));
				if (Tv_LoadSize != sizeof(St_GrElmDevHiV2FrmHd))
				{
					//data ended
					break;
				}
				//check valid frame
				if ( 0x5641444E == Tv_PtrFrm->Fcc )
				{
					//check video frame and channel
					if (	(0 == Tv_PtrFrm->Ch) && (1 >= Tv_PtrFrm->Type) && 
						( Tv_IsStart || ( 0 == Tv_PtrFrm->Type ) )
						)
					{
						//copy contents
						Tv_PtrData	=	(void*)((DWORD)Tv_PtrFrm + sizeof(St_GrElmDevHiV2FrmHd));
						Tv_LoadSize	=	(Tv_PtrFrm->DataSize + 0x1F) & (~0x1F);
						if ((Tv_VdoSize + sizeof(St_GrElmDevHiV2FrmHd)+ Tv_LoadSize) >= E_GrElmDevHiV2AvBufSize)
						{
							//finished
							break;
						}
						if ( Tv_LoadSize != Tv_Fc->Read( Tv_PtrData, Tv_LoadSize ) )
						{
							//data finished
							break;
						}
						Tv_IsStart	=	TRUE;
						m_EmuVdoCnt ++;
						Tv_VdoSize	=	Tv_VdoSize + Tv_LoadSize + sizeof(St_GrElmDevHiV2FrmHd);
					}
					else
					{
						//skip contents
						Tv_Fptr	=	(DWORD)Tv_Fc->GetPos() + Tv_PtrFrm->DataSize;
						Tv_Fptr	=	(Tv_Fptr + 0x1F) & (~0x1F);
						if ( Tv_Fptr != (DWORD)Tv_Fc->Seek( Tv_Fptr ) )
						{
							//data end, finished
							break;
						}
					}
				}
				else
				{
					//bad stream
					break;
				}

				//next

			}

		}
		else
		{
			DbgMsgPrint( "Cls_GrElmDevHiV2::LcEmuAvLoad - emulation av data open fail.\n" );
		}
		delete	Tv_Fc;
}
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_Init()
	{
		// local -------------------
			HI_S32	Tv_Result;
		// code --------------------
			Tv_Result	=	HI_FAILURE;

			// init FO table
			V_GrElmDevHiV2Fo.open			=	GrEmuDevHiV2_open;
			V_GrElmDevHiV2Fo.release	=	GrEmuDevHiV2_release;
			V_GrElmDevHiV2Fo.ioctl		=	GrEmuDevHiV2_ioctl;
			V_GrElmDevHiV2Fo.mmap			=	GrEmuDevHiV2_mmap;

			// register driver 1
			V_GrEmlDevHiV2FbNodeTbl[1]	=	GrElaDrvAdd( "fb1", &V_GrElmDevHiV2Fo );
			if ( 0 <= V_GrEmlDevHiV2FbNodeTbl[1] )
			{

				Tv_Result	=	HI_SUCCESS;
			}

			return	Tv_Result;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_Exit()
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_SetConf(const MPP_SYS_CONF_S *pstSysConf)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_GetConf(MPP_SYS_CONF_S *pstSysConf)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32  HI_MPI_SYS_Bind(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32  HI_MPI_SYS_UnBind(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32  HI_MPI_SYS_GetBindbyDest(MPP_CHN_S *pstDestChn, MPP_CHN_S *pstSrcChn)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_GetVersion(MPP_VERSION_S *pstVersion)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_GetCurPts(HI_U64 *pu64CurPts)
	{
		*pu64CurPts	=	0;
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_InitPtsBase(HI_U64 u64PtsBase)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_SyncPts(HI_U64 u64PtsBase)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_MmzAlloc(HI_U32 *pu32PhyAddr, HI_VOID **ppVirtAddr, 
        const HI_CHAR *strMmb, const HI_CHAR *strZone, HI_U32 u32Len)
	{
		// local -------------------
			HI_S32	Tv_Result;
		// code --------------------
			Tv_Result	=	HI_FAILURE;

			*ppVirtAddr	=	malloc( u32Len );
			if ( NULL != (*ppVirtAddr) )
			{
				*pu32PhyAddr	=	(HI_U32)*ppVirtAddr;
				Tv_Result	=	HI_SUCCESS;
			}
			return	Tv_Result;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_MmzAlloc_Cached(HI_U32 *pu32PhyAddr, HI_VOID **ppVitAddr, 
        const HI_CHAR *pstrMmb, const HI_CHAR *pstrZone, HI_U32 u32Len)
	{
		// local -------------------
			HI_S32	Tv_Result;
		// code --------------------
			Tv_Result	=	HI_FAILURE;

			*ppVitAddr	=	malloc( u32Len );
			if ( NULL != (*ppVitAddr) )
			{
				*pu32PhyAddr	=	(HI_U32)*ppVitAddr;
				Tv_Result	=	HI_SUCCESS;
			}
			return	Tv_Result;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_MmzFree(HI_U32 u32PhyAddr, HI_VOID *pVirtAddr)
	{
		// local -------------------
			
		// code --------------------
			if ( NULL != pVirtAddr )
			{
				free( pVirtAddr );
			}

			return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_MmzFlushCache(HI_U32 u32PhyAddr, HI_VOID *pVitAddr, HI_U32 u32Size)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_VOID * HI_MPI_SYS_Mmap(HI_U32 u32PhyAddr, HI_U32 u32Size)
	{
		return	(HI_VOID*)u32PhyAddr;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_Munmap(HI_VOID* pVirAddr, HI_U32 u32Size)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_SetReg(HI_U32 u32Addr, HI_U32 u32Value)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_GetReg(HI_U32 u32Addr, HI_U32 *pu32Value)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_SetMemConf(MPP_CHN_S *pstMppChn,const HI_CHAR *pcMmzName)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_GetMemConf(MPP_CHN_S *pstMppChn,HI_CHAR *pcMmzName)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_SYS_CloseFd(HI_VOID)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_VI_SetDevAttr(VI_DEV ViDev, const VI_DEV_ATTR_S *pstDevAttr)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_VI_GetDevAttr(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_VI_EnableDev(VI_DEV ViDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_DisableDev(VI_DEV ViDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_SetChnAttr(VI_CHN ViChn, const VI_CHN_ATTR_S *pstAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_GetChnAttr(VI_CHN ViChn, VI_CHN_ATTR_S *pstAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_SetChnMinorAttr(VI_CHN ViChn,const VI_CHN_ATTR_S *pstAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_GetChnMinorAttr(VI_CHN ViChn,VI_CHN_ATTR_S *pstAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_ClearChnMinorAttr(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_EnableChn(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_DisableChn(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_EnableChnInterrupt(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_DisableChnInterrupt(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_GetFrame(VI_CHN ViChn, VIDEO_FRAME_INFO_S *pstFrameInfo, HI_S32 s32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_GetFrameTimeOut(VI_CHN ViChn, VIDEO_FRAME_INFO_S *pstFrameInfo, HI_U32 u32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_ReleaseFrame(VI_CHN ViChn, VIDEO_FRAME_INFO_S *pstFrameInfo)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_SetFrameDepth(VI_CHN ViChn, HI_U32 u32Depth)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_GetFrameDepth(VI_CHN ViChn, HI_U32 *pu32Depth)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_SetUserPic(VI_CHN ViChn, VI_USERPIC_ATTR_S *pstUsrPic)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_EnableUserPic(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_DisableUserPic(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_EnableCascade(VI_DEV ViDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_DisableCascade(VI_DEV ViDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_EnableCascadeChn(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_DisableCascadeChn(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_ChnBind(VI_CHN ViChn, const VI_CHN_BIND_ATTR_S *pstChnBindAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_ChnUnBind(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_GetChnBind(VI_CHN ViChn, VI_CHN_BIND_ATTR_S *pstChnBindAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_SetDevAttrEx(VI_DEV ViDev, const VI_DEV_ATTR_EX_S *pstDevAttrEx)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_GetDevAttrEx(VI_DEV ViDev, VI_DEV_ATTR_EX_S *pstDevAttrEx)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_GetFd(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_Query(VI_CHN ViChn, VI_CHN_STAT_S *pstStat)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_SetSkipMode(VI_CHN ViChn, VI_SKIP_MODE_E enSkipMode)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_SetVbiAttr(VI_CHN ViChn, VI_VBI_ARG_S *pstVbiAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_EnableVbi(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VI_DisableVbi(VI_CHN ViChn)
	{
		return	HI_SUCCESS;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_VO_Enable (VO_DEV VoDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_Disable(VO_DEV VoDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetPubAttr(VO_DEV VoDev, const VO_PUB_ATTR_S *pstPubAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetPubAttr(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_CloseFd(HI_VOID)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_EnableVideoLayer (VO_DEV VoDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_DisableVideoLayer(VO_DEV VoDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetVideoLayerAttr(VO_DEV VoDev, const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetVideoLayerAttr(VO_DEV VoDev, VO_VIDEO_LAYER_ATTR_S *pstLayerAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_PipLayerBindDev(VO_DEV VoTargetDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_PipLayerUnBindDev(VO_DEV VoTargetDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetPipLayerAttr(const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetPipLayerAttr(VO_VIDEO_LAYER_ATTR_S *pstLayerAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_EnablePipLayer (HI_VOID)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_DisablePipLayer(HI_VOID)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_EnableChn (VO_DEV VoDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_DisableChn(VO_DEV VoDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetChnAttr(VO_DEV VoDev, VO_CHN VoChn, const VO_CHN_ATTR_S *pstChnAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetChnAttr(VO_DEV VoDev, VO_CHN VoChn, VO_CHN_ATTR_S *pstChnAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetChnDispPos(VO_DEV VoDev, VO_CHN VoChn, const POINT_S *pstDispPos)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetChnDispPos(VO_DEV VoDev, VO_CHN VoChn, POINT_S *pstDispPos)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetChnField(VO_DEV VoDev, VO_CHN VoChn, const VO_DISPLAY_FIELD_E enField)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetChnField(VO_DEV VoDev, VO_CHN VoChn, VO_DISPLAY_FIELD_E *pField)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetChnFrameRate(VO_DEV VoDev, VO_CHN VoChn, HI_S32 s32ChnFrmRate)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetChnFrameRate(VO_DEV VoDev, VO_CHN VoChn, HI_S32 *ps32ChnFrmRate)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetChnFrame(VO_LAYER VoLayer, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstFrame, HI_S32 s32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_ReleaseChnFrame(VO_LAYER VoLayer, VO_CHN VoChn, const VIDEO_FRAME_INFO_S *pstFrame)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_ChnPause (VO_DEV VoDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_ChnResume(VO_DEV VoDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_ChnStep  (VO_DEV VoDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_ChnRefresh(VO_DEV VoDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_ChnShow(VO_DEV VoDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_ChnHide(VO_DEV VoDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetZoomInWindow(VO_DEV VoDev, VO_CHN VoChn, const VO_ZOOM_ATTR_S *pstZoomAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetZoomInWindow(VO_DEV VoDev, VO_CHN VoChn, VO_ZOOM_ATTR_S *pstZoomAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetChnPts   (VO_DEV VoDev, VO_CHN VoChn, HI_U64 *pu64ChnPts)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_QueryChnStat(VO_DEV VoDev, VO_CHN VoChn, VO_QUERY_STATUS_S *pstStatus)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SendFrame(VO_LAYER VoLayer, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstVFrame, HI_S32 s32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SendFrameTimeOut(VO_DEV VoDev, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstVFrame, HI_U32 u32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_ClearChnBuffer(VO_DEV VoDev, VO_CHN VoChn, HI_BOOL bClrAll)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetAttrBegin(VO_DEV VoDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetAttrEnd  (VO_DEV VoDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetPlayToleration(VO_DEV VoDev, HI_U32 u32Toleration)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetPlayToleration(VO_DEV VoDev, HI_U32 *pu32Toleration)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetScreenFrame(VO_LAYER VoLayer, VIDEO_FRAME_INFO_S *pstVFrame, HI_S32 s32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_ReleaseScreenFrame(VO_DEV VoDev, VIDEO_FRAME_INFO_S *pstVFrame)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetDispBufLen(VO_DEV VoDev, HI_U32 u32BufLen)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetDispBufLen(VO_DEV VoDev, HI_U32 *pu32BufLen)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_EnableWbc(VO_DEV VoDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_DisableWbc(VO_DEV VoDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetWbcAttr(VO_DEV VoDev, const VO_WBC_ATTR_S *pstWbcAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetWbcAttr(VO_DEV VoDev, VO_WBC_ATTR_S *pstWbcAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetWbcMode(VO_DEV VoDev, VO_WBC_MODE_E enWbcMode)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetWbcMode(VO_DEV VoDev, VO_WBC_MODE_E *penWbcMode)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetWbcDepth(VO_DEV VoDev, HI_U32 u32Depth)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetWbcDepth(VO_DEV VoDev, HI_U32 *pu32Depth)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_WbcGetScreenFrame(VO_DEV VoDev, VIDEO_FRAME_INFO_S *pstVFrame)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_WbcReleaseScreenFrame(VO_DEV VoDev, VIDEO_FRAME_INFO_S *pstVFrame)
	{
		return	HI_SUCCESS;
	}
	
		HI_S32 HI_MPI_VO_SetCascadeAttr(const VO_CAS_ATTR_S *pstCasAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetCascadeAttr(VO_CAS_ATTR_S *pstCasAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_EnableCascadeDev (VO_DEV VoCasDev)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_DisableCascadeDev(VO_DEV VoCasDev)
	{
		return	HI_SUCCESS;
	}
	
	HI_S32 HI_MPI_VO_SetCascadePattern(VO_DEV VoCasDev, HI_U32 u32Pattern)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetCascadePattern(VO_DEV VoCasDev, HI_U32 *pu32Pattern)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_CascadePosBindChn  (HI_U32 u32Pos, VO_DEV VoCasDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_CascadePosUnBindChn(HI_U32 u32Pos, VO_DEV VoCasDev, VO_CHN VoChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_EnableCascade (HI_VOID)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_DisableCascade(HI_VOID)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetDevCSC(VO_DEV VoDev, VO_CSC_S *pstDevCSC)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetDevCSC(VO_DEV VoDev, VO_CSC_S *pstDevCSC)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_GetVgaParam(VO_DEV VoDev, VO_VGA_PARAM_S *pstVgaParam)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VO_SetVgaParam(VO_DEV VoDev, VO_VGA_PARAM_S *pstVgaParam)
	{
		return	HI_SUCCESS;
	}

};
//--------------------------------------------------------------------
extern	"C"
{

	HI_S32 HI_MPI_VPSS_CreateGrp(VPSS_GRP VpssGrp, VPSS_GRP_ATTR_S *pstGrpAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_DestroyGrp(VPSS_GRP VpssGrp)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_StartGrp(VPSS_GRP VpssGrp)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_StopGrp(VPSS_GRP VpssGrp)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_ResetGrp(VPSS_GRP VpssGrp)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_EnableChn(VPSS_GRP VpssGrp, VPSS_CHN s32VpssChnl)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_DisableChn(VPSS_GRP VpssGrp, VPSS_CHN s32VpssChnl)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetGrpAttr(VPSS_GRP VpssGrp, VPSS_GRP_ATTR_S *pstGrpAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetGrpAttr(VPSS_GRP VpssGrp, VPSS_GRP_ATTR_S *pstGrpAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetChnAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VPSS_CHN_ATTR_S *pstChnAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetChnAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VPSS_CHN_ATTR_S *pstChnAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetGrpParam(VPSS_GRP VpssGrp, VPSS_GRP_PARAM_S *pstVpssParam)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetGrpParam(VPSS_GRP VpssGrp, VPSS_GRP_PARAM_S *pstVpssParam)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetCropCfg(VPSS_GRP VpssGrp,  VPSS_CROP_INFO_S *pstCropInfo)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetCropCfg(VPSS_GRP VpssGrp,  VPSS_CROP_INFO_S *pstCropInfo)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetChnMode(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VPSS_CHN_MODE_S *pstVpssMode)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetChnMode(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VPSS_CHN_MODE_S *pstVpssMode)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetDepth(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, HI_U32 u32Depth)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetDepth(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, HI_U32 *pu32Depth)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_UserSendFrame(VPSS_GRP VpssGrp,  VIDEO_FRAME_INFO_S *pstVideoFrame)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_UserSendFrameTimeout(VPSS_GRP VpssGrp,  VIDEO_FRAME_INFO_S *pstVideoFrame, HI_U32 u32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_UserGetFrame(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,  VIDEO_FRAME_INFO_S *pstVideoFrame)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_UserReleaseFrame(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,  VIDEO_FRAME_INFO_S *pstVideoFrame)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_UserGetGrpFrame(VPSS_GRP VpssGrp, VIDEO_FRAME_INFO_S *pstVideoFrame, HI_U32 u32FrameIndex)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_UserReleaseGrpFrame(VPSS_GRP VpssGrp,  VIDEO_FRAME_INFO_S *pstVideoFrame)
	{
		return	HI_SUCCESS;
	}

		HI_S32 HI_MPI_VPSS_SetPreScale(VPSS_GRP VpssGrp,VPSS_PRESCALE_INFO_S *pstPreScaleInfo)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetPreScale(VPSS_GRP VpssGrp,VPSS_PRESCALE_INFO_S *pstPreScaleInfo)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetChnField(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VPSS_CAPSEL_E enCapSel)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetChnField(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VPSS_CAPSEL_E *enCapSel)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetGrpSizer(VPSS_GRP VpssGrp, VPSS_SIZER_INFO_S *pstVpssSizerInfo)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetGrpSizer(VPSS_GRP VpssGrp, VPSS_SIZER_INFO_S *pstVpssSizerInfo)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetGrpFrameRate(VPSS_GRP VpssGrp, VPSS_FRAME_RATE_S *pstVpssFrameRate)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetGrpFrameRate(VPSS_GRP VpssGrp, VPSS_FRAME_RATE_S *pstVpssFrameRate)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_SetDelay(VPSS_GRP VpssGrp, HI_U32 u32Delay)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VPSS_GetDelay(VPSS_GRP VpssGrp, HI_U32 *pu32Delay)
	{
		return	HI_SUCCESS;
	}


};
//--------------------------------------------------------------------
extern	"C"
{
	HI_S32 HI_MPI_HDMI_Init(HI_HDMI_INIT_PARA_S *pstHdmiPara)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_DeInit(void)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_Open(HI_HDMI_ID_E enHdmi)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_Close(HI_HDMI_ID_E enHdmi)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_GetSinkCapability(HI_HDMI_ID_E enHdmi, HI_HDMI_SINK_CAPABILITY_S *pstSinkCap)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_SetAttr(HI_HDMI_ID_E enHdmi, HI_HDMI_ATTR_S *pstAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_GetAttr(HI_HDMI_ID_E enHdmi, HI_HDMI_ATTR_S *pstAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_Start(HI_HDMI_ID_E enHdmi)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_Stop(HI_HDMI_ID_E enHdmi)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_SetAVMute(HI_HDMI_ID_E enHdmi, HI_BOOL bAvMute)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_Force_GetEDID(HI_HDMI_ID_E enHdmi, HI_HDMI_EDID_S *pstEdidData)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_SetDeepColor(HI_HDMI_ID_E enHdmi, HI_HDMI_DEEP_COLOR_E enDeepColor)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_HDMI_GetDeepColor(HI_HDMI_ID_E enHdmi, HI_HDMI_DEEP_COLOR_E *penDeepColor)
	{
		return	HI_SUCCESS;
	}


};
//--------------------------------------------------------------------
extern	"C"
{

	VB_POOL HI_MPI_VB_CreatePool(HI_U32 u32BlkSize,HI_U32 u32BlkCnt,const HI_CHAR *pcMmzName)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VB_DestroyPool(VB_POOL Pool)
	{
		return	HI_SUCCESS;
	}

	VB_BLK HI_MPI_VB_GetBlock(VB_POOL Pool, HI_U32 u32BlkSize,const HI_CHAR *pcMmzName)
	{
		// local -------------------
			void*	Tv_Result;
		// code --------------------
			Tv_Result	=	malloc( u32BlkSize );
			if ( NULL == Tv_Result )
			{
				Tv_Result	=	(void*)0xFFFFFFFF;
			}
			return	(VB_BLK)Tv_Result;
	}

	HI_S32 HI_MPI_VB_ReleaseBlock(VB_BLK Block)
	{
		return	HI_SUCCESS;
	}

	HI_U32 HI_MPI_VB_Handle2PhysAddr(VB_BLK Block)
	{
		return	(HI_U32)Block;
	}

	VB_POOL HI_MPI_VB_Handle2PoolId(VB_BLK Block)
	{
		return	(VB_POOL)Block;
	}

	HI_S32 HI_MPI_VB_Init (HI_VOID)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VB_Exit (HI_VOID)
	{
		return	HI_SUCCESS;
	}
		
	HI_S32 HI_MPI_VB_SetConf (const VB_CONF_S *pstVbConf)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VB_GetConf (VB_CONF_S *pstVbConf)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VB_MmapPool(VB_POOL Pool)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VB_MunmapPool(VB_POOL Pool)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VB_GetBlkVirAddr(VB_POOL Pool, HI_U32 u32PhyAddr, HI_VOID **ppVirAddr)
	{
		return	HI_SUCCESS;
	}

};
//--------------------------------------------------------------------
//--------------------------------------------------------------------
extern	"C"
{

	HI_S32 HI_MPI_VDEC_CreateChn(VDEC_CHN VdChn, const VDEC_CHN_ATTR_S *pstAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_DestroyChn(VDEC_CHN VdChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_GetChnAttr(VDEC_CHN VdChn, VDEC_CHN_ATTR_S *pstAttr)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_SendStream(VDEC_CHN VdChn, const VDEC_STREAM_S *pstStream, HI_S32 s32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_StartRecvStream(VDEC_CHN VdChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_StopRecvStream(VDEC_CHN VdChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_Query(VDEC_CHN VdChn,VDEC_CHN_STAT_S *pstStat)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_GetFd(VDEC_CHN VdChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_ResetChn(VDEC_CHN VdChn)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_QueryData(VDEC_CHN VdChn,HI_BOOL *pbIsData)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_SetChnParam(VDEC_CHN VdChn, VDEC_CHN_PARAM_S* pstParam)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_GetChnParam(VDEC_CHN VdChn, VDEC_CHN_PARAM_S* pstParam)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_SetPrtclParam(VDEC_CHN VdChn,VDEC_PRTCL_PARAM_S *pstParam)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_GetPrtclParam(VDEC_CHN VdChn,VDEC_PRTCL_PARAM_S *pstParam)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_GetImage(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo,HI_S32 s32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_ReleaseImage(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_GetUserData(VDEC_CHN VdChn, VDEC_USERDATA_S *pstUserData, HI_S32 s32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_ReleaseUserData(VDEC_CHN VdChn, VDEC_USERDATA_S *pstUserData)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_SendStream_TimeOut(VDEC_CHN VdChn, const VDEC_STREAM_S *pstStream, HI_U32 u32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_GetImage_TimeOut(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo,HI_U32 u32MilliSec)
	{
		return	HI_SUCCESS;
	}

	HI_S32 HI_MPI_VDEC_GetUserData_TimeOut(VDEC_CHN VdChn, VDEC_USERDATA_S *pstUserData, HI_U32 u32MilliSec)
	{
		return	HI_SUCCESS;
	}

};
//--------------------------------------------------------------------
HI_S32 HI_MPI_VENC_CreateGroup(VENC_GRP VeGroup)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_DestroyGroup(VENC_GRP VeGroup)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_CreateChn(VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_DestroyChn(VENC_CHN VeChn)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_RegisterChn(VENC_GRP VeGroup, VENC_CHN VeChn)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_UnRegisterChn(VENC_CHN VeChn)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_StartRecvPic(VENC_CHN VeChn)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_StopRecvPic(VENC_CHN VeChn)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_Query(VENC_CHN VeChn, VENC_CHN_STAT_S *pstStat)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetChnAttr(VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetChnAttr(VENC_CHN VeChn, VENC_CHN_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream, HI_BOOL bBlockFlag)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_ReleaseStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_InsertUserData(VENC_CHN VeChn, HI_U8 *pu8Data, HI_U32 u32Len)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SendFrame(VENC_GRP VeGroup, VIDEO_FRAME_INFO_S *pstFrame)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetMaxStreamCnt(VENC_CHN VeChn, HI_U32 u32MaxStrmCnt)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetMaxStreamCnt(VENC_CHN VeChn, HI_U32 *pu32MaxStrmCnt)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_RequestIDR(VENC_CHN VeChn)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetFd(VENC_CHN VeChn)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetRoiCfg(VENC_CHN VeChn, VENC_ROI_CFG_S *pstVencRoiCfg)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetRoiCfg(VENC_CHN VeChn, HI_U32 u32Index, VENC_ROI_CFG_S *pstVencRoiCfg)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetH264SliceSplit(VENC_CHN VeChn, const VENC_PARAM_H264_SLICE_SPLIT_S *pstSliceSplit)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetH264SliceSplit(VENC_CHN VeChn, VENC_PARAM_H264_SLICE_SPLIT_S *pstSliceSplit)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetH264InterPred(VENC_CHN VeChn, const VENC_PARAM_H264_INTER_PRED_S *pstH264InterPred)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetH264InterPred(VENC_CHN VeChn, VENC_PARAM_H264_INTER_PRED_S *pstH264InterPred)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetH264IntraPred(VENC_CHN VeChn, const VENC_PARAM_H264_INTRA_PRED_S *pstH264InterPred)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetH264IntraPred(VENC_CHN VeChn, VENC_PARAM_H264_INTRA_PRED_S *pstH264InterPred)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetH264Trans(VENC_CHN VeChn, const VENC_PARAM_H264_TRANS_S *pstH264Trans)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetH264Trans(VENC_CHN VeChn, VENC_PARAM_H264_TRANS_S *pstH264Trans)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetH264Entropy(VENC_CHN VeChn, const VENC_PARAM_H264_ENTROPY_S *pstH264EntropyEnc)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetH264Entropy(VENC_CHN VeChn, VENC_PARAM_H264_ENTROPY_S *pstH264EntropyEnc)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetH264Poc(VENC_CHN VeChn, const VENC_PARAM_H264_POC_S *pstH264Poc)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetH264Poc(VENC_CHN VeChn, VENC_PARAM_H264_POC_S *pstH264Poc)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetH264Dblk(VENC_CHN VeChn, const VENC_PARAM_H264_DBLK_S *pstH264Dblk)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetH264Dblk(VENC_CHN VeChn, VENC_PARAM_H264_DBLK_S *pstH264Dblk)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetH264Vui(VENC_CHN VeChn, const VENC_PARAM_H264_VUI_S *pstH264Vui)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetH264Vui(VENC_CHN VeChn, VENC_PARAM_H264_VUI_S *pstH264Vui)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetJpegParam(VENC_CHN VeChn, const VENC_PARAM_JPEG_S *pstJpegParam)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetJpegParam(VENC_CHN VeChn, VENC_PARAM_JPEG_S *pstJpegParam)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetMjpegParam(VENC_CHN VeChn, const VENC_PARAM_MJPEG_S *pstMjpegParam)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetMjpegParam(VENC_CHN VeChn, VENC_PARAM_MJPEG_S *pstMjpegParam)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetRcPara(VENC_CHN VeChn, VENC_RC_PARAM_S *pstRcPara)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetRcPara(VENC_CHN VeChn, VENC_RC_PARAM_S *pstRcPara)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_EnableIDR(VENC_CHN VeChn, HI_BOOL bEnableIDR)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetSuperFrameCfg(VENC_CHN VeChn, const VENC_SUPERFRAME_CFG_S *pstSuperFrmParam)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_GetSuperFrameCfg(VENC_CHN VeChn,VENC_SUPERFRAME_CFG_S *pstSuperFrmParam)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VDEC_SetModParam(VDEC_MOD_PARAM_S *pstModParam)
{
	return	HI_SUCCESS;
}

HI_S32 HI_MPI_VDEC_GetModParam(VDEC_MOD_PARAM_S *pstModParam)
{
	return	HI_SUCCESS;
}

//--------------------------------------------------------------------
HI_S32 HI_MPI_ADEC_CreateChn(ADEC_CHN AdChn, ADEC_CHN_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_ADEC_DestroyChn(ADEC_CHN AdChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_ADEC_SendStream(ADEC_CHN AdChn, const AUDIO_STREAM_S *pstStream, HI_BOOL bBlock)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_ADEC_ClearChnBuf(ADEC_CHN AdChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_ADEC_RegeisterDecoder(HI_S32 *ps32Handle, ADEC_DECODER_S *pstDecoder)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_ADEC_UnRegisterDecoder(HI_S32 s32Handle)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_ADEC_GetFrame(ADEC_CHN AdChn, AUDIO_FRAME_INFO_S *pstFrmInfo, HI_BOOL bBlock)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_ADEC_ReleaseFrame(ADEC_CHN AdChn, AUDIO_FRAME_INFO_S *pstFrmInfo)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AENC_CreateChn(AENC_CHN AeChn, const AENC_CHN_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AENC_DestroyChn(AENC_CHN AeChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AENC_SendFrame(AENC_CHN AeChn,
	const AUDIO_FRAME_S *pstFrm, const AEC_FRAME_S *pstAecFrm)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AENC_GetStream(AENC_CHN AeChn, AUDIO_STREAM_S *pstStream, HI_BOOL bBlock)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AENC_ReleaseStream(AENC_CHN AeChn, const AUDIO_STREAM_S *pstStream)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AENC_GetFd(AENC_CHN AeChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AENC_Save_File(AENC_CHN AeChn, AUDIO_SAVE_FILE_INFO_S *pstSaveFileInfo)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AENC_RegeisterEncoder(HI_S32 *ps32Handle, AENC_ENCODER_S *pstEncoder)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AENC_UnRegisterEncoder(HI_S32 s32Handle)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_SetPubAttr(AUDIO_DEV AudioDevId, const AIO_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_GetPubAttr(AUDIO_DEV AudioDevId, AIO_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_Enable(AUDIO_DEV AudioDevId)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_Disable(AUDIO_DEV AudioDevId)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_EnableChn(AUDIO_DEV AudioDevId, AI_CHN AiChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_DisableChn(AUDIO_DEV AudioDevId, AI_CHN AiChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_GetFrame(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_FRAME_S
	*pstFrm, AEC_FRAME_S *pstAecFrm, HI_S32 s32MilliSec)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_ReleaseFrame(AUDIO_DEV AudioDevId, AI_CHN AiChn,
	AUDIO_FRAME_S *pstFrm, AEC_FRAME_S *pstAecFrm)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_SetChnParam(AUDIO_DEV AudioDevId, AI_CHN AiChn, AI_CHN_PARAM_S *pstChnParam)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_GetChnParam(AUDIO_DEV AudioDevId, AI_CHN AiChn, AI_CHN_PARAM_S *pstChnParam)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_EnableAec(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_DEV AoDevId, AO_CHN AoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_DisableAec(AUDIO_DEV AiDevId, AI_CHN AiChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_EnableReSmp(AUDIO_DEV AudioDevId, AI_CHN AiChn, AUDIO_RESAMPLE_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_DisableReSmp(AUDIO_DEV AudioDevId, AI_CHN AiChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_EnableAnr(AUDIO_DEV AudioDevId, AI_CHN AiChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_DisableAnr(AUDIO_DEV AudioDevId, AI_CHN AiChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_GetFd(AUDIO_DEV AudioDevId, AI_CHN AiChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_SetVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_DEV AoDevId, AO_CHN AoChn, AI_VQE_CONFIG_S *pstVqeConfig)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_GetVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn, AI_VQE_CONFIG_S *pstVqeConfig)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_EnableVqe(AUDIO_DEV AiDevId, AI_CHN AiChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_DisableVqe(AUDIO_DEV AiDevId, AI_CHN AiChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_Save_File(AUDIO_DEV AudioDevId, AI_CHN AiChn, AUDIO_SAVE_FILE_INFO_S *pstSaveFileInfo)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AI_ClrPubAttr(AUDIO_DEV AudioDevId)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_SetPubAttr(AUDIO_DEV AudioDevId, const AIO_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_GetPubAttr(AUDIO_DEV AudioDevId, AIO_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_Enable(AUDIO_DEV AudioDevId)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_Disable(AUDIO_DEV AudioDevId)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_EnableChn(AUDIO_DEV AudioDevId, AO_CHN AoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_DisableChn(AUDIO_DEV AudioDevId, AO_CHN AoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_SendFrame(AUDIO_DEV AoDevId, AO_CHN AoChn, const AUDIO_FRAME_S *pstData, HI_S32 s32MilliSec)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_EnableReSmp(AUDIO_DEV AudioDevId, AO_CHN AoChn, AUDIO_RESAMPLE_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_DisableReSmp(AUDIO_DEV AudioDevId, AO_CHN AoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_ClearChnBuf(AUDIO_DEV AudioDevId, AO_CHN AoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_QueryChnStat(AUDIO_DEV AudioDevId, AO_CHN AoChn, AO_CHN_STATE_S *pstStatus)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_PauseChn(AUDIO_DEV AudioDevId, AO_CHN AoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_ResumeChn(AUDIO_DEV AudioDevId, AO_CHN AoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_SetVolume(AUDIO_DEV AudioDevId, AO_CHN AoChn, HI_S32 s32VolumeDb)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_GetVolume(AUDIO_DEV AudioDevId, AO_CHN AoChn, HI_S32 *ps32VolumeDb)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_GetFd(AUDIO_DEV AudioDevId, AO_CHN AoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_AO_ClrPubAttr(AUDIO_DEV AudioDevId)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_CreateChn(VDA_CHN VdaChn, const VDA_CHN_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_DestroyChn(VDA_CHN VdaChn){
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_GetChnAttr(VDA_CHN VdaChn, VDA_CHN_ATTR_S *pstAttr){
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_SetChnAttr(VDA_CHN VdaChn, const VDA_CHN_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_StartRecvPic(VDA_CHN VdaChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_StopRecvPic(VDA_CHN VdaChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_GetData(VDA_CHN VdaChn, VDA_DATA_S *pstVdaData, HI_S32 s32MilliSec)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_ReleaseData(VDA_CHN VdaChn, const VDA_DATA_S* pstVdaData)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_ResetOdRegion(VDA_CHN VdaChn, HI_S32 s32RgnIndex)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_Query(VDA_CHN VdaChn, VDA_CHN_STAT_S *pstChnStat)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_GetFd(VDA_CHN VdaChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_UpdateRef(VDA_CHN VdaChn, const VIDEO_FRAME_INFO_S *pstRefFrame)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDA_UserSendPic(VDA_CHN VdaChn, const VIDEO_FRAME_INFO_S *pstUserFrame, HI_BOOL bBlock, HI_U32 u32MilliSec)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_Create(PCIV_CHN pcivChn, PCIV_ATTR_S *pPcivAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_Destroy(PCIV_CHN pcivChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_SetAttr(PCIV_CHN pcivChn, PCIV_ATTR_S *pPcivAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_GetAttr(PCIV_CHN pcivChn, PCIV_ATTR_S *pPcivAttr)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_Start(PCIV_CHN pcivChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_Stop(PCIV_CHN pcivChn)
{
	return	HI_SUCCESS;
}
HI_S32  HI_MPI_PCIV_DmaTask(PCIV_DMA_TASK_S *pTask)
{
	return	HI_SUCCESS;
}
HI_U32  HI_MPI_PCIV_Malloc(HI_U32 u32BlkSize, HI_U32 u32BlkCnt, HI_U32 u32PhyAddr[])
{
	u32PhyAddr[0]	=	(HI_U32)malloc(u32BlkSize);
	if(0 == u32PhyAddr[0])
	{
		return	HI_FAILURE;
	}
	return	HI_SUCCESS;
}
HI_S32  HI_MPI_PCIV_Free(HI_U32 u32BlkCnt, HI_U32 u32PhyAddr[])
{
	free((void*)u32PhyAddr[0]);
	return	HI_SUCCESS;
}
HI_S32  HI_MPI_PCIV_GetLocalId(HI_VOID)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_EnumChip(HI_S32 s32ChipID[PCIV_MAX_CHIPNUM])
{
	return	HI_SUCCESS;
}
HI_S32  HI_MPI_PCIV_GetBaseWindow(HI_S32 s32ChipId, PCIV_BASEWINDOW_S *pBase)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_WinVbCreate(PCIV_WINVBCFG_S *pCfg)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_WinVbDestroy(HI_VOID)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_Show(PCIV_CHN pcivChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_Hide(PCIV_CHN pcivChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_GetPreProcCfg(PCIV_CHN pcivChn, PCIV_PREPROC_CFG_S *pstCfg)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_PCIV_SetPreProcCfg(PCIV_CHN pcivChn, PCIV_PREPROC_CFG_S *pstCfg)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDEC_SetProtocolParam(VDEC_CHN VdChn,VDEC_PRTCL_PARAM_S *pstParam)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDEC_GetProtocolParam(VDEC_CHN VdChn,VDEC_PRTCL_PARAM_S *pstParam)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VENC_SetRefParam(VENC_CHN VeChn, const VENC_PARAM_REF_S *pstRefParam)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VO_PauseChn (VO_LAYER VoLayer, VO_CHN VoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VO_ResumeChn(VO_LAYER VoLayer, VO_CHN VoChn)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VPSS_GetGrpFrame(VPSS_GRP VpssGrp, VIDEO_FRAME_INFO_S *pstVideoFrame, HI_U32 u32FrameIndex)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VPSS_ReleaseGrpFrame(VPSS_GRP VpssGrp,  VIDEO_FRAME_INFO_S *pstVideoFrame)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VPSS_SendFrame(VPSS_GRP VpssGrp,  VIDEO_FRAME_INFO_S *pstVideoFrame, HI_S32 s32MilliSec)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VPSS_SetGrpCrop(VPSS_GRP VpssGrp,  VPSS_CROP_INFO_S *pstCropInfo)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VB_InitModCommPool(VB_UID_E enVbUid)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VB_ExitModCommPool(VB_UID_E enVbUid)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VO_BindGraphicLayer(GRAPHIC_LAYER GraphicLayer, VO_DEV VoDev)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VO_UnBindGraphicLayer(GRAPHIC_LAYER GraphicLayer, VO_DEV VoDev)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VB_SetModPoolConf(VB_UID_E enVbUid, const VB_CONF_S *pstVbConf)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VDEC_SetChnVBCnt(VDEC_CHN VdChn, HI_U32 u32BlkCnt)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VENC_GetRcParam(VENC_CHN VeChn, VENC_RC_PARAM_S *pstRcParam)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VENC_SetRcParam(VENC_CHN VeChn, const VENC_RC_PARAM_S *pstRcParam)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VI_SetSkipModeEx(VI_CHN ViChn, VI_SKIP_MODE_EX_S* pstModeEx)
{
	return	HI_SUCCESS;
}
HI_S32 HI_MPI_VI_GetSkipModeEx(VI_CHN ViChn, VI_SKIP_MODE_EX_S* pstModeEx)
{
	return	HI_SUCCESS;
}
HI_S32      HI_TDE2_Open(HI_VOID)
{
	return	HI_SUCCESS;
}
HI_VOID     HI_TDE2_Close(HI_VOID)
{

}
TDE_HANDLE  HI_TDE2_BeginJob(HI_VOID)
{
	return	0;
}
HI_S32      HI_TDE2_EndJob(TDE_HANDLE s32Handle, HI_BOOL bSync, HI_BOOL bBlock, HI_U32 u32TimeOut)
{
	return	HI_SUCCESS;
}
HI_S32      HI_TDE2_QuickFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect,
	HI_U32 u32FillData)
{
	return	HI_SUCCESS;
}
